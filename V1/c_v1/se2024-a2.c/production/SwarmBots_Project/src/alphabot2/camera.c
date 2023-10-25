/**
 * \file  camera.c
 * \version  0.1
 * \author Louison LEGROS
 * \date May 14, 2023
 * \brief Source file of the module in charge of streaming the camera over UDP.
 *
 * \see camera.h
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
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

#include "camera.h"
#include "../logs/controller_logger.h"
#include <gst/gst.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def NAME_MQ_BOX
 * Name of the camera message queue
 */
#define NAME_MQ_BOX  "/camera_mq"
/**
 * \def MQ_MSG_COUNT
 * Max messages that can be stored into the camera message queue
 */
#define MQ_MSG_COUNT (10)
/**
 * \def PIPELINE_DESCRIPTION
 * Pipeline used to stream the camera.
 */
#define PIPELINE_DESCRIPTION "v4l2src device=/dev/video0 ! video/x-h264, width=320, height=240, framerate=10/1, profile=baseline, stream-format=byte-stream, bitrate=500000, tune=zerolatency ! udpsink host=%s port=%s"
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
 * \enum state_e
 * \brief Defines the different states of the camera module.
 */
typedef enum {
    S_FORGET = 0,
    S_WAITING_INFO,
    S_ON,
    S_OFF,
    S_DEATH,
    S_NB
} mae_state_e;
/**
 * \enum event_e
 * \brief Defines the different events handled by the camera module.
 */
typedef enum {
    E_ENABLE_CAMERA = 0,
    E_DISABLE_CAMERA,
    E_SET_IHM_INFO,
    E_DISCONNECT_CAMERA,
    E_STOP,
    E_NB
} event_e;
/**
 * \enum action_e
 * \brief Defines the different actions performed by the camera module.
 */
