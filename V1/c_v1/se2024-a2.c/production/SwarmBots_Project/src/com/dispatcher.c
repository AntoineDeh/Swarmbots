/**
 * \file  dispatcher.c
 * \version  1.1
 * \author Joshua MONTREUIL
 * \date May 7, 2023
 * \brief Source file for the dispatcher module. The dispatcher is in charge of the reading, parsing and dispatching of
 * any messages on the postman's socket.
 *
 * \see dispatcher.h
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
 */

/* ----------------------  INCLUDES  ---------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include "dispatcher.h"
#include "postman.h"
#include "../alphabot2/camera.h"
#include "../controller/controller_ringer.h"
#include "../controller/controller_core.h"
#include "../controller/pilot.h"
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
#define STATE_GENERATION S(S_IDLE) S(S_READING_MSG) S(S_STOP) S(S_WAITING_RECONNECTION)
#define S(x) x,
typedef enum {STATE_GENERATION STATE_NB} State_Machine;
#undef STATE_GENERATION
#undef S
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static void * DISPATCHER_run(void * arg)
 * \brief Called by a thread. This function is the "active" part of dispatcher.
 * It reads any messages on postman's socket then parse and dispatch it.
 * \author Joshua MONTREUIL
 * \see postman.h
 *
 * \param arg : argument pointer.
 *
 * \return void * : generic pointer (0 on success -1 on error).
 */
static void * DISPATCHER_run(void * arg);
/**
 * \fn static void DISPATCHER_dispatch_received_msg(Communication_Protocol_Head msg)
 * \brief Used to parse the incoming msg and to dispatch and pass data to the right functions.
 * \author Joshua MONTREUIL
 *
 * \see CONTROLLER_CORE_ask_set_state(int id_robot, State state)
 * \see CONTROLLER_CORE_ask_mode(int id_robot)
 * \see CONTROLLER_CORE_ask_set_mode(int id_robot, Operating_Mode operating_mode)
 * \see CONTROLLER_RINGER_ask_availability(int id_robot)
 * \see PILOT_ask_cmd(Command cmd)
 * \see CONTROLLER_LOGGER_ask_logs(int id_robot)
 *
 * \param msg : message received from postman's socket. Type : Communication_Protocol_Head.
 * \see Communication_Protocol_Head
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int DISPATCHER_dispatch_received_msg(Communication_Protocol_Head msg);
/**
 * \fn static Communication_Protocol_Head DISPATCHER_decode_message(uint8_t* raw_message)
 * \brief Used to decode the raw message from the socket. Separation between the message type, the data size and the rest of the informations.
 * \author Joshua MONTREUIL
 *
 * \param raw_message : raw message from the socket.
 *
 * \return The header of the message.
 * \see Communication_Protocol_Head
 */
static Communication_Protocol_Head DISPATCHER_decode_message(uint8_t* raw_message);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/**
 * \var static State_Machine state
 * \brief Dispatcher state Machine.
 */
static State_Machine state;
/**
 * \var static pthread_t dispatcher_thread
 * \brief Dispatcher thread.
 */
static pthread_t dispatcher_thread;
/**
 * \var static uint8_t * data_received
 * \brief raw data received from socket;
 */
static uint8_t * data_received;
/**
 * \var static pthread_mutex_t dispatcher_mutex
 * \brief Mutex used to safely read state from state machine
 */
static pthread_mutex_t dispatcher_mutex = PTHREAD_MUTEX_INITIALIZER;
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int DISPATCHER_create(void) {
    data_received = (uint8_t *) malloc(MAX_RECEIVED_BYTES);
    return 0;
}

int DISPATCHER_start(void) {
    state = S_IDLE;
    if(pthread_create(&dispatcher_thread, NULL, DISPATCHER_run, NULL) != 0 ) {
        CONTROLLER_LOGGER_log(ERROR, "On thread_create(): error while creating dispatcher thread.");
        return -1;
    }
    return 0;
}

void DISPATCHER_start_reading(void) {
    pthread_mutex_lock(&dispatcher_mutex);
    state = S_READING_MSG;
    pthread_mutex_unlock(&dispatcher_mutex);
}

int DISPATCHER_stop(void) {
    pthread_mutex_lock(&dispatcher_mutex);
    state = S_STOP;
    pthread_mutex_unlock(&dispatcher_mutex);
    if(pthread_join(dispatcher_thread, NULL) != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On pthread_join(): error while waiting the termination of dispatcher thread.");
        return -1;
    }
    return 0;
}

