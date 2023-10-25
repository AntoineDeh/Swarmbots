/**
 * \file  pilot.c
 * \version  0.1
 * \author Florentin LEPELTIER(inspiré des travaux de Jérôme Delatour)
 * \author Louison LEGROS
 * \date May 11, 2023
 * \brief Source file of the pilot module. This module manages the motor of the robots and change their directions.
 *
 * \see pilot.h
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
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <errno.h>

#include "../lib/watchdog.h"
#include "../lib/defs.h"
#include "../logs/controller_logger.h"
#include "../com/gui_secretary_proxy.h"
#include "alphabot2/radar.h"
#include "alphabot2/motor.h"
#include "pilot.h"
#include "controller_core.h"
#include "state_indicator.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def OBSTACLE_REFRESH_PERIOD_CHECK
 * Period in milliseconds between checks of an obstacle when moving forward
 */
#define OBSTACLE_REFRESH_PERIOD_CHECK 250
/**
 * \def NAME_MQ_BOX
 * Name of the pilot message queue
 */
#define NAME_MQ_BOX  "/pilot_mq"
/**
 * \def MQ_MSG_COUNT
 * Max messages that can be stored into the pilot message queue
 */
#define MQ_MSG_COUNT 10
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/**
 * \enum state_e
 * \brief Defines the different states of the pilot module.
 */
typedef enum  {
    S_FORGET = 0, 
    S_IDLE, 
    S_MODE_FORWARD, 
    S_CHOICE, 
    S_DEATH, 
    S_NB
} state_e;
/**
 * \enum event_e
 * \brief Defines the different events handled by the pilot module.
 */
typedef enum  {
    E_ASK_CMD = 0, 
    E_OBSTACLE_DETECTED, 
    E_GO_MOVE_FORWARD, 
    E_GO_IDLE, 
    E_TIME_OUT_RADAR, 
    E_STOP, 
    E_NB
} event_e;
/**
 * \enum action_e
 * \brief Defines the different actions performed by the pilot module.
 */
typedef enum  {
    A_NOP = 0,
    A_EVALUATE_COMMAND, 
    A_MOVE_ROBOT, 
    A_MOVE_ROBOT_FORWARD, 
    A_CHECK_RADAR, 
    A_CHECK_RADAR_MOVING_FORWARD, 
    A_STOP_TO_OBSTACLE, 
    A_STOP, 
    ACTION_NB
} action_e ;
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/**
 * \struct transition_t
 * \brief Contains the dest_state and the action to perform. It is used to switch state inside the module.
 */
typedef struct
{
    state_e dest_state;
    action_e action;
} transition_t;
/**
 * \struct mq_msg_data_t
 * \brief Contains the data that can be passed through the message.
 */
typedef struct
{
    event_e event;
    Command cmd;
} mq_msg_data_t;
/**
 * \union mq_msg_data_t
 * \brief Contains the entire message manipulated by the module and the message queue.
 */
typedef union
{
    mq_msg_data_t data;
    char buffer[sizeof(mq_msg_data_t)];
} mq_msg;
/**
 * \typedef int (*action_ptr)(mq_msg *msg)
 * \brief Definition of function pointer for the actions to perform.
 */
typedef int (*action_ptr)(mq_msg *msg);

/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/*------------------------STATE MACHINE RELATED FUNCTIONS------------------------*/
/**
 * \fn static int PILOT_get_msg_from_queue(mq_msg* msg)
 * \brief Gets the last message from pilot message queue
 * \author Florentin LEPELTIER
 * 
 * \param msg [out] message received from the queue
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_get_msg_from_queue(mq_msg* msg);
/**
 * \fn static int PILOT_add_msg_to_queue(mq_msg* msg)
 * \brief Adds a new message to pilot message queue
 * \author Florentin LEPELTIER
 * 
 * \param msg [in] message to add in the queue
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_add_msg_to_queue(mq_msg* msg);
/**
 * \fn static void* PILOT_run(void* param)
 * \brief Blocking function running machine state of the module
 * \author Florentin LEPELTIER
 *
 * \param param: to be used if needed
 */
