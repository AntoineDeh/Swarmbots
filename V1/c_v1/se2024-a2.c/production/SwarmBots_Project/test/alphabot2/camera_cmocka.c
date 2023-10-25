/**
 * \file  camera_cmocka.c
 * \author Louison LEGROS
 * \author Fatoumata TRAORE
 * \date Jun 12, 2023
 * \brief mock file of camera.
 *
 * \see ../../src/alphabot2/camera.c
 * \see ../../src/alphabot2/camera.h
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
#include "../../src/alphabot2/camera.h"

/**
 * \fn int __wrap_CAMERA_enable_camera(void)
 * \brief Mock function of enable_camera.
 * \author Louison LEGROS
 *
 * \see ../../src/alphabot2/camera.c
 */
int __wrap_CAMERA_enable_camera(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_CAMERA_disable_camera(void)
 * \brief Mock function of disable_camera.
 * \author Louison LEGROS
 *
 * \see ../../src/alphabot2/camera.c
 */
int __wrap_CAMERA_disable_camera(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_CAMERA_disconnect_camera(void)
 * \brief Mock function of disconnect_camera.
 * \author Louison LEGROS
 *
 * \see ../../src/alphabot2/camera.c
 */
int __wrap_CAMERA_disconnect_camera(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_CAMERA_set_up_ihm_info(char * ip_address, uint16_t port)
 * \brief Mock function of set_up_ihm_info.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/alphabot2/camera.c
 */
int __wrap_CAMERA_set_up_ihm_info(char * ip_address, uint16_t port) {
    function_called();

    check_expected_ptr(ip_address);
    check_expected(port);

    return (int) mock();
}