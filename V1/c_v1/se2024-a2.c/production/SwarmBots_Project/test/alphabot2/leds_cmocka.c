/**
 * \file  leds_cmocka.c
 * \version  1.1
 * \author Joshua MONTREUIL
 * \date Jun 06, 2023
 * \brief mock file of leds.
 *
 * \see ../../src/alphabot2/leds.c
 * \see ../../src/alphabot2/leds.h
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
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"
#include "../../src/alphabot2/leds.h"

/**
 * \fn int __wrap_LEDS_set_color(id_led_t p_id_led, color_e p_color)
 * \brief Mock function of set_color.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/alphabot2/leds.c
 */
int __wrap_LEDS_set_color(id_led_t p_id_led, color_e p_color) {
    function_called();

    check_expected(p_id_led);
    check_expected(p_color);

    return (int) mock();
}
/**
 * \fn int __wrap_LEDS_start_blinking(void)
 * \brief Mock function of start_blinking.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/alphabot2/leds.c
 */
int __wrap_LEDS_start_blinking(void) {
    function_called();

    return (int) mock();
}




