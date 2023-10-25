/**
 * \file  dispatcherest.c
 * \version  0.1
 * \author Fatoumata TRAORE
 * \date Jun 09, 2023
 * \brief Test module for the buzzer module.
 *
 * \see ../../src/com/dispatcher.c
 * \see ../../src/com/dispatcher.h
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

#include "cmocka.h"
#include "../../src/com/dispatcher.c"

/*
 * \var static uint8_t *fake_data_received = NULL
 * Global variable for memory allocation simulation.
 */
static uint8_t *fake_data_received = NULL;

static int set_up(void **state)
{
    fake_data_received = (uint8_t *) malloc(sizeof(uint8_t) * MAX_RECEIVED_BYTES);
    if (fake_data_received == NULL) {
        return -1;
    }
    return 0;
}

static int tear_down(void **state) {

    free(fake_data_received);
    fake_data_received = NULL;
    return 0;
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_ASK_AVAILABILITY(void **state)
 * \brief Unit test of dispatch_received_msg when we have a ASK_AVAILABILITY message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_ASK_AVAILABILITY(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = ASK_AVAILABILITY;
    int ret_mock = 0;
    int ret = 0;

    expect_function_call(__wrap_CONTROLLER_RINGER_ask_availability);
    expect_value(__wrap_CONTROLLER_RINGER_ask_availability, id_robot, ID_ROBOT);
    will_return(__wrap_CONTROLLER_RINGER_ask_availability, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_ASK_CMD(void **state)
 * \brief Unit test of dispatch_received_msg when we have a ASK_CMD message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_ASK_CMD(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = ASK_CMD;
    int ret_mock = 0;
    int ret = 0;
    data_received[0] = 1;

    Command dt_cmd = RIGHT;
    expect_function_call(__wrap_PILOT_ask_cmd);
    expect_value(__wrap_PILOT_ask_cmd, cmd,(Command)data_received[0]);
    will_return(__wrap_PILOT_ask_cmd, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_SET_STATE(void **state)
 * \brief Unit test of dispatch_received_msg when we have a SET_STATE message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_SET_STATE(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = SET_STATE;
    int ret_mock = 0;
    int ret = 0;
    data_received[0] = 1;
    expect_function_call(__wrap_CONTROLLER_CORE_ask_set_state);
    expect_value(__wrap_CONTROLLER_CORE_ask_set_state, id_robot, ID_ROBOT);
    expect_value(__wrap_CONTROLLER_CORE_ask_set_state, state, (State)data_received[0]);
    will_return(__wrap_CONTROLLER_CORE_ask_set_state, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_ASK_MODE(void **state)
 * \brief Unit test of dispatch_received_msg when we have a ASK_MODE message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_ASK_MODE(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = ASK_MODE;
    int ret_mock = 0;
    int ret = 0;

    expect_function_call(__wrap_CONTROLLER_CORE_ask_mode);
    expect_value(__wrap_CONTROLLER_CORE_ask_mode, id_robot, ID_ROBOT);
    will_return(__wrap_CONTROLLER_CORE_ask_mode, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_SET_MODE(void **state)
 * \brief Unit test of dispatch_received_msg when we have a SET_MODE message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_SET_MODE(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = SET_MODE;
    int ret_mock = 0;
    int ret = 0;
    data_received[0] = 1;
    data_received[1] = 2;
    data_received[2] = 3;
    data_received[3] = 4;

    expect_function_call(__wrap_CONTROLLER_CORE_ask_set_mode);
    expect_value(__wrap_CONTROLLER_CORE_ask_set_mode, id_robot, ID_ROBOT);
    expect_value(__wrap_CONTROLLER_CORE_ask_set_mode, operating_mode.camera_mode, (Mode)data_received[0]);
    expect_value(__wrap_CONTROLLER_CORE_ask_set_mode, operating_mode.radar_mode, (Mode)data_received[1]);
    expect_value(__wrap_CONTROLLER_CORE_ask_set_mode, operating_mode.buzzer_mode, (Mode)data_received[2]);
    expect_value(__wrap_CONTROLLER_CORE_ask_set_mode, operating_mode.leds_mode, (Mode)data_received[3]);
    will_return(__wrap_CONTROLLER_CORE_ask_set_mode, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_ASK_LOGS(void **state)
 * \brief Unit test of dispatch_received_msg when we have a ASK_LOGS message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_ASK_LOGS(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = ASK_LOGS;
    int ret_mock = 0;
    int ret = 0;

    expect_function_call(__wrap_CONTROLLER_LOGGER_ask_logs);
    expect_value(__wrap_CONTROLLER_LOGGER_ask_logs, id_robot, ID_ROBOT);
    will_return(__wrap_CONTROLLER_LOGGER_ask_logs, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_ASK_TO_DISCONNECT(void **state)
 * \brief Unit test of dispatch_received_msg when we have a ASK_TO_DISCONNECT message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_ASK_TO_DISCONNECT(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = ASK_TO_DISCONNECT;
    int ret_mock = 0;
    int ret = 0;
    State_Machine ret_state = S_WAITING_RECONNECTION;

    expect_function_call(__wrap_CONTROLLER_CORE_ask_to_disconnect);
    expect_value(__wrap_CONTROLLER_CORE_ask_to_disconnect, id_robot, ID_ROBOT);
    will_return(__wrap_CONTROLLER_CORE_ask_to_disconnect, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
    assert_int_equal(state, ret_state);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_SET_CURRENT_TIME(void **state)
 * \brief Unit test of dispatch_received_msg when we have a SET_CURRENT_TIME message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_SET_CURRENT_TIME(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = SET_CURRENT_TIME;
    int ret_mock = 0;
    int ret = 0;
    data_received[1] = 22;
    data_received[2] = 6;
    data_received[3] = 12;
    data_received[4] = 10;
    data_received[5] = 30;
    data_received[6] = 0;

    expect_function_call(__wrap_CONTROLLER_LOGGER_ask_set_rtc);
    expect_value(__wrap_CONTROLLER_LOGGER_ask_set_rtc, id_robot, ID_ROBOT);
    expect_value(__wrap_CONTROLLER_LOGGER_ask_set_rtc, rtc, 1678915800);
    will_return(__wrap_CONTROLLER_LOGGER_ask_set_rtc, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_LOGS_RECEIVED(void **state)
 * \brief Unit test of dispatch_received_msg when we have a SET_IP_PORT message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_SET_IP_PORT(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = SET_IP_PORT;
    int ret_mock = 0;
    int ret = 0;
    data_received[0] = 192;
    data_received[1] = 168;
    data_received[2] = 0;
    data_received[3] = 1;
    data_received[4] = 10;

    expect_function_call(__wrap_CAMERA_set_up_ihm_info);
    expect_value(__wrap_CAMERA_set_up_ihm_info, id_robot, ID_ROBOT);
    expect_string(__wrap_CAMERA_set_up_ihm_info, ip_address, "192.168.0.1");
    expect_value(__wrap_CAMERA_set_up_ihm_info, port, 10);
    will_return(__wrap_CAMERA_set_up_ihm_info, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_dispatch_received_msg_LOGS_RECEIVED(void **state)
 * \brief Unit test of dispatch_received_msg when we have a LOGS_RECEIVED message type with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_dispatch_received_msg_LOGS_RECEIVED(void** state) {
    Communication_Protocol_Head dt_msg;
    dt_msg.msg_type = LOGS_RECEIVED;
    int ret_mock = 0;
    int ret = 0;

    expect_function_call(__wrap_CONTROLLER_LOGGER_logs_saved);
    expect_value(__wrap_CONTROLLER_LOGGER_logs_saved, id_robot, ID_ROBOT);
    will_return(__wrap_CONTROLLER_LOGGER_logs_saved, ret_mock);

    int expected = DISPATCHER_dispatch_received_msg(dt_msg);
    assert_int_equal(expected, ret);
}
/**
 * \fn static void test_DISPATCHER_decode_message(void **state)
 * \brief Unit test of decode_message with CMOCKA.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/com/dispatcher.c
 */
static void test_DISPATCHER_decode_message(void **state) {
    uint8_t raw_message[] = {0x00, 0x05, 0x12, 0x34, 0x56, 0x78};
    Communication_Protocol_Head expected_msg;
    expected_msg.msg_size = 5;
    expected_msg.msg_type = 0x1234;

    Communication_Protocol_Head result_msg = DISPATCHER_decode_message(raw_message);

    assert_int_equal(result_msg.msg_size, expected_msg.msg_size);
    assert_int_equal(result_msg.msg_type, expected_msg.msg_type);
}

/**
 * \struct CMUnitTest
 * \brief Lists the test suite for the module
 */
static const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_ASK_AVAILABILITY),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_ASK_CMD),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_SET_STATE),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_ASK_MODE),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_SET_MODE),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_ASK_LOGS),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_ASK_TO_DISCONNECT),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_SET_CURRENT_TIME),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_SET_IP_PORT),
    cmocka_unit_test(test_DISPATCHER_dispatch_received_msg_LOGS_RECEIVED),
    cmocka_unit_test(test_DISPATCHER_decode_message),
};

/**
 * \fn int DISPATCHER_run_tests()
 * \brief Module tests suite launch.
 */
int DISPATCHER_run_tests() {
        return cmocka_run_group_tests_name("Test du module ...", tests, set_up, tear_down);
}
