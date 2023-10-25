/**
 * \file  controller_ringer_cmocka.c
 * \version  1.1
 * \author Louison LEGROS
 * \author Joshua MONTREUIL
 * \author Fatoumata TRAORE
 * \date Jun 12, 2023
 * \brief mock file of controller_ringer.
 *
 * \see ../../src/controller/controller_ringer.c
 * \see ../../src/controller/controller_ringer.h
 * \see controller_ringer.c
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
#include "../../src/controller/controller_ringer.h"
#include "../../src/lib/defs.h"

/**
 * \fn int __wrap_CONTROLLER_RINGER_init_failed_pings_var(void)
 * \brief Mock function of init_failed_pings_var.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_ringer.c
 */
int __wrap_CONTROLLER_RINGER_init_failed_pings_var(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_RINGER_ask_availability(void)
 * \brief Mock function of ask_availability.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/controller/controller_ringer.c
 */
int __wrap_CONTROLLER_RINGER_ask_availability(Id_Robot id_robot) {
    function_called();

    check_expected(id_robot);

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_RINGER_can_still_fail_pings()
 * \brief Mock function of can_still_fail_pings.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
int __wrap_CONTROLLER_RINGER_can_still_fail_pings() {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_RINGER_has_too_much_failed_pings()
 * \brief Mock function of has_too_much_failed_pings.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
int __wrap_CONTROLLER_RINGER_has_too_much_failed_pings() {
    function_called();

    return (int) mock();
}