static void* PILOT_run(void* param);
/*------------------------ACTIONS RELATED FUNCTIONS------------------------*/
/**
 * \fn static int PILOT_action_nop(mq_msg * msg)
 * \brief For transitions without action needed.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_action_nop(mq_msg *msg);
/**
 * \fn static int PILOT_action_evaluate_cmd(void)
 * \brief evaluate the command and add associated state in queue
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_action_evaluate_cmd(mq_msg * msg);
/**
 * \fn static int PILOT_action_move_robot(mq_msg *msg)
 * \brief Calls motor.c functions to move the robot
 * \author Florentin LEPELTIER
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_action_move_robot(mq_msg *msg);
/**
 * \fn static int PILOT_action_move_robot_forward(mq_msg *msg)
 * \brief Calls motor.c functions to move the robot forward and evaluate radar
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_action_move_robot_forward(mq_msg *msg);
/**
 * \fn static int PILOT_action_check_radar(mq_msg *msg)
 * \brief check radar and send the value if a change is detected.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_action_check_radar(mq_msg *msg);
/**
 * \fn static int PILOT_action_check_radar_moving_forward(mq_msg *msg)
 * \brief check radar and send the value if a change is detected.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_action_check_radar_moving_forward(mq_msg *msg);
/**
 * \fn static int PILOT_action_stop_to_obstacle(mq_msg *msg)
 * \brief stop the robot after detecting an obstacle
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_action_stop_to_obstacle(mq_msg *msg);
/**
 * \fn static int PILOT_action_stop(mq_msg *msg)
 * \brief stop the state machine
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int PILOT_action_stop(mq_msg *msg);
/**
 * \fn static void PILOT_check_radar_time_out(watchdog_t * watchdog)
 * \brief callback : check radar and send the value if a change is detected.
 * \author Louison LEGROS
 *
 * \param watchdog : watchdog used to trigger the radar check
 */
static void PILOT_check_radar_time_out(watchdog_t * watchdog);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/**
 * \var pilot_thread
 * \brief Thread used by the module to execute the run method.
 */
static pthread_t pilot_thread;
/**
 * \var pilot_message_queue
 * \brief Message queue used by the module to handle events and manage module state machine.
 */
static 	mqd_t pilot_message_queue;
/**
 * \var watchdog_t *pilot_radar_check_watchdog
 * \brief watchdog used to trigger the radar check
 */
watchdog_t *pilot_radar_check_watchdog;
/**
 * \brief Defines the commands as strings.
 */
char *command_to_string[] = {
    "FORWARD",
    "RIGHT",
    "LEFT",
    "BACKWARD",
    "STOP"
};
/**
 * \var pilot_state_machine
 * \brief Struct describing the machine state of the module
 */
static transition_t pilot_state_machine [S_NB-1][E_NB] = {
        [S_IDLE][E_ASK_CMD]                     = {S_CHOICE, A_EVALUATE_COMMAND},
        [S_IDLE][E_TIME_OUT_RADAR]              = {S_IDLE, A_CHECK_RADAR},
        [S_MODE_FORWARD][E_ASK_CMD]             = {S_CHOICE, A_EVALUATE_COMMAND},
        [S_MODE_FORWARD][E_TIME_OUT_RADAR]      = {S_MODE_FORWARD, A_CHECK_RADAR_MOVING_FORWARD},
        [S_MODE_FORWARD][E_OBSTACLE_DETECTED]   = {S_IDLE, A_STOP_TO_OBSTACLE},
        [S_CHOICE][E_GO_IDLE]                   = {S_IDLE, A_MOVE_ROBOT},
        [S_CHOICE][E_GO_MOVE_FORWARD]           = {S_MODE_FORWARD, A_MOVE_ROBOT_FORWARD},
        [S_CHOICE][E_TIME_OUT_RADAR]            = {S_CHOICE, A_CHECK_RADAR},
        [S_IDLE][E_STOP]                        = {S_DEATH, A_STOP},
        [S_MODE_FORWARD][E_STOP]                = {S_DEATH, A_STOP},
        [S_CHOICE][E_STOP]                      = {S_DEATH, A_STOP}
};
/**
 * \var static const Action_Pt actions_tab[ACTION_NB]
 * \brief Array of function pointer to call from action to perform.
 */
static const action_ptr actions_tab[ACTION_NB] = {
    &PILOT_action_nop,
    &PILOT_action_evaluate_cmd,
    &PILOT_action_move_robot,
    &PILOT_action_move_robot_forward,
    &PILOT_action_check_radar,
    &PILOT_action_check_radar_moving_forward,
    &PILOT_action_stop_to_obstacle,
    &PILOT_action_stop
};
/**
 * \var obstacle_state
 * \brief actual obstacle state from the radar.
 */
