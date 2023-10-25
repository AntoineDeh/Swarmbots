/**
 * \file  controller_logger.c
 * \version  1.1
 * \author Florentin LEPELTIER
 * \author Joshua MONTREUIL
 * \date Apr 29, 2023
 * \brief Source file of the robot's logger module. It manage the log file and save the logs inside it.
 *
 * \see controller_logger.h
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
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#include "controller_logger.h"
#include "../com/gui_proxy.h"
#include "../com/logs_manager_proxy.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
#define STATE_GENERATION S(S_FORGET) S(S_IDLE) S(S_WAITING_ACTION) S(S_CHOICE) S(S_MEMORY_FULL) S(S_FLUSHING) S(S_DEATH)
#define S(x) x,
typedef enum {STATE_GENERATION STATE_NB} State_Machine;
#undef STATE_GENERATION
#undef S

#define ACTION_GENERATION A(A_NOP) A(A_SETUP_RTC_SAVE_TEMP_LOGS) A(A_MEMORY_CHECK) A(A_RAISE_MEMORY_ALERT) A(A_SAVE_LOGS) A(A_REMEMBER_LOGS) A(A_LOAD_LOGS) A(A_REMOVE_LOGS) A(A_STOP)
#define A(x) x,
typedef enum {ACTION_GENERATION ACTION_NB} Action;
#undef ACTION_GENERATION
#undef A

#define EVENT_GENERATION E(E_ASK_SET_RTC) E(E_LOG) E(E_MEMORY_OK) E(E_MEMORY_ALERT) E(E_MEMORY_CRITICAL) E(E_ASK_LOGS) E(E_LOGS_SAVED) E(E_STOP)
#define E(x) x,
typedef enum {EVENT_GENERATION EVENT_NB} Event;
#undef EVENT_GENERATION
#undef E

/**
 * \def MQ_CONTROLLER_LOGGER_BOX_NAME
 * Name of the message queue
 */
#define MQ_CONTROLLER_LOGGER_BOX_NAME "/mb_controller_logger"
/**
 * \def MQ_MSG_COUNT
 * Max amount of message into the message queue.
 */
#define MQ_MSG_COUNT 50
/**
 * \def DEBUG_STRING
 * String for the debug level
 */
#define DEBUG_STRING    "DEBUG"
/**
 * \def INFO_STRING
 * String for the info level
 */
#define INFO_STRING     "INFO"
/**
 * \def WARNING_STRING
 * String for the warning level
 */
#define WARNING_STRING  "WARNING"
/**
 * \def ERROR_STRING
 * String for the error level
 */
#define ERROR_STRING    "ERROR"
/**
 * \def NONE_STRING
 * String for the none level
 */
#define NONE_STRING    "NONE"
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
* \struct Mq_Msg_Data
* \brief Definition of the Mq_Msg_Data type.
*
* Mq_Msg_Data contains an event for the state machine.
*/
typedef struct {
    Event event; /**< Event to change the state of the state machine. */
    time_t rtc;
    char log_msg[CONFIG_LOGGER_LOG_SIZE];
    log_level_e level;
} Mq_Msg_Data;
/**
 * \union Mq_Msg
 * \brief Message queue type as an union.
 *
 * This union allows to transform a buffer into the Mq_Msg_Data structure.
 */
typedef union {
    Mq_Msg_Data msg_data; /**< Data structure. */
    char buffer[sizeof(Mq_Msg_Data)]; /**< Raw message. */
} Mq_Msg;
/**
 * \struct Transition
 * \brief Gives an action and state destination for a transition.
 *
 * Transition gives the action to perform when an event has been fired. Also gives the destination state.
 */
typedef struct {
    State_Machine state_destination; /**< Next state. */
    Action action; /**< Action to perform from previous the event. */
} Transition;

/**
 * \typedef void(*Action_Pt)(Action_param param)
 * \brief Definition of function pointer for the actions to perform.
 */
typedef int(*Action_Pt)(const char * string_to_log, log_level_e level_to_log);

