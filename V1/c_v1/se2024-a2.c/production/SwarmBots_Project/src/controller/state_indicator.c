/**
 * \file  state_indicator.c
 * \version  0.1
 * \author LEGROS Louison
 * \date May 11, 2023
 * \brief Source file of the module in charge of indicating the robot state with the leds and buzzer.
 *
 * \see state_indicator.h
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
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>

#include "state_indicator.h"
#include "../alphabot2/buzzer.h"
#include "../alphabot2/leds.h"
#include "../lib/watchdog.h"
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def NAME_MQ_BOX
 * Name of the state indicator message queue
 */
#define NAME_MQ_BOX  "/state_indicator_mq"
/**
 * \def MQ_MSG_COUNT
 * Max messages that can be stored into the state indicator message queue
 */
#define MQ_MSG_COUNT (10)
/**
 * \def EMERGENCY_TIME_OUT
 * Time out in millisecond to return to the state selected after an emergency
 */
#define EMERGENCY_TIME_OUT (1000)
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
 * \enum state_e
 * \brief Defines the different states of the state indicator module.
 */
typedef enum {
    S_FORGET = 0,
    S_WAITING_CONNECTION,
    S_EMERGENCY,
    S_SELECTED,
    S_NOT_SELECTED,
    S_CHOICE,
    S_DEATH,
    S_NB
} mae_state_e;
/**
 * \enum event_e
 * \brief Defines the different events handled by the state indicator module.
 */
typedef enum {
    E_SET_STATE = 0,
    E_TIME_OUT_EMERGENCY,
    E_CALL_EMERGENCY,
    E_CALL_WAITING_CONNECTION,
    E_CALL_SELECTED,
    E_CALL_NOT_SELECTED,
    E_ENABLE_LED,
    E_DISABLE_LED,
    E_ENABLE_BUZZER,
    E_DISABLE_BUZZER,
    E_STOP,
    E_NB
} event_e;
/**
 * \enum action_e
 * \brief Defines the different actions performed by the state indicator module.
 */
typedef enum {
    A_NOP = 0,
    A_NOTIFY_WAITING_CONNECTION,
    A_NOTIFY_SELECTED,
    A_NOTIFY_NOT_SELECTED,
    A_START_EMERGENCY,
    A_STOP_EMERGENCY,
    A_CHECK_STATE,
    A_ACTIVATE_LED,
    A_DEACTIVATE_LED,
    A_ACTIVATE_BUZZER,
    A_DEACTIVATE_BUZZER,
    A_STOP,
    ACTION_NB
} action_e ;
/**
 * \struct transition_t
 * \brief Contains the dest_state and the action to perform. It is used to switch state inside the module.
 */
typedef struct {
    mae_state_e dest_state;
    action_e action;
} transition_t;
/**
 * \struct mq_msg_data_t
 * \brief Contains the data that can be passed through the message.
 */
typedef struct {
    event_e event;
    State state;
} mq_msg_data_t;
/**
 * \union mq_msg_data_t
 * \brief Contains the entire message manipulated by the module and the message queue.
 */
