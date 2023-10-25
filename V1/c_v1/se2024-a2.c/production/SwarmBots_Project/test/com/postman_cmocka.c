/**
 * \file  postman_cmocka.c
 * \author Louison LEGROS
 * \author Fatoumata TRAORE
 * \date Jun 12, 2023
 * \brief mock file of postman.
 *
 * \see ../../src/com/postman.c
 * \see ../../src/com/postman.h
 * \see postman_test.c
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
/* ----------------------  INCLUDES  ---------------------------------------- */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include "../../src/com/postman.h"

/**
 * \fn int __wrap_POSTMAN_disconnect(void)
 * \brief Mock function of disconnect.
 * \author Lousion LEGROS
 *
 * \see ../../src/com/postman.c
 */
int __wrap_POSTMAN_disconnect(void) {
    function_called();

    return (int) mock();
}
/**
 * \fn uint8_t* __wrap_POSTMAN_read_request(void)
 * \brief Mock function of read_request.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/postman.c
 */
uint8_t* __wrap_POSTMAN_read_request(void) {
    function_called();

    return (uint8_t*)mock();
}

/**
 * \fn int __wrap_POSTMAN_send_request(uint8_t * data)
 * \brief Mock function of send_request.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/postman.c
 */
int __wrap_POSTMAN_send_request(uint8_t * data) {
    function_called();

    check_expected_ptr(data);

    return (int) mock();
}