/*
 * \typedef FILE* Id_file
 * \brief Definition of id_file has FILE.
 */
typedef FILE* Id_file;
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/* ----- PASSIVES ----- */
/**
 * \fn static int CONTROLLER_LOGGER_setup_rtc(time_t rtc)
 * \brief Setups the internal rtc of the system.
 * \author Joshua MONTREUIL
 *
 * \param rtc : timestamp.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_setup_rtc(time_t rtc);
/**
 * \fn static uint32_t CONTROLLER_LOGGER_check_memory(void)
 * \brief Checks if enough memory is available on the Raspberry Pi for next log storage.
 * \author Joshua MONTREUIL
 *
 * \return The size of the log file or -1 on error.
 */
static uint32_t CONTROLLER_LOGGER_check_memory(void);
/**
 * \fn static int CONTROLLER_LOGGER_store_temp_logs(Log log)
 * \brief temporarily store un-timestamped strings.
 * \author Joshua MONTREUIL
 *
 * \param log : logs to be stored.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_store_temp_logs(Log log);
/**
 * \fn static int CONTROLLER_LOGGER_save_temp_logs(void)
 * \brief Saves into the log file the unstored logs with a timestamp.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_save_temp_logs(void);
/**
 * \fn static int CONTROLLER_LOGGER_save_logs(const char * string_to_log, log_level_e level_to_log)
 * \brief Saves the log into the log file.
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_save_logs(const char * string_to_log, log_level_e level_to_log);
/**
 * \fn static int CONTROLLER_LOGGER_load_logs(void)
 * \brief Reads the the log file.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0 and puts the logs into the global buffer. On error, returns -1.
 */
static int CONTROLLER_LOGGER_load_logs(void);
/**
 * \fn static int CONTROLLER_LOGGER_remove_logs(void)
 * \brief Remove the logs from the file.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_remove_logs(void);
/**
 * \fn static int CONTROLLER_LOGGER_get_current_time(char * time_buffer)
 * \brief gets the current time and stores it in a buffer
 * \author Florentin LEPELTIER
 * \param time_buffer : to contain current time string
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_get_current_time(char * time_buffer);
/* ----- INTERNAL ----- */
/**
 * \fn static char* CONTROLLER_LOGGER_get_string_level(int log_level)
 * \brief returns log_level as a string
 * \author Florentin LEPELTIER
 *
 * \param log_level : given log_level.
 *
 * \return Log: given log_level in string.
 */
