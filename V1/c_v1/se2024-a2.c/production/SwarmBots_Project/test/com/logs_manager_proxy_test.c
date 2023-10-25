/**
 * \file  logs_manager_proxy_test.c
 * \version  0.1
 * \author Fatoumata TRAORE
 * \date jun 04, 2023
 * \brief Test module for logs manager proxy.
 *
 * \see ../../src/com/logs_manager_proxy.c
 * \see ../../src/com/logs_manager_proxy.h
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

#include "../../src/com/logs_manager_proxy.c"

static int set_up(void **state) {
        return 0;
}

static int tear_down(void **state) {
        return 0;
}

/**
 * \fn static void test_LOGS_MANAGER_PROXY_set_logs(void **state)
 * \brief Unit test of set_logs with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/logs_manager_proxy.c
 */
static void test_LOGS_MANAGER_PROXY_set_logs(void **state) {
    uint8_t log_list[] = {0x00, 0x04, 0x01, 0x02, 0x03, 0x04};
    uint8_t expected_data [8];
    Communication_Protocol_Head msg_to_send;
    msg_to_send.msg_size = htons(0x0006);
    msg_to_send.msg_type = htons(SET_MODE);

    expect_function_call(__wrap_POSTMAN_send_request);
    expect_memory_count(__wrap_POSTMAN_send_request, data, expected_data,8,1);
    will_return(__wrap_POSTMAN_send_request,0);

    // Perform the function call
    int result = LOGS_MANAGER_PROXY_set_logs(log_list);

    // Check the result and mock function calls
    assert_int_equal(result, 0);
    free(log_list);
}

/**
 * \struct CMUnitTest
 * \brief Lists the test suite for the module
 */
static const struct CMUnitTest tests[] = {
	    cmocka_unit_test(test_LOGS_MANAGER_PROXY_set_logs),
};

/**
 * \fn int LOGS_MANAGER_PROXY_TEST_run_tests()
 * \brief Module tests suite launch.
 */
int LOGS_MANAGER_PROXY_TEST_run_tests() {
        return cmocka_run_group_tests_name("Test du module ...", tests, set_up, tear_down);
}
