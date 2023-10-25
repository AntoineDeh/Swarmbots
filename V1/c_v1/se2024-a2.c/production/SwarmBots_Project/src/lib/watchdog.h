/**
 * \file  watchdog.h
 * \version  1.1
 * \author Dimitri SOLET
 * \author Louison LEGROS
 * \date Mar 14, 2023
 * \brief Watchdog module for callback functions with timer.
 *
 * \see watchdog.c
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
#ifndef _WATCHDOG_H
#define _WATCHDOG_H
/* ----------------------  INCLUDES ------------------------------------------*/
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/**
 * \typedef watchdog_t
 * \brief definition of watchdog object.
 */
typedef struct watchdog_t watchdog_t;
/**
 * \typedef void (* watchdog_callback)(watchdog_t * watchdog)
 * \brief callback watchdog definition
 */
typedef void (* watchdog_callback)(watchdog_t * watchdog);
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn watchdog_t * watchdog_create(unsigned int delay, watchdog_callback callback)
 * \brief Creation of watchdog create.
 * \author Dimitri SOLET
 *
 * \param delay : Timeout before callback call.
 * \param callback : Reference of the callback function to call.
 *
 * \return watchdog object.
 */
watchdog_t * watchdog_create(unsigned int delay, watchdog_callback callback);
/**
 * \fn void watchdog_start(watchdog_t * watchdog)
 * \brief Creation of watchdog create.
 * \author Dimitri SOLET
 * \author Louison LEGROS
 *
 * \param watchdog : watchdog object reference.
 */
void watchdog_start(watchdog_t * watchdog);
/**
 * \fn void watchdog_cancel(watchdog_t * watchdog)
 * \brief Creation of watchdog create.
 * \author Dimitri SOLET
 *
 * \param watchdog : watchdog object reference.
 */
void watchdog_cancel(watchdog_t * watchdog);
/**
 * \fn void watchdog_destroy(watchdog_t * watchdog)
 * \brief Creation of watchdog create.
 * \author Dimitri SOLET
 *
 * \param watchdog : watchdog object reference.
 */
void watchdog_destroy(watchdog_t * watchdog);

#endif /* _WATCHDOG_H */