static Log CONTROLLER_LOGGER_get_string_level(int log_level);
/**
 * \fn static int CONTROLLER_LOGGER_print_logs_on_terminal(Log log)
 * \brief Prints the given log entry into the terminal.
 * \author Joshua MONTREUIL
 *
 * \param log : log to print.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static void CONTROLLER_LOGGER_print_logs_on_terminal(Log log);
/* ----- ACTIONS ----- */
/**
 * \fn static int CONTROLLER_LOGGER_action_nop(const char * string_to_log, log_level_e level_to_log)
 * \brief Used for S_FORGET states;
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_action_nop(const char * string_to_log, log_level_e level_to_log);
/**
 * \fn static int CONTROLLER_LOGGER_action_setup_rtc_and_save_logs(const char * string_to_log, log_level_e level_to_log)
 * \brief Sets the internal rtc.
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_action_setup_rtc_and_save_logs(const char * string_to_log, log_level_e level_to_log);
/**
 * \fn static int CONTROLLER_LOGGER_action_evaluate_memory(const char * string_to_log, log_level_e level_to_log)
 * \brief Gets the size of the log file and evaluates it.
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_action_evaluate_memory(const char * string_to_log, log_level_e level_to_log);
/**
 * \fn static int CONTROLLER_LOGGER_action_raise_alert(const char * string_to_log, log_level_e level_to_log)
 * \brief Save or print (or both the) given log entry and raise a memory alert to gui proxy.
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_action_raise_alert(const char * string_to_log, log_level_e level_to_log);
/**
 * \fn static int CONTROLLER_LOGGER_action_remember_logs(const char * string_to_log, log_level_e level_to_log)
 * \brief Saves all the logs (string and level) into a temporary buffer during the waiting of the rtc from SB_IHM.
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_action_remember_logs(const char * string_to_log, log_level_e level_to_log);
/**
 * \fn static int CONTROLLER_LOGGER_action_save_logs(const char * string_to_log, log_level_e level_to_log)
 * \brief Save or print (or both) the given log entry.
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_action_save_logs(const char * string_to_log, log_level_e level_to_log);
/**
 * \fn static int CONTROLLER_LOGGER_action_load_and_send_logs(const char * string_to_log, log_level_e level_to_log)
 * \brief Loads, parse into different frames and sends it to logs manager proxy.
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_action_load_and_send_logs(const char * string_to_log, log_level_e level_to_log);
/**
 * \fn static int CONTROLLER_LOGGER_action_remove_logs(const char * string_to_log, log_level_e level_to_log)
 * \brief Action used into the SM to remove the logs.
 * \author Joshua MONTREUIL
 *
 * \param string_to_log : string to be logged.
 * \param level_to_log : log level to be logged.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_action_remove_logs(const char * string_to_log, log_level_e level_to_log);
/* ----- ACTIVE ----- */
/**
 * \fn static void * CONTROLLER_LOGGER_run(void * arg)
 * \brief Called by a thread. This function is the "active" part of controller logger.
 * \author Joshua MONTREUIL
 *
 * \param arg : argument pointer.
 *
 * \return void * : generic pointer.
 */
static void * CONTROLLER_LOGGER_run(void* arg);
/**
 * \fn static int CONTROLLER_LOGGER_mq_receive(Mq_Msg * a_msg)
 * \brief Receives the messages from the queue.
 * \author Joshua MONTREUIL
 *
 * \param a_msg : pointer to Mq_Msg struct.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_mq_receive(Mq_Msg * a_msg);
/**
 * \fn static int CONTROLLER_LOGGER_mq_send(Mq_Msg * a_msg)
 * \brief Sends a message into the queue.
 * \author Joshua MONTREUIL
 *
 * \param a_msg : pointer to Mq_Msg struct.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CONTROLLER_LOGGER_mq_send(Mq_Msg * a_msg);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/**
 * \var print_mode_set
 * \brief print_mode, can be set to 0:TERMINAL ONLY | 1:FILE ONLY | 2:BOTH |
 */
static print_mode print_mode_set;
/**
 * \var static pthread_t controller_logger_thread
 * \brief Postman thread.
 */
static pthread_t controller_logger_thread;
/**
 * \var static mqd_t my_mail_box
 * \brief Message queue reference.
 */
static mqd_t my_mail_box;
/**
 * \var filepath
 * \brief filepath of the log file
 */
static char* filepath = CONFIG_LOG_FILE_PATH;
/**
 * \var static time_t rtc
 * \brief internal rtc from SB_IHM.
 */
static time_t robot_rtc;
/**
 * \var static uint32_t current_file_size
 * \brief Actual size of the log file.
 */
static uint32_t current_file_size = 0;
/**
 * \var id_file
 * \brief Identifier of the log file.
 */
static Id_file id_file;
/**
 * \var id_temp_file
 * \brief Identifier of the temp log file.
 */
static Id_file id_temp_file;
/**
 * \var log_list
 * \brief List of logs.
 */
static Log_List log_list;
/**
 * \var level
 * \brief Log level, can be set to 0:DEBUG | 1:INFO | 2:WARNING | 3:ERROR |
 */
static log_level_e level;
/**
 * \var static const Action_Pt actions_tab[ACTION_NB]
 * \brief Array of function pointer to call from action to perform.
 */
