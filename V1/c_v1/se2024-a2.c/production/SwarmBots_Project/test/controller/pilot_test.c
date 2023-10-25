/**
 * \file pilot_test.c
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date Apr 30, 2023
 * \brief Test module for the pilot module.
 *
 * \see ../../src/controller/pilot.c
 * \see ../../src/controller/pilot.h
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

#include "../../src/controller/pilot.c"

/**
 * \var mq_msg_test
 * \brief used to verify the mq_msg sent through the mq.
 */
static mq_msg * mq_msg_test;

/**
 * \fn int __wrap_PILOT_add_msg_to_queue(Mq_Msg * a_msg)
 * \brief Mock function of mq_send. Define here because of Mq_Msg being defined in pilot.c
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
int __wrap_PILOT_add_msg_to_queue(mq_msg * msg) {
    function_called();
    mq_msg_test = msg;

    return (int) mock();
}
/**
 * \fn int __wrap_PILOT_action_check_radar(Mq_Msg * a_msg)
 * \brief Mock function of check_radar. Define here because of obstacle_state and Mq_Msg being defined in pilot.c
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
int __wrap_PILOT_action_check_radar(mq_msg * msg) {
    function_called();

    obstacle_state = (bool_e) mock();

    return (int) mock();
}

static int set_up(void **state) {
	return 0;
}

static int tear_down(void **state) {
	return 0;
}

/**
 * \fn static void test_PILOT_ask_cmd(void **state)
 * \brief Unit test of ask_cmd with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_ask_cmd(void **state) {
    int mock_ret = 0;
    Command dt_command = FORWARD, expected_command = dt_command; /* < for Forward command Should work for all command.*/
    event_e expected_event = E_ASK_CMD;
    int expected_return = 0, fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_PILOT_add_msg_to_queue);
    will_return(__wrap_PILOT_add_msg_to_queue, mock_ret);
#endif

    fct_return = PILOT_ask_cmd(dt_command);

    assert_int_equal(expected_command,mq_msg_test->data.cmd);
    assert_int_equal(expected_event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_PILOT_action_nop(void **state)
 * \brief Unit test of action_nop with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_action_nop(void **state) {
    int expected_return = 0, fct_return;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = 0};

    fct_return = PILOT_action_nop(&expected_msg);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_PILOT_action_evaluate_cmd(void **state)
 * \brief Unit test of action_evaluate_cmd with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_action_evaluate_cmd(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = FORWARD}; /* < If we are moving forward */
    event_e expected_event = E_GO_MOVE_FORWARD;

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_PILOT_add_msg_to_queue);
    will_return(__wrap_PILOT_add_msg_to_queue, mock_ret);
#endif

    fct_return = PILOT_action_evaluate_cmd(&expected_msg);

    assert_int_equal(expected_event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);

    expected_msg.data.cmd = BACKWARD; /* < If we are moving backward or else */
    expected_event = E_GO_IDLE;

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_PILOT_add_msg_to_queue);
    will_return(__wrap_PILOT_add_msg_to_queue, mock_ret);
#endif

    fct_return = PILOT_action_evaluate_cmd(&expected_msg);

    assert_int_equal(expected_event,mq_msg_test->data.event);
    assert_int_equal(expected_msg.data.cmd,mq_msg_test->data.cmd);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_PILOT_action_move_robot(void **state)
 * \brief Unit test of action_move_robot with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_action_move_robot(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    Command dt_cmd = LEFT, expected_cmd = dt_cmd;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = dt_cmd};

    expect_function_call(__wrap_MOTOR_set_velocity);
    expect_value(__wrap_MOTOR_set_velocity,cmd,expected_cmd);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    fct_return = PILOT_action_move_robot(&expected_msg);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_PILOT_action_move_robot_forward(void **state)
 * \brief Unit test of action_move_robot_forward with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_action_move_robot_forward(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    Command dt_cmd = FORWARD, expected_cmd = dt_cmd;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = dt_cmd};
    obstacle_state = TRUE;
    event_e expected_event = E_OBSTACLE_DETECTED;

    expect_function_call(__wrap_MOTOR_set_velocity);
    expect_value(__wrap_MOTOR_set_velocity,cmd,expected_cmd);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_PILOT_add_msg_to_queue);
    will_return(__wrap_PILOT_add_msg_to_queue, mock_ret);
