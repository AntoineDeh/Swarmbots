/**
 * \file  controller_ringer.c
 * \version  0.1
 * \author Louisons LEGROS
 * \date Jun 4, 2023
 * \brief Source file of the ringer on the SB_C side. This module watch the connection
 * status between SB_IHM and SB_C by responding "pong" when a "ping" is received.
 *
 * \see controller_ringer.h
 *
 * \section License
 *
 * The MIT License
 *
 * Copyright (c) 2023, Prose A2 2023
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * \copyright Prose A2 2023
 *
 */

/* ----------------------  INCLUDES  ---------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <errno.h>

#include "../lib/watchdog.h"
#include "controller_ringer.h"
#include "controller_core.h"
#include "../logs/controller_logger.h"
#include "../com/gui_ringer_proxy.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def MAX_FAILED_PINGS
 * Limits of allowed failed pings before considering disconnection.
 */
#define MAX_FAILED_PINGS 10
/**
 * \def TIME_OUT_PINGS
 * Time in millisecond to wait before estimating that the ping will not arrive.
 */
#define TIME_OUT_PINGS 30000
/**
 * \def CONTROLLER_RINGER_MQ_BOX
 * Name of the controller_ringer message queue
 */
#define CONTROLLER_RINGER_MQ_BOX "/controller_ringer_mq"
/**
 * \def MQ_MSG_COUNT
 * Max messages that can be stored into the controller_ringer message queue
 */
#define MQ_MSG_COUNT 10
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/**
 * \enum state_e
 * \brief Defines the different states of the controller_ringer module.
 */
typedef enum {
    S_FORGET = 0,
    S_DISCONNECTED,
    S_WAITING_END_INIT,
    S_WAITING_PING,
    S_CHOICE,
    S_DEATH,
    S_NB
} state_e;
/**
 * \enum event_e
 * \brief Defines the different events handled by the controller_ringer module.
 */
typedef enum {
    E_INIT = 0,
    E_ASK_AVAILABILITY,
    E_TIME_OUT_PING,
    E_CAN_STILL_FAIL_PINGS,
    E_HAS_TOO_MUCH_FAILED_PINGS,
    E_STOP,
    E_NB
} event_e;
/**
 * \enum action_e
 * \brief Defines the different actions performed by the controller_ringer module.
 */
typedef enum {
    A_NOP = 0,
    A_INIT,
    A_SET_AVAILABILITY,
    A_CHECK_FAILED_PINGS,
    A_INCREASE_FAILED_PINGS,
    A_NOTIFY_LOST_CONNECTION,
    A_STOP,
    ACTION_NB
} action_e;
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/**
 * \struct transition_t
 * \brief Contains the dest_state and the action to perform. It is used to switch state inside the module.
 */
typedef struct {
    state_e dest_state;
    action_e action;
} transition_t;
/**
 * \struct mq_msg_data_t
 * \brief Contains the data that can be passed through the message.
 */
typedef struct {
    event_e event;
    int id_robot;
} mq_msg_data_t;
/**
 * \union mq_msg_data_t
 * \brief Contains the entire message manipulated by the module and the message queue.
 */
typedef union {
    mq_msg_data_t data;
    char buffer[sizeof(mq_msg_data_t)];
} mq_msg;
/**
 * \typedef int (*action_ptr)(mq_msg *msg)
 * \brief Definition of function pointer for the actions to perform.
 */
typedef int (*action_ptr)(mq_msg *msg);
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static void* CONTROLLER_RINGER_run(void* param)
 * \brief Blocking function running machine state of the module
 * \author Louison LEGROS
 *
 * \param param: to be used if needed
 */