static const Action_Pt actions_tab[ACTION_NB] = {
    &CONTROLLER_LOGGER_action_nop,
    &CONTROLLER_LOGGER_action_setup_rtc_and_save_logs,
    &CONTROLLER_LOGGER_action_evaluate_memory,
    &CONTROLLER_LOGGER_action_raise_alert,
    &CONTROLLER_LOGGER_action_save_logs,
    &CONTROLLER_LOGGER_action_remember_logs,
    &CONTROLLER_LOGGER_action_load_and_send_logs,
    &CONTROLLER_LOGGER_action_remove_logs,
    &CONTROLLER_LOGGER_action_nop,
};
/**
 * \var static Transition my_state_machine [STATE_NB -1][EVENT_NB]
 * \brief Array representing the state machine.
 */
static Transition my_state_machine [STATE_NB -1][EVENT_NB] = {
    [S_IDLE]           [E_LOG]             = {S_IDLE,           A_REMEMBER_LOGS},
    [S_IDLE]           [E_STOP]            = {S_DEATH,          A_STOP},
    [S_IDLE]           [E_ASK_SET_RTC]     = {S_WAITING_ACTION, A_SETUP_RTC_SAVE_TEMP_LOGS},
    [S_CHOICE]         [E_MEMORY_OK]       = {S_WAITING_ACTION, A_SAVE_LOGS},
    [S_CHOICE]         [E_MEMORY_ALERT]    = {S_WAITING_ACTION, A_RAISE_MEMORY_ALERT},
    [S_CHOICE]         [E_MEMORY_CRITICAL] = {S_MEMORY_FULL,    A_RAISE_MEMORY_ALERT},
    [S_CHOICE]         [E_STOP]            = {S_DEATH,          A_STOP},
    [S_FLUSHING]       [E_STOP]            = {S_DEATH,          A_STOP},
    [S_FLUSHING]       [E_LOGS_SAVED]      = {S_WAITING_ACTION, A_REMOVE_LOGS},
    [S_MEMORY_FULL]    [E_STOP]            = {S_DEATH,          A_STOP},
    [S_MEMORY_FULL]    [E_ASK_LOGS]        = {S_FLUSHING,       A_LOAD_LOGS},
    [S_WAITING_ACTION] [E_LOG]             = {S_CHOICE,         A_MEMORY_CHECK},
    [S_WAITING_ACTION] [E_STOP]            = {S_DEATH,          A_STOP},
    [S_WAITING_ACTION] [E_ASK_LOGS]        = {S_FLUSHING,       A_LOAD_LOGS},
};
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int CONTROLLER_LOGGER_create(void) {
    struct mq_attr mqa;
    mqa.mq_maxmsg = MQ_MSG_COUNT;
    mqa.mq_msgsize = sizeof(Mq_Msg);
    errno = 0;
    if((my_mail_box = mq_open(MQ_CONTROLLER_LOGGER_BOX_NAME, O_CREAT | O_RDWR | O_EXCL , 0644 ,&mqa )) == -1 ) {
        if(errno == EEXIST) {
            mq_unlink(MQ_CONTROLLER_LOGGER_BOX_NAME);
            if((my_mail_box = mq_open(MQ_CONTROLLER_LOGGER_BOX_NAME, O_CREAT | O_RDWR , 0644 ,&mqa )) == -1 ) {
                /* Cannot be logged but error on mq_open here. */
                printf("ERROR on mq_open for controller_logger\n");
                return -1;
            }
        } else {
            /* Cannot be logged but error on mq_open here. */
            printf("ERROR on mq_open for controller_logger\n");
            return -1;
        }
    }
    level = CONFIG_LOGGER_LOG_LEVEL;
    print_mode_set = CONFIG_LOGGER_PRINT_MODE;
    if((id_file = fopen(filepath,"a+") ) == NULL) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fopen(): controller logger has failed to open the log file."};
        if (CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
            goto error_fopen;
        }
        goto error_fopen;
    }
    return 0;

    error_fopen :
        mq_close(my_mail_box);
        mq_unlink(MQ_CONTROLLER_LOGGER_BOX_NAME);
        return -1;
}

