/**
 * \file  controller_core.c
 * \version  1.1
 * \author Joshua MONTREUIL
 * \date Apr 29, 2023
 * \brief source file of the "core" module of the robot. This module manages the
 * operating mode of its peripherals and indicates to its sub-modules its own state.
 * It also establish the connection between SB_C and SB_IHM.
 *
 * \see controller_core.h
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
#include "controller_core.h"
#include <pthread.h>
#include <mqueue.h>
#include <errno.h>

#include "state_indicator.h"
#include "controller_ringer.h"
#include "pilot.h"
#include "../com/postman.h"
#include "../com/gui_secretary_proxy.h"
#include "../com/dispatcher.h"
#include "../alphabot2/camera.h"
#include "../alphabot2/servo_motor.h"
#include "../logs/controller_logger.h"
#include "../lib/watchdog.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
#define STATE_GENERATION S(S_FORGET) S(S_ON_DISCONNECTED) S(S_ON_CONNECTED_WAITING_ACTION) S(S_ON_CONNECTED_CHOICE) S(S_DEATH)
#define S(x) x,
typedef enum {STATE_GENERATION STATE_NB} State_Machine;
#undef STATE_GENERATION
#undef S

#define ACTION_GENERATION A(A_NOP) A(A_CONNECTION) A(A_SET_MODE) A(A_APPLY_MODE) A(A_SET_ROBOT_STATE) A(A_EVAL_ROBOT_STATE) A(A_DISCONNECT_OK) A(A_DISCONNECT) A(A_INIT) A(A_STOP)
#define A(x) x,
typedef enum {ACTION_GENERATION ACTION_NB} Action;
#undef ACTION_GENERATION
#undef A

#define EVENT_GENERATION E(E_ASK_TO_CONNECT) E(E_ASK_MODE) E(E_ASK_SET_MODE) E(E_ASK_SET_ROBOT_STATE) E(E_ROBOT_STATE_EVALUATED) E(E_DISCONNECTION) E(E_CONNECTION_LOST) E(E_STOP)
#define E(x) x,
typedef enum {EVENT_GENERATION EVENT_NB} Event;
#undef EVENT_GENERATION
#undef E

/**
 * \def MQ_CONTROLLER_CORE_BOX_NAME
 * Name of the message queue
 */
#define MQ_CONTROLLER_CORE_BOX_NAME "/mb_controller_core"
/**
 * \def MQ_MSG_COUNT
 * Max amount of message into the message queue.
 */
#define MQ_MSG_COUNT 10
/**
 * \def SERVO_X_POSITION_INIT
 * Init position of the servo.
 */
#define SERVO_X_POSITION_INIT 144.0f
/**
 * \def SERVO_Y_POSITION_INIT
 * Init position of the servo.
 */
#define SERVO_Y_POSITION_INIT 36.0f
/**
 * \def SERVO_PERIOD_UNTIL_SLEEP
 * Period until deactivation of the servo.
 */
#define SERVO_PERIOD_UNTIL_SLEEP 500
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
 * \struct Action_Param_Data controller_core.c "controller/controller_core.c"
 * \brief Definition of the Action_Param_Data type.
 *
 * Action_Param_Data contains different parameters that can be used between two states transitions such as the robot identifier, the robot state and the peripheral operating modes.
 */
typedef struct {
    Id_Robot id_robot; /**< Robot identifier. */
    State robot_state; /**< Robot state. */
    Operating_Mode operating_mode; /**< Peripherals operating modes. */
} Action_Param_Data;
/**
* \struct Mq_Msg_Data controller_core.c "controller/controller_core.c"
* \brief Definition of the Mq_Msg_Data type.
*
* Mq_Msg_Data contains an event for the state machine. And some parameter used in state machine transitions.
*/
typedef struct {
    Event event; /**< Event to change the state of the state machine. */
    Action_Param_Data action_data; /**< Action data paramater used during transitions. */
} Mq_Msg_Data;
/**
 * \union Mq_Msg controller_core.c "controller/controller_core.c"
 * \brief Message queue type as an union.
 *
 * This union allows to transform a buffer into the Mq_Msg_Data structure.
 */
