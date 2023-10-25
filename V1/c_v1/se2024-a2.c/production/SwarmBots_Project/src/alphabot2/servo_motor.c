/**
 * \file  servo_motor.c
 * \version  0.1
 * \author joshua
 * \date Apr 30, 2023
 * \brief Source file of the servo-motor peripheral.
 *
 * \see servo_motor.h
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
#include "servo_motor.h"
#include "../lib/PCA9685.h"
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def PCA9685ADRR
 * PCA9685 driver address.
 */
#define PCA9685ADRR 0x40
/**
 * \def FREQ
 * Pwm frequency.
 */
#define FREQ 50
/**
 * \def DELAYTIME
 * Delay time between two pwm.
 */
#define DELAYTIME 10
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static float SERVO_MOTOR_calc_impulse_ms(float angle)
 * \brief Computes the impulse MS from the angle.
 * \author Joshua MONTREUIL
 *
 * \param angle : angle.
 *
 * \return the computed impulse.
 */
static float SERVO_MOTOR_calc_impulse_ms(float angle);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/*
 * \var pca9685_fd
 * pca9685 file descriptor
 */
static int pca9685_fd;
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int SERVO_MOTOR_create(void) {
    pca9685_fd = PCA9685_Init(PCA9685ADRR,FREQ);
    if(pca9685_fd == -1 ) {
        CONTROLLER_LOGGER_log(ERROR, "On PCA9685_Init() : error while init the PCA9685 Chip.");
        return -1;
    }
    PCA9685_PWM_Reset(pca9685_fd, SERVO1_ADDRESS);
    PCA9685_PWM_Reset(pca9685_fd, SERVO2_ADDRESS);
    return 0;
}

void SERVO_MOTOR_enable_servo_motor(void) {
    PCA9685_Wake_Up(pca9685_fd);
}

void SERVO_MOTOR_disable_servo_motor(void) {
    PCA9685_Sleep(pca9685_fd);
}

int SERVO_MOTOR_destroy(void) {
    PCA9685_Sleep(pca9685_fd);
    return 0;
}

int SERVO_MOTOR_set_position(int servo_addr, float angle) {
    float impulse_Ms = SERVO_MOTOR_calc_impulse_ms(angle);
    if(impulse_Ms < 1 || impulse_Ms > 2)
    {
        CONTROLLER_LOGGER_log(ERROR,"On SERVO_MOTOR_calc_impulse_ms : the computed impulse isn't in range.");
        return -1;
    }
    if(PCA9685_Write_PWM(pca9685_fd,servo_addr, impulse_Ms, DELAYTIME, FREQ) == -1)
    {
        CONTROLLER_LOGGER_log(ERROR,"The delay before PWM high + duration of PWM high exceeds the pwm period.");
        return -1;
    }
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static float SERVO_MOTOR_calc_impulse_ms(float angle) {
    return (((1.0f/180.0f) * angle) + 1.0f);
}