int CONTROLLER_LOGGER_start(void) {
    if(pthread_create(&controller_logger_thread,NULL, CONTROLLER_LOGGER_run, NULL) != 0 ) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On pthread_create() : error while creating controller logger thread."};
        if (CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
            return -1;
        }
        return -1;
    }
    return 0;
}

int CONTROLLER_LOGGER_ask_logs(Id_Robot id_robot) {
    Mq_Msg my_msg_ask_logs = {.msg_data.event = E_ASK_LOGS,0,""};
    if (CONTROLLER_LOGGER_mq_send(&my_msg_ask_logs) == -1) {
        return -1;
    }
    return 0;
}

int CONTROLLER_LOGGER_log(log_level_e log_level, const char* msg) {
    Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = log_level};
    memcpy(my_msg_log.msg_data.log_msg, msg, strlen(msg));
    if (CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
        return -1;
    }
    return 0;
}

int CONTROLLER_LOGGER_ask_set_rtc(Id_Robot id_robot,time_t rtc) {
    Mq_Msg my_msg_rtc = {.msg_data.event = E_ASK_SET_RTC, .msg_data.rtc = rtc};
    if (CONTROLLER_LOGGER_mq_send(&my_msg_rtc) == -1) {
        return -1;
    }
    return 0;
}

int CONTROLLER_LOGGER_logs_saved(Id_Robot id_robot) {
    Mq_Msg my_msg = {.msg_data.event = E_LOGS_SAVED};
    if(CONTROLLER_LOGGER_mq_send(&my_msg) == -1 ) {
        return -1;
    }
    return 0;
}

int CONTROLLER_LOGGER_stop(void) {
    int ret = 0;
    Mq_Msg my_msg_stop = {.msg_data.event = E_STOP,0,""};
    if(CONTROLLER_LOGGER_mq_send(&my_msg_stop) == 0 ) {
        if(pthread_join(controller_logger_thread, NULL) != 0) {
            /* Cannot be logged but error on pthread_join here. */
            printf("ERROR on pthread_join for controller_logger\n");
            ret = -1;
        }
    }
    else {
        ret = -1;
    }
    return ret;
}

int CONTROLLER_LOGGER_destroy(void) {
    int ret = 0;
    if(mq_close(my_mail_box) == -1 ) {
        /* Cannot be logged but error on mq_close here. */
        printf("ERROR on mq_close for controller_logger\n");
        ret = -1;
    }
    if(mq_unlink(MQ_CONTROLLER_LOGGER_BOX_NAME) == -1 ) {
        /* Cannot be logged but error on mq_unlink here. */
        printf("ERROR on mq_unlink for controller_logger\n");
        ret = -1;
    }
    return ret;
}

/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
/* ----- ACTIVE ----- */
static void * CONTROLLER_LOGGER_run(void* arg) {
    Mq_Msg msg;
    State_Machine my_state = S_WAITING_ACTION;
    Transition * my_transition;
    char string[CONFIG_LOGGER_LOG_SIZE];
    log_level_e level_to_log = 0;
    while(my_state != S_DEATH) {
        if(CONTROLLER_LOGGER_mq_receive(&msg) == -1) {
            /* Cannot be logged but error on mq here. */
            printf("ERROR on controller_logger_mq\n");
            return NULL;
        }
        my_transition = &my_state_machine[my_state][msg.msg_data.event];
        if(msg.msg_data.event == E_LOG) {
            level_to_log = msg.msg_data.level;
            memset(string,0,CONFIG_LOGGER_LOG_SIZE);
            strcpy(string,msg.msg_data.log_msg);
        }
        else if(msg.msg_data.event == E_ASK_SET_RTC) {
            robot_rtc = msg.msg_data.rtc;
            level_to_log = 0;
            memset(string,0,CONFIG_LOGGER_LOG_SIZE);
        }
        else if(msg.msg_data.event != E_MEMORY_OK) {
            level_to_log = 0;
            memset(string,0,CONFIG_LOGGER_LOG_SIZE);
        }
        if(my_transition->state_destination != S_FORGET) {
            if(actions_tab[my_transition->action](string,level_to_log) == -1) {
                /* Cannot be logged but error on actions_tab here. */
                printf("ERROR on controller_logger action_tab\n");
                return NULL;
            }
            my_state = my_transition->state_destination;
        }
    }
    return 0;
}

