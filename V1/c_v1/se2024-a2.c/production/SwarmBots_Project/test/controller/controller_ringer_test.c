/**
 * \file controller_ringer_test.c
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date Apr 30, 2023
 * \brief Test module for the controller ringer module.
 *
 * \see ../../src/controller/controller_ringer.c
 * \see ../../src/controller/controller_ringer.h
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

#include "../../src/controller/controller_ringer.c"

/**
 * \var mq_msg_test
 * \brief used to verify the mq_msg sent through the mq.
 */
static mq_msg * mq_msg_test;

/**
 * \fn int __wrap_CONTROLLER_RINGER_add_msg_to_queue(Mq_Msg * a_msg)
 * \brief Mock function of mq_send. Define here because of Mq_Msg being defined in controller_ringer.c
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
int __wrap_CONTROLLER_RINGER_add_msg_to_queue(mq_msg * msg) { //La position n'est pas "propre" ou "idéale" mais nécessaire pour vérifier le passage d'un bon msg dans la mq.
    function_called();
    /* Paramètre non vérifié car il ne fait pas parti d'un effet de bord */
    mq_msg_test = msg;

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_RINGER_update_failed_pings(Mq_Msg * a_msg)
 * \brief Mock function of update_failed_pings. Define here because of failed_pings being defined in controller_ringer.c
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
int __wrap_CONTROLLER_RINGER_update_failed_pings(int value) {
    function_called();

    failed_pings = (int) mock();

    return (int) mock();
}

static int set_up(void **state) {
	return 0;
}

static int tear_down(void **state) {
	return 0;
}

/**
 * \fn static void test_CONTROLLER_RINGER_ask_availability(void **state)
 * \brief Unit test of ask_availability with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_ask_availability(void **state) {
    int mock_ret = 0;
    int dt_id_robot = 0x02, expected_id_robot = dt_id_robot;
    mq_msg expected_msg = {.data.event = E_ASK_AVAILABILITY, .data.id_robot = dt_id_robot};
    int expected_return = 0, fct_return;

#ifdef _WRAP_MQ_CONTROLLER_RINGER_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_add_msg_to_queue);
    will_return(__wrap_CONTROLLER_RINGER_add_msg_to_queue, mock_ret);
#endif

    fct_return = CONTROLLER_RINGER_ask_availability(dt_id_robot);

    assert_int_equal(expected_id_robot,mq_msg_test->data.id_robot);
    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_init_failed_pings_var(void **state)
 * \brief Unit test of init_failed_pings_var with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_init_failed_pings_var(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg = {.data.event = E_INIT, .data.id_robot = 0};
    int expected_return = 0, fct_return;

#ifdef _WRAP_MQ_CONTROLLER_RINGER_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_add_msg_to_queue);
    will_return(__wrap_CONTROLLER_RINGER_add_msg_to_queue, mock_ret);
#endif

    fct_return = CONTROLLER_RINGER_init_failed_pings_var();

    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_action_nop(void **state)
 * \brief Unit test of action_nop with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_action_nop(void **state) {
    mq_msg expected_msg = {.data.event = 0, .data.id_robot = 0};
    int expected_return = 0, fct_return;

    fct_return = CONTROLLER_RINGER_action_nop(&expected_msg);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_action_init(void **state)
 * \brief Unit test of action_init with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_action_init(void **state) {
    int mock_ret = 0, mock_attr = 0;
    mq_msg expected_msg = {.data.event = 0, .data.id_robot = 0};
    int expected_return = 0, fct_return, expected_attr = mock_attr;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_update_failed_pings);
    will_return(__wrap_CONTROLLER_RINGER_update_failed_pings, mock_attr);
    will_return(__wrap_CONTROLLER_RINGER_update_failed_pings, mock_ret);
#endif

    fct_return = CONTROLLER_RINGER_action_init(&expected_msg);

    assert_int_equal(expected_attr, failed_pings);
    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_action_notify_availability(void **state)
 * \brief Unit test of action_notify_availability with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_action_notify_availability(void **state) {
    int mock_ret = 0, mock_attr = 0;
    mq_msg expected_msg = {.data.event = 0, .data.id_robot = ID_ROBOT};
    int expected_return = 0, fct_return, expected_attr = mock_attr;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_update_failed_pings);
    will_return(__wrap_CONTROLLER_RINGER_update_failed_pings, mock_attr);
    will_return(__wrap_CONTROLLER_RINGER_update_failed_pings, mock_ret);
#endif

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    expect_function_call(__wrap_GUI_RINGER_PROXY_set_availability);
    expect_value(__wrap_GUI_RINGER_PROXY_set_availability, id_robot, expected_msg.data.id_robot);
    will_return(__wrap_GUI_RINGER_PROXY_set_availability, mock_ret);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    expect_function_call(__wrap_watchdog_start);
    expect_value(__wrap_watchdog_start, watchdog, controller_ringer_ping_watchdog);

    fct_return = CONTROLLER_RINGER_action_notify_availability(&expected_msg);

    assert_int_equal(expected_attr, failed_pings);
    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_action_check_failed_pings(void **state)
 * \brief Unit test of action_check_failed_pings with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_action_check_failed_pings(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg = {.data.event = 0, .data.id_robot = 0};
    int expected_return = 0, fct_return;
    failed_pings = 0; /* < When failed pings <= MAX_FAILED_PINGS */

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_can_still_fail_pings);
    will_return(__wrap_CONTROLLER_RINGER_can_still_fail_pings, mock_ret);
