/**
 * \file  state_indicator_cmocka.c
 * \version  1.1
 * \author Louison LEGROS
 * \author Joshua MONTREUIL
 * \author Fatoumata TRAORE
 * \date Jun 06, 2023
 * \brief mock file of controller_core.
 *
 * \see ../../src/controller/controller_core.c
 * \see ../../src/controller/controller_core.h
 * \see controller_core_test.c
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

#include "../../src/lib/defs.h"
#include "../../src/controller/controller_core.h"

/**
 * \fn int __wrap_CONTROLLER_CORE_apply_edit_mode(Operating_Mode operating_mode)
 * \brief Mock function of apply_edit_mode.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_apply_edit_mode(Operating_Mode operating_mode) {
    function_called();

    check_expected(operating_mode.camera_mode);
    check_expected(operating_mode.radar_mode);
    check_expected(operating_mode.buzzer_mode);
    check_expected(operating_mode.leds_mode);

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_CORE_set_operating_mode(Operating_Mode operating_mode)
 * \brief Mock function of set_operating_mode.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
void __wrap_CONTROLLER_CORE_set_operating_mode(Operating_Mode operating_mode) {
    function_called();

    check_expected(operating_mode.camera_mode);
    check_expected(operating_mode.radar_mode);
    check_expected(operating_mode.buzzer_mode);
    check_expected(operating_mode.leds_mode);
}
/**
 * \fn int __wrap_CONTROLLER_CORE_disconnect_core(void)
 * \brief Mock function of disconnect_core.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_disconnect_core(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_CORE_init_hardware(void)
 * \brief Mock function of init_hardware.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_init_hardware(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_CORE_connection_lost(void)
 * \brief Mock function of connection_lost.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_connection_lost(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn Operating_Mode __wrap_CONTROLLER_CORE_get_mode(void)
 * \brief Mock function of get_mode.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_core.c
 */
Operating_Mode __wrap_CONTROLLER_CORE_get_mode(void) {
    function_called();

    Operating_Mode op = {mock(),mock(),mock(),mock()};

    return op;
}
/**
 * \fn Id_Robot __wrap_CONTROLLER_CORE_get_id_robot(void)
 * \brief Mock function of get_id_robot.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_core.c
 */
Id_Robot __wrap_CONTROLLER_CORE_get_id_robot(void) {
    function_called();

    return (Id_Robot) mock();
}
/**
 * \fn int __wrap_CONTROLLER_CORE_ask_set_state(Id_Robot id_robot, State state)
 * \brief Mock function of ask_set_state.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_ask_set_state(Id_Robot id_robot, State state) {
    function_called();

    check_expected(id_robot);
    check_expected(state);

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_CORE_ask_mode(Id_Robot id_robot)
 * \brief Mock function of ask_mode.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_ask_mode(Id_Robot id_robot) {
    function_called();

    check_expected(id_robot);

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_CORE_ask_set_mode(Id_Robot id_robot, Operating_Mode operating_mode)
 * \brief Mock function of ask_set_mode.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_ask_set_mode(Id_Robot id_robot, Operating_Mode operating_mode) {
    function_called();

    check_expected(id_robot);
    check_expected(operating_mode.camera_mode);
    check_expected(operating_mode.radar_mode);
    check_expected(operating_mode.buzzer_mode);
    check_expected(operating_mode.leds_mode);

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_CORE_ask_to_disconnect(Id_Robot id_robot)
 * \brief Mock function of ask_to_disconnect.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_ask_to_disconnect(Id_Robot id_robot) {
    function_called();
    check_expected(id_robot);
    return (int) mock();
}