static void *CONTROLLER_RINGER_run(void *param);
/**
 * \fn static int CONTROLLER_RINGER_get_msg_from_queue(mq_msg* msg)
 * \brief Gets the last message from controller_ringer message queue
 * \author Louison LEGROS
 * 
 * \param msg : data structure to fill with the message queue data.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_get_msg_from_queue(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_add_msg_to_queue(mq_msg* msg)
 * \brief Adds a new message to controller_ringer message queue
 * \author Louison LEGROS
 * 
 * \param msg : data structure to add to the message queue.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_add_msg_to_queue(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_action_nop(mq_msg * msg)
 * \brief For transitions without action needed.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_action_nop(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_action_init(mq_msg * msg)
 * \brief Initializes the variable failed_pings, correspond to A_INIT action.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_action_init(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_action_set_availability(mq_msg * msg)
 * \brief Sets the availability of the robot to SB_IHM, correspond to A_SET_AVAILABILITY action.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_action_notify_availability(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_check_failed_pings(mq_msg * msg)
 * \brief Checks if the failed pings max isreached, correspond to A_CHECK_FAILED_PINGS action.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_action_check_failed_pings(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_action_increase_failed_pings(mq_msg * msg)
 * \brief Increases the failed pings var, correspond to A_INCREASE_FAILED_PINGS action.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_action_increase_failed_pings(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_action_notify_lost_connection(mq_msg * msg)
 * \brief Notifies the lost connection to controller_core, correspond to A_NOTIFY_LOST_CONNECTION action.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_action_notify_lost_connection(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_action_stop(mq_msg * msg)
 * \brief stop action of the state machine.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_action_stop(mq_msg *msg);
/**
 * \fn static int CONTROLLER_RINGER_update_failed_pings(int value)
 * \brief Changes the value of the failed pings var.
 * \author Louison LEGROS
 *
 * \param value : value to put into the failed pings var.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_update_failed_pings(int value);
/**
 * \fn static int CONTROLLER_RINGER_can_still_fail_pings(void)
 * \brief Push the event E_CAN_STILL_FAIL_PINGS in the state machine.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_can_still_fail_pings(void);
/**
 * \fn static int CONTROLLER_RINGER_has_too_much_failed_pings(void)
 * \brief Push the event E_HAS_TOO_MUCH_FAILED_PINGS in the state machine.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_RINGER_has_too_much_failed_pings(void);
/**
 * \fn static void CONTROLLER_RINGER_ping_time_out(watchdog_t * watchdog)
 * \brief Callback function called when the ping watchdog is triggered.
 * \author Louison LEGROS
 *
 * \param watchdog : watchdog that triggered the callback.
 */
static void CONTROLLER_RINGER_ping_time_out(watchdog_t *watchdog);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/**
 * \var controller_ringer_thread
 * \brief Thread used by the module to execute the run method.
 */
static pthread_t controller_ringer_thread;
/**
 * \var controller_ringer_message_queue
 * \brief Message queue used by the module to handle events and manage module state machine.
 */
static mqd_t controller_ringer_message_queue;
/**
 * \var watchdog_t *controller_ringer_ping_watchdog
 * \brief watchdog used to trigger the radar check
 */
watchdog_t *controller_ringer_ping_watchdog;
/**
 * \var static const Action_Pt actions_tab[ACTION_NB]
 * \brief Array of function pointer to call from action to perform.
 */
static const action_ptr actions_tab[ACTION_NB] = {
    &CONTROLLER_RINGER_action_nop,
    &CONTROLLER_RINGER_action_init,
    &CONTROLLER_RINGER_action_notify_availability,
    &CONTROLLER_RINGER_action_check_failed_pings,
    &CONTROLLER_RINGER_action_increase_failed_pings,
    &CONTROLLER_RINGER_action_notify_lost_connection,
    &CONTROLLER_RINGER_action_stop
};
/**
 * \var controller_ringer_state_machine
 * \brief Struct describing the machine state of the module
 */
static transition_t controller_ringer_state_machine[S_NB - 1][E_NB] = {
    [S_DISCONNECTED]    [E_INIT]                        = {S_WAITING_END_INIT,  A_INIT},
    [S_WAITING_END_INIT][E_ASK_AVAILABILITY]            = {S_WAITING_PING,      A_SET_AVAILABILITY},
    [S_WAITING_PING]    [E_TIME_OUT_PING]               = {S_CHOICE,            A_CHECK_FAILED_PINGS},
    [S_WAITING_PING]    [E_ASK_AVAILABILITY]            = {S_WAITING_PING,      A_SET_AVAILABILITY},
    [S_CHOICE]          [E_CAN_STILL_FAIL_PINGS]        = {S_WAITING_PING,      A_INCREASE_FAILED_PINGS},
    [S_CHOICE]          [E_HAS_TOO_MUCH_FAILED_PINGS]   = {S_DISCONNECTED,      A_NOTIFY_LOST_CONNECTION},
    [S_DISCONNECTED]    [E_STOP]                        = {S_DEATH,             A_NOP},
    [S_WAITING_END_INIT][E_STOP]                        = {S_DEATH,             A_NOP},
    [S_WAITING_PING]    [E_STOP]                        = {S_DEATH,             A_NOP},
    [S_CHOICE]          [E_STOP]                        = {S_DEATH,             A_NOP}
};
/**
 * \var failed_pings
 * \brief Numbers of non-received pings.
 */