#endif

    expect_function_call(__wrap_watchdog_cancel);
    expect_value(__wrap_watchdog_cancel, watchdog, controller_ringer_ping_watchdog);

    fct_return = CONTROLLER_RINGER_action_check_failed_pings(&expected_msg);

    assert_int_equal(expected_return, fct_return);

    failed_pings = 11; /* < When failed pings > MAX_FAILED_PINGS */

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_has_too_much_failed_pings);
    will_return(__wrap_CONTROLLER_RINGER_has_too_much_failed_pings, mock_ret);
#endif

    expect_function_call(__wrap_watchdog_cancel);
    expect_value(__wrap_watchdog_cancel, watchdog, controller_ringer_ping_watchdog);

    fct_return = CONTROLLER_RINGER_action_check_failed_pings(&expected_msg);

    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_action_increase_failed_pings(void **state)
 * \brief Unit test of action_increase_failed_pings with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_action_increase_failed_pings(void **state) {
    int mock_ret = 0, mock_attr = 8;
    mq_msg expected_msg = {.data.event = 0, .data.id_robot = 0};
    int expected_return = 0, fct_return, expected_attr = mock_attr;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_update_failed_pings);
    will_return(__wrap_CONTROLLER_RINGER_update_failed_pings, mock_attr);
    will_return(__wrap_CONTROLLER_RINGER_update_failed_pings, mock_ret);
