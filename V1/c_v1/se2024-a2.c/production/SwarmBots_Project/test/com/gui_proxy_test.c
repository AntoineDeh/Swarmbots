/**
 * \file  gui_proxy.c
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date May 3, 2023
 * \brief Test module for the gui_proxy.
 *
 * \see ../../src/com/gui_proxy.c
 * \see ../../src/com/gui_proxy.h
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
#include "cmocka.h"

#include "../../src/com/gui_proxy.c"
#include "../../src/config.h"

static int set_up(void **state) {
    return 0;
}

static int tear_down(void **state) {
    return 0;
}

/**
 * \fn static void test_GUI_PROXY_raise_memory_alert(void **state)
 * \brief Unit test of raise_memory_alert with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/com/gui_proxy.c
 */
static void test_GUI_PROXY_raise_memory_alert(void** state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    Communication_Protocol_Head expected_msg_to_send;
    expected_msg_to_send.msg_type = htons(SET_AVAILABILITY);
    expected_msg_to_send.msg_size = htons(0x0003);
    uint8_t buf[1] = {MEMORY};
    uint8_t expected_data[5];
    memcpy(expected_data, &expected_msg_to_send, 4);
    memcpy(expected_data + 4, &buf, 1);

    expect_function_call(__wrap_POSTMAN_send_request);
    expect_memory(__wrap_POSTMAN_send_request, data, expected_data, 4);
    will_return(__wrap_POSTMAN_send_request,mock_ret);

    fct_return = GUI_PROXY_raise_memory_alert(ID_ROBOT);

    assert_int_equal(expected_return, fct_return);
}

/**
 * \struct CMUnitTest
 * \brief Lists the test suite for the module
 */
static const struct CMUnitTest tests[] = {
        cmocka_unit_test(GUI_PROXY_raise_memory_alert),
};

/**
 * \fn int GUI_PROXY_TEST_run_tests()
 * \brief Module tests suite launch.
 */
int GUI_PROXY_TEST_run_tests() {
    return cmocka_run_group_tests_name("Test du module ...", tests, set_up, tear_down);
}