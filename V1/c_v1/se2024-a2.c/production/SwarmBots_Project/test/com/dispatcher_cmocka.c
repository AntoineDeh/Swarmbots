/**
 * \file  dispatcher_cmocka.c
 * \author Fatoumata TRAORE
 * \date Jun 12, 2023
 * \brief mock file of dispatcher.
 *
 * \see ../../src/com/dispatcher.c
 * \see ../../src/com/dispatcher.h
 * \see dispatcher_test.c
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

#include "../../src/com/dispatcher.h"
#include "../../src/lib/defs.h"

/**
 * \fn Communication_Protocol_Head __wrap_DISPATCHER_decode_message(uint8_t* raw_message)
 * \brief Mock function of decode_message.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
Communication_Protocol_Head __wrap_DISPATCHER_decode_message(uint8_t* raw_message) {
    function_called();
    check_expected_ptr(raw_message);
    Communication_Protocol_Head msg = {mock(),mock()};
    return msg;
}
/**
 * \fn int __wrap_DISPATCHER_dispatch_received_msg(Communication_Protocol_Head msg)
 * \brief Mock function of received_msg.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
int __wrap_DISPATCHER_dispatch_received_msg(Communication_Protocol_Head msg) {
    function_called();
    check_expected(msg.msg_size);
    check_expected(msg.msg_type);
    return (int)mock();
}
