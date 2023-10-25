/**
 * \file  logs_manager_proxy.c
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date May 8, 2023
 * \brief Proxy Source file of the logs manager class.
 *
 * \see logs_manager_proxy.h
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
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "logs_manager_proxy.h"
#include "postman.h"
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int LOGS_MANAGER_PROXY_set_logs(Log_List log_list) {
    Communication_Protocol_Head msg_to_send;
    uint8_t log_buff_size = (uint8_t) (log_list[0] << 8 | log_list[1]);
    msg_to_send.msg_size = (int16_t) (2 + log_buff_size);
    uint8_t * data = (uint8_t*) malloc(2 + msg_to_send.msg_size);
    uint8_t buff[log_buff_size];
    memcpy(buff,(log_list + 2) ,sizeof(buff));
    free(log_list);
    msg_to_send.msg_type = htons(SET_LOGS);
    msg_to_send.msg_size = htons(msg_to_send.msg_size);
    memcpy(data,&msg_to_send,4);
    memcpy(data + 4,buff,sizeof(buff));

    if(POSTMAN_send_request(data) == -1) {
        CONTROLLER_LOGGER_log(ERROR,"On POSTMAN_send_request() : logs manager proxy has failed to request a data write on postman's mq.");
        return -1;
    }
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */