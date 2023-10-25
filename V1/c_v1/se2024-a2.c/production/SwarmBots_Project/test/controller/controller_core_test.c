/**
 * \file  controller_core_test.c
 * \version  0.1
 * \author Louison LEGROS
 * \date 09 Jun, 2023
 * \brief Test module for the controller core module.
 *
 * \see ../../src/controller/controller_core.c
 * \see ../../src/controller/controller_core.h
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

#include "../../src/lib/defs.h"

#include "../../src/controller/controller_core.c"

/* ASSERT MACROS */

#define ASSERT_MODE_EQUAL(expected, actual) \
    assert_true(expected == actual)

#define ASSERT_OPERATING_MODE_EQUAL(expected, actual) \
    ASSERT_MODE_EQUAL(expected.camera_mode, actual.camera_mode); \
    ASSERT_MODE_EQUAL(expected.leds_mode, actual.leds_mode); \
    ASSERT_MODE_EQUAL(expected.radar_mode, actual.radar_mode); \
    ASSERT_MODE_EQUAL(expected.buzzer_mode, actual.buzzer_mode); \

/**
 * \var mq_msg_test
 * \brief used to verify the mq_msg sent through the mq.
 */
static Mq_Msg * mq_msg_test;

/**
 * \fn int __wrap_CONTROLLER_CORE_mq_send(Mq_Msg * a_msg)
 * \brief Mock function of mq_send. Define here because of Mq_Msg being defined in controller_core.c
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
int __wrap_CONTROLLER_CORE_mq_send(Mq_Msg * a_msg) {
    function_called();
    
    mq_msg_test = a_msg;

    return (int) mock();
}

static int set_up(void **state) {
    if(CONTROLLER_CORE_create() != 0) {
        return -1;
    }
	return 0;
}

static int tear_down(void **state) {
    if(CONTROLLER_CORE_destroy() != 0) {
        return -1;
    }
	return 0;
}

/* PUBLIC FUNCTIONS TESTS */

