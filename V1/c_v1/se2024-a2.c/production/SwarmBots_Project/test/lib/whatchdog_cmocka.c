/**
 * \file  watch_cmocka.c
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date Jun 06, 2023
 * \brief mock file of watchdog.
 *
 * \see ../../src/lib/watchdog.c
 * \see ../../src/lib/watchdog.h
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
#include "../../src/lib/watchdog.h"

/**
 * \fn void __wrap_watchdog_cancel(watchdog_t * watchdog)
 * \brief Mock function of watchdog_cancel.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/lib/watchdog.c
 */
void __wrap_watchdog_cancel(watchdog_t * watchdog) {
    function_called();

    check_expected_ptr(watchdog);
}
/**
 * \fn void __wrap_watchdog_start(watchdog_t * watchdog)
 * \brief Mock function of watchdog_start.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/lib/watchdog.c
 */
void __wrap_watchdog_start(watchdog_t * watchdog) {
    function_called();

    check_expected_ptr(watchdog);
}

