/**
 * \file  leds_ws2811.c
 * \version  0.1
 * \author LEGROS Louison
 * \date May 09, 2023
 * \brief Source file of the leds peripheral using ws2811 as IHM.
 *
 * \see leds_ws2811.h
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
#include <ws2811.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

#include "../lib/defs.h"
#include "leds.h"
#include "../lib/watchdog.h"
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def FREQ
 *  WS2811 FREQ
 */
#define FREQ                WS2811_TARGET_FREQ
/**
 * \def GPIO_PIN
 * leds pin.
 */
#define GPIO_PIN            18
/**
 * \def DMA
 * DMA number.
 */
#define DMA                 10
/**
 * \def STRIP_TYPE
 * Type of led strype.
 */
#define STRIP_TYPE          WS2811_STRIP_GBR
/**
 * \def NAME_MQ_BOX
 * Name of the leds message queue
 */
#define NAME_MQ_BOX  "/leds_mq"
/**
 * \def MQ_MSG_COUNT
 * Max messages that can be stored into the leds message queue
 */
#define MQ_MSG_COUNT (10)
/**
 * \def BLINKING_TIME_OUT
 * Time out in second to return to the state selected after an emergency
 */
#define BLINKING_TIME_OUT (300)
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
 * \enum state_e
 * \brief Defines the different states of the leds module.
 */
typedef enum {
    S_FORGET = 0,
    S_STILL,
    S_BLINK_ON,
    S_BLINK_OFF,
    S_CHOICE,
    S_DEATH,
    S_NB
} mae_state_e;
/**
 * \enum event_e
 * \brief Defines the different events handled by the leds module.
 */
typedef enum {
    E_SET_COLOR,
    E_ASK_BLINK,
    E_GO_BLINK,
    E_GO_STILL,
    E_BLINK_TIME_OUT,
    E_STOP,
    E_NB
} event_e;
/**
 * \enum action_e
 * \brief Defines the different actions performed by the leds module.
 */
typedef enum {
    A_NOP = 0,
    A_CHANGE_COLOR,
    A_CHECK_COLOR,
    A_STOP_BLINK,
    A_BLINK_SET_ON,
    A_BLINK_SET_OFF,
    ACTION_NB
} action_e ;
/**
 * \struct transition_t
 * \brief Contains the dest_state and the action to perform. It is used to switch state inside the module.
 */
typedef struct {
    mae_state_e dest_state;
    action_e action;
} transition_t;
/**
 * \struct mq_msg_data_t
 * \brief Contains the data that can be passed through the message.
 */
typedef struct {
    event_e event;
    id_led_t id_led;
    color_e color;
} mq_msg_data_t;
/**
 * \union mq_msg_data_t
 * \brief Contains the entire message manipulated by the module and the message queue.
 */
typedef union {
    mq_msg_data_t data;
    char buffer[sizeof(mq_msg_data_t)];
} mq_msg;
/**
 * \typedef int (*action_ptr)(mq_msg *msg)
 * \brief Definition of function pointer for the actions to perform.
 */
typedef int (*action_ptr)(mq_msg *msg);
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static int LEDS_add_msg_to_queue(mq_msg* msg)
 * \brief gets a new message from the message queue
 * \author Louison LEGROS
 * 
 * \param msg [out] message received from the queue
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_get_msg_from_queue(mq_msg * msg);
/**
 * \fn static int LEDS_add_msg_to_queue(mq_msg* msg)
 * \brief adds a new message on top of the queue
 * \author Louison LEGROS
 * 
 * \param msg [in] message to add in the queue
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_add_msg_to_queue(mq_msg * msg);
/**
 * \fn static void* LEDS_run(void* param)
 * \brief Blocking function running machine state of the module
 * \author Louison LEGROS
 *
 * \param param: to be used if needed
 */
