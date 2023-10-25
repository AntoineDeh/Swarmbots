/**
 * \file  leds.h
 * \version  0.1
 * \author LEGROS Louison
 * \date May 09, 2023
 * \brief Header file of the leds peripheral.
 *
 * \see leds_std.c / leds_ws2811.c
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
#ifndef SRC_ALPHABOT2_LEDS_H_
#define SRC_ALPHABOT2_LEDS_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include <stdint.h>
/* ----------------------  PUBLIC CONFIGURATION ---------------------------*/
/**
 * \def NUMBER_OF_LEDS
 * Number of leds on the robot.
 */
#define NUMBER_OF_LEDS  4
/**
 * \def ALL_LEDS
 * Used to set all leds at the same time.
 */
#define ALL_LEDS        -1
/**
 * \brief enumeration of available colors
 */
typedef enum {
    NO_COLOR = 0,
    RED = 0x00000020,
    BLUE = 0x00200000,
    GREEN = 0x00002000,
    PINK = 0x00200010
}color_e;

/**
 * \typedef int id_led_t
 * leds identifier.
 */
typedef int id_led_t;
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int LEDS_create(void)
 * \brief Initializes the object leds.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int LEDS_create(void);
/**
 * \fn extern int LEDS_destroy(void)
 * \brief Destroys the object leds.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int LEDS_destroy(void);
/**
 * \fn extern int LEDS_start(void)
 * \brief Starts the module.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int LEDS_start(void);
/**
 * \fn extern int LEDS_stop(void)
 * \brief Stops the module.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int LEDS_stop(void);
/**
 * \fn extern int LEDS_set_color(id_led_t p_id_led, color_e p_color)
 * \brief Changes the color of the led. If blinking is activated, the blinking will be stopped.
 * \author Louison LEGROS
 *
 * \param p_id_led id of the targeted led, -1 to target all leds
 * \param p_color color to apply to the targeted led
 * 
 * \return On success, returns 0. On error, returns -1.
 */
extern int LEDS_set_color(id_led_t p_id_led, color_e p_color);
/**
 * \fn extern int LEDS_start_blinking()
 * \brief Starts the blinking of the leds.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
*/
extern int LEDS_start_blinking();

#endif // SRC_ALPHABOT2_LEDS_H_