/**
 * \file  controller_core.h
 * \version  1.1
 * \author Joshua MONTREUIL
 * \date Apr 29, 2023
 * \brief header file of the "core" module of the robot. This module manages the
 * operating mode of its peripherals and indicates to its sub-modules its own state.
 * It also establish the connection between SB_C and SB_IHM.
 *
 *
 * \see controller_core.c
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

#ifndef SRC_CONTROLLER_CONTROLLER_CORE_H_
#define SRC_CONTROLLER_CONTROLLER_CORE_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include "../lib/defs.h"
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIABLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern void CONTROLLER_CORE_create(void)
 * \brief Initialize in memory the object Controller Core.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_create(void);
/**
 * \fn extern void CONTROLLER_CORE_destroy(void)
 * \brief Destruct the object Controller Core from memory.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_destroy(void);
/**
 * \fn extern void CONTROLLER_CORE_start(void)
 * \brief Starts the module.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_start(void);
/**
 * \fn extern void CONTROLLER_CORE_stop(void)
 * \brief Stops the module.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_stop(void);
/**
 * \fn extern int CONTROLLER_CORE_ask_to_connect(Id_Robot id_robot)
 * \brief Establishment of a connection between SB_IHM and the robot.
 * \author Joshua MONTREUIL.
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_ask_to_connect(Id_Robot id_robot);
/**
 * \fn extern int CONTROLLER_CORE_ask_to_disconnect(Id_Robot id_robot)
 * \brief Disconnects the robot from the network.
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_ask_to_disconnect(Id_Robot id_robot);
/**
 * \fn extern int CONTROLLER_CORE_ask_set_state(Id_Robot id_robot, State state)
 * \brief Changes the state of the robot.
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 * \param state : state to be changed into.
 * \see State
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_ask_set_state(Id_Robot id_robot, State state);
/**
 * \fn extern int CONTROLLER_CORE_ask_mode(Id_Robot id_robot)
 * \brief Gives the robot mode.
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_ask_mode(Id_Robot id_robot);
/**
 * \fn extern int CONTROLLER_CORE_ask_set_mode(Id_Robot id_robot, Operating_Mode operating_mode)
 * \brief Sets the robot mode.
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 * \param operating_mode : operating mode to be changed into.
 * \see Operating_Mode
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_ask_set_mode(Id_Robot id_robot, Operating_Mode operating_mode);
/**
 * \fn extern int CONTROLLER_CORE_get_id_robot(void)
 * \brief Gets the robot's identifier.
 * \author Joshua MONTREUIL
 *
 * \return Returns the robot identifier (Id_Robot).
 * \see Id_Robot
 */
extern Id_Robot CONTROLLER_CORE_get_id_robot(void);
/**
 * \fn extern Operating_Mode CONTROLLER_CORE_get_mode(void)
 * \brief Gets the robot's operating mode.
 * \author Joshua MONTREUIL
 *
 * \return Returns the robot operating mode (Operating_Mode).
 * \see Operating_Mode
 */
extern Operating_Mode CONTROLLER_CORE_get_mode(void);
/**
 * \fn extern int CONTROLLER_CORE_connection_lost(void)
 * \brief Indicates the loss of connection with SB_IHM.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CONTROLLER_CORE_connection_lost(void);

#endif /* SRC_CONTROLLER_CONTROLLER_CORE_H_ */

