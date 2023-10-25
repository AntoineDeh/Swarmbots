/**
 * \file  pilot.h
 * \version  0.1
 * \author Florentin LEPELTIER
 * \author Louison LEGROS
 * \date may 11, 2023
 * \brief Header file of the Pilot module. This module manages the motor of the robots and change their directions.
 *
 * \see pilot.c
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

#ifndef SRC_CONTROLLER_PILOT_H_
#define SRC_CONTROLLER_PILOT_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include "../lib/defs.h"
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int PILOT_create(void)
 * \brief Initializes the Pilot object in memory and creates the pilot message queue.
 * \author Florentin LEPELTIER
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int PILOT_create(void);
/**
 * \fn extern int PILOT_destroy(void)
 * \brief Destructs the Pilot object in memory and unlinks the pilot message queue. 
 * \author Florentin LEPELTIER
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int PILOT_destroy(void);
/**
 * \fn extern int PILOT_start(void)
 * \brief Starts the module.
 * \author Florentin LEPELTIER
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int PILOT_start(void);
/**
 * \fn extern int PILOT_stop(void)
 * \brief Stops the module.
 * \author Florentin LEPELTIER
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int PILOT_stop(void);
/**
 * \fn extern int PILOT_ask_cmd(Command cmd)
 * \brief Calls motor.c functions to change robot's behavior
 * \author Florentin LEPELTIER
 *
 * \param cmd : Direction to go.
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int PILOT_ask_cmd(Command cmd);

#endif /* SRC_CONTROLLER_PILOT_H_ */