static int CONTROLLER_LOGGER_mq_receive(Mq_Msg * a_msg) {
    if((mq_receive(my_mail_box,a_msg->buffer,sizeof(Mq_Msg), 0) == -1)) {
        /* Cannot be logged but error on mq_receive here. */
        printf("ERROR on mq_receive for controller_logger\n");
        mq_close(my_mail_box);
        mq_unlink(MQ_CONTROLLER_LOGGER_BOX_NAME);
        return -1;
    }
    return 0;
}

static int CONTROLLER_LOGGER_mq_send(Mq_Msg * a_msg) {
    if(mq_send(my_mail_box,a_msg->buffer, sizeof(Mq_Msg),0) == -1 ) {
        /* Cannot be logged but error on mq here. */
        printf("ERROR on controller_logger_mq\n");
        mq_close(my_mail_box);
        mq_unlink(MQ_CONTROLLER_LOGGER_BOX_NAME);
        return -1;
    }
    return 0;
}
/* ----- ACTIONS ----- */
static int CONTROLLER_LOGGER_action_nop(const char * string_to_log, log_level_e level_to_log) { return 0; }

static int CONTROLLER_LOGGER_action_setup_rtc_and_save_logs(const char * string_to_log, log_level_e level_to_log) {
    if(CONTROLLER_LOGGER_setup_rtc(robot_rtc) == -1 ) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On CONTROLLER_LOGGER_setup_rtc() : controller logger has failed to setup its rtc."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    if(CONTROLLER_LOGGER_save_temp_logs() == -1) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On CONTROLLER_LOGGER_save_temp_log() : controller logger has failed to store temps logs."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    return 0;
}

static int CONTROLLER_LOGGER_action_evaluate_memory(const char * string_to_log, log_level_e level_to_log) {
    if((current_file_size = CONTROLLER_LOGGER_check_memory()) == -1) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On GUI_PROXY_raise_memory_alert() : error reading file size."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    if(current_file_size < 1500000) {
        Mq_Msg my_msg = {.msg_data.event = E_MEMORY_OK,0,""};
        if(CONTROLLER_LOGGER_mq_send(&my_msg) == -1 ) {
            return -1;
        }
    }
    else if(current_file_size < 2000000) {
        Mq_Msg my_msg = {.msg_data.event = E_MEMORY_ALERT,0,""};
        if(CONTROLLER_LOGGER_mq_send(&my_msg) == -1 ) {
            return -1;
        }
    }
    else {
        Mq_Msg my_msg = {.msg_data.event = E_MEMORY_CRITICAL,0,""};
        if(CONTROLLER_LOGGER_mq_send(&my_msg) == -1 ) {
            return -1;
        }
    }
    return 0;
}

static int CONTROLLER_LOGGER_action_raise_alert(const char * string_to_log, log_level_e level_to_log) {
    if(GUI_PROXY_raise_memory_alert(ID_ROBOT) == -1) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On GUI_PROXY_raise_memory_alert() : error while sending a memory alert to gui proxy."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    return 0;
}

static int CONTROLLER_LOGGER_action_save_logs(const char * string_to_log, log_level_e level_to_log) {
    if(CONTROLLER_LOGGER_save_logs(string_to_log,level_to_log) == -1) {
        return -1;
    }
    return 0;
}

static int CONTROLLER_LOGGER_action_remember_logs(const char * string_to_log, log_level_e level_to_log) {
    char string[strlen(string_to_log) + 1];
    sprintf(string,"%d\n%s\n", level_to_log, string_to_log);
    if(CONTROLLER_LOGGER_store_temp_logs(string) == -1) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On CONTROLLER_LOGGER_store_temp_logs() : error while saving log into a temp buffer."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    return 0;
}