typedef union {
    Mq_Msg_Data msg_data; /**< Data structure. */
    char buffer[sizeof(Mq_Msg_Data)]; /**< Raw message. */
} Mq_Msg;
/**
 * \struct Transition controller_core.c "controller/controller_core.c"
 * \brief Gives an action and state destination for a transition.
 *
 * Transition gives the action to perform when an event has been fired. Also gives the destination state.
 */
typedef struct {
    State_Machine state_destination; /**< Next state. */
    Action action; /**< Action to perform from previous the event. */
} Transition;
/**
 * \typedef void(*Action_Pt)(Action_Param_Data * action_parameters)
 * \brief Definition of function pointer for the actions to perform.
 */
typedef int(*Action_Pt)(Action_Param_Data * action_parameters);
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/* ----- PASSIVES ----- */
/**
 * \fn static int CONTROLLER_CORE_disconnect_core(void)
 * \brief Disconnects the module from the network.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_disconnect_core(void);
/**
 * \fn static void CONTROLLER_CORE_set_operating_mode(Operating_Mode operating_mode)
 * \brief Changes the internal operating mode variable.
 * \author Joshua MONTREUIL
 *
 * \param operating_mode : operating mode to be set.
 * \see Operating_Mode
 */
static void CONTROLLER_CORE_set_operating_mode(Operating_Mode operating_mode);
/**
 * \fn static int CONTROLLER_CORE_apply_edit_mode(Operating_Mode operating_mode)
 * \brief Changes the peripherals' operating modes.
 * \author Joshua MONTREUIL
 *
 * \param operating_mode : operating mode that the peripherals need to be changes into.
 * \see Operating_Mode
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_apply_edit_mode(Operating_Mode operating_mode);
/**
 * \fn static void CONTROLLER_CORE_init_hardware(void)
 * \brief Inits the hardware of the alphabot2. (servo)
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
static void CONTROLLER_CORE_init_hardware(void);
/* ----- ACTIVE ----- */
/**
 * \fn static int CONTROLLER_CORE_mq_receive(Mq_Msg * a_msg)
 * \brief Receives the messages from the queue.
 * \author Joshua MONTREUIL
 *
 * \param a_msg : pointer to Mq_Msg struct.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_mq_receive(Mq_Msg * a_msg);
/**
 * \fn static int CONTROLLER_CORE_mq_send(Mq_Msg * a_msg)
 * \brief Sends a message into the queue.
 * \author Joshua MONTREUIL
 *
 * \param a_msg : pointer to Mq_Msg struct.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_mq_send(Mq_Msg * a_msg);
/**
 * \fn static void * CONTROLLER_CORE_run(void * arg)
 * \brief Called by a thread. This function is the "active" part of controller core.
 * \author Joshua MONTREUIL
 *
 * \param arg : argument pointer.
 *
 * \return void * : generic pointer.
 */
static void * CONTROLLER_CORE_run(void* arg);
/**
 * \fn static void CONTROLLER_CORE_stop_servo_motor(watchdog_t * watchdog)
 * \brief callback : stops the servo-motor pwm.
 * \author Joshua MOONTREUIL
 *
 * \param watchdog : watchdog used to trigger the radar check
 */
