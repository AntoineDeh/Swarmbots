/**
 * \file gui_secretary_proxy.h
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date May 8, 2023
 * \brief Proxy header file of the gui secretary class.
 *
 * \see gui_secretary_proxy.c
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

#ifndef SRC_COM_GUI_SECRETARY_PROXY_H_
#define SRC_COM_GUI_SECRETARY_PROXY_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include "../lib/defs.h"
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIABLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern void GUI_SECRETARY_PROXY_set_mode(Operating_Mode operating_mode, IdRobot id_robot)
 * \brief Sets the SB_C mode.
 * \author Joshua MONTREUIL
 *
 * \param operating_mode : SB_C current peripherals operating mode.
 * \see Operating_Mode
 * \param id_robot : robot identifier.
 * \see Id_Robot
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int GUI_SECRETARY_PROXY_set_mode(Id_Robot id_robot, Operating_Mode operating_mode);
/**
 * \fn extern void GUI_SECRETARY_PROXY_ack_connection(IdRobot, id_robot)
 * \brief Acks the connection.
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 */
extern void GUI_SECRETARY_PROXY_ack_connection(Id_Robot id_robot);
/**
 * \fn extern int GUI_SECRETARY_PROXY_disconnected_ok(Id_Robot id_robot)
 * \brief SB_C acks the disconnection of SB_IHM.
 * \author Joshua MONTREUIL
 *
 * \param id_robot : robot identifier.
 * \see Id_Robot
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int GUI_SECRETARY_PROXY_disconnected_ok(Id_Robot id_robot);
/**
 * \fn extern int GUI_SECRETARY_PROXY_set_radar(Id_Robot id_robot, bool_e radar)
 * \brief Sends the radar detection data.
 * \author Joshua Montreuil
 *
 * \param id_robot : robot identifier.
 * \param radar : radar data.
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int GUI_SECRETARY_PROXY_set_radar(Id_Robot id_robot, bool_e radar);

#endif /* SRC_COM_GUI_SECRETARY_PROXY_H_ */