static int CONTROLLER_LOGGER_action_load_and_send_logs(const char * string_to_log, log_level_e level_to_log) {
    if(CONTROLLER_LOGGER_load_logs() == -1) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On CONTROLLER_LOGGER_load_logs() : error while loading logs from the file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            free(log_list);
            return -1;
        }
        free(log_list);
        return -1;
    }
    if(current_file_size != 0) {
        uint32_t max_page = (uint32_t) ceil(((double)current_file_size)/((double)0xFFFB));
        for(int i = 0; i < max_page; ++i) {
            uint32_t page_log_size;
            if((i+1) <= (max_page - 1)) {
                page_log_size = 0xFFFB;
            }
            else {
                page_log_size = current_file_size - (0xFFFB * i);
            }
            Log_List log_list_to_send = (Log_List) malloc(page_log_size + 4);
            uint8_t size_and_page_buff[4] = {((page_log_size >> 8) & 0xFF),(page_log_size & 0xFF),(i+1),max_page};
            memcpy(log_list_to_send,size_and_page_buff,sizeof(size_and_page_buff));
            memcpy((log_list_to_send + 4),(log_list + (0xFFFB * i)), page_log_size);
            LOGS_MANAGER_PROXY_set_logs(log_list_to_send);
        }
    }
    else {
        LOGS_MANAGER_PROXY_set_logs((unsigned char*)"Empty\n");
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "The log file has a size of 0."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            free(log_list);
            return -1;
        }
        free(log_list);
        return -1;
    }
    free(log_list);
    return 0;
}

static int CONTROLLER_LOGGER_action_remove_logs(const char * string_to_log, log_level_e level_to_log) {
    CONTROLLER_LOGGER_remove_logs();
    return 0;
}
/* ----- PASSIVES ----- */
static uint32_t CONTROLLER_LOGGER_check_memory(void){
    if(fseek(id_file, 0L, SEEK_END) != 0) { /* Puts the reading cursor to the end of the file */
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fseek() : failed to read the file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    uint32_t size = ftell(id_file); /* Indicates where the reading cursor is = file size */
    if(size == -1 ) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On ftell() : failed to compute file size."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    return size;
}

static int CONTROLLER_LOGGER_save_logs(const char * string_to_log, log_level_e level_to_log){
    Log str_level ="";
    if(level_to_log >= level) {
        str_level = CONTROLLER_LOGGER_get_string_level(level_to_log);
    }
    char current_time[30];
    if(CONTROLLER_LOGGER_get_current_time(current_time) == -1) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On CONTROLLER_LOGGER_get_current_time() : error while getting current time."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
            return -1;
        }
        return -1;
    }
    char log[strlen(string_to_log)+ strlen(current_time) + strlen(str_level)];
    sprintf(log,"%s : %s - %s\n", str_level, current_time, string_to_log);
    if(print_mode_set == FILE_ONLY) {
        fprintf(id_file,"%s",log);
    }
    else if(print_mode_set == TERMINAL_ONLY) {
        CONTROLLER_LOGGER_print_logs_on_terminal(log);
    }
    else if(print_mode_set == BOTH){
        fprintf(id_file,"%s",log);
        CONTROLLER_LOGGER_print_logs_on_terminal(log);
    }
    return 0;
}

static int CONTROLLER_LOGGER_load_logs(void){
    log_list = (Log_List) malloc(current_file_size);
    char line_read[100];
    size_t current_size = 0;
    while(fgets(line_read, sizeof(line_read),id_temp_file) != NULL) {
        size_t line_read_size = strlen(line_read);
        strcpy((char *)log_list + current_size, line_read + 1);
        current_size += line_read_size;
    }
    return 0;
}

static int CONTROLLER_LOGGER_remove_logs(void) {
    if(fclose(id_file) != 0) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fclose() : Failed to close the file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    if((id_file = fopen(filepath,"w")) == NULL) { /* Opening the file into write mode to delete the logs */
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fopen(): controller logger has failed to open the log file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
            return -1;
        }
        return -1;
    }
    if(fclose(id_file) != 0) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fclose() : Failed to close the file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    if((id_file = fopen(filepath,"a+") ) == NULL) { /* Opening the file with the right mode */
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fopen(): controller logger has failed to open the log file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
            return -1;
        }
        return -1;
    }
    return 0;
}