#endif

    fct_return = PILOT_action_move_robot_forward(&expected_msg);

    assert_int_equal(expected_event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_PILOT_action_check_radar(void **state)
 * \brief Unit test of action_check_radar with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_action_check_radar(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = 0};
    obstacle_state = FALSE; /* < obstacle state is different from the previous state */
    bool_e expected_obstacle_state = TRUE, expected_radar = TRUE;
    int expected_return = 0, fct_return;

    expect_function_call(__wrap_CONTROLLER_CORE_get_mode);
    will_return(__wrap_CONTROLLER_CORE_get_mode, ENABLED);
    will_return(__wrap_CONTROLLER_CORE_get_mode, ENABLED);
    will_return(__wrap_CONTROLLER_CORE_get_mode, ENABLED);
    will_return(__wrap_CONTROLLER_CORE_get_mode, ENABLED);

    expect_function_call(__wrap_RADAR_get_radar);
    will_return(__wrap_RADAR_get_radar, expected_radar);
    will_return(__wrap_RADAR_get_radar,mock_ret);

    expect_function_call(__wrap_CONTROLLER_CORE_get_id_robot);
    will_return(__wrap_CONTROLLER_CORE_get_id_robot,ID_ROBOT);

    expect_function_call(__wrap_GUI_SECRETARY_PROXY_set_radar);
    will_return(__wrap_GUI_SECRETARY_PROXY_set_radar,mock_ret);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    expect_function_call(__wrap_watchdog_start);
    expect_value(__wrap_watchdog_start, watchdog, pilot_radar_check_watchdog);

    fct_return = PILOT_action_check_radar(&expected_msg);

    assert_int_equal(expected_obstacle_state,obstacle_state);
    assert_int_equal(expected_return,fct_return);

    obstacle_state = FALSE; /* < obstacle state isn't different from the previous state */
    expected_obstacle_state = FALSE;
    expected_radar = FALSE;

    expect_function_call(__wrap_CONTROLLER_CORE_get_mode);
    will_return(__wrap_CONTROLLER_CORE_get_mode, ENABLED);
    will_return(__wrap_CONTROLLER_CORE_get_mode, ENABLED);
    will_return(__wrap_CONTROLLER_CORE_get_mode, ENABLED);
    will_return(__wrap_CONTROLLER_CORE_get_mode, ENABLED);

    expect_function_call(__wrap_RADAR_get_radar);
    will_return(__wrap_RADAR_get_radar, expected_radar);
    will_return(__wrap_RADAR_get_radar,mock_ret);

    expect_function_call(__wrap_watchdog_start);
    expect_value(__wrap_watchdog_start, watchdog, pilot_radar_check_watchdog);

    fct_return = PILOT_action_check_radar(&expected_msg);

    assert_int_equal(expected_obstacle_state,obstacle_state);
    assert_int_equal(expected_return,fct_return);

}
/**
 * \fn static void test_PILOT_action_check_radar_moving_forward(void **state)
 * \brief Unit test of action_check_radar_moving_forward with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_action_check_radar_moving_forward(void **state) {
    int mock_ret = 0;
    bool_e expected_obstacle_state = TRUE;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = 0};
    event_e expected_event = E_OBSTACLE_DETECTED;
    int expected_return = 0, fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_PILOT_action_check_radar);
    will_return(__wrap_PILOT_action_check_radar, expected_obstacle_state);
    will_return(__wrap_PILOT_action_check_radar,mock_ret);
#endif

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_PILOT_add_msg_to_queue);
    will_return(__wrap_PILOT_add_msg_to_queue, mock_ret);
#endif

    fct_return = PILOT_action_check_radar_moving_forward(&expected_msg);

    assert_int_equal(expected_event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);

}
/**
 * \fn static void test_PILOT_action_stop_to_obstacle(void **state)
 * \brief Unit test of action_stop_to_obstacle with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_action_stop_to_obstacle(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = 0};

    expect_function_call(__wrap_MOTOR_set_velocity);
    expect_value(__wrap_MOTOR_set_velocity,cmd,STOP);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    expect_function_call(__wrap_STATE_INDICATOR_set_state);
    expect_value(__wrap_STATE_INDICATOR_set_state,state,EMERGENCY);
    will_return(__wrap_STATE_INDICATOR_set_state, mock_ret);

    fct_return = PILOT_action_stop_to_obstacle(&expected_msg);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_PILOT_action_stop(void **state)
 * \brief Unit test of action_stop with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_action_stop(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = 0};

    expect_function_call(__wrap_watchdog_cancel);
    expect_value(__wrap_watchdog_cancel, watchdog, pilot_radar_check_watchdog);

    expect_function_call(__wrap_MOTOR_set_velocity);
    expect_value(__wrap_MOTOR_set_velocity,cmd,STOP);

    fct_return = PILOT_action_stop(&expected_msg);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_PILOT_check_radar_time_out(void **state)
 * \brief Unit test of check_radar_time_out with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/pilot.c
 */
static void test_PILOT_check_radar_time_out(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg = {.data.event = 0, .data.cmd = 0};
    event_e expected_event = E_TIME_OUT_RADAR;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_PILOT_add_msg_to_queue);
    will_return(__wrap_PILOT_add_msg_to_queue, mock_ret);
#endif

    PILOT_check_radar_time_out(pilot_radar_check_watchdog);

    assert_int_equal(expected_event,mq_msg_test->data.event);
}

/**
 * \struct CMUnitTest
 * \brief Lists the test suite for the module
 */
static const struct CMUnitTest tests[] = {

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    cmocka_unit_test(test_PILOT_action_check_radar),
#else
    cmocka_unit_test(test_PILOT_action_stop),
    cmocka_unit_test(test_PILOT_check_radar_time_out),
    cmocka_unit_test(test_PILOT_action_stop_to_obstacle),
    cmocka_unit_test(test_PILOT_action_check_radar_moving_forward),
    cmocka_unit_test(test_PILOT_action_move_robot_forward),
    cmocka_unit_test(test_PILOT_action_move_robot),
    cmocka_unit_test(test_PILOT_action_evaluate_cmd),
    cmocka_unit_test(test_PILOT_action_nop),
    cmocka_unit_test(test_PILOT_ask_cmd),
#endif
};

/**
 * \fn int PILOT_TEST_run_tests()
 * \brief Module tests suite launch.
 */
int PILOT_TEST_run_tests() {
	return cmocka_run_group_tests_name("Test du module ...", tests, set_up, tear_down);
}


