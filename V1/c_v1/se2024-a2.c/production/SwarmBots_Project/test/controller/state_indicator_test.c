/**
 * \file  state_indicator_test.c
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date Apr 30, 2023
 * \brief Test module for the state indicator module.
 *
 * \see ../../src/controller/state_indicator.c
 * \see ../../src/controller/state_indicator.h
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

#include "../../src/controller/state_indicator.c"

/**
 * \var mq_msg_test
 * \brief used to verify the mq_msg sent through the mq.
 */
static mq_msg * mq_msg_test;
/**
 * \fn int __wrap_STATE_INDICATOR_add_msg_to_queue(Mq_Msg * a_msg)
 * \brief Mock function of mq_send. Define here because of Mq_Msg being defined in state_indicator.c
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
int __wrap_STATE_INDICATOR_add_msg_to_queue(mq_msg * msg) {
    function_called();
    mq_msg_test = msg;

    return (int) mock();
}
/**
 * \fn int __wrap_STATE_INDICATOR_action_notify_selected(Mq_Msg * a_msg)
 * \brief Mock function of action_notify_selected. Define here because of Mq_Msg being defined in state_indicator.c
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
int __wrap_STATE_INDICATOR_action_notify_selected(mq_msg * msg) {
    function_called();
#ifdef _VERIF_EFFETS_BORDS
    check_expected_ptr(msg);
#endif
    return (int) mock();
}

static int set_up(void **state) {
	return 0;
}

static int tear_down(void **state) {
	return 0;
}
/**
 * \fn static void test_STATE_INDICATOR_set_state(void **state)
 * \brief Unit test of set_state with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_set_state(void **state) {
    int mock_ret = 0;
    State expected_state, dt_state;
    mq_msg expected_msg = {.data.event = 0, .data.state = 0};
    int expected_return = 0;
    int fct_return;

    /* Test for dt_state -> WAITING_FOR_CONNECTION */
    expected_state = WAITING_FOR_CONNECTION;
    dt_state = WAITING_FOR_CONNECTION;
    expected_msg.data.state = dt_state;
    expected_msg.data.event = E_SET_STATE;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_set_state(dt_state);

    assert_int_equal(expected_state,mq_msg_test->data.state);
    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);

    /* Test for dt_state -> EMERGENCY */
    expected_state = EMERGENCY;
    dt_state = EMERGENCY;
    expected_msg.data.state = EMERGENCY;
    expected_msg.data.event = E_SET_STATE;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_set_state(dt_state);

    assert_int_equal(expected_state,mq_msg_test->data.state);
    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);

    /* Test for dt_state -> SELECTED */
    expected_state = SELECTED;
    dt_state = SELECTED;
    expected_msg.data.state = SELECTED;
    expected_msg.data.event = E_SET_STATE;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_set_state(dt_state);

    assert_int_equal(expected_state,mq_msg_test->data.state);
    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);

    /* Test for dt_state -> NOT_SELECTED */
    expected_state = NOT_SELECTED;
    dt_state = NOT_SELECTED;
    expected_msg.data.state = NOT_SELECTED;
    expected_msg.data.event = E_SET_STATE;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_set_state(dt_state);

    assert_int_equal(expected_state,mq_msg_test->data.state);
    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_enable_led(void **state)
 * \brief Unit test of enable_led with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_enable_led(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg;
    expected_msg.data.event = E_ENABLE_LED;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_enable_led();

    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_disable_led(void **state)
 * \brief Unit test of disable_led with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_disable_led(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg;
    expected_msg.data.event = E_DISABLE_LED;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_disable_led();

    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_enable_buzzer(void **state)
 * \brief Unit test of enable_buzzer with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_enable_buzzer(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg;
    expected_msg.data.event = E_ENABLE_BUZZER;
    int expected_return = 0;
    int fct_return;


#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_enable_buzzer();

    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_disable_buzzer(void **state)
 * \brief Unit test of disable_buzzer with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_disable_buzzer(void **state) {
    int mock_ret = 0;
    mq_msg expected_msg;
    expected_msg.data.event = E_DISABLE_BUZZER;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_disable_buzzer();

    assert_int_equal(expected_msg.data.event,mq_msg_test->data.event);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_STATE_INIDCATOR_flashing_for_connection(void **state)
 * \brief Unit test of flashing_for_connection with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INIDCATOR_flashing_for_connection(void **state) {
    int mock_ret = 0;
    id_led_t expected_p_id_led = ALL_LEDS;
    color_e expected_color = GREEN;

    int expected_ret = 0, fct_return;
    led_activated = TRUE;
    mq_msg expected_msg = {{0,0}};

    expect_function_call(__wrap_LEDS_set_color);
    expect_value(__wrap_LEDS_set_color, p_id_led, expected_p_id_led);
    expect_value(__wrap_LEDS_set_color, p_color, expected_color);
    will_return(__wrap_LEDS_set_color, mock_ret);

    expect_function_call(__wrap_LEDS_start_blinking);
    will_return(__wrap_LEDS_start_blinking, mock_ret);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    fct_return = STATE_INDICATOR_action_flashing_for_connection(&expected_msg);

    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_notify_selected(void **state)
 * \brief Unit test of action_notify_selected with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_notify_selected(void **state) {
    int mock_ret = 0;
    id_led_t expected_p_id_led = ALL_LEDS;
    color_e expected_color = BLUE;

    int expected_ret = 0, fct_return;
    led_activated = TRUE; /* When leds are activated */
    mq_msg expected_msg = {{0,0}};

    expect_function_call(__wrap_LEDS_set_color);
    expect_value(__wrap_LEDS_set_color, p_id_led, expected_p_id_led);
    expect_value(__wrap_LEDS_set_color, p_color, expected_color);
    will_return(__wrap_LEDS_set_color, mock_ret);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    fct_return = STATE_INDICATOR_action_notify_selected(&expected_msg);

    assert_int_equal(expected_ret,fct_return);

    expected_color = NO_COLOR;
    led_activated = FALSE; /* When leds are deactivated */

    expect_function_call(__wrap_LEDS_set_color);
    expect_value(__wrap_LEDS_set_color, p_id_led, expected_p_id_led);
    expect_value(__wrap_LEDS_set_color, p_color, expected_color);
    will_return(__wrap_LEDS_set_color, mock_ret);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    fct_return = STATE_INDICATOR_action_notify_selected(&expected_msg);

    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_notify_not_selected(void **state)
 * \brief Unit test of action_notify_not_selected with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_notify_not_selected(void **state) {
    int mock_ret = 0;
    id_led_t expected_p_id_led = ALL_LEDS;
    color_e expected_color = GREEN;

    int expected_ret = 0, fct_return;
    led_activated = TRUE;
    mq_msg expected_msg = {{0,0}};

    expect_function_call(__wrap_LEDS_set_color);
    expect_value(__wrap_LEDS_set_color, p_id_led, expected_p_id_led);
    expect_value(__wrap_LEDS_set_color, p_color, expected_color);
    will_return(__wrap_LEDS_set_color, mock_ret);

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    fct_return = STATE_INDICATOR_action_notify_not_selected(&expected_msg);

    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_start_notify_emergency(void **state)
 * \brief Unit test of action_start_notify_emergency with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_start_notify_emergency(void **state) {
    int mock_ret = 0;
    id_led_t expected_p_id_led = ALL_LEDS;
    color_e expected_color = RED;

    int expected_ret = 0, fct_return;
    mq_msg expected_msg = {{0,0}};
    led_activated = TRUE;
    buzzer_activated = TRUE;

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    expect_function_call(__wrap_LEDS_set_color);
    expect_value(__wrap_LEDS_set_color, p_id_led, expected_p_id_led);
    expect_value(__wrap_LEDS_set_color, p_color, expected_color);
    will_return(__wrap_LEDS_set_color, mock_ret);

    expect_function_call(__wrap_LEDS_start_blinking);
    will_return(__wrap_LEDS_start_blinking, mock_ret);

    expect_function_call(__wrap_BUZZER_enable_buzzer);
    will_return(__wrap_BUZZER_enable_buzzer, mock_ret);

    expect_function_call(__wrap_watchdog_start);
    expect_value(__wrap_watchdog_start, watchdog, state_indicator_emergency_watchdog);

    fct_return = STATE_INDICATOR_action_start_notify_emergency(&expected_msg);

    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_stop_notify_emergency(void **state)
 * \brief Unit test of action_stop_notify_emergency with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_stop_notify_emergency(void **state) {
    int mock_ret = 0;
    int expected_ret = 0, fct_return;
    mq_msg expected_msg = {{0,0}};

    expect_function_call(__wrap_BUZZER_disable_buzzer);
    will_return(__wrap_BUZZER_disable_buzzer, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_action_notify_selected);
#ifdef _VERIF_EFFETS_BORDS
    expect_value(__wrap_STATE_INDICATOR_action_notify_selected, msg, &expected_msg);
#endif
    will_return(__wrap_STATE_INDICATOR_action_notify_selected, mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_stop_notify_emergency(&expected_msg);

    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_check_state(void **state)
 * \brief Unit test of action_check_state with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_check_state(void **state) {
    int mock_ret = 0;
    int expected_ret = 0, fct_return;
    mq_msg expected_msg_check_state = {.data.event = 0,.data.state = WAITING_FOR_CONNECTION}; /* < WAITING_FOR_CONNECTION state */
    event_e event_from_mq = E_CALL_WAITING_CONNECTION; /* < Should put WAITING CONNECTION into the mq */


#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_check_state(&expected_msg_check_state);

    assert_int_equal(event_from_mq,mq_msg_test->data.event);
    assert_int_equal(expected_ret,fct_return);

    expected_msg_check_state.data.state = EMERGENCY; /* < EMERGENCY state */
    mae_state = S_SELECTED; /* < with Selected mae state */
    event_from_mq = E_CALL_EMERGENCY; /* < Should put CALL EMERGENCY into the mq */

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_check_state(&expected_msg_check_state);

    assert_int_equal(event_from_mq,mq_msg_test->data.event);
    assert_int_equal(expected_ret,fct_return);

    expected_msg_check_state.data.state = SELECTED; /* < SELECTED state */
    mae_state = S_WAITING_CONNECTION; /* < with Waiting connection mae state */
    event_from_mq = E_CALL_SELECTED; /* < Shouldn't put CALL SELECTED into the mq */

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_check_state(&expected_msg_check_state);

    assert_int_not_equal(event_from_mq,mq_msg_test->data.event);
    assert_int_equal(expected_ret,fct_return);

    expected_msg_check_state.data.state = SELECTED; /* < SELECTED state */
    mae_state = S_SELECTED; /* < with Selected mae state */
    //event_from_mq = E_CALL_SELECTED; /* < Should put CALL SELECTED into the mq */

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_check_state(&expected_msg_check_state);

    assert_int_equal(event_from_mq,mq_msg_test->data.event);
    assert_int_equal(expected_ret,fct_return);

    expected_msg_check_state.data.state = NOT_SELECTED; /* < NOT SELECTED state */
    mae_state = S_EMERGENCY; /* < with Selected mae state */
    event_from_mq = E_CALL_NOT_SELECTED; /* < Should put CALL NOT SELECTED into the mq */

    expect_function_call(__wrap_watchdog_cancel);
    expect_value(__wrap_watchdog_cancel, watchdog, state_indicator_emergency_watchdog);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_check_state(&expected_msg_check_state);

    assert_int_equal(event_from_mq,mq_msg_test->data.event);
    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_activate_led(void **state)
 * \brief Unit test of action_activate_led with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_activate_led(void **state) {
    int mock_ret = 0;
    int expected_ret = 0, fct_return;
    bool_e expected_led_activated_state = TRUE;
    mq_msg expected_msg_activate_led = {.data.event = 0,.data.state = 0}; /* < this action function isn't affected by mq msg data */

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_action_notify_selected);
    will_return(__wrap_STATE_INDICATOR_action_notify_selected,mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_activate_led(&expected_msg_activate_led);

    assert_int_equal(expected_led_activated_state,led_activated);
    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_deactivate_led(void **state)
 * \brief Unit test of action_deactivate_led with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_deactivate_led(void **state) {
    int mock_ret = 0;
    int expected_ret = 0, fct_return;
    bool_e expected_led_deactivated_state = FALSE;
    mq_msg expected_msg_deactivate_led = {.data.event = 0,.data.state = 0}; /* < this action function isn't affected by mq msg data */

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_action_notify_selected);
    will_return(__wrap_STATE_INDICATOR_action_notify_selected,mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_deactivate_led(&expected_msg_deactivate_led);

    assert_int_equal(expected_led_deactivated_state,led_activated);
    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_activate_buzzer(void **state)
 * \brief Unit test of action_activate_buzzer with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_activate_buzzer(void **state) {
    int mock_ret = 0;
    int expected_ret = 0, fct_return;
    bool_e expected_buzzer_activated_state = TRUE;
    mq_msg expected_msg_activate_buzzer = {.data.event = 0,.data.state = 0}; /* < this action function isn't affected by mq msg data */

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_action_notify_selected);
    will_return(__wrap_STATE_INDICATOR_action_notify_selected,mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_activate_buzzer(&expected_msg_activate_buzzer);

    assert_int_equal(expected_buzzer_activated_state,buzzer_activated);
    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_deactivate_buzzer(void **state)
 * \brief Unit test of action_deactivate_buzzer with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_deactivate_buzzer(void **state) {
    int mock_ret = 0;
    int expected_ret = 0, fct_return;
    bool_e expected_buzzer_deactivated_state = FALSE;
    mq_msg expected_msg_deactivate_buzzer = {.data.event = 0,.data.state = 0}; /* < this action function isn't affected by mq msg data */

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_STATE_INDICATOR_action_notify_selected);
    will_return(__wrap_STATE_INDICATOR_action_notify_selected,mock_ret);
#endif

    fct_return = STATE_INDICATOR_action_deactivate_buzzer(&expected_msg_deactivate_buzzer);

    assert_int_equal(expected_buzzer_deactivated_state,buzzer_activated);
    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_action_stop(void **state)
 * \brief Unit test of action_stop with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_stop(void **state) {
    int mock_ret = 0;
    int expected_ret = 0, fct_return;
    id_led_t expected_p_id_led = ALL_LEDS;
    color_e expected_color = NO_COLOR;
    mq_msg expected_msg_stop = {.data.event = 0,.data.state = 0}; /* < this action function isn't affected by mq msg data */

    expect_function_call(__wrap_LEDS_set_color);
    expect_value(__wrap_LEDS_set_color, p_id_led, expected_p_id_led);
    expect_value(__wrap_LEDS_set_color, p_color, expected_color);
    will_return(__wrap_LEDS_set_color, mock_ret);

    fct_return = STATE_INDICATOR_action_stop(&expected_msg_stop);

    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_STATE_INDICATOR_emergency_time_out(void **state)
 * \brief Unit test of emergency_time_out with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_emergency_time_out(void **state) {
    int mock_ret = 0;
    event_e expected_event = E_TIME_OUT_EMERGENCY;

    expect_function_call(__wrap_STATE_INDICATOR_add_msg_to_queue);
    will_return(__wrap_STATE_INDICATOR_add_msg_to_queue, mock_ret);

    STATE_INDICATOR_emergency_time_out(state_indicator_emergency_watchdog);

    assert_int_equal(expected_event,mq_msg_test->data.event);
}
/**
 * \fn static void test_STATE_INDICATOR_action_nop(void **state)
 * \brief Unit test of action_nop with CMOCKA.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/controller/state_indicator.c
 */
static void test_STATE_INDICATOR_action_nop(void **state) { /* < Unuseful but for coverage purpose */
    int expected_ret = 0, fct_return;
    mq_msg expected_msg_nop = {.data.event = 0,.data.state = 0}; /* < this action function isn't affected by mq msg data */

    fct_return = STATE_INDICATOR_action_nop(&expected_msg_nop);

    assert_int_equal(expected_ret,fct_return);
}

/**
 * \struct CMUnitTest
 * \brief Lists the test suite for the module
 */
static const struct CMUnitTest tests[] = {

#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    cmocka_unit_test(test_STATE_INDICATOR_action_notify_selected),
#else
    cmocka_unit_test(test_STATE_INIDCATOR_flashing_for_connection),
    cmocka_unit_test(test_STATE_INDICATOR_set_state),
    cmocka_unit_test(test_STATE_INDICATOR_enable_led),
    cmocka_unit_test(test_STATE_INDICATOR_disable_led),
    cmocka_unit_test(test_STATE_INDICATOR_enable_buzzer),
    cmocka_unit_test(test_STATE_INDICATOR_disable_buzzer),
    cmocka_unit_test(test_STATE_INDICATOR_action_notify_not_selected),
    cmocka_unit_test(test_STATE_INDICATOR_action_start_notify_emergency),
    cmocka_unit_test(test_STATE_INDICATOR_action_stop_notify_emergency),
    cmocka_unit_test(test_STATE_INDICATOR_action_check_state),
    #ifndef _VERIF_EFFETS_BORDS
        cmocka_unit_test(test_STATE_INDICATOR_action_activate_led),
        cmocka_unit_test(test_STATE_INDICATOR_action_deactivate_led),
        cmocka_unit_test(test_STATE_INDICATOR_action_activate_buzzer),
        cmocka_unit_test(test_STATE_INDICATOR_action_deactivate_buzzer),
    #endif
    cmocka_unit_test(test_STATE_INDICATOR_action_stop),
    cmocka_unit_test(test_STATE_INDICATOR_emergency_time_out),
    cmocka_unit_test(test_STATE_INDICATOR_action_nop),
#endif
};

/**
 * \fn int STATE_INDICATOR_TEST_run_tests()
 * \brief Module tests suite launch.
 */
int STATE_INDICATOR_TEST_run_tests() {
	return cmocka_run_group_tests_name("Test du module ...", tests, set_up, tear_down);
}