static void * LEDS_run(void* param);
/**
 * \fn static int LEDS_action_nop(mq_msg * msg)
 * \brief For transitions without action needed.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_action_nop(mq_msg * msg);
/**
 * \fn static int LEDS_action_change_color(mq_msg * msg)
 * \brief Changes the color of the led.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_action_change_color(mq_msg * msg);
/**
 * \fn static int LEDS_action_check_color(mq_msg * msg)
 * \brief Checks the color of the led.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_action_check_color(mq_msg * msg);
/**
 * \fn static int LEDS_action_stop_blinking(mq_msg * msg)
 * \brief Stops the blinking of the led.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_action_stop_blinking(mq_msg * msg);
/**
 * \fn static int LEDS_action_blink_set_on(mq_msg * msg)
 * \brief Sets the blinking of the led to on.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_action_blink_set_on(mq_msg * msg);
/**
 * \fn static int LEDS_action_blink_set_off(mq_msg * msg)
 * \brief Sets the blinking of the led to off.
 * \author Louison LEGROS
 *
 * \param msg : data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_action_blink_set_off(mq_msg * msg);
/**
 * \fn static int LEDS_render(void)
 * \brief Renders the color of the led to the current color.
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_render(void);
/**
 * \fn static int LEDS_render_no_color(void)
 * \brief Renders the color to no color (power off).
 * \author Louison LEGROS
 * 
 * \return On success, returns 0. On error, returns -1.
 */
static int LEDS_render_no_color(void);
/**
 * \fn static int LEDS_action_blinking_time_out(mq_msg * msg)
 * \brief Callback function called when the blinking time out is reached.
 * \author Louison LEGROS
 * 
 * \param watchdog : watchdog used to trigger the radar check
 */
static void LEDS_blinking_time_out(watchdog_t * watchdog);
/**
 * \fn static void LEDS_change_current_color(id_led_t id_led, color_e color)
 * \brief Changes the current color of the led.
 * \author Louison LEGROS
 * 
 * \param id_led : id of the led to change the color
 * \param color : new color of the led
 */
static void LEDS_change_current_color(id_led_t id_led, color_e color);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/**
 * \var leds_thread
 * \brief Thread used by the module to execute the run method.
 */
static pthread_t leds_thread;
/**
 * \var leds_message_queue
 * \brief Message queue used by the module to handle events and manage module state machine.
 */
static mqd_t leds_message_queue;
/**
 * \var led_blink_watchdog
 * \brief watchdog used to notify the end of the emergency state
 */
watchdog_t *led_blink_watchdog;
/**
 * \var leds_state_machine
 * \brief Struct describing the machine state of the module
 */
static transition_t leds_state_machine [S_NB-1][E_NB] = {
    [S_STILL]       [E_ASK_BLINK]       = {S_CHOICE, A_CHECK_COLOR},
    [S_CHOICE]      [E_GO_BLINK]        = {S_BLINK_ON, A_BLINK_SET_ON},
    [S_CHOICE]      [E_GO_STILL]        = {S_STILL, A_NOP},
    [S_STILL]       [E_SET_COLOR]       = {S_STILL, A_CHANGE_COLOR},
    [S_BLINK_OFF]   [E_BLINK_TIME_OUT]  = {S_BLINK_ON, A_BLINK_SET_ON},
    [S_BLINK_OFF]   [E_SET_COLOR]       = {S_STILL, A_STOP_BLINK},
    [S_BLINK_ON]    [E_BLINK_TIME_OUT]  = {S_BLINK_OFF, A_BLINK_SET_OFF},
    [S_BLINK_ON]    [E_SET_COLOR]       = {S_STILL, A_STOP_BLINK},
    [S_STILL]       [E_STOP]            = {S_DEATH, A_NOP},
    [S_BLINK_OFF]   [E_STOP]            = {S_DEATH, A_NOP},
    [S_BLINK_ON]    [E_STOP]            = {S_DEATH, A_NOP}
};
/**
 * \var static const Action_Pt actions_tab[ACTION_NB]
 * \brief Array of function pointer to call from action to perform.
 */