static int failed_pings;
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int CONTROLLER_RINGER_create()
{
    controller_ringer_ping_watchdog = watchdog_create(TIME_OUT_PINGS, CONTROLLER_RINGER_ping_time_out);

    struct mq_attr mq_a = {
        .mq_maxmsg = MQ_MSG_COUNT,
        .mq_msgsize = sizeof(mq_msg),
        .mq_flags = 0,
    };

    if((controller_ringer_message_queue = mq_open(CONTROLLER_RINGER_MQ_BOX, O_CREAT | O_RDWR | O_EXCL , 0644 ,&mq_a )) == -1 ) {
        if(errno == EEXIST) {
            mq_unlink(CONTROLLER_RINGER_MQ_BOX);
            if((controller_ringer_message_queue = mq_open(CONTROLLER_RINGER_MQ_BOX, O_CREAT | O_RDWR , 0644 ,&mq_a )) == -1 ) {
                CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for controller ringer after unlinking the mq.");
                return -1;
            }
        } else {
            CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for controller ringer.");
            return -1;
        }
    }
    return 0;
}

int CONTROLLER_RINGER_destroy() {
    int ret = 0;
    if (mq_close(controller_ringer_message_queue) == -1)
    {
        CONTROLLER_LOGGER_log(ERROR, "On mq_close(): error while closing the mq of controller ringer.");
        ret = -1;
    }
    else if (mq_unlink(CONTROLLER_RINGER_MQ_BOX) == -1)
    {
        CONTROLLER_LOGGER_log(ERROR, "On mq_unlink(): error while unlinking the mq of controller ringer.");
        ret = -1;
    }

    watchdog_destroy(controller_ringer_ping_watchdog);
    return ret;
}

int CONTROLLER_RINGER_start()
{
    if (pthread_create(&controller_ringer_thread, NULL, CONTROLLER_RINGER_run, NULL) != 0)
    {
        CONTROLLER_LOGGER_log(ERROR, "On pthread_create() : error while creating controller ringer thread.");
        return -1;
    }
    return 0;
}

int CONTROLLER_RINGER_stop()
{
    mq_msg msg = {.data.event = E_STOP};
    if(CONTROLLER_RINGER_add_msg_to_queue(&msg) == 0) {
        if (pthread_join(controller_ringer_thread, NULL) != 0)
        {
            CONTROLLER_LOGGER_log(ERROR, "On pthread_join(): error while waiting the termination of controller ringer thread.");
            return -1;
        }
    }
    else {
        return -1;
    }
    return 0;
}

int CONTROLLER_RINGER_ask_availability(int id_robot)
{
    mq_msg msg = {.data.event = E_ASK_AVAILABILITY, .data.id_robot = id_robot};
    if (CONTROLLER_RINGER_add_msg_to_queue(&msg) == -1)
    {
        return -1;
    }
    return 0;
}