static void CONTROLLER_CORE_stop_servo_motor(watchdog_t * watchdog);
/* ----- ACTIONS ----- */
/**
 * \fn static int CONTROLLER_CORE_action_nop(Action_Param_Data * action_parameters)
 * \brief For "out of state" transitions.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_nop(Action_Param_Data * action_parameters);
/**
 * \fn static itn CONTROLLER_CORE_action_connection(Action_Param_Data * action_parameters)
 * \brief Performs actions related to the "E_ASK_TO_CONNECT" event.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_connection(Action_Param_Data * action_parameters);
/**
 * \fn static int CONTROLLER_CORE_action_set_mode(Action_Param_Data * action_parameters)
 * \brief Performs actions related to the "E_SET_MODE" event.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_set_mode(Action_Param_Data * action_parameters);
/**
 * \fn static int CONTROLLER_CORE_action_apply_mode(Action_Param_Data * action_parameters)
 * \brief Performs actions related to the "E_ASK_SET_MODE" event.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_apply_mode(Action_Param_Data * action_parameters);
/**
 * \fn static int CONTROLLER_CORE_action_set_robot_state(Action_Param_Data * action_parameters)
 * \brief Performs actions related to the "E_ROBOT_STATE_EVALUATED" event.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_set_robot_state(Action_Param_Data * action_parameters);
/**
 * \fn static int CONTROLLER_CORE_action_evaluate_robot_state(Action_Param_Data * action_parameters)
 * \brief Performs actions related to the "E_ASK_SET_ROBOT_STATE" event.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_evaluate_robot_state(Action_Param_Data * action_parameters);
/**
 * \fn static int CONTROLLER_CORE_action_disconnect_ok(Action_Param_Data * action_parameters)
 * \brief Performs actions related to the "E_DISCONNECTION" event.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_disconnect_ok(Action_Param_Data * action_parameters);
/**
 * \fn static int CONTROLLER_CORE_action_disconnect(Action_Param_Data * action_parameters)
 * \brief Performs actions related to the "E_CONNECTION_LOST" event.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_disconnect(Action_Param_Data * action_parameters);
/**
 * \fn static int CONTROLLER_CORE_action_init(Action_Param_Data * action_parameters)
 * \brief Performs actions at the start of the SM.
 * \author Joshua MONTREUIL
 *
 * \param action_parameters : data struct with robot identifier, robot state and peripheral operating modes.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_CORE_action_init(Action_Param_Data * action_parameters);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/**
 * \var id_robot
 * \brief Robot's identifier.
 */
static Id_Robot robot_id = ID_ROBOT;

/**
 * \var operating_mode
 * \brief Operating Modes of the robot's peripherals.
 */
static Operating_Mode robot_operating_mode;
/**
 * \var robot_state
 * \brief State of the robot.
 */
static State robot_state;
/**
 * \var static pthread_t controller_core_thread
 * \brief Postman thread.
 */
static pthread_t controller_core_thread;
/**
 * \var static mqd_t my_mail_box
 * \brief Message queue reference.
 */
static mqd_t my_mail_box;
/**
 * \var static pthread_mutex_t controller_core_mutex_operating_mode
 * \brief Mutex used to safely read the operating mode
 */
static pthread_mutex_t controller_core_mutex_operating_mode = PTHREAD_MUTEX_INITIALIZER;
/**
 * \var static pthread_mutex_t controller_core_mutex_id_robot
 * \brief Mutex used to safely read the id_robot
 */
static pthread_mutex_t controller_core_mutex_id_robot = PTHREAD_MUTEX_INITIALIZER;
/**
 * \var controller_core_servo_motor_watchdog
 * \brief Used to wait until the servo is in position.
 */
watchdog_t * controller_core_servo_motor_watchdog;
/**
 * \var static const Action_Pt actions_tab[ACTION_NB]
 * \brief Array of function pointer to call from action to perform.
 */
static const Action_Pt actions_tab[ACTION_NB] = {
    &CONTROLLER_CORE_action_nop,
    &CONTROLLER_CORE_action_connection,
    &CONTROLLER_CORE_action_set_mode,
    &CONTROLLER_CORE_action_apply_mode,
    &CONTROLLER_CORE_action_set_robot_state,
    &CONTROLLER_CORE_action_evaluate_robot_state,
    &CONTROLLER_CORE_action_disconnect_ok,
    &CONTROLLER_CORE_action_disconnect,
    &CONTROLLER_CORE_action_init,
    &CONTROLLER_CORE_action_nop,
};
/**
 * \var static Transition my_state_machine [STATE_NB -1][EVENT_NB]
 * \brief Array representing the state machine.
 */