typedef union {
    mq_msg_data_t data;
    char buffer[sizeof(mq_msg_data_t)];
} mq_msg;
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static int STATE_INDICATOR_get_msg_from_queue(mq_msg* msg)
 * \brief Gets the last message from state indicator message queue
 * \author Louison LEGROS
 * 
 * \param msg [out] message received from the queue
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_get_msg_from_queue(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_add_msg_to_queue(mq_msg* msg)
 * \brief Adds a message to the state indicator message queue
 * \author Louison LEGROS
 * 
 * \param msg [in] message to send to the queue
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_add_msg_to_queue(mq_msg * msg);
/**
 * \fn static void* STATE_INDICATOR_run(void* param)
 * \brief Function running machine state of the module
 * \author Louison LEGROS
 *
 * \param param: to be used if needed
 */
static void * STATE_INDICATOR_run(void * param);
/**
 * \fn static STATE_INDICATOR_action_nop(mq_msg * msg)
 * \brief For transitions without action needed.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_nop(mq_msg * msg);
/**
 * \fn static STATE_INDICATOR_action_flashing_for_connection(mq_msg * msg)
 * \brief notifies to the user that the robot is waiting for a connection
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 * 
 */
static int STATE_INDICATOR_action_flashing_for_connection(mq_msg * msg);
/**
 * \fn static STATE_INDICATOR_action_notify_selected(mq_msg * msg)
 * \brief notifies to the user that the robot is selected
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_notify_selected(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_notify_not_selected(mq_msg * msg)
 * \brief notifies to the user that the robot is not selected
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_notify_not_selected(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_start_notify_emergency(mq_msg * msg)
 * \brief starts notifying to the user that the robot is in emergency state 
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_start_notify_emergency(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_stop_notify_emergency(mq_msg * msg)
 * \brief stops notifying to the user that the robot is in emergency state 
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_stop_notify_emergency(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_check_state(mq_msg * msg)
 * \brief check the state passed when using the event E_SET_STATE to call for a new state for the state machine
 * \author Louison LEGROS
 * 
 * \param msg the message that called the event E_SET_STATE with an associated state
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_check_state(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_activate_led(mq_msg * msg)
 * \brief activates the led
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_activate_led(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_deactivate_led(mq_msg * msg)
 * \brief deactivates the led
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_deactivate_led(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_activate_buzzer(mq_msg * msg)
 * \brief activates the buzzer
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_activate_buzzer(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_deactivate_buzzer(mq_msg * msg)
 * \brief deactivates the buzzer
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_deactivate_buzzer(mq_msg * msg);
/**
 * \fn static int STATE_INDICATOR_action_stop(mq_msg * msg)
 * \brief stop action of the state machine
 * \author Louison LEGROS
 * 
 * \param msg : data structure pushed by the trigger event.
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int STATE_INDICATOR_action_stop(mq_msg * msg);
/**
 * \fn static void STATE_INDICATOR_emergency_time_out(watchdog_t * watchdog)
 * \brief Callback function when the emergency time out is reached.
 * \author Louison LEGROS
 * 
 * \param watchdog pointer on the watchdog that called this event
 */
static void STATE_INDICATOR_emergency_time_out(watchdog_t * watchdog);
/**
 * \typedef int (*action_ptr)(mq_msg *msg)
 * \brief Definition of function pointer for the actions to perform.
 */
typedef int (*action_ptr)(mq_msg *msg);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/**
 * \var static pthread_t state_indicator_thread
 * \brief Thread used by the module to execute the run method.
 */
static pthread_t state_indicator_thread;
/**
 * \var static mqd_t state_indicator_message_queue
 * \brief Message queue used by the module to handle events and manage module state machine.
 */
static mqd_t state_indicator_message_queue;
/**
 * \var watchdog_t *state_indicator_emergency_watchdog
 * \brief watchdog used to notify the end of the emergency state
 */
watchdog_t *state_indicator_emergency_watchdog;
/**
 * \var static transition_t state_indicator_state_machine
 * \brief Struct describing the machine state of the module
 */
static transition_t state_indicator_state_machine [S_NB-1][E_NB] = {
    [S_WAITING_CONNECTION]  [E_SET_STATE]               = {S_CHOICE,                A_CHECK_STATE},
    [S_EMERGENCY]           [E_SET_STATE]               = {S_CHOICE,                A_CHECK_STATE},
    [S_SELECTED]            [E_SET_STATE]               = {S_CHOICE,                A_CHECK_STATE},
    [S_NOT_SELECTED]        [E_SET_STATE]               = {S_CHOICE,                A_CHECK_STATE},
    [S_CHOICE]              [E_CALL_WAITING_CONNECTION] = {S_WAITING_CONNECTION,    A_NOTIFY_WAITING_CONNECTION},
    [S_CHOICE]              [E_CALL_SELECTED]           = {S_SELECTED,              A_NOTIFY_SELECTED},
    [S_CHOICE]              [E_CALL_NOT_SELECTED]       = {S_NOT_SELECTED,          A_NOTIFY_NOT_SELECTED},
    [S_CHOICE]              [E_CALL_EMERGENCY]          = {S_EMERGENCY,             A_START_EMERGENCY},
    [S_EMERGENCY]           [E_TIME_OUT_EMERGENCY]      = {S_SELECTED,              A_STOP_EMERGENCY},
    [S_SELECTED]            [E_ENABLE_LED]              = {S_SELECTED,              A_ACTIVATE_LED},
    [S_SELECTED]            [E_DISABLE_LED]             = {S_SELECTED,              A_DEACTIVATE_LED},
    [S_SELECTED]            [E_ENABLE_BUZZER]           = {S_SELECTED,              A_ACTIVATE_BUZZER},
    [S_SELECTED]            [E_DISABLE_BUZZER]          = {S_SELECTED,              A_DEACTIVATE_BUZZER},
    [S_WAITING_CONNECTION]  [E_STOP]                    = {S_DEATH,                 A_STOP},
    [S_EMERGENCY]           [E_STOP]                    = {S_DEATH,                 A_STOP},
    [S_SELECTED]            [E_STOP]                    = {S_DEATH,                 A_STOP},
    [S_NOT_SELECTED]        [E_STOP]                    = {S_DEATH,                 A_STOP},
    [S_CHOICE]              [E_STOP]                    = {S_DEATH,                 A_STOP},
};
/**
 * \var static const Action_Pt actions_tab[ACTION_NB]
 * \brief Array of function pointer to call from action to perform.
 */
static const action_ptr actions_tab[ACTION_NB] = {
    &STATE_INDICATOR_action_nop,
    &STATE_INDICATOR_action_flashing_for_connection,
    &STATE_INDICATOR_action_notify_selected,
    &STATE_INDICATOR_action_notify_not_selected,
    &STATE_INDICATOR_action_start_notify_emergency,
    &STATE_INDICATOR_action_stop_notify_emergency,
    &STATE_INDICATOR_action_check_state,
    &STATE_INDICATOR_action_activate_led,
    &STATE_INDICATOR_action_deactivate_led,
    &STATE_INDICATOR_action_activate_buzzer,
    &STATE_INDICATOR_action_deactivate_buzzer,
    &STATE_INDICATOR_action_stop
};
/**
 * \var led_activated
 * \brief Current state of the led peripheral
 */
static bool_e led_activated = TRUE;
/**
 * \var buzzer_activated
 * \brief Current state of the buzzer peripheral
 */
static bool_e buzzer_activated = TRUE;
/**
 * \var state
 * \brief State of the system
 */
static State current_state;

/**
 * \var mae_state
 * \brief State machine state.
 */
static mae_state_e mae_state;
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
extern int STATE_INDICATOR_create(void) {
    state_indicator_emergency_watchdog = watchdog_create(EMERGENCY_TIME_OUT, STATE_INDICATOR_emergency_time_out);
    if(LEDS_create() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On LEDS_create(): state indicator failed to create the leds.");
        return -1;
    }
    BUZZER_create();
    
    struct mq_attr mq_a = {
            .mq_maxmsg = MQ_MSG_COUNT,
            .mq_msgsize = sizeof(mq_msg),
            .mq_flags = 0,
    };
    if((state_indicator_message_queue = mq_open(NAME_MQ_BOX, O_CREAT | O_RDWR | O_EXCL , 0644 ,&mq_a )) == -1 ) {
        if(errno == EEXIST) {
            mq_unlink(NAME_MQ_BOX);
            if((state_indicator_message_queue = mq_open(NAME_MQ_BOX, O_CREAT | O_RDWR , 0644 ,&mq_a )) == -1 ) {
                CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for state indicator after unlinking the mq.");
                goto error_mq;
            }
        } else {
            CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for state indicator.");
            goto error_mq;
        }
    }
    return 0;

    error_mq:
    if(LEDS_destroy() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On LEDS_destroy(): state indicator failed to destroy leds.");
    }
    return -1; 
}

extern int STATE_INDICATOR_destroy(void) {
    int ret = 0;
    if(mq_close(state_indicator_message_queue) == -1)
    {
        CONTROLLER_LOGGER_log(ERROR, "On mq_close(): error while closing the mq of state indicator.");
        ret = -1;
    }
    if(mq_unlink(NAME_MQ_BOX) == -1)
    {
        CONTROLLER_LOGGER_log(ERROR, "On mq_unlink(): error while unlinking the mq of state indicator.");
        ret = -1;
    }
    
    watchdog_destroy(state_indicator_emergency_watchdog);

    if(LEDS_destroy() != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On LEDS_destroy(): leds creation failed.");
        ret = -1;
    }
    BUZZER_destroy();
    return ret;
}

extern int STATE_INDICATOR_start(void) {
    if(LEDS_start() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On LEDS_start(): state indicator failed to start the leds.");
        return -1;
    }
    if(pthread_create(&state_indicator_thread, NULL, STATE_INDICATOR_run, NULL) != 0 ) {
        CONTROLLER_LOGGER_log(ERROR, "On pthread_create() : error while creating state indicator thread.");
        if(LEDS_stop() == -1) {
            CONTROLLER_LOGGER_log(ERROR, "On LEDS_stop(): state indicator failed to stop the leds.");
            return -1;
        }
        return -1;
    }
    return 0;
}

extern int STATE_INDICATOR_stop(void) {
    int ret = 0;
    mq_msg msg = {.data.event = E_STOP, 0};
    if(STATE_INDICATOR_add_msg_to_queue(&msg) == 0) {
        if(pthread_join(state_indicator_thread, NULL) != 0) {
            CONTROLLER_LOGGER_log(ERROR, "On pthread_join(): error while waiting the termination of state indicator thread.");
            ret = -1;
        }
    }
    else {
        ret = -1;
    }
    if(LEDS_stop() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On LEDS_stop(): state indicator failed to stop the leds.");
        ret = -1;
    }

    return ret;
}

extern int STATE_INDICATOR_set_state(State state) {
    mq_msg msg = {.data.event = 0, .data.state = 0};
    msg.data.event = E_SET_STATE;
    msg.data.state = state;
        if(STATE_INDICATOR_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}

extern int STATE_INDICATOR_enable_led(void) {
    mq_msg msg = {.data.event = 0, .data.state = 0};
    msg.data.event = E_ENABLE_LED;
    if(STATE_INDICATOR_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}

extern int STATE_INDICATOR_disable_led(void) {
    mq_msg msg = {.data.event = 0, .data.state = 0};
    msg.data.event = E_DISABLE_LED;
    if(STATE_INDICATOR_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}

extern int STATE_INDICATOR_enable_buzzer(void) {
    mq_msg msg = {.data.event = 0, .data.state = 0};
    msg.data.event = E_ENABLE_BUZZER;
    if(STATE_INDICATOR_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}

extern int STATE_INDICATOR_disable_buzzer(void) {
    mq_msg msg = {.data.event = 0, .data.state = 0};
    msg.data.event = E_DISABLE_BUZZER;
    if(STATE_INDICATOR_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}

/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static int STATE_INDICATOR_get_msg_from_queue(mq_msg* msg) {
    if(mq_receive(state_indicator_message_queue, msg->buffer, sizeof(mq_msg), NULL) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_receive(): STATE INDICATOR has failed to receive a message on the mq.");
        return -1;
    }
    return 0;
}

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int STATE_INDICATOR_add_msg_to_queue(mq_msg* msg) {
    if(mq_send(state_indicator_message_queue, msg->buffer, sizeof(mq_msg), 0) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_send(): STATE INDICATOR has failed to send a message on the mq.");
        return -1;
    }
    return 0;
}
#else
int STATE_INDICATOR_add_msg_to_queue(mq_msg* msg);
#endif

static void* STATE_INDICATOR_run(void * param) {
    mq_msg msg;
    mae_state = S_WAITING_CONNECTION;
    if(STATE_INDICATOR_action_flashing_for_connection(&msg) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On actions_tab() : failed to execute the first action for STATE INDICATOR.");
        return NULL;
    }

    transition_t * current_transition;
    while (mae_state != S_DEATH) {
        if(STATE_INDICATOR_get_msg_from_queue(&msg) == -1) {
            return NULL;
        }
        current_transition = &state_indicator_state_machine[mae_state][msg.data.event];
        if (current_transition->dest_state != S_FORGET) {
            if (actions_tab[current_transition->action](&msg) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On actions_tab() : failed to execute the action for STATE INDICATOR.");
                return NULL;
            }
            mae_state = current_transition->dest_state;
        }
    }
    return NULL;
}

static int STATE_INDICATOR_action_nop(mq_msg * msg) {
    return 0;
}

static int STATE_INDICATOR_action_flashing_for_connection(mq_msg * msg) {
    if(led_activated)
    {
        if(LEDS_set_color(ALL_LEDS, GREEN) == -1) {
            CONTROLLER_LOGGER_log(ERROR, "On LEDS_set_color() : STATE INDICATOR failed to set the color of the LED.");
            return -1;
        }
        if(LEDS_start_blinking() == -1) {
            CONTROLLER_LOGGER_log(ERROR, "On LEDS_start_blinking() : STATE INDICATOR failed to start blinking the LED.");
            return -1;
        }
    }
    CONTROLLER_LOGGER_log(INFO, "STATE INDICATOR : the robot is in waiting for connection state.");
    return 0;
}

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int STATE_INDICATOR_action_notify_selected(mq_msg * msg) {
    if(led_activated)
    {
        if(LEDS_set_color(ALL_LEDS, BLUE)) {
            CONTROLLER_LOGGER_log(ERROR, "On LEDS_set_color() : STATE INDICATOR failed to set the color of the LED.");
            return -1;
        }
    }
    else {
        if(LEDS_set_color(ALL_LEDS, NO_COLOR)) {
            CONTROLLER_LOGGER_log(ERROR, "On LEDS_set_color() : STATE INDICATOR failed to set the color of the LED.");
            return -1;
        }
    }
    CONTROLLER_LOGGER_log(INFO, "STATE INDICATOR : the robot is in selected state.");
    return 0;
}
#else
static int STATE_INDICATOR_action_notify_selected(mq_msg * msg);
#endif

static int STATE_INDICATOR_action_notify_not_selected(mq_msg * msg) {
    if(led_activated)
    {
        if(LEDS_set_color(ALL_LEDS, GREEN) == -1)
        {
            CONTROLLER_LOGGER_log(ERROR, "On LEDS_set_color() : STATE INDICATOR failed to set the color of the LED.");
            return -1;
        }
    }
    CONTROLLER_LOGGER_log(INFO, "STATE INDICATOR : the robot is in not selected state.");
    return 0;
}


static int STATE_INDICATOR_action_start_notify_emergency(mq_msg * msg) {
    int ret = 0;

    CONTROLLER_LOGGER_log(INFO, "STATE INDICATOR : the robot is in emergency state.");

    if(led_activated) {
        if(LEDS_set_color(ALL_LEDS, RED) == -1) {
            CONTROLLER_LOGGER_log(ERROR, "On LED_set_color() : STATE INDICATOR failed to set the color of the LED.");
            ret = -1;
        }
        if(LEDS_start_blinking() == -1) {
            CONTROLLER_LOGGER_log(ERROR, "On LEDS_start_blinking() : STATE INDICATOR failed to start the blinking.");
            ret = -1;
        }
    }
    
    if(buzzer_activated) {
        BUZZER_enable_buzzer();
    }

    watchdog_start(state_indicator_emergency_watchdog);

    return ret;
}

static int STATE_INDICATOR_action_stop_notify_emergency(mq_msg * msg) {

    BUZZER_disable_buzzer();

    if(STATE_INDICATOR_action_notify_selected(msg) == -1)
    {
        return -1;
    }
    return 0;
}

static int STATE_INDICATOR_action_check_state(mq_msg * msg) {
    current_state = msg->data.state;

    mq_msg next_event = {.data.event = 0, .data.state = 0};

    switch (current_state)
    {
    case WAITING_FOR_CONNECTION:
        next_event.data.event = E_CALL_WAITING_CONNECTION;
        break;

    case EMERGENCY:
        if(mae_state == S_SELECTED)
        {
            next_event.data.event = E_CALL_EMERGENCY;
        }
        break;

    case SELECTED:
        if(mae_state != S_WAITING_CONNECTION)
        {
            next_event.data.event = E_CALL_SELECTED;
        }
        break;

    case NOT_SELECTED:
        next_event.data.event = E_CALL_NOT_SELECTED;
        break;
    default:
        CONTROLLER_LOGGER_log(WARNING, "In STATE_INDICATOR_action_check_state: current_state is in unknown state.");
        break;
    }

    /* Avoid timer to continue running after changing state */
    if(mae_state == S_EMERGENCY) {
        watchdog_cancel(state_indicator_emergency_watchdog);
    }

    if(STATE_INDICATOR_add_msg_to_queue(&next_event) == -1) {
        return -1;
    }
    return 0;
}

static int STATE_INDICATOR_action_activate_led(mq_msg * msg) {
    led_activated = TRUE;
    CONTROLLER_LOGGER_log(INFO, "STATE INDICATOR : LEDs as been activated.");
    mq_msg dummy_msg = {.data.event = 0, .data.state = 0};
    if(STATE_INDICATOR_action_notify_selected(&dummy_msg) == -1) {
        return -1;
    }
    return 0;
}

static int STATE_INDICATOR_action_deactivate_led(mq_msg * msg) {
    led_activated = FALSE;
    CONTROLLER_LOGGER_log(INFO, "STATE INDICATOR : LEDs as been deactivated.");
    mq_msg dummy_msg = {.data.event = 0, .data.state = 0};
    if(STATE_INDICATOR_action_notify_selected(&dummy_msg) == -1) {
        return -1;
    }
    return 0;
}

static int STATE_INDICATOR_action_activate_buzzer(mq_msg * msg) {
    buzzer_activated = TRUE;
    CONTROLLER_LOGGER_log(INFO, "STATE INDICATOR : Buzzer as been activated.");
    mq_msg dummy_msg = {.data.event = 0, .data.state = 0};
    if(STATE_INDICATOR_action_notify_selected(&dummy_msg) == -1) {
        return -1;
    }
    return 0;
}

static int STATE_INDICATOR_action_deactivate_buzzer(mq_msg * msg) {
    buzzer_activated = FALSE;
    CONTROLLER_LOGGER_log(INFO, "STATE INDICATOR : Buzzer as been deactivated.");
    mq_msg dummy_msg = {.data.event = 0, .data.state = 0};
    if(STATE_INDICATOR_action_notify_selected(&dummy_msg) == -1) {
        return -1;
    }
    return 0;
}

static int STATE_INDICATOR_action_stop(mq_msg * msg) {
    if(LEDS_set_color(ALL_LEDS, NO_COLOR) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On LEDS_set_color() : STATE INDICATOR failed to set the color of the LED.");
        return -1;
    }
    return 0;
}

static void STATE_INDICATOR_emergency_time_out(watchdog_t * watchdog)
{
    mq_msg msg = {.data.event = E_TIME_OUT_EMERGENCY, 0};
    if(STATE_INDICATOR_add_msg_to_queue(&msg) != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On STATE_INDICATOR_add_msg_to_queue() : STATE_INDICATOR_emergency_time_out callback failed to send a message to the mq.");
    }
}
