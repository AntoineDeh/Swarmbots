/**
 * \file  state_indicator_cmocka.c
 * \version  1.1
 * \author Louison LEGROS
 * \date Jun 12, 2023
 * \brief mock file of state_indicator.
 *
 * \see ../../src/controller/state_indicator.c
 * \see ../../src/controller/state_indicator.h
 * \see state_indicator_test.c
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
#include "../../src/controller/state_indicator.h"

/**
 * \fn int __wrap_STATE_INDICATOR_enable_led(void)
 * \brief Mock function of enable_led.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/state_indicator.c
 */
int __wrap_STATE_INDICATOR_enable_led(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_STATE_INDICATOR_disable_led(void)
 * \brief Mock function of disable_led.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/state_indicator.c
 */
int __wrap_STATE_INDICATOR_disable_led(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_STATE_INDICATOR_enable_buzzer(void)
 * \brief Mock function of enable_buzzer.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/state_indicator.c
 */
int __wrap_STATE_INDICATOR_enable_buzzer(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_STATE_INDICATOR_disable_buzzer(void)
 * \brief Mock function of disable_buzzer.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/state_indicator.c
 */
int __wrap_STATE_INDICATOR_disable_buzzer(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_STATE_INDICATOR_set_state(void)
 * \brief Mock function of set_state.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/state_indicator.c
 */
int __wrap_STATE_INDICATOR_set_state(State state) {
    function_called();

    check_expected(state);

    return (int) mock();
}