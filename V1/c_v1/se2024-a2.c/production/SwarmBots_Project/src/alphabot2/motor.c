/**
 * \file  motor.c
 * \version  0.1
 * \author joshua
 * \date Apr 30, 2023
 * \brief Source file of the motor peripheral.
 *
 * \see motor.h
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
 */
/* ----------------------  INCLUDES  ---------------------------------------- */
#include "motor.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def AIN1
 * Motor A pin 1.
 */
#define AIN1 26
/**
 * \def AIN2
 * Motor A pin 2.
 */
#define AIN2 23
/**
 * \def PWM_A
 * Motor A pwm pin.
 */
#define PWM_A 22
/**
 * \def BIN1
 * Motor B pin 1.
 */
#define BIN1 28
/**
 * \def BIN2
 * Motor B pin 2.
 */
#define BIN2 29
/**
 * \def PWM_B
 * Motor B pwm pin.
 */
#define PWM_B 25
/**
 * \def VELOCITY_DEFAULT
 * Default velocity
 */
#define VELOCITY_DEFAULT 50
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int MOTOR_create(void) {
    pinMode(AIN1, OUTPUT); /* Init of the gpio's Motor pin as output */
    pinMode(AIN2, OUTPUT); /* Init of the gpio's Motor pin as output */

    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    if (softPwmCreate(PWM_A, 0, 100) < 0) {
        CONTROLLER_LOGGER_log(ERROR,"On softPwmCreate() : ERROR while creating software pwm on pin A.");
        return -1;
    }
    if (softPwmCreate(PWM_B, 0, 100) < 0) {
        CONTROLLER_LOGGER_log(ERROR,"On softPwmCreate() : ERROR while creating software pwm on pin A.");
        return -1;
    }
    return 0;
}

void MOTOR_set_velocity(Command cmd) {
    softPwmWrite(PWM_A, VELOCITY_DEFAULT);
    softPwmWrite(PWM_B, VELOCITY_DEFAULT);
    switch (cmd) {
        case FORWARD : {
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, HIGH);
            digitalWrite(BIN1, LOW);
            digitalWrite(BIN2, HIGH);
            break;
        }
        case RIGHT : {
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, HIGH);
            digitalWrite(BIN1, HIGH);
            digitalWrite(BIN2, LOW);
            break;
        }
        case LEFT : {
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
            digitalWrite(BIN1, LOW);
            digitalWrite(BIN2, HIGH);
            break;
        }
        case BACKWARD : {
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
            digitalWrite(BIN1, HIGH);
            digitalWrite(BIN2, LOW);
            break;
        }
        case STOP : {
            softPwmWrite(PWM_A, 0);
            softPwmWrite(PWM_B, 0);
            break;
        }
        default : {
            break;
        }
    }
}

int MOTOR_destroy(void) {
    softPwmStop(PWM_A);
    softPwmStop(PWM_B);
    return 0;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