typedef enum {
    A_NOP = 0,
    A_SET_INFO,
    A_START_STREAMING,
    A_STOP_STREAMING,
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
    char * ip_address;
    uint16_t port;
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
 * \typedef void(*action_ptr)(mq_msg *msg)
 * \brief Definition of function pointer for the actions to perform.
 */
typedef int (*action_ptr)(mq_msg *msg);
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static int CAMERA_get_msg_from_queue(mq_msg * msg)
 * \brief gets the last message of the queue
 * \author Louison LEGROS
 * \param msg [out] message received from the queue
 * \return int error code, 0 if successfull, -1 if unable to access the queue
 */
static int CAMERA_get_msg_from_queue(mq_msg * msg);
/**
 * \fn static int CAMERA_add_msg_to_queue(mq_msg * msg)
 * \brief adds a new message on top of the queue
 * \author Louison LEGROS
 *
 * \param msg
 * \return int
 */
static int CAMERA_add_msg_to_queue(mq_msg * msg);
/**
 * \fn static void * CAMERA_run()
 * \brief running function that runs the state machine
 * \author Louison LEGROS
 *
 * \return void* value used by the thread that joins this thread
 */
static void * CAMERA_run(void * arg);
/**
 * \fn static int CAMERA_no_operation(mq_msg * msg)
 * \brief For transitions without action needed.
 * \author Louison LEGROS
 *
 * \param msg data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CAMERA_no_operation(mq_msg * msg);
/**
 * \fn static int CAMERA_start_streaming(mq_msg * msg)
 * \brief running function that runs the state machine
 * \author Louison LEGROS
 *
 * \param msg data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CAMERA_start_streaming(mq_msg * msg);
/**
 * \fn static int CAMERA_stop_streaming(mq_msg * msg)
 * \brief running function that runs the state machine
 * \author Louison LEGROS
 *
 * \param msg data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CAMERA_stop_streaming(mq_msg * msg);
/**
 * \fn static int CAMERA_set_ihm_info(mq_msg * msg)
 * \brief running function that runs the state machine
 * \author Louison LEGROS
 *
 * \param msg data structure pushed by the trigger event.
 *
 * \return On success, returns 0. On error, returns -1.
 */
static int CAMERA_set_ihm_info(mq_msg * msg);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/**
 * \var camera_thread
 * \brief Thread used by the module to execute the run method.
 */
static pthread_t camera_thread;
/**
 * \var camera_message_queue
 * \brief Message queue used by the module to handle events and manage module state machine.
 */
static mqd_t camera_message_queue;
/**
 * \var *pipeline
 * \brief Pipeline used by gstreamer to handle the camera stream.
 */
GstElement *pipeline;
/**
 * \brief *pipeline_string
 * \brief Pipeline string parsed by gstreamer to generate the pipeline.
 */
char *pipeline_string;
/**
 * \brief camera_state_machine
 * \brief Struct describing the machine state of the module
 */

static transition_t camera_state_machine [S_NB-1][E_NB] =
        {
                [S_WAITING_INFO] [E_SET_IHM_INFO]   = {S_ON,            A_SET_INFO},
                [S_OFF] [E_DISCONNECT_CAMERA]       = {S_WAITING_INFO,  A_NOP},
                [S_ON]  [E_DISCONNECT_CAMERA]       = {S_WAITING_INFO,  A_STOP_STREAMING},
                [S_OFF] [E_ENABLE_CAMERA]           = {S_ON,            A_START_STREAMING},
                [S_ON]  [E_DISABLE_CAMERA]          = {S_OFF,           A_STOP_STREAMING},
                [S_WAITING_INFO] [E_STOP]           = {S_DEATH,         A_NOP},
                [S_OFF] [E_STOP]                    = {S_DEATH,         A_NOP},
                [S_ON]  [E_STOP]                    = {S_DEATH,         A_STOP_STREAMING}
        };

/**
 * \var static const Action_Pt actions_tab[ACTION_NB]
 * \brief Array of function pointer to call from action to perform.
 */
static const action_ptr actions_tab[ACTION_NB] = {
    &CAMERA_no_operation,
    &CAMERA_set_ihm_info,
    &CAMERA_start_streaming,
    &CAMERA_stop_streaming,
};
/**
 * \var gui_ip
 * \brief IP address of SB_IHM.
 */
char gui_ip[16];
/**
 * \var gui_port
 * \brief port of SB_IHM.
 */
char gui_port[7];
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
extern int CAMERA_create(void) {
    /* Initialize GStreamer */
    gst_init(NULL, NULL);

    int size = snprintf(NULL, 0, PIPELINE_DESCRIPTION, gui_ip, gui_port);
    pipeline_string = (char *)malloc((size + 1) * sizeof(char));

    struct mq_attr mq_a = {
            .mq_maxmsg = MQ_MSG_COUNT,
            .mq_msgsize = sizeof(mq_msg),
            .mq_flags = 0,
    };
    if((camera_message_queue = mq_open(NAME_MQ_BOX, O_CREAT | O_RDWR | O_EXCL , 0644 ,&mq_a )) == -1 ) {
        if(errno == EEXIST) {
            mq_unlink(NAME_MQ_BOX);
            if((camera_message_queue = mq_open(NAME_MQ_BOX, O_CREAT | O_RDWR , 0644 ,&mq_a )) == -1 ) {
                CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for camera after unlinking the mq.");
                goto error_mq;
            }
        } else {
            CONTROLLER_LOGGER_log(ERROR, "On mq_open(): mq failed to be opened for camera.");
            goto error_mq;
        }
    }
    return 0;

    error_mq:
    free(pipeline_string);
    return -1;
}

extern int CAMERA_destroy(void) {
    if(mq_close(camera_message_queue) == -1)
    {
        CONTROLLER_LOGGER_log(ERROR, "On mq_close(): error while closing the mq of camera.");
        return -1;
    }
    if(mq_unlink(NAME_MQ_BOX) == -1)
    {
        CONTROLLER_LOGGER_log(ERROR, "On mq_unlink(): error while unlinking the mq of camera.");
        return -1;
    }

    free(pipeline_string);

    return 0;
}

extern int CAMERA_start(void) {
    if(pthread_create(&camera_thread, NULL, CAMERA_run, NULL) != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On pthread_create() : error while creating camera thread.");
        return -1;
    }
    return 0;
}

extern int CAMERA_stop(void) {
    mq_msg msg = {.data.event = E_STOP};
    if(CAMERA_add_msg_to_queue(&msg) == 0) {
        if(pthread_join(camera_thread, NULL) != 0) {
            CONTROLLER_LOGGER_log(ERROR, "On pthread_join(): error while waiting the termination of camera thread.");
            return -1;
        }
    }
    else {
        return -1;
    }
    return 0;
}

extern int CAMERA_enable_camera(void) {
    mq_msg msg = {.data.event = E_ENABLE_CAMERA, .data.ip_address = NULL, .data.port = 0};
    if(CAMERA_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}

extern int CAMERA_disable_camera(void) {
    mq_msg msg = {.data.event = E_DISABLE_CAMERA, .data.ip_address = NULL, .data.port = 0};
    if(CAMERA_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}

extern int CAMERA_set_up_ihm_info(char * ip_address, uint16_t port) {
    mq_msg msg = {.data.event = E_SET_IHM_INFO, .data.ip_address = ip_address, .data.port = port};
    if(CAMERA_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}

extern int CAMERA_disconnect_camera(void) {
    mq_msg msg = {.data.event = E_DISCONNECT_CAMERA, .data.ip_address = NULL, .data.port = 0};
    if(CAMERA_add_msg_to_queue(&msg) == -1) {
        return -1;
    }
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static int CAMERA_get_msg_from_queue(mq_msg * msg) {
    if(mq_receive(camera_message_queue, msg->buffer, sizeof(mq_msg), NULL) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_receive(): CAMERA has failed to receive a message on the mq.");
        return -1;
    }
    return 0;
}

static int CAMERA_add_msg_to_queue(mq_msg * msg) {
    if(mq_send(camera_message_queue, msg->buffer, sizeof(mq_msg), 0) == -1) {
        CONTROLLER_LOGGER_log(ERROR, "On mq_send(): CAMERA has failed to receive a message on the mq.");
        return -1;
    }
    return 0;
}

static void* CAMERA_run(void * arg){
    mq_msg msg;
    mae_state_e mae_state = S_WAITING_INFO;
    transition_t * current_transition;

    while (mae_state != S_DEATH) {
        if(CAMERA_get_msg_from_queue(&msg) == -1) {
            return NULL;
        }
        current_transition = &camera_state_machine[mae_state][msg.data.event];
        if (current_transition->dest_state != S_FORGET) {
            if (actions_tab[current_transition->action](&msg) == -1) {
                CONTROLLER_LOGGER_log(ERROR, "On actions_tab() : failed to execute the action for CAMERA.");
                return NULL;
            }
            mae_state = current_transition->dest_state;
        }
    }
    return NULL;
}

static int CAMERA_no_operation(mq_msg * msg) {
    return 0;
}

static int CAMERA_start_streaming(mq_msg * msg) {
    /* Build the pipeline */
    pipeline = gst_parse_launch(pipeline_string, NULL);

    /* Start playing */
    if(gst_element_set_state(pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
        CONTROLLER_LOGGER_log(ERROR, "On gst_element_set_state(): failed to start the pipeline.");
        return -1;
    }

    CONTROLLER_LOGGER_log(INFO, "CAMERA : Stream has been started.");

    return 0;
}

static int CAMERA_stop_streaming(mq_msg * msg) {
    int ret = 0;
    /* Stop playing */
    if(gst_element_set_state (pipeline, GST_STATE_NULL) == GST_STATE_CHANGE_FAILURE) {
        CONTROLLER_LOGGER_log(ERROR, "On gst_element_set_state(): failed to stop the pipeline.");
        ret = -1;
    }
    /* Free pipeline */
    gst_object_unref(pipeline);

    if(ret == 0) {
        CONTROLLER_LOGGER_log(INFO, "CAMERA : Stream has been stopped.");
    }
    return ret;
}

static int CAMERA_set_ihm_info(mq_msg * msg) {
    strcpy(gui_ip, msg->data.ip_address);
    sprintf(gui_port, "%u", msg->data.port);
    sprintf(pipeline_string, PIPELINE_DESCRIPTION, gui_ip, gui_port);

    char log_msg[100];
    sprintf(log_msg, "CAMERA : ihm info setup with ip = %s, port = %s", gui_ip, gui_port);
    CONTROLLER_LOGGER_log(DEBUG, log_msg);

    if(CAMERA_start_streaming(NULL) == -1) {
        return -1;
    }
    return 0;
}