static const action_ptr actions_tab[ACTION_NB] = {
    &LEDS_action_nop,
    &LEDS_action_change_color,
    &LEDS_action_check_color,
    &LEDS_action_stop_blinking,
    &LEDS_action_blink_set_on,
    &LEDS_action_blink_set_off
};
/* Our led strip that contains our configuration and each of our leds */
static ws2811_t led_strip =
{
    .freq = FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = NUMBER_OF_LEDS,
            .strip_type = STRIP_TYPE,
            .brightness = 255,
        },
        [1] =
        {
            .gpionum = 0,
            .invert = 0,
            .count = 0,
            .brightness = 0,
        },
    },
};
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
extern int LEDS_create() {
    if(ws2811_init(&led_strip) != WS2811_SUCCESS) {
        CONTROLLER_LOGGER_log(ERROR, "On ws2811_init(&led_strip): Failed to init leds strip.");
        return -1;
    }
    led_blink_watchdog = watchdog_create(BLINKING_TIME_OUT, LEDS_blinking_time_out);

    struct mq_attr mq_a = {
            .mq_maxmsg = MQ_MSG_COUNT,
            .mq_msgsize = sizeof(mq_msg),
            .mq_flags = 0,
    };
    if((leds_message_queue = mq_open(NAME_MQ_BOX, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &mq_a)) == -1 ) {
        if(errno == EEXIST) {
            mq_unlink(NAME_MQ_BOX);
            if((leds_message_queue = mq_open(NAME_MQ_BOX, O_CREAT | O_RDWR , 0644 ,&mq_a )) == -1 ) {
                CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for leds after unlinking the mq.");
                goto error_mq;
            }
        } else {
            CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for leds.");
            goto error_mq;
        }
    }
    return 0;

    error_mq:
    ws2811_fini(&led_strip);
    return-1;
}

extern int LEDS_destroy(void) {
    int ret = 0;

    if(mq_close(leds_message_queue) == -1 ) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_close(): error while closing the mq of leds.");
        ret = -1;
    }
    if(mq_unlink(NAME_MQ_BOX) == -1 ) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_unlink(): error while unlinking the mq of leds.");
        ret = -1;
    }
    ws2811_fini(&led_strip);

    return ret;
}

extern int LEDS_start(void) {
    if(pthread_create(&leds_thread, NULL, LEDS_run, NULL) != 0)
    {
        CONTROLLER_LOGGER_log(ERROR, "On pthread_create() : error while creating leds thread.");
        return -1;
    }
    return 0;
}

extern int LEDS_stop(void) {
    int ret = 0;
    mq_msg msg = {.data.event = E_STOP, .data.id_led = 0, .data.color = 0};

    if(LEDS_add_msg_to_queue(&msg) == 0) {
        if(pthread_join(leds_thread, NULL) != 0) {
            CONTROLLER_LOGGER_log(ERROR, "On pthread_join(): error while waiting the termination of leds thread.");
            ret = -1;
        }
    }
    else {
        ret = -1;
    }
    return ret;
}


extern int LEDS_set_color(id_led_t p_id_led, color_e p_color)
{
    mq_msg msg;
    msg.data.event = E_SET_COLOR;
    msg.data.id_led = p_id_led;
    msg.data.color = p_color;

    if(LEDS_add_msg_to_queue(&msg) != 0) {
        return -1;
    }
    return 0;
}

