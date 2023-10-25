/**
 * \file gui_secretary_proxy_test.c
 * \version  0.1
 * \author Fatoumata TRAORE
 * \date Apr 30, 2023
 * \brief Test module for gui secretary proxy.
 *
 * \see ../../src/com/gui_secretary_proxy.c
 * \see ../../src/com/gui_secretary_proxy.h
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

#include "../../src/com/gui_secretary_proxy.c"

static int set_up(void **state) {
   return 0;
}

static int tear_down(void **state) {
        return 0;
}

/**
 * \fn static void test_GUI_SECRETARY_PROXY_set_mode(void **state)
 * \brief Unit test of set_mode with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/gui_secretary_proxy.c
 */
static void test_GUI_SECRETARY_PROXY_set_mode(void **state) {
    Operating_Mode expected_mode;
    expected_mode.camera_mode = ENABLED;
    expected_mode.radar_mode = ENABLED;
    expected_mode.buzzer_mode = ENABLED;
    expected_mode.leds_mode = ENABLED;
    Communication_Protocol_Head expected_msg;
    expected_msg.msg_size = htons(0x0006);
    expected_msg.msg_type = htons(SET_MODE);
    uint8_t expected_data[6] = {0x00, 0x01, 0x01, 0x00, 0x00, 0x00};

    expect_function_call(__wrap_POSTMAN_send_request);
    expect_memory(__wrap_POSTMAN_send_request, data, &expected_msg, 4);
    //expect_memory(__wrap_POSTMAN_send_request, data + 4, expected_data, 6);
    will_return(__wrap___wrap_POSTMAN_send_request,0);

    int result = GUI_SECRETARY_PROXY_set_mode(ID_ROBOT, expected_mode);

    assert_int_equal(result, 0);
}
/**
 * \fn static void test_GUI_SECRETARY_PROXY_disconnected_ok(void **state)
 * \brief Unit test of disconnected_ok with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/gui_secretary_proxy.c
 */
static void test_GUI_SECRETARY_PROXY_disconnected_ok(void **state) {
        Communication_Protocol_Head expected_msg;
        expected_msg.msg_size = htons(0x0002);
        expected_msg.msg_type = htons(ACK_DISCONNECTION);
        uint8_t expected_data[6] = {0x00, 0x02, 0x00, 0x04};

        expect_function_call(__wrap_POSTMAN_send_request);
        expect_memory(__wrap_POSTMAN_send_request, data, expected_data, sizeof(expected_data));
        will_return(__wrap___wrap_POSTMAN_send_request,0);

        int expected = GUI_SECRETARY_PROXY_disconnected_ok(ID_ROBOT);

        assert_int_equal(expected, 0);       
}
/**
 * \fn static void test_GUI_SECRETARY_PROXY_set_radar(void **state)
 * \brief Unit test of set_radar with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/gui_secretary_proxy.c
 */
static void test_GUI_SECRETARY_PROXY_set_radar(void **state) {
    // Set up test data
    Communication_Protocol_Head expected_msg;
    expected_msg.msg_size = htons(0x0003);
    expected_msg.msg_type = htons(SET_RADAR);
    uint8_t expected_data[6] = {0x00, 0x03, 0x00, 0x05, 0x01};

    expect_function_call(__wrap_POSTMAN_send_request);
    expect_memory(__wrap_POSTMAN_send_request, data, expected_data, sizeof(expected_data));
    will_return(__wrap___wrap_POSTMAN_send_request,0);
    int result = GUI_SECRETARY_PROXY_set_radar(ID_ROBOT, TRUE);

    // Check the result and mock function calls
    assert_int_equal(result, 0);
}   

/**
 * \struct CMUnitTest
 * \brief Lists the test suite for the module
 */
static const struct CMUnitTest tests[] = {

        cmocka_unit_test(test_GUI_SECRETARY_PROXY_set_mode),
        //cmocka_unit_test(test_GUI_SECRETARY_PROXY_disconnected_ok),
        //cmocka_unit_test(test_GUI_SECRETARY_PROXY_set_radar),

};

/**
 * \fn int GUI_SECRETARY_PROXY_TEST_run_tests()
 * \brief Module tests suite launch.
 */
int GUI_SECRETARY_PROXY_TEST_run_tests() {

        return cmocka_run_group_tests_name("Test du module ...", tests, set_up, tear_down);

}