static Transition my_state_machine [STATE_NB -1][EVENT_NB] = {
    [S_ON_DISCONNECTED]             [E_STOP]                  = {S_DEATH,                       A_STOP},
    [S_ON_DISCONNECTED]             [E_ASK_TO_CONNECT]        = {S_ON_CONNECTED_WAITING_ACTION, A_CONNECTION},
    [S_ON_CONNECTED_CHOICE]         [E_STOP]                  = {S_DEATH,                       A_STOP},
    [S_ON_CONNECTED_CHOICE]         [E_ROBOT_STATE_EVALUATED] = {S_ON_CONNECTED_WAITING_ACTION, A_SET_ROBOT_STATE},
    [S_ON_CONNECTED_WAITING_ACTION] [E_STOP]                  = {S_DEATH,                       A_STOP},
    [S_ON_CONNECTED_WAITING_ACTION] [E_ASK_MODE]              = {S_ON_CONNECTED_WAITING_ACTION, A_SET_MODE},
    [S_ON_CONNECTED_WAITING_ACTION] [E_ASK_SET_MODE]          = {S_ON_CONNECTED_WAITING_ACTION, A_APPLY_MODE},
    [S_ON_CONNECTED_WAITING_ACTION] [E_DISCONNECTION]         = {S_ON_DISCONNECTED,             A_DISCONNECT_OK},
    [S_ON_CONNECTED_WAITING_ACTION] [E_CONNECTION_LOST]       = {S_ON_DISCONNECTED,             A_DISCONNECT},
    [S_ON_CONNECTED_WAITING_ACTION] [E_ASK_SET_ROBOT_STATE]   = {S_ON_CONNECTED_CHOICE,         A_EVAL_ROBOT_STATE},
};
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int CONTROLLER_CORE_create(void) {
    struct mq_attr mqa;
    mqa.mq_maxmsg = MQ_MSG_COUNT;
    mqa.mq_msgsize = sizeof(Mq_Msg);

    controller_core_servo_motor_watchdog = watchdog_create(SERVO_PERIOD_UNTIL_SLEEP,CONTROLLER_CORE_stop_servo_motor);

    if(SERVO_MOTOR_create() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On SERVO_MOTOR_create(): controller core failed to create the servo-motor.");
        return -1;
    }
    if(CAMERA_create() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CAMERA_create(): controller core failed to create the camera module.");
        goto error_camera;
    }
    errno = 0;
    if((my_mail_box = mq_open(MQ_CONTROLLER_CORE_BOX_NAME, O_CREAT | O_RDWR | O_EXCL , 0644 ,&mqa )) == -1 ) {
        if(errno == EEXIST) {
            mq_unlink(MQ_CONTROLLER_CORE_BOX_NAME);
            if((my_mail_box = mq_open(MQ_CONTROLLER_CORE_BOX_NAME, O_CREAT | O_RDWR , 0644 ,&mqa )) == -1 ) {
                CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for controller core after unlinking the mq.");
                goto error_mq;
            }
        } else {
            CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for controller core.");
            goto error_mq;
        }
    }
    robot_operating_mode.buzzer_mode = ENABLED;
    robot_operating_mode.radar_mode = ENABLED;
    robot_operating_mode.leds_mode = ENABLED;
    robot_operating_mode.camera_mode = ENABLED;
    return 0;

    error_mq:
        CAMERA_destroy();
    error_camera:
        SERVO_MOTOR_destroy();
        return -1;
}

int CONTROLLER_CORE_start(void) {
    if(CAMERA_start() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CAMERA_start(): controller core failed to start the camera module.");
        return -1;
    }
    if(pthread_create(&controller_core_thread, NULL, CONTROLLER_CORE_run, NULL) != 0 ) {
        CONTROLLER_LOGGER_log(ERROR, "On pthread_create() : error while creating controller_core thread.");
        CAMERA_stop();
        return -1;
    }
    return 0;
}

int CONTROLLER_CORE_ask_to_connect(Id_Robot id_robot) {
    Mq_Msg my_msg = {.msg_data.event = E_ASK_TO_CONNECT, {id_robot,0,{0,0,0,0}}};
    if(CONTROLLER_CORE_mq_send(&my_msg) == -1) {
        return -1;
    }
    return 0;
}

int CONTROLLER_CORE_ask_to_disconnect(Id_Robot id_robot) {
    Mq_Msg my_msg = {.msg_data.event = E_DISCONNECTION, {id_robot,0,{0,0,0,0}}};
    if(CONTROLLER_CORE_mq_send(&my_msg) == -1) {
        return -1;
    }
    return 0;
}

int CONTROLLER_CORE_ask_set_state(Id_Robot id_robot, State state) {
    Mq_Msg my_msg = {.msg_data.event = E_ASK_SET_ROBOT_STATE, {id_robot,state, {0,0,0,0}}};
    if(CONTROLLER_CORE_mq_send(&my_msg) == -1) {
        return -1;
    }
    return 0;
}