#endif

    expect_function_call(__wrap_watchdog_start);
    expect_value(__wrap_watchdog_start, watchdog, controller_ringer_ping_watchdog);

    fct_return = CONTROLLER_RINGER_action_increase_failed_pings(&expected_msg);

    assert_int_equal(expected_attr, failed_pings);
    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_action_notify_lost_connection(void **state)
 * \brief Unit test of action_notify_lost_connection with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_action_notify_lost_connection(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg = {.data.event = 0, .data.id_robot = 0};
    int expected_return = 0, fct_return;

    expect_function_call(__wrap_CONTROLLER_CORE_connection_lost);
    will_return(__wrap_CONTROLLER_CORE_connection_lost, mock_ret);

    fct_return = CONTROLLER_RINGER_action_notify_lost_connection(&expected_msg);

    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_action_stop(void **state)
 * \brief Unit test of action_stop with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_action_stop(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg = {.data.event = 0, .data.id_robot = 0};
    int expected_return = 0, fct_return;

    expect_function_call(__wrap_watchdog_cancel);
    expect_value(__wrap_watchdog_cancel, watchdog, controller_ringer_ping_watchdog);

    fct_return = CONTROLLER_RINGER_action_stop(&expected_msg);

    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_update_failed_pings(void **state)
 * \brief Unit test of update_failed_pings with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_update_failed_pings(void **state) {
    int expected_return = 0, fct_return, attr_dt = 5, expected_attr = attr_dt;

    fct_return = CONTROLLER_RINGER_update_failed_pings(attr_dt);

    assert_int_equal(expected_attr, failed_pings);
    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_has_too_much_failed_pings(void **state)
 * \brief Unit test of has_too_much_failed_pings with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_has_too_much_failed_pings(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    event_e expected_event = E_HAS_TOO_MUCH_FAILED_PINGS;

#ifdef _WRAP_MQ_CONTROLLER_RINGER_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_add_msg_to_queue);
    will_return(__wrap_CONTROLLER_RINGER_add_msg_to_queue, mock_ret);
#endif

    fct_return = CONTROLLER_RINGER_has_too_much_failed_pings();

    assert_int_equal(expected_event,mq_msg_test->data.event);
    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_can_still_fail_pings(void **state)
 * \brief Unit test of can_still_fail_pings with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_can_still_fail_pings(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    event_e expected_event = E_CAN_STILL_FAIL_PINGS;

#ifdef _WRAP_MQ_CONTROLLER_RINGER_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_add_msg_to_queue);
    will_return(__wrap_CONTROLLER_RINGER_add_msg_to_queue, mock_ret);
#endif

    fct_return = CONTROLLER_RINGER_can_still_fail_pings();

    assert_int_equal(expected_event,mq_msg_test->data.event);
    assert_int_equal(expected_return, fct_return);
}
/**
 * \fn static void test_CONTROLLER_RINGER_ping_time_out(void **state)
 * \brief Unit test of ping_time_out with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/controller_ringer.c
 */
static void test_CONTROLLER_RINGER_ping_time_out(void **state) {
    int mock_ret = 0;
    event_e expected_event = E_TIME_OUT_PING;

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

#ifdef _WRAP_MQ_CONTROLLER_RINGER_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_RINGER_add_msg_to_queue);
    will_return(__wrap_CONTROLLER_RINGER_add_msg_to_queue, mock_ret);
#endif

    CONTROLLER_RINGER_ping_time_out(controller_ringer_ping_watchdog);

    assert_int_equal(expected_event,mq_msg_test->data.event);
}

/**
 * \struct CMUnitTest
 * \brief Lists the test suite for the module
 */
static const struct CMUnitTest tests[] = {

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    cmocka_unit_test(test_CONTROLLER_RINGER_can_still_fail_pings),
    cmocka_unit_test(test_CONTROLLER_RINGER_has_too_much_failed_pings),
    cmocka_unit_test(test_CONTROLLER_RINGER_update_failed_pings),
#else
    cmocka_unit_test(test_CONTROLLER_RINGER_ping_time_out),
    cmocka_unit_test(test_CONTROLLER_RINGER_ask_availability),
    cmocka_unit_test(test_CONTROLLER_RINGER_init_failed_pings_var),
    cmocka_unit_test(test_CONTROLLER_RINGER_action_stop),
    cmocka_unit_test(test_CONTROLLER_RINGER_action_notify_lost_connection),
    cmocka_unit_test(test_CONTROLLER_RINGER_action_increase_failed_pings),
    cmocka_unit_test(test_CONTROLLER_RINGER_action_check_failed_pings),
    cmocka_unit_test(test_CONTROLLER_RINGER_action_notify_availability),
    cmocka_unit_test(test_CONTROLLER_RINGER_action_nop),
#endif
};

/**
 * \fn int CONTROLLER_RINGER_TEST_run_tests()
 * \brief Module tests suite launch.
 */
int CONTROLLER_RINGER_TEST_run_tests()
{
	return cmocka_run_group_tests_name("Test du module ...", tests, set_up, tear_down);
}


