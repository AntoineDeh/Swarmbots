/**
 * \file  controller_ringer.h
 * \version  0.1
 * \author Louison LEGROS
 * \date Jun 4, 2023
 * \brief Header file of the ringer on the SB_C side. This module watch the connection
 * status between SB_IHM and SB_C by responding "pong" when a "ping" is received.
 *
 * \see controller_ringer.c
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
#ifndef SRC_CONTROLLER_CONTROLLER_RINGER_H_
#define SRC_CONTROLLER_CONTROLLER_RINGER_H_
/* ----------------------  INCLUDES ------------------------------------------*/
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int CONTROLLER_RINGER_create(void)
 * \brief Initialize in memory the object Controller Ringer.
 * \author Louison LEGROS
 */
extern int CONTROLLER_RINGER_create(void);
/**
 * \fn extern int CONTROLLER_RINGER_destroy(void)
 * \brief Destruct the object Controller Ringer from memory.
 * \author Louison LEGROS
 */
extern int CONTROLLER_RINGER_destroy(void);
/**
 * \fn extern int CONTROLLER_RINGER_start(void)
 * \brief Starts the module.
 * \author Louison LEGROS
 */
extern int CONTROLLER_RINGER_start(void);
/**
 * \fn extern int CONTROLLER_RINGER_stop(void)
 * \brief Stops the module.
 * \author Louison LEGROS
 */
extern int CONTROLLER_RINGER_stop(void);
/**
 * \fn extern int CONTROLLER_RINGER_ask_availability(int id_robot)
 * \brief Answers with a "pong" from the "ping" sent by GUI_Ringer.
 * \author Louison LEGROS
 *
 * \param id_robot : robot identifier.
 */
extern int CONTROLLER_RINGER_ask_availability(int id_robot);
/**
 * \fn extern int CONTROLLER_RINGER_init_failed_pings_var(void)
 * \brief Initialization of the var for the count of missed ping.
 * \author Louison LEGROS
 */
extern int CONTROLLER_RINGER_init_failed_pings_var(void);

#endif /* SRC_CONTROLLER_CONTROLLER_RINGER_H_ */