static bool_e obstacle_state = FALSE;
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
extern int PILOT_create(void) {
    pilot_radar_check_watchdog = watchdog_create(OBSTACLE_REFRESH_PERIOD_CHECK, PILOT_check_radar_time_out);

    if(MOTOR_create() != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On MOTOR_create(): motor creation failed.");
        return -1;
    }
    RADAR_create();
    if(RADAR_create() != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On RADAR_create(): radar creation failed.");
        goto error_radar;
    }

    struct mq_attr mq_a = {
            .mq_maxmsg = MQ_MSG_COUNT,
            .mq_msgsize = sizeof(mq_msg),
            .mq_flags = 0,
    };

    if((pilot_message_queue = mq_open(NAME_MQ_BOX, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mq_a)) == -1 ) {
        if(errno == EEXIST) {
            mq_unlink(NAME_MQ_BOX);
            if((pilot_message_queue = mq_open(NAME_MQ_BOX, O_CREAT | O_RDWR , 0644 ,&mq_a )) == -1 ) {
                CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for pilot after unlinking the mq.");
                goto error_mq;
            }
        } else {
            CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for pilot.");
            goto error_mq;
        }
    }
    return 0;

    error_mq:
    if(RADAR_destroy() != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On RADAR_destroy(): radar destroy failed.");
    }
    error_radar:
    if(MOTOR_destroy() != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On MOTOR_destroy(): motor destroy failed.");
    }
    return -1;
}

extern int PILOT_destroy(void) {
    int ret = 0;

    if(mq_close(pilot_message_queue) == -1 ) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_close(): error while closing the mq of pilot.");
        ret = -1;
    }
    if(mq_unlink(NAME_MQ_BOX) == -1 ) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_unlink(): error while unlinking the mq of pilot.");
        ret = -1;
    }

    watchdog_destroy(pilot_radar_check_watchdog);

    if(RADAR_destroy() != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On RADAR_destroy(): radar creation failed.");
        ret = -1;
    }
    if(MOTOR_destroy() != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On MOTOR_destroy(): motor creation failed.");
        ret = -1;
    }
    return ret;
}

extern int PILOT_start(void) {
    if(pthread_create(&pilot_thread, NULL, PILOT_run, NULL) != 0 ) {
        CONTROLLER_LOGGER_log(ERROR, "On pthread_create() : error while creating pilot thread.");
        return -1;
    }
    return 0;
}

extern int PILOT_stop(void) {
    mq_msg msg = {.data.event = E_STOP, 0};
    if(PILOT_add_msg_to_queue(&msg) == 0) {
        if(pthread_join(pilot_thread, NULL) != 0) {
            CONTROLLER_LOGGER_log(ERROR, "On pthread_join(): error while waiting the termination of pilot thread.");
            return -1;
        }
    }
    return 0;
}

