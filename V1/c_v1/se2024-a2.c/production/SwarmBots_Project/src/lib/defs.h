/**
 * \file  defs.h
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date Apr 29, 2023
 * \brief Definition of types usable from all of the modules.
 *
 * \see defs.c
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

#ifndef SRC_LIB_DEFS_H_
#define SRC_LIB_DEFS_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/**
 * \typedef Id_Robot
 * \brief Defines the robot identifier has an integer.
 */
typedef int Id_Robot;
/**
 * \enum State
 * \brief Defines different states of the robot.
 *
 * State gives the states in which the robot can be.
 */
typedef enum __attribute__((__packed__)){
    WAITING_FOR_CONNECTION, /**< WAITING_FOR_CONNECTION : the robot waits for a connection. */
    EMERGENCY,            /**< EMERGENCY : the robot has detected an emergency. */
    SELECTED,             /**< SELECTED : the robot is currently selected by the user. */
    NOT_SELECTED           /**< NOT_SELECTED : the robot is connected but unselected. */
} State;
/**
 * \enum bool_e
 * \brief Boolean
 *
 * bool_e represents a boolean, it exists in two states.
 */
typedef enum {
    FALSE = 0,/**< FALSE */
    TRUE = 1  /**< TRUE */
} bool_e;
/**
 * \enum Id_Alert
 * \brief lists of alert.
 *
 * Id_Alert gives the alert codes of the system.
 */
typedef enum {
    MEMORY = 0, /**< Memory alert. */
}Id_Alert;
/**
 * \enum Mode
 * \brief Defines two modes for the peripherals.
 *
 * Mode gives the two modes in which the peripherals can be.
 */
typedef enum __attribute__((__packed__)){
    DISABLED = 0, /**< DISABLED : the peripheral is off. */
    ENABLED = 1, /**< ENABLED : the peripheral is on. */
} Mode;
/**
 * \enum Command
 * \brief Defines five commands.
 *
 * Command gives the directions where the robot can go.
 */
typedef enum __attribute__((__packed__)){
    FORWARD, /**< FORWARD : motors clockwise. */
    RIGHT,   /**< RIGHT : left motor clockwise, right  motor anti-clockwise. */
    LEFT,    /**< LEFT : left  motor anti-clockwise, right  motor clockwise. */
    BACKWARD,/**< BACKWARD :  motors anti-clockwise. */
    STOP     /**< STOP :  motors stop. */
} Command;
/**
 * \struct Operating_Mode defs.h "lib/defs.h"
 * \brief Lists the modes of the peripherals.
 *
 * Operating_Mode gives a list of the robot's peripheral and the modes in which they are.
 */
typedef struct {
    Mode camera_mode;
    Mode radar_mode;
    Mode buzzer_mode;
    Mode leds_mode;
} Operating_Mode;
/**
 * \enum Message_Type
 * \brief Defines message types.
 *
 * Messag_Type gives all the message type that can go threw TCP.
 */
typedef enum  __attribute__((__packed__)){
    ASK_AVAILABILITY = 0x0100,  /**< ASK_AVAILABILITY : ping from SB_IHM. */
    SET_AVAILABILITY = 0x0200,  /**< SET_AVAILABILITY : pong from SB_C. */
    ASK_CMD = 0x0300,           /**< ASK_CMD : command change from SB_IHM. */
    SET_STATE = 0x0400,         /**< SET_STATE : state change from SB_IHM. */
    ASK_MODE = 0x0500,          /**< ASK_MODE : SB_IHM wants SB_C's mode. */
    SET_MODE = 0x0600,          /**< SET_MODE : SB_C gives its mode to SB_IHM. Or mode change from SB_IHM. */
    ASK_LOGS = 0x0700,          /**< ASK_LOGS : SB_IHM wants SB_C's logs. */
    SET_LOGS = 0x0800,          /**< SET_LOGS : SB_C gives its logs. */
    ALERT = 0x0900,             /**< ALERT : alert raise. */
    ASK_TO_DISCONNECT = 0x1000, /**< ASK_TO_DISCONNECT : SB_IHM asks SB_C to disconnect. */
    ACK_DISCONNECTION = 0x1100, /**< ACK_DISCONNECTION : SB_C acks to SB_IHM its disconnection. */
    SET_RADAR = 0x1200,         /**< SET_RADAR : SB_C sends the radar data when its value changes. */
    SET_CURRENT_TIME = 0x1300,  /**< SET_CURRENT_TIME : SB_IHM sends its current system time to calibrate SB_C current time. */
    SET_IP_PORT = 0x1400,       /**< SET_IP_PORT : SB_IHM sends its camera udp information to SB_C in order to broadcast to SB_IHM. */
    LOGS_RECEIVED = 0x1500,     /**< LOGS_RECEIVED : SB_IHM indicates that the logs has been received fully. */
} Message_Type;
/**
 * \struct Communication_Protocol_Head defs.h "lib/defs.h"
 * \brief Lists the head sections of a message.
 *
 * Communication_Protocol_Head gives a list of header section for the TCP communication protocol.
 */
typedef struct __attribute__((__packed__, aligned(1))){
    int16_t msg_size; /**< Gives the Message size (type + data) in bytes. Minimum = 2 bytes Maximum = 0xFFFF. */
    Message_Type msg_type; /**< Gives the Message type. Size = 2 bytes */
} Communication_Protocol_Head ;
/**
 * \typedef Log_List
 * \brief Logs storage buffer.
 */
typedef uint8_t* Log_List;
/**
 * \typedef Log
 * \brief a log.
 */
typedef char* Log;
/**
 * \enum print_mode
 * \brief Defines the available print modes
 */
typedef enum{
    TERMINAL_ONLY = 0, /**< TERMINAL_ONLY : Only printf on the terminal. **/
    FILE_ONLY = 1, /**< FILE_ONLY : Only into the file. */
    BOTH = 2, /** BOTH : Logs on the terminal and into the file. */
}print_mode;
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIABLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
#endif /* SRC_LIB_DEFS_H_ */