int CONTROLLER_RINGER_init_failed_pings_var(void)
{
    mq_msg msg = {.data.event = E_INIT};
    if (CONTROLLER_RINGER_add_msg_to_queue(&msg) == -1)
    {
        return -1;
    }
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static int CONTROLLER_RINGER_get_msg_from_queue(mq_msg *msg)
{
    if (mq_receive(controller_ringer_message_queue, msg->buffer, sizeof(mq_msg), NULL) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_receive(): CONTROLLER RINGER has failed to receive a message on the mq.");
        return -1;
    }
    return 0;
}

#ifndef _WRAP_MQ_CONTROLLER_RINGER_MOCKERY_CMOCKA
static int CONTROLLER_RINGER_add_msg_to_queue(mq_msg *msg)
{
    if (mq_send(controller_ringer_message_queue, msg->buffer, sizeof(mq_msg), 0) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_send(): CONTROLLER RINGER has failed to send a message on the mq.");
        return -1;
    }
    return 0;
}
#else
int CONTROLLER_RINGER_add_msg_to_queue(mq_msg *msg);
#endif

static void *CONTROLLER_RINGER_run(void *param)
{
    mq_msg msg;
    state_e current_state = S_DISCONNECTED;
    transition_t *current_transition;
    while (current_state != S_DEATH)
    {
        if (CONTROLLER_RINGER_get_msg_from_queue(&msg) != 0)
        {
            CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_RINGER_mq_receive() : failed to read controller_ringer's mq.");
            return NULL;
        }
        current_transition = &controller_ringer_state_machine[current_state][msg.data.event];
        if (current_transition->dest_state != S_FORGET)
        {
            if (actions_tab[current_transition->action](&msg) == -1)
            {
                CONTROLLER_LOGGER_log(ERROR, "On actions_tab() : failed to execute the action.");
                return NULL;
            }
            current_state = current_transition->dest_state;
        }
    }
    return NULL;
}

/* ACTION METHODS */
static int CONTROLLER_RINGER_action_nop(mq_msg *msg) {
    return 0;
}

static int CONTROLLER_RINGER_action_init(mq_msg *msg) {
    if (CONTROLLER_RINGER_update_failed_pings(0) == -1) {
        return -1;
    }
    return 0;
}

static int CONTROLLER_RINGER_action_notify_availability(mq_msg *msg) {
    int ret = 0;
    if (CONTROLLER_RINGER_update_failed_pings(0) == -1) {
        ret = -1;
    }
    CONTROLLER_LOGGER_log(DEBUG, "CONTROLLER RINGER : failed pings updated");

    if (GUI_RINGER_PROXY_set_availability(msg->data.id_robot) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On GUI_RINGER_PROXY_set_availability() : failed to set availability.");
        ret = -1;
    }
    CONTROLLER_LOGGER_log(DEBUG, "CONTROLLER RINGER : availability set");

    watchdog_start(controller_ringer_ping_watchdog);
    return ret;
}

static int CONTROLLER_RINGER_action_check_failed_pings(mq_msg *msg) {
    if (failed_pings <= MAX_FAILED_PINGS) {
        if (CONTROLLER_RINGER_can_still_fail_pings() == -1) {
            return -1;
        }
    }
    else {
        if (CONTROLLER_RINGER_has_too_much_failed_pings() == -1) {
            return -1;
        }
    }

    watchdog_cancel(controller_ringer_ping_watchdog);
    return 0;
}

static int CONTROLLER_RINGER_action_increase_failed_pings(mq_msg *msg) {
    if (CONTROLLER_RINGER_update_failed_pings(failed_pings + 1) == -1) {
        return -1;
    }
    watchdog_start(controller_ringer_ping_watchdog);
    return 0;
}

static int CONTROLLER_RINGER_action_notify_lost_connection(mq_msg *msg) {
    if (CONTROLLER_CORE_connection_lost() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_CORE_connection_lost() : Failed to notify the lost connection to controller_core.");
        return -1;
    }
    return 0;
}

static int CONTROLLER_RINGER_action_stop(mq_msg *msg) {
    watchdog_cancel(controller_ringer_ping_watchdog);
    return 0;
}

/* PRIVATE METHODS */
#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int CONTROLLER_RINGER_update_failed_pings(int value)
{
    failed_pings = value;
    return 0;
}
#else
int CONTROLLER_RINGER_update_failed_pings(int value);
#endif

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int CONTROLLER_RINGER_has_too_much_failed_pings()
{
    mq_msg msg = {.data.event = E_HAS_TOO_MUCH_FAILED_PINGS};
    if (CONTROLLER_RINGER_add_msg_to_queue(&msg) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_RINGER_add_msg_to_queue() : Putting a request into controller_ringer's mq has failed.");
        return -1;
    }
    return 0;
}
#else
int CONTROLLER_RINGER_has_too_much_failed_pings();
#endif

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int CONTROLLER_RINGER_can_still_fail_pings()
{
    mq_msg msg = {.data.event = E_CAN_STILL_FAIL_PINGS};
    if (CONTROLLER_RINGER_add_msg_to_queue(&msg) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_RINGER_add_msg_to_queue() : Putting a request into controller_ringer's mq has failed.");
        return -1;
    }
    return 0;
}
#else
int CONTROLLER_RINGER_can_still_fail_pings();
#endif

static void CONTROLLER_RINGER_ping_time_out(watchdog_t *watchdog)
{
    CONTROLLER_LOGGER_log(DEBUG, "CONTROLLER RINGER : ping time out reached");
    mq_msg msg = {.data.event = E_TIME_OUT_PING};
    if (CONTROLLER_RINGER_add_msg_to_queue(&msg) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_RINGER_add_msg_to_queue() : Putting a request into controller_ringer's mq has failed.");
    }
}