extern int PILOT_ask_cmd(Command cmd) {
    mq_msg msg = {.data.event = E_ASK_CMD ,.data.cmd = cmd};
    if(PILOT_add_msg_to_queue(&msg) == -1) {
        return -1;
    }

    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static int PILOT_get_msg_from_queue(mq_msg* msg) {
    if(mq_receive(pilot_message_queue, msg->buffer, sizeof(mq_msg), NULL) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_receive(): Pilot has failed to receive a message on the mq.");
        return -1;
    }
    return 0;
}

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int PILOT_add_msg_to_queue(mq_msg* msg) {
    if(mq_send(pilot_message_queue, msg->buffer, sizeof(mq_msg), 0) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_send(): Pilot has failed to send a message on the mq.");
        return -1;
    }
    return 0;
}
#else
int PILOT_add_msg_to_queue(mq_msg* msg);
#endif

static void* PILOT_run(void* param) {
    mq_msg msg;
    transition_t * current_transition;
    state_e current_state = S_IDLE;
    watchdog_start(pilot_radar_check_watchdog);

    while (current_state != S_DEATH) {
        if(PILOT_get_msg_from_queue(&msg) == -1) {
            return NULL;
        }
        current_transition = &pilot_state_machine[current_state][msg.data.event];
        if (current_transition->dest_state != S_FORGET) {
            if (actions_tab[current_transition->action](&msg) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On actions_tab() : failed to execute the action for PILOT.");
                return NULL;
            }
            current_state = current_transition->dest_state;
        }
    }
    return NULL;
}

// ACTION METHODS

static int PILOT_action_nop(mq_msg * msg) {
    return 0;
}

static int PILOT_action_evaluate_cmd(mq_msg * msg) {
    char log_msg_cmd[35];
    sprintf(log_msg_cmd, "PILOT: Command asked : %s", command_to_string[msg->data.cmd]);
    CONTROLLER_LOGGER_log(DEBUG,log_msg_cmd);

    if(msg->data.cmd == FORWARD) {
        msg->data.event = E_GO_MOVE_FORWARD;
        msg->data.cmd = 0;
    }
    else {
        msg->data.event = E_GO_IDLE;
    }

    if (PILOT_add_msg_to_queue(msg) == -1) {
        return -1;
    }
    return 0;
}

static int PILOT_action_move_robot(mq_msg * msg) {

    MOTOR_set_velocity(msg->data.cmd);
    char log_msg[45];
    sprintf(log_msg, "PILOT : robot direction changed to %s", command_to_string[msg->data.cmd]);
    CONTROLLER_LOGGER_log(INFO, log_msg);
    return 0;
}

static int PILOT_action_move_robot_forward(mq_msg * msg) {

    MOTOR_set_velocity(FORWARD);

    CONTROLLER_LOGGER_log(INFO,"PILOT : robot direction changed to FORWARD");

    if(obstacle_state) {
        mq_msg msg = {.data.event = E_OBSTACLE_DETECTED, 0};
        if (PILOT_add_msg_to_queue(&msg) == -1)
        {
            return -1;
        }
    }
    return 0;
}

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int PILOT_action_check_radar(mq_msg * msg) {
    int ret = 0;

    if(CONTROLLER_CORE_get_mode().radar_mode == ENABLED) {
        bool_e new_obstacle_state;
        if(RADAR_get_radar(&new_obstacle_state) != 0) {
            CONTROLLER_LOGGER_log(ERROR, "On RADAR_get_radar() : PILOT failed to get radar state.");
            return -1;
        }
        if(new_obstacle_state != obstacle_state)
        {
            obstacle_state = new_obstacle_state;
            
            Id_Robot id_robot = CONTROLLER_CORE_get_id_robot();
            if(GUI_SECRETARY_PROXY_set_radar(id_robot, obstacle_state) != 0) {
                CONTROLLER_LOGGER_log(ERROR, "On GUI_SECRETARY_PROXY_set_radar() : PILOT failed to set radar state.");
                ret = -1;
            }

            CONTROLLER_LOGGER_log(DEBUG,"PILOT : Radar changed state");
        }
    }
    watchdog_start(pilot_radar_check_watchdog);
    return ret;
}
#else
int PILOT_action_check_radar(mq_msg * msg);
#endif

static int PILOT_action_check_radar_moving_forward(mq_msg * msg) {
    int ret = 0;
    if(PILOT_action_check_radar(NULL) != 0) {
        ret = -1;
    }

    if(obstacle_state) {
        mq_msg msg = {.data.event = E_OBSTACLE_DETECTED, 0};
        if (PILOT_add_msg_to_queue(&msg) == -1)
        {
            ret = -1;
        }
    }
    return ret;
}


static int PILOT_action_stop_to_obstacle(mq_msg * msg) {
    MOTOR_set_velocity(STOP);

    CONTROLLER_LOGGER_log(INFO,"PILOT : Obstacle detected");

    if (STATE_INDICATOR_set_state(EMERGENCY) == -1)
    {
        CONTROLLER_LOGGER_log(ERROR, "On STATE_INDICATOR_set_state(EMERGENCY) : failed to set robot state.");
        return -1;
    }
    return 0;
}

static int PILOT_action_stop(mq_msg *msg) {
    watchdog_cancel(pilot_radar_check_watchdog);
    MOTOR_set_velocity(STOP);
    return 0;
}

// watchdog callback
static void PILOT_check_radar_time_out(watchdog_t * watchdog) {
    mq_msg msg = {.data.event = E_TIME_OUT_RADAR};
    if(PILOT_add_msg_to_queue(&msg) != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On PILOT_add_msg_to_queue(&msg) : PILOT_check_radar_time_out callback failed to send a message to the mq.");
    }
}