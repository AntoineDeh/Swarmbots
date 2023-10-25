/**
 * \file  buzzer.c
 * \version  0.1
 * \author Louison LEGROS
 * \date May 18, 2023
 * \brief Source file of the buzzer peripheral.
 *
 * \see buzzer.h
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
#include "../lib/defs.h"
#include <wiringPi.h>
#include "buzzer.h"
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/**
 * \def BUZZER_PIN
 * Buzzer pin.
 */
#define BUZZER_PIN 7
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
extern int BUZZER_create() {
    if((wiringPiSetup()) != 0) {
        CONTROLLER_LOGGER_log(ERROR, "On wiringPiSetup() : Buzzer failed to initialize wiringPi.");
       return -1;
    }
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    return 0;
}

extern void BUZZER_destroy() {
    digitalWrite(BUZZER_PIN, LOW);
}

extern void BUZZER_enable_buzzer() {
    digitalWrite(BUZZER_PIN, HIGH);
}

extern void BUZZER_disable_buzzer() {
    digitalWrite(BUZZER_PIN, LOW);
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */