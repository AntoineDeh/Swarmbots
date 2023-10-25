/**
 * \file  radar.h
 * \version  0.1
 * \author Louison LEGROS
 * \date May 30, 2023
 * \brief Header file of the radar peripheral.
 *
 * \see radar.c
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
#ifndef SRC_ALPHABOT2_RADAR_H_
#define SRC_ALPHABOT2_RADAR_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include "../lib/defs.h"
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int RADAR_create()
 * \brief Initialize in memory the object Radar.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int RADAR_create();
/**
 * \fn extern int RADAR_destroy()
 * \brief Destruct the object Radar.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int RADAR_destroy();
/**
 * \fn extern int RADAR_get_radar()
 * \brief Checks the presence or not of the obstacle.
 * \author Louison LEGROS
 * 
 * \param obstacle_state : pointer to fill with obstacle state. 1 if an obstacle is present, 0 otherwise.
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int RADAR_get_radar(bool_e * obstacle_state);

#endif /* SRC_ALPHABOT2_RADAR_H_ */

