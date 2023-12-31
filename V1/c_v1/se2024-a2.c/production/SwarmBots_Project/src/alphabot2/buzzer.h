/**
 * \file  buzzer.h
 * \version  0.1
 * \author Louison LEGROS
 * \date May 18, 2023
 * \brief Header file of the buzzer peripheral.
 *
 * \see buzzer.c
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
#ifndef SRC_ALPHABOT2_BUZZER_H_
#define SRC_ALPHABOT2_BUZZER_H_
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int BUZZER_create()
 * \brief Initialize in memory the object Buzzer.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int BUZZER_create();
/**
 * \fn extern void BUZZER_destroy()
 * \brief Destruct the object Buzzer.
 * \author Louison LEGROS
 */
extern void BUZZER_destroy();
/**
 * \fn extern void BUZZER_enable_buzzer()
 * \brief start the buzzing.
 * \author Louison LEGROS
 */
extern void BUZZER_enable_buzzer();
/**
 * \fn extern void BUZZER_disable_buzzer()
 * \brief stop the buzzing.
 * \author Louison LEGROS
 */
extern void BUZZER_disable_buzzer();

#endif /* SRC_ALPHABOT2_BUZZER_H_ */