/**
 * \fn static void test_CONTROLLER_CORE_ask_to_connect(void **state)
 * \brief Unit test of ask_to_connect with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_ask_to_connect(void **state) {
    int mock_ret = 0;
    Mq_Msg expected_msg = {0};
    expected_msg.msg_data.event = E_ASK_TO_CONNECT;
    expected_msg.msg_data.action_data.id_robot = 10;
    Id_Robot dt_id_robot = 10;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_mq_send);
    will_return(__wrap_CONTROLLER_CORE_mq_send, mock_ret);
#endif

    fct_return = CONTROLLER_CORE_ask_to_connect(dt_id_robot);

    assert_int_equal(expected_msg.msg_data.event,mq_msg_test->msg_data.event);
    assert_int_equal(expected_msg.msg_data.action_data.id_robot,mq_msg_test->msg_data.action_data.id_robot);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_ask_to_disconnect(void **state)
 * \brief Unit test of ask_to_disconnect with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_ask_to_disconnect(void **state) {
    int mock_ret = 0;
    Mq_Msg expected_msg = {0};
    expected_msg.msg_data.event = E_DISCONNECTION;
    expected_msg.msg_data.action_data.id_robot = 0;
    Id_Robot dt_id_robot = 0;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_mq_send);
    will_return(__wrap_CONTROLLER_CORE_mq_send, mock_ret);
#endif

    fct_return = CONTROLLER_CORE_ask_to_disconnect(dt_id_robot);

    assert_int_equal(expected_msg.msg_data.event,mq_msg_test->msg_data.event);
    assert_int_equal(expected_msg.msg_data.action_data.id_robot,mq_msg_test->msg_data.action_data.id_robot);
    assert_int_equal(expected_return,fct_return);
}

State dt_ask_set_state_0[] = {WAITING_FOR_CONNECTION};
State dt_ask_set_state_1[] = {EMERGENCY};
State dt_ask_set_state_2[] = {SELECTED};
State dt_ask_set_state_3[] = {NOT_SELECTED};
/**
 * \fn static void test_CONTROLLER_CORE_ask_set_state(void **state)
 * \brief Unit test of ask_set_state with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_ask_set_state(void **state) {
    State * dt_state = (State *)*state;

    int mock_ret = 0;
    Mq_Msg expected_msg = {0};
    expected_msg.msg_data.event = E_ASK_SET_ROBOT_STATE;
    expected_msg.msg_data.action_data.id_robot = 200;
    Id_Robot dt_id_robot = 200;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_mq_send);
    will_return(__wrap_CONTROLLER_CORE_mq_send, mock_ret);
#endif

    fct_return = CONTROLLER_CORE_ask_set_state(dt_id_robot, dt_state[0]);

    assert_int_equal(expected_msg.msg_data.event,mq_msg_test->msg_data.event);
    assert_int_equal(expected_msg.msg_data.action_data.id_robot,mq_msg_test->msg_data.action_data.id_robot);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_ask_mode(void **state)
 * \brief Unit test of ask_mode with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_ask_mode(void **state) {
    int mock_ret = 0;
    Mq_Msg expected_msg = {0};
    expected_msg.msg_data.event = E_ASK_MODE;
    expected_msg.msg_data.action_data.id_robot = 18;
    Id_Robot dt_id_robot = 18;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_mq_send);
    will_return(__wrap_CONTROLLER_CORE_mq_send, mock_ret);
#endif

    fct_return = CONTROLLER_CORE_ask_mode(dt_id_robot);

    assert_int_equal(expected_msg.msg_data.event,mq_msg_test->msg_data.event);
    assert_int_equal(expected_msg.msg_data.action_data.id_robot,mq_msg_test->msg_data.action_data.id_robot);
    assert_int_equal(expected_return,fct_return);
}

Action_Param_Data dt_ask_set_mode_0[] = {{1, 0, {0, 0, 0, 0}}};
Action_Param_Data dt_ask_set_mode_1[] = {{1, 0, {1, 0, 0, 0}}};
Action_Param_Data dt_ask_set_mode_2[] = {{1, 0, {0, 1, 0, 0}}};
Action_Param_Data dt_ask_set_mode_3[] = {{1, 0, {0, 0, 1, 0}}};
Action_Param_Data dt_ask_set_mode_4[] = {{1, 0, {0, 0, 0, 1}}};
Action_Param_Data dt_ask_set_mode_5[] = {{1, 0, {1, 1, 1, 1}}};

/**
 * \fn static void test_CONTROLLER_CORE_ask_set_mode(void **state)
 * \brief Unit test of ask_set_mode with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_ask_set_mode(void **state) {
    Action_Param_Data * dt_state = (Action_Param_Data *)*state;

    int mock_ret = 0;
    Mq_Msg expected_msg = {0};
    expected_msg.msg_data.event = E_ASK_SET_MODE;
    expected_msg.msg_data.action_data.id_robot = dt_state[0].id_robot;
    expected_msg.msg_data.action_data.operating_mode = dt_state[0].operating_mode;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_mq_send);
    will_return(__wrap_CONTROLLER_CORE_mq_send, mock_ret);
#endif

    fct_return = CONTROLLER_CORE_ask_set_mode(dt_state[0].id_robot, dt_state[0].operating_mode);

    assert_int_equal(expected_msg.msg_data.event,mq_msg_test->msg_data.event);
    assert_int_equal(expected_msg.msg_data.action_data.id_robot,mq_msg_test->msg_data.action_data.id_robot);
    ASSERT_OPERATING_MODE_EQUAL(expected_msg.msg_data.action_data.operating_mode,mq_msg_test->msg_data.action_data.operating_mode);
    assert_int_equal(expected_return,fct_return);

}
/**
 * \fn static void test_CONTROLLER_CORE_get_id_robot(void **state)
 * \brief Unit test of get_id_robot with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_get_id_robot(void **state) {
    Id_Robot expected_return = ID_ROBOT;
    Id_Robot fct_return;

    fct_return = CONTROLLER_CORE_get_id_robot();

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_get_mode(void **state)
 * \brief Unit test of get_mode with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_get_mode(void **state) {
    // Default modes set in CONTROLLER_CORE_create() are expected, 
    Operating_Mode expected_return = {ENABLED, ENABLED, ENABLED, ENABLED};
    Operating_Mode fct_return;

    fct_return = CONTROLLER_CORE_get_mode();

    ASSERT_OPERATING_MODE_EQUAL(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_connection_lost(void **state)
 * \brief Unit test of connection_lost with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_connection_lost(void **state) {
    int mock_ret = 0;
    Mq_Msg expected_msg = {0};
    expected_msg.msg_data.event = E_CONNECTION_LOST;
    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_mq_send);
    will_return(__wrap_CONTROLLER_CORE_mq_send, mock_ret);
#endif

    fct_return = CONTROLLER_CORE_connection_lost();

    assert_int_equal(expected_msg.msg_data.event,mq_msg_test->msg_data.event);
    assert_int_equal(expected_return,fct_return);

}

/* PRIVATE FUNCTIONS TESTS */
/**
 * \fn static void test_CONTROLLER_CORE_action_nop(void **state)
 * \brief Unit test of action_nop with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_nop(void **state) {
    int expected_ret = 0, fct_return;
    Action_Param_Data dt_action_param = {0, 0, {0, 0, 0, 0}};

    // We check no attribute is modified
    Id_Robot previous_robot_id = robot_id;
    Operating_Mode previous_robot_operating_mode = robot_operating_mode;
    State previous_robot_state = robot_state;

    fct_return = CONTROLLER_CORE_action_nop(&dt_action_param);

    assert_int_equal(previous_robot_id,robot_id);
    ASSERT_OPERATING_MODE_EQUAL(previous_robot_operating_mode,robot_operating_mode);
    assert_int_equal(previous_robot_state,robot_state);

    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_action_connection(void **state)
 * \brief Unit test of action_connection with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_connection(void **state) {
    int mock_ret = 0;
    int expected_ret = 0, fct_return;
    Id_Robot dt_robot_id = 0;
    Action_Param_Data dt_action_param = {dt_robot_id, 0, {0, 0, 0, 0}};
    State expected_robot_state = NOT_SELECTED;

    expect_function_call(__wrap_GUI_SECRETARY_PROXY_ack_connection);
    expect_value(__wrap_GUI_SECRETARY_PROXY_ack_connection, id_robot, dt_action_param.id_robot);
    will_return(__wrap_GUI_SECRETARY_PROXY_ack_connection, mock_ret);

    expect_function_call(__wrap_CONTROLLER_RINGER_init_failed_pings_var);
    will_return(__wrap_CONTROLLER_RINGER_init_failed_pings_var, mock_ret);

    expect_function_call(__wrap_STATE_INDICATOR_set_state);
    expect_value(__wrap_STATE_INDICATOR_set_state, state, expected_robot_state);
    will_return(__wrap_STATE_INDICATOR_set_state, mock_ret);

    fct_return = CONTROLLER_CORE_action_connection(&dt_action_param);

    assert_int_equal(expected_ret,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_action_set_mode(void **state)
 * \brief Unit test of action_set_mode with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_set_mode(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    Id_Robot dt_id_robot = 15;
    Action_Param_Data dt_action_param = {dt_id_robot, 0, {0, 0, 0, 0}};

    expect_function_call(__wrap_GUI_SECRETARY_PROXY_set_mode);
    expect_value(__wrap_GUI_SECRETARY_PROXY_set_mode, id_robot, dt_id_robot);
    expect_value(__wrap_GUI_SECRETARY_PROXY_set_mode, operating_mode.camera_mode, robot_operating_mode.camera_mode);
    expect_value(__wrap_GUI_SECRETARY_PROXY_set_mode, operating_mode.radar_mode, robot_operating_mode.radar_mode);
    expect_value(__wrap_GUI_SECRETARY_PROXY_set_mode, operating_mode.buzzer_mode, robot_operating_mode.buzzer_mode);
    expect_value(__wrap_GUI_SECRETARY_PROXY_set_mode, operating_mode.leds_mode, robot_operating_mode.leds_mode);
    will_return(__wrap_GUI_SECRETARY_PROXY_set_mode, mock_ret);

    fct_return = CONTROLLER_CORE_action_set_mode(&dt_action_param);

    assert_int_equal(expected_return,fct_return);

}

Mode dt_action_apply_mode_0[] = {ENABLED, ENABLED, ENABLED, ENABLED};
Mode dt_action_apply_mode_1[] = {DISABLED, DISABLED, ENABLED, ENABLED};
Mode dt_action_apply_mode_2[] = {DISABLED, DISABLED, DISABLED, DISABLED};

/**
 * \fn static void test_CONTROLLER_CORE_action_apply_mode(void **state)
 * \brief Unit test of action_apply_mode with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_apply_mode(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;

    Mode * dt_mode = (Mode *)*state;
    Operating_Mode dt_operating_mode = {dt_mode[0], dt_mode[1], dt_mode[2], dt_mode[3]};
    Operating_Mode expected_robot_operating_mode = {dt_mode[0], dt_mode[1], dt_mode[2], dt_mode[3]};

    Action_Param_Data dt_action_param = {0, 0, dt_operating_mode};

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA


    expect_function_call(__wrap_CONTROLLER_CORE_set_operating_mode);
    expect_value(__wrap_CONTROLLER_CORE_set_operating_mode, operating_mode.camera_mode, expected_robot_operating_mode.camera_mode);
    expect_value(__wrap_CONTROLLER_CORE_set_operating_mode, operating_mode.radar_mode, expected_robot_operating_mode.radar_mode);
    expect_value(__wrap_CONTROLLER_CORE_set_operating_mode, operating_mode.buzzer_mode, expected_robot_operating_mode.buzzer_mode);
    expect_value(__wrap_CONTROLLER_CORE_set_operating_mode, operating_mode.leds_mode, expected_robot_operating_mode.leds_mode);
    
    expect_function_call(__wrap_CONTROLLER_CORE_apply_edit_mode);
    expect_value(__wrap_CONTROLLER_CORE_apply_edit_mode, operating_mode.camera_mode, robot_operating_mode.camera_mode);
    expect_value(__wrap_CONTROLLER_CORE_apply_edit_mode, operating_mode.radar_mode, robot_operating_mode.radar_mode);
    expect_value(__wrap_CONTROLLER_CORE_apply_edit_mode, operating_mode.buzzer_mode, robot_operating_mode.buzzer_mode);
    expect_value(__wrap_CONTROLLER_CORE_apply_edit_mode, operating_mode.leds_mode, robot_operating_mode.leds_mode);
    will_return(__wrap_CONTROLLER_CORE_apply_edit_mode, mock_ret);

#endif

    fct_return = CONTROLLER_CORE_action_apply_mode(&dt_action_param);
    assert_int_equal(expected_return,fct_return);
}

State dt_action_set_robot_state_0[] = {NOT_SELECTED};
State dt_action_set_robot_state_1[] = {SELECTED};

/**
 * \fn static void test_CONTROLLER_CORE_action_set_robot_state(void **state)
 * \brief Unit test of action_set_robot_state with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_set_robot_state(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;

    State * dt_state = (State *)*state;
    State dt_robot_state = dt_state[0];
    State expected_robot_state = dt_state[0];

    Action_Param_Data dt_action_param = {0, dt_robot_state, {0, 0, 0, 0}};

    expect_function_call(__wrap_STATE_INDICATOR_set_state);
    expect_value(__wrap_STATE_INDICATOR_set_state, state, expected_robot_state);
    will_return(__wrap_STATE_INDICATOR_set_state, mock_ret);

    fct_return = CONTROLLER_CORE_action_set_robot_state(&dt_action_param);

    assert_int_equal(expected_return,fct_return);
}

State dt_action_evaluate_robot_state_0[] = {NOT_SELECTED};
State dt_action_evaluate_robot_state_1[] = {SELECTED};
State dt_action_evaluate_robot_state_2[] = {EMERGENCY};
State dt_action_evaluate_robot_state_3[] = {WAITING_FOR_CONNECTION};

/**
 * \fn static void test_CONTROLLER_CORE_action_evaluate_robot_state(void **state)
 * \brief Unit test of action_evaluate_robot_state with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_evaluate_robot_state(void **state) {
    int mock_ret = 0;

    State * dt_state = (State *)*state;
    Action_Param_Data dt_action_param = {0, dt_state[0], {0, 0, 0, 0}};

    Mq_Msg expected_msg = {0};
    expected_msg.msg_data.event = E_ROBOT_STATE_EVALUATED;
    expected_msg.msg_data.action_data.robot_state = dt_state[0];

    int expected_return = 0;
    int fct_return;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_mq_send);
    will_return(__wrap_CONTROLLER_CORE_mq_send, mock_ret);
#endif

    fct_return = CONTROLLER_CORE_action_evaluate_robot_state(&dt_action_param);

    assert_int_equal(expected_msg.msg_data.event,mq_msg_test->msg_data.event);
    assert_int_equal(expected_msg.msg_data.action_data.robot_state,mq_msg_test->msg_data.action_data.robot_state);
    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_action_disconnect_ok(void **state)
 * \brief Unit test of action_disconnect_ok with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_disconnect_ok(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    Id_Robot dt_id_robot = 159;
    Action_Param_Data dt_action_param = {dt_id_robot, 0, {0, 0, 0, 0}};

    Command expected_cmd = STOP;
    State expected_robot_state = WAITING_FOR_CONNECTION;
    
    expect_function_call(__wrap_GUI_SECRETARY_PROXY_disconnected_ok);
    expect_value(__wrap_GUI_SECRETARY_PROXY_disconnected_ok, id_robot, dt_id_robot);
    will_return(__wrap_GUI_SECRETARY_PROXY_disconnected_ok, mock_ret);

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_disconnect_core);
    will_return(__wrap_CONTROLLER_CORE_disconnect_core, mock_ret);
#endif

    expect_function_call(__wrap_PILOT_ask_cmd);
    expect_value(__wrap_PILOT_ask_cmd, cmd, expected_cmd);
    will_return(__wrap_PILOT_ask_cmd, mock_ret);

    expect_function_call(__wrap_CAMERA_disconnect_camera);
    will_return(__wrap_CAMERA_disconnect_camera, mock_ret);

    /* Those two following functions are in the code but not in the design document */

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    expect_function_call(__wrap_STATE_INDICATOR_set_state);
    expect_value(__wrap_STATE_INDICATOR_set_state, state, expected_robot_state);
    will_return(__wrap_STATE_INDICATOR_set_state, mock_ret);

    fct_return = CONTROLLER_CORE_action_disconnect_ok(&dt_action_param);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_action_disconnect(void **state)
 * \brief Unit test of action_disconnect with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_disconnect(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    Action_Param_Data dt_action_param = {0, 0, {0, 0, 0, 0}};

    Command expected_cmd = STOP;
    State expected_robot_state = WAITING_FOR_CONNECTION;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_disconnect_core);
    will_return(__wrap_CONTROLLER_CORE_disconnect_core, mock_ret);
