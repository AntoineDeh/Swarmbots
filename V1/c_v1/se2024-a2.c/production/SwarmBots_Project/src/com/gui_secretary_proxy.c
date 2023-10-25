/**
 * \file  gui_secretary_proxy.c
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date May 8, 2023
 * \brief Proxy source file of the gui secretary class.
 *
 * \see gui_secretary_proxy.h
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "gui_secretary_proxy.h"
#include "postman.h"
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int GUI_SECRETARY_PROXY_set_mode(Id_Robot id_robot, Operating_Mode operating_mode) {
    Communication_Protocol_Head msg_to_send;
    msg_to_send.msg_size = 0x0006;
    uint8_t buf[4] = {operating_mode.camera_mode, operating_mode.radar_mode, operating_mode.buzzer_mode, operating_mode.leds_mode};
    uint8_t * data = (uint8_t*) malloc(2 + msg_to_send.msg_size);
    msg_to_send.msg_size = htons(0x0006);
    msg_to_send.msg_type = htons(SET_MODE);

    memcpy(data,&msg_to_send,4);
    memcpy(data + 4,buf,sizeof(buf));
    if(POSTMAN_send_request(data) == -1) {
        CONTROLLER_LOGGER_log(ERROR,"On POSTMAN_send_request() : gui secretary proxy has failed to request a data write on postman's mq.");
        return -1;
    }
    return 0;
}

void GUI_SECRETARY_PROXY_ack_connection(Id_Robot id_robot) {
    CONTROLLER_LOGGER_log(INFO,"Acknowledgment of the connection.");
}

int GUI_SECRETARY_PROXY_disconnected_ok(Id_Robot id_robot) {
    Communication_Protocol_Head msg_to_send;
    msg_to_send.msg_size = htons(0x0002);
    msg_to_send.msg_type = htons(ACK_DISCONNECTION);
    uint8_t * data = (uint8_t*) malloc(2 + msg_to_send.msg_size);
    memcpy(data,&msg_to_send,4);
    if(POSTMAN_send_request(data) == -1) {
        CONTROLLER_LOGGER_log(ERROR,"On POSTMAN_send_request() : gui secretary proxy has failed to request a data write on postman's mq.");
        return -1;
    }
    return 0;
}

int GUI_SECRETARY_PROXY_set_radar(Id_Robot id_robot, bool_e radar) {
    Communication_Protocol_Head msg_to_send;
    msg_to_send.msg_size = htons(0x0003);
    msg_to_send.msg_type = htons(SET_RADAR);
    uint8_t buf[1] = {radar};
    uint8_t * data = (uint8_t*) malloc(2 + msg_to_send.msg_size);
    memcpy(data,&msg_to_send,4);
    memcpy(data + 4,buf,sizeof(buf));
    if(POSTMAN_send_request(data) == -1) {
        CONTROLLER_LOGGER_log(ERROR,"On POSTMAN_send_request() : viewer proxy has failed to request a data write on postman's mq.");
        return -1;
    }
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */