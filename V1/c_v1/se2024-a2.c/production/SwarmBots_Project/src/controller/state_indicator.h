/**
 * \file  state_indicator.h
 * \version  0.1
 * \author Louison LEGROS
 * \date May 11, 2023
 * \brief Header file of the module in charge of indicating the robot state using the leds and buzzer.
 *
 * \see state_indicator.c
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

#ifndef SRC_CONTROLLER_STATE_INDICATOR_H_
#define SRC_CONTROLLER_STATE_INDICATOR_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include "../lib/defs.h"
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int STATE_INDICATOR_create()
 * \brief Initialize the object State Indicator.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_create();
/**
 * \fn extern int STATE_INDICATOR_destroy()
 * \brief Destroys the object State Indicator.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_destroy();
/**
 * \fn extern int STATE_INDICATOR_start()
 * \brief Starts the module.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_start();
/**
 * \fn extern int STATE_INDICATOR_stop()
 * \brief Stops the module.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_stop();
/**
 * \fn extern int STATE_INDICATOR_set_state(State state)
 * \brief Changes the state to be indicated.
 * \author Louison LEGROS
 *
 * \param state : new state describing the system
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_set_state(State state);
/**
 * \fn extern int STATE_INDICATOR_enable_buzzer()
 * \brief Enables the buzzer.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_enable_buzzer();
/**
 * \fn extern int STATE_INDICATOR_disable_buzzer()
 * \brief Disables the buzzer.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_disable_buzzer();
/**
 * \fn extern int STATE_INDICATOR_enable_led()
 * \brief Enables the led.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_enable_led();
/**
 * \fn extern int STATE_INDICATOR_disable_led()
 * \brief Disables the led.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int STATE_INDICATOR_disable_led();

#endif /* SRC_CONTROLLER_STATE_INDICATOR_H_ */