int DISPATCHER_destroy(void) {
    free(data_received);
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static void * DISPATCHER_run(void * arg) {
    State_Machine my_state;
    pthread_mutex_lock(&dispatcher_mutex);
    my_state = state;
    pthread_mutex_unlock(&dispatcher_mutex);
    while(my_state != S_STOP) {
        pthread_mutex_lock(&dispatcher_mutex);
        my_state = state;
        pthread_mutex_unlock(&dispatcher_mutex);
        if(my_state == S_READING_MSG) {
            uint8_t* raw_message = POSTMAN_read_request();
            if(raw_message == NULL) {
                CONTROLLER_LOGGER_log(ERROR,"Dispatcher has received an empty message.");
                return NULL;
            }
            else {
                uint8_t error_value = *raw_message;
                if(error_value == EBADF) {
                    CONTROLLER_LOGGER_log(WARNING, "On POSTMAN_read_request() : The data socket for reading has been closed, a disconnection has been asked or detected.");
                    pthread_mutex_lock(&dispatcher_mutex);
                    state = S_WAITING_RECONNECTION;
                    pthread_mutex_unlock(&dispatcher_mutex);
                }
                else {
                    Communication_Protocol_Head msg_decoded = DISPATCHER_decode_message(raw_message);
                    DISPATCHER_dispatch_received_msg(msg_decoded);
                }
                free(raw_message);
            }
        }
    }
    return 0;
}

static int DISPATCHER_dispatch_received_msg(Communication_Protocol_Head msg) {
    switch(msg.msg_type)
    {
        case ASK_AVAILABILITY :
        {
            if(CONTROLLER_RINGER_ask_availability(ID_ROBOT) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_RINGER_ask_availability() : Dispatcher has failed to put a msg into Controller Ringer's mq.");
                return -1;
            }
            break;
        }
        case ASK_CMD :
        {
            char log_msg_cmd[20];
            sprintf(log_msg_cmd, "Command asked : %d",data_received[0]);
            CONTROLLER_LOGGER_log(DEBUG,log_msg_cmd);
            Command command_from_msg = (Command) data_received[0];
            if(PILOT_ask_cmd(command_from_msg) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On PILOT_ask_cmd() : Dispatcher has failed to put a msg into Pilot's mq.");
                return -1;
            }
            break;
        }
        case SET_STATE :
        {
            char log_msg_state[20];
            sprintf(log_msg_state, "State asked : %d",data_received[0]);
            CONTROLLER_LOGGER_log(DEBUG,log_msg_state);
            State state_from_msg = (State) data_received[0];
            if(CONTROLLER_CORE_ask_set_state(ID_ROBOT, state_from_msg) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_CORE_ask_set_state() : Dispatcher has failed to put a msg into Controller Core's mq.");
                return -1;
            }
            break;
        }
        case ASK_MODE :
        {
            if(CONTROLLER_CORE_ask_mode(ID_ROBOT) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_CORE_ask_mode() : Dispatcher has failed to put a msg into Controller Core's mq.");
                return -1;
            }
            break;
        }
        case SET_MODE :
        {
            Operating_Mode operating_mode_from_msg;
            operating_mode_from_msg.camera_mode = (Mode) data_received[0];
            operating_mode_from_msg.radar_mode = (Mode) data_received[1];
            operating_mode_from_msg.buzzer_mode = (Mode) data_received[2];
            operating_mode_from_msg.leds_mode = (Mode) data_received[3];
            if(CONTROLLER_CORE_ask_set_mode(ID_ROBOT,operating_mode_from_msg) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_CORE_ask_set_mode() : Dispatcher has failed to put a msg into Controller Core's mq.");
                return -1;
            }
            break;
        }
        case ASK_LOGS :
        {
            if(CONTROLLER_LOGGER_ask_logs(ID_ROBOT) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_LOGGER_ask_logs() : Dispatcher has failed to put a msg into controller logger's mq.");
                return -1;
            }
            break;
        }
        case ASK_TO_DISCONNECT :
        {
            if(CONTROLLER_CORE_ask_to_disconnect(ID_ROBOT) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_CORE_ask_to_disconnect() : Dispatcher has failed to put a msg into Controller Core's mq.");
                return -1;
            }
            pthread_mutex_lock(&dispatcher_mutex);
            state = S_WAITING_RECONNECTION;
            pthread_mutex_unlock(&dispatcher_mutex);
            break;
        }
        case SET_CURRENT_TIME : {
            struct tm rtc;
            rtc.tm_year = ((data_received[1] + 2000) - 1900);
            rtc.tm_mon  = (int) data_received[2] -1;
            rtc.tm_mday = (int) data_received[3];
            rtc.tm_hour = (int) data_received[4];
            rtc.tm_min  = (int) data_received[5];
            rtc.tm_sec  = (int) data_received[6];
            rtc.tm_isdst = -1;

            time_t rtc_timestamp = mktime(&rtc);
            if(CONTROLLER_LOGGER_ask_set_rtc(ID_ROBOT,rtc_timestamp) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_LOGGER_ask_set_rtc() : Dispatcher has failed to put a msg into Controller Logger's mq.");
                return -1;
            }
            break;
        }
        case SET_IP_PORT : {
            char ip_address[16];
            uint16_t port;
            sprintf(ip_address,"%d.%d.%d.%d",(uint8_t)(data_received[0]),(uint8_t)(data_received[1]),(uint8_t)(data_received[2]),(uint8_t)(data_received[3]));
            if(msg.msg_size > 5) {
                port = (uint16_t)(data_received[4] << 8) | data_received[5];
            }
            else {
                port = (0x00 << 8) | data_received[4];
            }
            if(CAMERA_set_up_ihm_info(ip_address, port) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CAMERA_set_up_ihm_info() : Dispatcher has failed to put a msg into Camera's mq.");
                return -1;
            }
            break;
        }
        case LOGS_RECEIVED : {
            if(CONTROLLER_LOGGER_logs_saved(ID_ROBOT) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On CONTROLLER_LOGGER_logs_saved() : Dispatcher has failed to put a msg into Controller Logger's mq.");
                return -1;
            }
            break;
        }
        default :
        {
            //Should not get here
            break;
        }
    }
    return 0;
}

static Communication_Protocol_Head DISPATCHER_decode_message(uint8_t* raw_message) {
    Communication_Protocol_Head msg;
    msg.msg_size = (raw_message[0] << 8) | raw_message[1];
    char log_msg_size[22];
    sprintf(log_msg_size, "Message size : %d",msg.msg_size);
    CONTROLLER_LOGGER_log(DEBUG,log_msg_size);
    msg.msg_type = ntohs((raw_message[2] << 8) | raw_message[3]);
    char log_msg_type[21];
    sprintf(log_msg_type, "Message type : %d",msg.msg_type);
    CONTROLLER_LOGGER_log(DEBUG,log_msg_type);
    if(msg.msg_size > 2) {
        memcpy(data_received, raw_message + 4,  msg.msg_size - 2);
    }
    return msg;
}