#endif

    expect_function_call(__wrap_PILOT_ask_cmd);
    expect_value(__wrap_PILOT_ask_cmd, cmd, expected_cmd);
    will_return(__wrap_PILOT_ask_cmd, mock_ret);

    expect_function_call(__wrap_CAMERA_disconnect_camera);
    will_return(__wrap_CAMERA_disconnect_camera, mock_ret);

    /* Those two following functions are in the code but not in the design document */

    expect_function_call(__wrap_CONTROLLER_LOGGER_log);
    will_return(__wrap_CONTROLLER_LOGGER_log, mock_ret);

    expect_function_call(__wrap_STATE_INDICATOR_set_state);
    expect_value(__wrap_STATE_INDICATOR_set_state, state, expected_robot_state);
    will_return(__wrap_STATE_INDICATOR_set_state, mock_ret);

    fct_return = CONTROLLER_CORE_action_disconnect(&dt_action_param);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_action_init(void **state)
 * \brief Unit test of action_init with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_action_init(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;
    Action_Param_Data dt_action_param = {0, 0, {0, 0, 0, 0}};

    State expected_robot_state = WAITING_FOR_CONNECTION;

#ifdef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    expect_function_call(__wrap_CONTROLLER_CORE_init_hardware);
    will_return(__wrap_CONTROLLER_CORE_init_hardware, mock_ret);
#endif

    expect_function_call(__wrap_STATE_INDICATOR_set_state);
    expect_value(__wrap_STATE_INDICATOR_set_state, state, expected_robot_state);
    will_return(__wrap_STATE_INDICATOR_set_state, mock_ret);

    fct_return = CONTROLLER_CORE_action_init(&dt_action_param);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_disconnect_core(void **state)
 * \brief Unit test of disconnect_core with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_disconnect_core(void **state) {
    int mock_ret = 0;
    int expected_return = 0, fct_return;

    expect_function_call(__wrap_POSTMAN_disconnect);
    will_return(__wrap_POSTMAN_disconnect, mock_ret);

    fct_return = CONTROLLER_CORE_disconnect_core();

    assert_int_equal(expected_return,fct_return);
}

                             /*camera   radar   buzzer  leds*/
Mode dt_set_operating_mode_0[] = {ENABLED, ENABLED, ENABLED, ENABLED};
Mode dt_set_operating_mode_1[] = {DISABLED, DISABLED, ENABLED, ENABLED};
Mode dt_set_operating_mode_2[] = {DISABLED, DISABLED, DISABLED, DISABLED};

/**
 * \fn static void test_CONTROLLER_CORE_set_operating_mode(void **state)
 * \brief Unit test of set_operating_mode with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_set_operating_mode(void **state) {
    Mode * dt_mode = (Mode *)*state;
    Operating_Mode dt_operating_mode = {dt_mode[0], dt_mode[1], dt_mode[2], dt_mode[3]};
    Operating_Mode expected_robot_operating_mode = {dt_mode[0], dt_mode[1], dt_mode[2], dt_mode[3]};

    CONTROLLER_CORE_set_operating_mode(dt_operating_mode);

    ASSERT_OPERATING_MODE_EQUAL(expected_robot_operating_mode,robot_operating_mode);
}

                             /*camera   radar   buzzer  leds*/
Mode dt_apply_edit_mode_0[] = {ENABLED, ENABLED, ENABLED, ENABLED};
Mode dt_apply_edit_mode_1[] = {DISABLED, DISABLED, ENABLED, ENABLED};
Mode dt_apply_edit_mode_2[] = {DISABLED, DISABLED, DISABLED, DISABLED};

/**
 * \fn static void test_CONTROLLER_CORE_apply_edit_mode(void **state)
 * \brief Unit test of apply_edit_mode with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_apply_edit_mode(void **state) {
    int mock_ret = 0;
    Mode * dt_mode = (Mode *)*state;
    Operating_Mode dt_operating_mode = {dt_mode[0], dt_mode[1], dt_mode[2], dt_mode[3]};
    int expected_return = 0, fct_return;

    if(dt_mode[0] == ENABLED) {
        expect_function_call(__wrap_CAMERA_enable_camera);
        will_return(__wrap_CAMERA_enable_camera, mock_ret);
    }
    else {
        expect_function_call(__wrap_CAMERA_disable_camera);
        will_return(__wrap_CAMERA_disable_camera, mock_ret);
    }

    if(dt_mode[3] == ENABLED) {
        expect_function_call(__wrap_STATE_INDICATOR_enable_led);
        will_return(__wrap_STATE_INDICATOR_enable_led, mock_ret);
    }
    else {
        expect_function_call(__wrap_STATE_INDICATOR_disable_led);
        will_return(__wrap_STATE_INDICATOR_disable_led, mock_ret);
    }

    if(dt_mode[2] == ENABLED) {
        expect_function_call(__wrap_STATE_INDICATOR_enable_buzzer);
        will_return(__wrap_STATE_INDICATOR_enable_buzzer, mock_ret);
    }
    else {
        expect_function_call(__wrap_STATE_INDICATOR_disable_buzzer);
        will_return(__wrap_STATE_INDICATOR_disable_buzzer, mock_ret);
    }

    fct_return = CONTROLLER_CORE_apply_edit_mode(dt_operating_mode);

    assert_int_equal(expected_return,fct_return);
}
/**
 * \fn static void test_CONTROLLER_CORE_init_hardware(void **state)
 * \brief Unit test of init_hardware with CMOCKA.
 * \author Louison LEGROS
 *
 * \see ../../src/controller/controller_core.c
 */
static void test_CONTROLLER_CORE_init_hardware(void **state) {
    int mock_ret = 0;

    int expected_servo1_addr = SERVO1_ADDRESS;
    int expected_servo2_addr = SERVO2_ADDRESS;
    float expected_angle_x = SERVO_X_POSITION_INIT;
    float expected_angle_y = SERVO_Y_POSITION_INIT;
    
    expect_function_call(__wrap_SERVO_MOTOR_set_position);
    expect_value(__wrap_SERVO_MOTOR_set_position, servo_addr, SERVO2_ADDRESS);
    expect_value(__wrap_SERVO_MOTOR_set_position, angle, SERVO_X_POSITION_INIT);
    will_return(__wrap_SERVO_MOTOR_set_position, mock_ret);

    expect_function_call(__wrap_SERVO_MOTOR_set_position);
    expect_value(__wrap_SERVO_MOTOR_set_position, servo_addr, SERVO1_ADDRESS);
    expect_value(__wrap_SERVO_MOTOR_set_position, angle, SERVO_Y_POSITION_INIT);
    will_return(__wrap_SERVO_MOTOR_set_position, mock_ret);

    expect_function_call(__wrap_watchdog_start);
    expect_value(__wrap_watchdog_start, watchdog, controller_core_servo_motor_watchdog);

    CONTROLLER_CORE_init_hardware();
}

/**
 * \struct CMUnitTest
 * \brief Lists the test suite for the module
 */
static const struct CMUnitTest tests[] = 
{
#ifndef _WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_set_operating_mode, &dt_set_operating_mode_0),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_set_operating_mode, &dt_set_operating_mode_1),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_set_operating_mode, &dt_set_operating_mode_2),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_apply_edit_mode, &dt_apply_edit_mode_0),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_apply_edit_mode, &dt_apply_edit_mode_1),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_apply_edit_mode, &dt_apply_edit_mode_2),
    cmocka_unit_test(test_CONTROLLER_CORE_disconnect_core),
    cmocka_unit_test(test_CONTROLLER_CORE_init_hardware)
#else
	cmocka_unit_test(test_CONTROLLER_CORE_ask_to_connect),
	cmocka_unit_test(test_CONTROLLER_CORE_ask_to_disconnect),
	cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_state,&dt_ask_set_state_0),
	cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_state,&dt_ask_set_state_1),
	cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_state,&dt_ask_set_state_2),
	cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_state,&dt_ask_set_state_3),
	cmocka_unit_test(test_CONTROLLER_CORE_ask_mode),
	cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_mode, &dt_ask_set_mode_0),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_mode, &dt_ask_set_mode_1),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_mode, &dt_ask_set_mode_2),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_mode, &dt_ask_set_mode_3),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_mode, &dt_ask_set_mode_4),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_ask_set_mode, &dt_ask_set_mode_5),
	cmocka_unit_test(test_CONTROLLER_CORE_get_id_robot),
	cmocka_unit_test(test_CONTROLLER_CORE_get_mode),
	cmocka_unit_test(test_CONTROLLER_CORE_connection_lost),

    cmocka_unit_test(test_CONTROLLER_CORE_action_nop),
    cmocka_unit_test(test_CONTROLLER_CORE_action_connection),
    cmocka_unit_test(test_CONTROLLER_CORE_action_set_mode),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_apply_mode, &dt_action_apply_mode_0),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_apply_mode, &dt_action_apply_mode_1),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_apply_mode, &dt_action_apply_mode_2),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_set_robot_state, &dt_action_set_robot_state_0),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_set_robot_state, &dt_action_set_robot_state_1),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_evaluate_robot_state, &dt_action_evaluate_robot_state_0),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_evaluate_robot_state, &dt_action_evaluate_robot_state_1),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_evaluate_robot_state, &dt_action_evaluate_robot_state_2),
    cmocka_unit_test_prestate(test_CONTROLLER_CORE_action_evaluate_robot_state, &dt_action_evaluate_robot_state_3),
    cmocka_unit_test(test_CONTROLLER_CORE_action_disconnect_ok),
    cmocka_unit_test(test_CONTROLLER_CORE_action_disconnect),
    cmocka_unit_test(test_CONTROLLER_CORE_action_init),

#endif
};

/**
 * \fn int CONTROLLER_CORE_TEST_run_tests()
 * \brief Module tests suite launch.
 */
int CONTROLLER_CORE_TEST_run_tests() {
	return cmocka_run_group_tests_name("Test du module ...", tests, set_up, tear_down);
}