int CONTROLLER_CORE_ask_mode(Id_Robot id_robot) {
    Mq_Msg my_msg = {.msg_data.event = E_ASK_MODE, {id_robot,0,{0,0,0,0}}};
    if(CONTROLLER_CORE_mq_send(&my_msg) == -1) {
        return -1;
    }
    return 0;
}

int CONTROLLER_CORE_ask_set_mode(Id_Robot id_robot, Operating_Mode operating_mode) {
    Mq_Msg my_msg = {.msg_data.event = E_ASK_SET_MODE, {id_robot,0,operating_mode}};
    if(CONTROLLER_CORE_mq_send(&my_msg) == -1) {
        return -1;
    }
    return 0;
}

Id_Robot CONTROLLER_CORE_get_id_robot(void) {
    pthread_mutex_lock(&controller_core_mutex_id_robot);
    Id_Robot my_id_robot = robot_id;
    pthread_mutex_unlock(&controller_core_mutex_id_robot);
    return my_id_robot;
}

Operating_Mode CONTROLLER_CORE_get_mode(void) {
    pthread_mutex_lock(&controller_core_mutex_operating_mode);
    Operating_Mode my_operating_mode = robot_operating_mode;
    pthread_mutex_unlock(&controller_core_mutex_operating_mode);
    return my_operating_mode;
}

int CONTROLLER_CORE_connection_lost(void) {
    Mq_Msg my_msg = {.msg_data.event = E_CONNECTION_LOST, {0,0,{0,0,0,0}}};
    if(CONTROLLER_CORE_mq_send(&my_msg) == -1) {
        return -1;
    }
    return 0;
}

int CONTROLLER_CORE_stop(void) {
    int ret = 0;
    if(CAMERA_stop() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CAMERA_stop(): controller core failed to stop the camera module.");
        ret = -1;
    }
    Mq_Msg my_msg = {.msg_data.event = E_STOP};
    if(CONTROLLER_CORE_mq_send(&my_msg) == 0) {
        if(pthread_join(controller_core_thread, NULL) != 0) {
            CONTROLLER_LOGGER_log(ERROR, "On pthread_join(): error while waiting the termination of controller core thread.");
            ret = -1;
        }
    }
    else {
        CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_CORE_mq_send() : Putting a request into controller_core's mq has failed.");
        ret = -1;
    }
    return ret;
}

int CONTROLLER_CORE_destroy(void) {
    int ret = 0;
    if(CAMERA_destroy() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CAMERA_destroy(): controller core failed to destroy the camera module.");
        ret = -1;
    }
    if(SERVO_MOTOR_destroy() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On SERVO_MOTOR_destroy(): error while destroying the servo-motor.");
        ret = -1;
    }
    if(mq_close(my_mail_box) == -1 ) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_close(): error while closing the mq of controller core.");
        ret = -1;
    }
    if(mq_unlink(MQ_CONTROLLER_CORE_BOX_NAME) == -1 ) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_unlink(): error while unlinking the mq of controller core.");
        ret = -1;
    }
    watchdog_destroy(controller_core_servo_motor_watchdog);
    return ret;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
/* ACTIVE */
static void * CONTROLLER_CORE_run(void* arg) {
    Mq_Msg msg;
    State_Machine my_state = S_ON_DISCONNECTED;
    Transition * my_transition;
    if(actions_tab[A_INIT](NULL) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On actions_tab() : failed to execute the action for controller core.");
        return NULL;
    }
    while(my_state != S_DEATH) {
        if(CONTROLLER_CORE_mq_receive(&msg) == -1) {
            CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_CORE_mq_receive() : failed to read controller_core's mq.");
            return NULL;
        }
        my_transition = &my_state_machine[my_state][msg.msg_data.event];
        if(my_transition->state_destination != S_FORGET) {
            if(actions_tab[my_transition->action](&msg.msg_data.action_data) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On actions_tab() : failed to execute the action for postman.");
                return NULL;
            }
            my_state = my_transition->state_destination;
        }
    }
    return 0;
}

