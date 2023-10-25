/**
 * \file  radar.c
 * \version  0.1
 * \author joshua
 * \date Apr 29, 2023
 * \brief Source file of the radar peripheral.
 *
 * \see radar.h
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
#include "radar.h"
#include <wiringPi.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def RADAR_LEFT_PIN
 * Pin of the left radar sensor.
 */
#define RADAR_LEFT_PIN 27
/**
 * \def RADAR_RIGHT_PIN
 * Pin of the right radar sensor.
 */
#define RADAR_RIGHT_PIN 24
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
extern int RADAR_create(void) {
    pinMode(RADAR_LEFT_PIN, INPUT);
    pinMode(RADAR_RIGHT_PIN, INPUT);
    return 0;
}

extern int RADAR_get_radar(bool_e * obstacle_state) {
    *obstacle_state = (!digitalRead(RADAR_LEFT_PIN) | !digitalRead(RADAR_RIGHT_PIN));
    return 0;
}

extern int RADAR_destroy(void) {
    return 0;
}