extern int LEDS_start_blinking()
{
    mq_msg msg;
    msg.data.event = E_ASK_BLINK;
    msg.data.id_led = 0;
    msg.data.color = 0;

    if(LEDS_add_msg_to_queue(&msg) != 0) {
        return -1;
    }
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static int LEDS_get_msg_from_queue(mq_msg * msg) {
    if(mq_receive(leds_message_queue, msg->buffer, sizeof(mq_msg), NULL) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_receive(): LEDS has failed to receive a message on the mq.");
        return -1;
    }
    return 0;
}

static int LEDS_add_msg_to_queue(mq_msg * msg) {
    if(mq_send(leds_message_queue, msg->buffer, sizeof(mq_msg), 0) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_send(): LEDS has failed to send a message on the mq.");
        return -1;
    }
    return 0;
}

static void* LEDS_run(void* param){
    mq_msg msg;
    transition_t * current_transition;
    mae_state_e mae_state = S_STILL;

    while (mae_state != S_DEATH) {
        if(LEDS_get_msg_from_queue(&msg) == -1) {
           return NULL;
        }
        current_transition = &leds_state_machine[mae_state][msg.data.event];
        if (current_transition->dest_state != S_FORGET) {
            if (actions_tab[current_transition->action](&msg) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On actions_tab() : failed to execute the action for LEDS.");
                return NULL;
            }
            mae_state = current_transition->dest_state;
        }
    }
    return NULL;
}
static int LEDS_action_nop(mq_msg * msg) {
    return 0;
}

static int LEDS_action_change_color(mq_msg * msg) {
    LEDS_change_current_color(msg->data.id_led, msg->data.color);
    if(LEDS_render() == -1) {
        return -1;
    }
    return 0;
}

static int LEDS_action_check_color(mq_msg * msg) {
    mq_msg next_msg;

    bool_e is_color_ok = FALSE;
    for(int i = 0; i < NUMBER_OF_LEDS; i++) {
        if(led_strip.channel[0].leds[i] != NO_COLOR) {
            is_color_ok = TRUE;
            break;
        }
    }

    if(is_color_ok) {
        next_msg.data.event = E_GO_BLINK;
    }
    else {
        next_msg.data.event = E_GO_STILL;
    }
    
    if(LEDS_add_msg_to_queue(&next_msg) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On LEDS_add_msg_to_queue() : failed to add a message to the queue.");
        return -1;
    }
    return 0;
}

static int LEDS_action_stop_blinking(mq_msg * msg) {
    watchdog_cancel(led_blink_watchdog);
    LEDS_change_current_color(msg->data.id_led, msg->data.color);
    if(LEDS_render() == -1) {
        return -1;
    }
    return 0;
}

static int LEDS_action_blink_set_on(mq_msg * msg) {
    watchdog_start(led_blink_watchdog);
    if(LEDS_render() == -1) {
        return -1;
    }
    return 0;
}

static int LEDS_action_blink_set_off(mq_msg * msg) {
    watchdog_start(led_blink_watchdog);
    if(LEDS_render_no_color() == -1) {
        return -1;
    }
    return 0;
}

static void LEDS_blinking_time_out(watchdog_t * watchdog)
{
    mq_msg msg = {.data.event = E_BLINK_TIME_OUT, .data.id_led = 0, .data.color = 0};
    if(LEDS_add_msg_to_queue(&msg) != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On LEDS_add_msg_to_queue(&msg) : LEDS_check_radar_time_out callback failed to send a message to the mq.");
    }
}

static int LEDS_render()
{
    if(ws2811_render(&led_strip) != WS2811_SUCCESS)
    {
        CONTROLLER_LOGGER_log(ERROR, "On ws2811_render() : LEDS has failed to render the led strip.");
        return -1;
    }
    return 0;
}

static int LEDS_render_no_color()
{
    ws2811_led_t leds_no_color[NUMBER_OF_LEDS];
    ws2811_led_t *leds_actual = led_strip.channel[0].leds;

    for(int i = 0; i<NUMBER_OF_LEDS; i++)
    {
        leds_no_color[i] = NO_COLOR;
    }
    led_strip.channel[0].leds = leds_no_color;
    if(LEDS_render() == -1) {
        return -1;
    }
    led_strip.channel[0].leds = leds_actual;

    return 0;
}

static void LEDS_change_current_color(id_led_t id_led, color_e color)
{
    if(id_led == -1)
    {
        for(int i = 0; i<NUMBER_OF_LEDS;i++)
        {
            led_strip.channel[0].leds[i] = color;
        }

    }
    else if(id_led >=0 && id_led<=NUMBER_OF_LEDS)
    {
        led_strip.channel[0].leds[id_led] = color;
    }
}