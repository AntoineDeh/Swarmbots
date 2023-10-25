/**
 * \file  controller_logger.h
 * \version  0.1
 * \author Florentin LEPELTIER
 * \author Joshua MONTREUIL
 * \date Apr 29, 2023
 * \brief Header file of the robot's logger module. It manage the log file and save the logs inside it.
 *
 * \see controller_logger.c
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

#ifndef SRC_LOGS_CONTROLLER_LOGGER_H_
#define SRC_LOGS_CONTROLLER_LOGGER_H_

/* ----------------------  INCLUDES ------------------------------------------*/
#include "../config.h"
#include "../lib/defs.h"
#include "time.h"
/* ----------------------  PUBLIC CONFIGURATIONS  ----------------------------*/
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/**
 * \enum log_level_e
 * \brief Defines available log levels
 */
typedef enum{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    NONE
}log_level_e;
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int CONTROLLER_LOGGER_create()
 * \brief Creates the module.
 * \author Florentin LEPELTIER
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_LOGGER_create(void);

/**
 * \fn extern int CONTROLLER_LOGGER_start(void)
 * \brief Starts the module.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_LOGGER_start(void);

/**
 * \fn extern int CONTROLLER_LOGGER_stop(void)
 * \brief Stops the module.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_LOGGER_stop(void);

/**
 * \fn extern int CONTROLLER_LOGGER_destroy()
 * \brief Destroys the module.
 * \author Florentin LEPELTIER
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_LOGGER_destroy(void);

/**
 * \fn extern int CONTROLLER_LOGGER_ask_logs(Id_Robot id_robot)
 * \brief Sends back the log file of the robot once loaded
 * \author Florentin LEPELTIER
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_LOGGER_ask_logs(Id_Robot id_robot);

/**
 * \fn extern int CONTROLLER_LOGGER_log(log_level_e log_level, const char* msg)
 * \brief Asks a log entry into the log file.
 * \author Florentin LEPELTIER
 * \author Joshua MONTREUIL
 *
 * \param level : criticality level of the log.
 * \param msg : String for the logger to be printed in the terminal and into the log file.
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_LOGGER_log(log_level_e log_level, const char* msg);

/**
 * \fn extern int CONTROLLER_LOGGER_ask_set_rtc(Id_Robot id_robot,time_t rtc)
 * \brief Requests to change the internal RTC.
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 * \param rtc : RTC given by SB_IHM.
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_LOGGER_ask_set_rtc(Id_Robot id_robot,time_t rtc);

/**
 * \fn extern int CONTROLLER_LOGGER_logs_saved(Id_Robot id_robot)
 * \brief Acks from SB_IHM that the logs has been received.
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_LOGGER_logs_saved(Id_Robot id_robot);

#endif /* SRC_LOGS_CONTROLLER_LOGGER_H_ */