static int CONTROLLER_CORE_mq_receive(Mq_Msg * a_msg) {
    if((mq_receive(my_mail_box,a_msg->buffer,sizeof(Mq_Msg), 0) == -1)) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_receive() : Controller Core has failed to receive a message on the mq.");
        mq_close(my_mail_box);
        mq_unlink(MQ_CONTROLLER_CORE_BOX_NAME);
        return -1;
    }
    return 0;
}

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int CONTROLLER_CORE_mq_send(Mq_Msg * a_msg) {
    if(mq_send(my_mail_box,a_msg->buffer, sizeof(Mq_Msg),0) == -1 ) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_send() : Controller Core has failed to send a message into the mq.");
        mq_close(my_mail_box);
        mq_unlink(MQ_CONTROLLER_CORE_BOX_NAME);
        return -1;
    }
    return 0;
}
#else
int CONTROLLER_CORE_mq_send(Mq_Msg * a_msg);
#endif

static void CONTROLLER_CORE_stop_servo_motor(watchdog_t * watchdog) {
    SERVO_MOTOR_disable_servo_motor(); /* Then disable the servo-motor */
}

/* ACTION TRANSITIONS */
static int CONTROLLER_CORE_action_nop(Action_Param_Data * action_parameters) { return 0; }

static int CONTROLLER_CORE_action_connection(Action_Param_Data * action_parameters) {
    DISPATCHER_start_reading();
    GUI_SECRETARY_PROXY_ack_connection(action_parameters->id_robot);
    if(CONTROLLER_RINGER_init_failed_pings_var() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_RINGER_init_failed_pings_var() : Failed to put a msg into controller_ringer's mq.");
        return -1;
    }
    robot_state = NOT_SELECTED;
    if(STATE_INDICATOR_set_state(robot_state) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On STATE_INDICATOR_set_state() : Putting a msg into state_indicator's mq has failed for controller_core.");
        return -1;
    }
    return 0;
}

static int CONTROLLER_CORE_action_set_mode(Action_Param_Data * action_parameters) {
    if(GUI_SECRETARY_PROXY_set_mode(action_parameters->id_robot, robot_operating_mode) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On GUI_SECRETARY_PROXY_set_mode() : Failed to put a msg into postman's mq.");
        return -1;
    }
    return 0;
}

static int CONTROLLER_CORE_action_apply_mode(Action_Param_Data * action_parameters) {
    CONTROLLER_CORE_set_operating_mode(action_parameters->operating_mode);
    if(CONTROLLER_CORE_apply_edit_mode(robot_operating_mode) == -1) {
        return -1;
    }
    return 0;
}

static int CONTROLLER_CORE_action_evaluate_robot_state(Action_Param_Data * action_parameters) {
    Mq_Msg my_msg = {.msg_data.event = E_ROBOT_STATE_EVALUATED, .msg_data.action_data.robot_state = action_parameters->robot_state};
    if(CONTROLLER_CORE_mq_send(&my_msg) == -1) {
        return -1;
    }
    return 0;
}

static int CONTROLLER_CORE_action_set_robot_state(Action_Param_Data * action_parameters) {
    if(STATE_INDICATOR_set_state(action_parameters->robot_state) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On STATE_INDICATOR_set_state() : Putting a msg into state_indicator's mq has failed for controller_core.");
        return -1;
    }
    return 0;
}

static int CONTROLLER_CORE_action_disconnect_ok(Action_Param_Data * action_parameters) {
    if(GUI_SECRETARY_PROXY_disconnected_ok(action_parameters->id_robot) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On GUI_SECRETARY_PROXY_disconnected_ok() : The request to gui secretary proxy has failed.");
        return -1;
    }
    if(CONTROLLER_CORE_disconnect_core() == -1) {
        return -1;
    }
    if(PILOT_ask_cmd(STOP) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On PILOT_ask_cmd() : Putting a msg into pilot's mq has failed for controller_core.");
        return -1;
    }
    if(CAMERA_disconnect_camera() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On  CAMERA_disconnect_camera() : Putting a msg into camera's mq has failed for controller_core.");
        return -1;
    }   
    CONTROLLER_LOGGER_log(INFO,"Controller_core has been disconnected.");
    robot_state = WAITING_FOR_CONNECTION;
    if(STATE_INDICATOR_set_state(robot_state) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On STATE_INDICATOR_set_state() : Putting a msg into state_indicator's mq has failed for controller_core.");
        return -1;
    }
    return 0;
}

static int CONTROLLER_CORE_action_disconnect(Action_Param_Data * action_parameters) {
    if(CONTROLLER_CORE_disconnect_core() == -1) {
        return -1;
    }
    if(PILOT_ask_cmd(STOP) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On PILOT_ask_cmd() : Putting a msg into pilot's mq has failed for controller_core.");
        return -1;
    }
    if(CAMERA_disconnect_camera() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On  CAMERA_disconnect_camera() : Putting a msg into camera's mq has failed for controller_core.");
        return -1;
    }   
    CONTROLLER_LOGGER_log(INFO,"Controller_core has been disconnected.");
    robot_state = WAITING_FOR_CONNECTION;
    if(STATE_INDICATOR_set_state(robot_state) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On STATE_INDICATOR_set_state() : Putting a msg into state_indicator's mq has failed for controller_core.");
        return -1;
    }
    return 0;
}

static int CONTROLLER_CORE_action_init(Action_Param_Data * action_parameters) {
    CONTROLLER_CORE_init_hardware();
    robot_state = WAITING_FOR_CONNECTION;
    if(STATE_INDICATOR_set_state(robot_state) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On STATE_INDICATOR_set_state() : Putting a msg into state_indicator's mq has failed for controller_core.");
        return -1;
    }
    return 0;
}

/* INTERNAL ACTIONS */
#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int CONTROLLER_CORE_disconnect_core(void) {
    if(POSTMAN_disconnect() == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On POSTMAN_disconnect() : controller core has failed to disconnect Postman.");
        return -1;
    }
    return 0;
}
#else
static int CONTROLLER_CORE_disconnect_core(void);
#endif

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static void CONTROLLER_CORE_set_operating_mode(Operating_Mode operating_mode) {
    pthread_mutex_lock(&controller_core_mutex_operating_mode);
    robot_operating_mode = operating_mode;
    pthread_mutex_unlock(&controller_core_mutex_operating_mode);
}
#else
static void CONTROLLER_CORE_set_operating_mode(Operating_Mode operating_mode);
#endif

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
static int CONTROLLER_CORE_apply_edit_mode(Operating_Mode operating_mode) {
    if(operating_mode.camera_mode) {
        if(CAMERA_enable_camera() == -1) {
            CONTROLLER_LOGGER_log(ERROR,"On CAMERA_enable_camera() : controller core failed to enable the camera.");
            return -1;
        }
    }
    else {
        if(CAMERA_disable_camera() == -1) {
            CONTROLLER_LOGGER_log(ERROR,"On CAMERA_enable_camera() : controller core failed to disable the camera.");
            return -1;
        }
    }
    if(operating_mode.leds_mode) {
        if(STATE_INDICATOR_enable_led() == -1) {
            CONTROLLER_LOGGER_log(ERROR,"On CAMERA_enable_camera() : controller core failed to enable the leds.");
            return -1;
        }
    }
    else {
        if(STATE_INDICATOR_disable_led() == -1) {
            CONTROLLER_LOGGER_log(ERROR,"On CAMERA_enable_camera() : controller core failed to disable the leds.");
            return -1;
        }
    }
    if(operating_mode.buzzer_mode) {
        if(STATE_INDICATOR_enable_buzzer() == -1) {
            CONTROLLER_LOGGER_log(ERROR,"On CAMERA_enable_camera() : controller core failed to enable the buzzer.");
            return -1;
        }
    }
    else {
        if(STATE_INDICATOR_disable_buzzer() == -1) {
            CONTROLLER_LOGGER_log(ERROR,"On CAMERA_enable_camera() : controller core failed to disable the buzzer.");
            return -1;
        }
    }
    return 0;
}
#else
static int CONTROLLER_CORE_apply_edit_mode(Operating_Mode operating_mode);
#endif

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
void CONTROLLER_CORE_init_hardware(void) {
    SERVO_MOTOR_set_position(SERVO2_ADDRESS,SERVO_X_POSITION_INIT);
    SERVO_MOTOR_set_position(SERVO1_ADDRESS,SERVO_Y_POSITION_INIT);
    watchdog_start(controller_core_servo_motor_watchdog);
}
#else
void CONTROLLER_CORE_init_hardware(void); 
#endif

