/**
 * \file  camera.h
 * \version  0.1
 * \author Louison LEGROS
 * \date May 14, 2023
 * \brief Header file of the Camera peripheral.
 *
 * \see camera.c
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
#ifndef SRC_ALPHABOT2_CAMERA_H_
#define SRC_ALPHABOT2_CAMERA_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include <stdint.h>
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int CAMERA_create()
 * \brief Initialize in memory the object Camera.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CAMERA_create(void);
/**
 * \fn extern int CAMERA_destroy()
 * \brief Destruct the object Camera from memory.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CAMERA_destroy(void);
/**
 * \fn extern int CAMERA_start()
 * \brief Starts the module.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CAMERA_start(void);
/**
 * \fn extern int CAMERA_stop()
 * \brief Stops the module.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CAMERA_stop(void);
/**
 * \fn extern int CAMERA_enable_camera()
 * \brief Enable the peripheral.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CAMERA_enable_camera(void);
/**
 * \fn extern int CAMERA_disable_camera()
 * \brief Disable the peripheral.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CAMERA_disable_camera(void);
/**
 * \fn extern int CAMERA_set_up_ihm_info(char * ip_address, int8_t data_received)
 * \brief Set up the IP address an port of the IHM.
 * \author Louison LEGROS
 * 
 * \param ip_address The IP address of the control device.
 * \param port The port of the control device.
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CAMERA_set_up_ihm_info(char * ip_address, uint16_t port);
/**
 * \fn extern int CAMERA_disconnect_camera()
 * \brief Disconnect the camera and wait for the IHM info.
 * \author Louison LEGROS
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int CAMERA_disconnect_camera(void);

#endif /* SRC_ALPHABOT2_CAMERA_H_ */

