/**
 * \file gui_secretary_proxy_cmocka.c
 * \version  1.1
 * \author Joshua MONTREUIL
 * \author Louison LEGROS
 * \date Jun 06, 2023
 * \brief mock file of gui secretary proxy.
 *
 * \see ../../src/com/gui_secretary_proxy.c
 * \see ../../src/com/gui_secretary_proxy.h
 * \see gui_secretary_proxy_test.c
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
#include "../../src/com/gui_secretary_proxy.h"

/**
 * \fn int __wrap_GUI_SECRETARY_PROXY_set_mode(Id_Robot id_robot, Operating_Mode operating_mode)
 * \brief Mock function of set_mode.
 * \author Lousion LEGROS
 *
 * \see ../../src/com/gui_secretary_proxy.c
 */
int __wrap_GUI_SECRETARY_PROXY_set_mode(Id_Robot id_robot, Operating_Mode operating_mode) {
    function_called();
    check_expected(id_robot);
    check_expected(operating_mode.camera_mode);
    check_expected(operating_mode.radar_mode);
    check_expected(operating_mode.buzzer_mode);
    check_expected(operating_mode.leds_mode);
    return (int) mock();
}
/**
 * \fn int __wrap_GUI_SECRETARY_PROXY_ack_connection(Id_Robot id_robot)
 * \brief Mock function of ack_connection.
 * \author Lousion LEGROS
 *
 * \see ../../src/com/gui_secretary_proxy.c
 */
int __wrap_GUI_SECRETARY_PROXY_ack_connection(Id_Robot id_robot) {
    function_called();
    check_expected(id_robot);
    return (int) mock();
}
/**
 * \fn int __wrap_GUI_SECRETARY_PROXY_disconnected_ok(Id_Robot id_robot)
 * \brief Mock function of disconnected_ok.
 * \author Lousion LEGROS
 *
 * \see ../../src/com/gui_secretary_proxy.c
 */
int __wrap_GUI_SECRETARY_PROXY_disconnected_ok(Id_Robot id_robot) {
    function_called();

    check_expected(id_robot);

    return (int) mock();
}
/**
 * \fn int __wrap_GUI_SECRETARY_PROXY_set_radar(Id_Robot id_robot, bool_e radar)
 * \brief Mock function of set_radar.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/com/gui_secretary_proxy.c
 */
int __wrap_GUI_SECRETARY_PROXY_set_radar(Id_Robot id_robot, bool_e radar) {
    function_called();

    return (int) mock();
}




