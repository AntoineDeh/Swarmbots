/**
 * \file  servo_motor.h
 * \version  0.1
 * \author Joshua MONTREUIL
 * \date Apr 30, 2023
 * \brief Header file of the servo-motor peripheral.
 *
 * \see servo_motor.c
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

#ifndef SRC_ALPHABOT2_SERVO_MOTOR_H_
#define SRC_ALPHABOT2_SERVO_MOTOR_H_
/* ----------------------  INCLUDES ------------------------------------------*/
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/**
 * \def SERVO1_ADDRESS
 * Address of the first servo.
 */
#define SERVO1_ADDRESS 0x06
/**
 * \def SERVO1_ADDRESS
 * Address of the second servo.
 */
#define SERVO2_ADDRESS 0x0A
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int SERVO_MOTOR_create(void)
 * \brief Initialize in memory the servo motor object.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int SERVO_MOTOR_create(void);
/**
 * \fn extern void SERVO_MOTOR_destroy(int)
 * \brief Destruct the object servo-motor from memory.
 * \author Joshua MONTREUIL
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int SERVO_MOTOR_destroy(void);
/**
 * \fn extern void SERVO_MOTOR_init(void)
 * \brief initializes the peripheral into position.
 * \author Joshua MONTREUIL
 */
extern void SERVO_MOTOR_init(void);
/**
 * \fn extern void SERVO_MOTOR_enable_servo_motor(void)
 * \brief Enable the peripheral.
 * \author Joshua MONTREUIL
 */
extern void SERVO_MOTOR_enable_servo_motor(void);
/**
 * \fn extern void SERVO_MOTOR_disable_servo_motor(void)
 * \brief Disable the peripheral.
 * \author Joshua MONTREUIL
 */
extern void SERVO_MOTOR_disable_servo_motor(void);
/**
 * \fn extern int SERVO_MOTOR_set_position(int servo_addr, int angle)
 * \brief Sets the servo motor into a position given by an angle
 * \author Joshua MONTREUIL
 *
 * \param servo_addr : register address of the servo_motor.
 * \param angle : angle to put the servo to.
 *
 * \return On success, returns 0. On error, returns -1.
 */
extern int SERVO_MOTOR_set_position(int servo_addr, float angle);

#endif /* SRC_ALPHABOT2_SERVO_MOTOR_H_ */