static int CONTROLLER_LOGGER_setup_rtc(time_t rtc) {
    struct timespec new_rtc;
    new_rtc.tv_sec = rtc;
    if(clock_settime(CLOCK_REALTIME,&new_rtc) == -1) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On settimeofday(): controller logger has failed to set the new system time."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
            return -1;
        }
        return -1;
    }
    struct timeval tv;
    gettimeofday(&tv, NULL); // Obtention de la valeur actuelle de temps
    return 0;
}

static int CONTROLLER_LOGGER_store_temp_logs(Log log) {
    if((id_temp_file = fopen(CONFIG_TEMP_LOG_FILE_PATH,"a+") ) == NULL) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fopen(): controller logger has failed to open the log file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
            return -1;
        }
        return -1;
    }
    fprintf(id_temp_file,"%s",log);
    fclose(id_temp_file);
    return 0;
}

static int CONTROLLER_LOGGER_save_temp_logs(void) {
    if((id_temp_file = fopen(CONFIG_TEMP_LOG_FILE_PATH,"a+") ) == NULL) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fopen(): controller logger has failed to open the log file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1) {
            return -1;
        }
        return -1;
    }
    if(fseek(id_temp_file, 0L, SEEK_END) != 0) { /* Puts the reading cursor to the end of the file */
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fseek() : failed to read the file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    uint32_t size = ftell(id_temp_file); /* Indicates where the reading cursor is = file size */
    log_list = (Log_List) malloc(size);
    if(fseek(id_temp_file, 0L, SEEK_SET) != 0) { /* Puts the reading cursor to the beginning of the file for reading. */
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On fseek() : failed to read the file."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    char line_read[100];
    size_t current_size = 0;
    while(fgets(line_read, sizeof(line_read),id_temp_file) != NULL) {
        size_t line_read_size = strlen(line_read);
        memcpy(log_list + current_size, line_read, line_read_size + 1);
        current_size += line_read_size;
    }
    log_level_e level_to_log;
    char string_to_log[CONFIG_LOGGER_LOG_SIZE];
    char *char_to_get = strtok((char*)log_list,"\n");
    while(char_to_get != NULL) {
        level_to_log = atoi(char_to_get);
        char_to_get = strtok(NULL, "\n");
        memcpy(string_to_log, char_to_get, strlen(char_to_get) + 1);
        CONTROLLER_LOGGER_save_logs(string_to_log, level_to_log);
        char_to_get = strtok(NULL, "\n");
    }
    free(log_list);
    return 0;
}
/* ----- INTERNAL ----- */
static int CONTROLLER_LOGGER_get_current_time(char * time_buffer){
    time_t current_time;
    time(&current_time);
    if(ctime_r(&current_time, time_buffer) == NULL) {
        Mq_Msg my_msg_log = {.msg_data.event = E_LOG, .msg_data.level = ERROR, .msg_data.log_msg = "On ctime_r() : error while getting current time."};
        if(CONTROLLER_LOGGER_mq_send(&my_msg_log) == -1 ) {
            return -1;
        }
        return -1;
    }
    time_buffer[strlen(time_buffer) - 1] = '\0'; //remove new line character
    return 0;
}

static Log CONTROLLER_LOGGER_get_string_level(int log_level) {
    Log str_level;
    switch(log_level) {
        case DEBUG :    str_level = DEBUG_STRING;       break;
        case INFO :     str_level = INFO_STRING;        break;
        case WARNING :  str_level = WARNING_STRING;     break;
        case ERROR :    str_level = ERROR_STRING;       break;
        default :       str_level = NONE_STRING;       break;
    }
    return str_level;
}

static void CONTROLLER_LOGGER_print_logs_on_terminal(Log log) {
    printf("%s",log);
}
