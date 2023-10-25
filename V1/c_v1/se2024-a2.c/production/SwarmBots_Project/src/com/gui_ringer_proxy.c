/**
 * \file gui_ringer_proxy.c
 * \version  1.1
 * \author Joshua MONTREUIL
 * \date May 8, 2023
 * \brief Proxy Source file of the gui ringer class.
 *
 * \see gui_ringer_proxy.h
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "gui_ringer_proxy.h"
#include "postman.h"
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/* ----------------------  PUBLIC  FUNCTIONS  -------------------------------- */
int GUI_RINGER_PROXY_set_availability(Id_Robot id_robot) {
    Communication_Protocol_Head msg_to_send;
    msg_to_send.msg_type = htons(SET_AVAILABILITY);
    msg_to_send.msg_size = htons(sizeof(msg_to_send.msg_type));
    uint8_t * data = (uint8_t*) malloc(4);
    memcpy(data,&msg_to_send,4);
    if(POSTMAN_send_request(data) == -1) {
        CONTROLLER_LOGGER_log(ERROR,"On POSTMAN_send_request() : gui ringer proxy has failed to request a data write on postman's mq.");
        return -1;
    }
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */