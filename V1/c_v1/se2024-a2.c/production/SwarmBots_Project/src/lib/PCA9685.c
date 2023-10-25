/**
 * \file  PCA9685.c
 * \version  1.1
 * \author Joshua MONTREUIL
 * \date Mar 14, 2023
 * \brief  SPCA9685 chip driver for the alphabot2 robot. Inspired from Reinhard
 * 		   Sprung's pca9685 software extension to wiringPi (init and set_freq).
 * 		   His software is under the GNU Lesser General Public License.
 *
 * \see PCA9685.h
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
#include "PCA9685.h"
#include <stdio.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#include <math.h>
#include "../logs/controller_logger.h"
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
 * \def PCA9685_MODE_1_ADDR
 * Mode 1 register address.
 */
#define PCA9685_MODE_1_ADDR 0x0
/**
 * \def PCA9685_PRESCALE_ADDR
 * Prescaler register address
 */
#define PCA9685_PRESCALE_ADDR 0xFE
/**
 * \def LEDALL_ON_L
 * ALL LEDS ON register address
 */
#define LEDALL_ON_L 0xFA
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static void PCA9685_set_PWM_Freq(int fd, float freq)
 * \brief Sets a PWM on the chip.
 * \author Joshua MONTREUIL
 *
 * \param fd : Linux file descriptor
 * \param freq : PWM frequency.
 */
static void PCA9685_set_PWM_Freq(int fd, float freq);
/**
 * \fn static int PCA9685_calc_ON_Time(float impulse_Ms, int freq)
 * \brief Computes the high period of the PWM.
 * \author Joshua MONTREUIL
 *
 * \param fd : Linux file descriptor
 * \param freq : PWM frequency.
 */
static int PCA9685_calc_ON_Time(float impulse_Ms, int freq);
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
int PCA9685_Init(const int i2c_Address, float freq) {
	int fd = wiringPiI2CSetup(i2c_Address);
	if (fd < 0) {
        CONTROLLER_LOGGER_log(ERROR, "On wiringPiI2CSetup() : PCA9685 has failed to put its setup.");
		return fd;
	}

	int wake_mode = 0;
	int init_setting = wiringPiI2CReadReg8(fd,PCA9685_MODE_1_ADDR); /* Auto_Increment + allcall on. */

	if(((init_setting & 0x10) >> 4 ) == 0x01) { /*if PCA9685 is in sleep mode */
		wake_mode = init_setting & 0xEF;
		wiringPiI2CWriteReg8(fd,PCA9685_MODE_1_ADDR, wake_mode);
	}

	int auto_inc = wake_mode | 0x20;
	wiringPiI2CWriteReg8(fd,PCA9685_MODE_1_ADDR, auto_inc);

	if(freq > 0) {
		PCA9685_set_PWM_Freq(fd,freq);
	}

	return fd;
}

int PCA9685_Write_PWM(int fd, int pin, float impulse_ms, int delay_time, int freq) {
	int on_time = PCA9685_calc_ON_Time(impulse_ms, freq);
	if((on_time + delay_time) > 100) {
		return -1;
	}
	int delay_before_on = (int) (delay_time * 4096) / 100;
	int ledx_ON_H = (delay_before_on & 0xF00) >> 8;
	int ledx_ON_L = delay_before_on & 0x0FF;

	int off_time = (int) ((on_time * 4096) / 100) + delay_before_on + 1;
	int ledx_OFF_H = (off_time & 0xF00) >> 8;
	int ledx_OFF_L = off_time & 0x0FF;

	wiringPiI2CWriteReg8(fd, pin , ledx_ON_L);
	wiringPiI2CWriteReg8(fd, (pin + 0x01), ledx_ON_H);
	wiringPiI2CWriteReg8(fd, (pin + 0x02), ledx_OFF_L);
	wiringPiI2CWriteReg8(fd, (pin + 0x03), ledx_OFF_H);
	return 0;
}

void PCA9685_PWM_Reset(int fd, int on_L_reg) {
	wiringPiI2CWriteReg8(fd,on_L_reg, 0x0);
	wiringPiI2CWriteReg8(fd,(on_L_reg + 0x01), 0x0); //on_H_reg to 0.
	wiringPiI2CWriteReg8(fd,(on_L_reg + 0x02), 0x0); //off_L_reg to 0.
	wiringPiI2CWriteReg8(fd,(on_L_reg + 0x03), 0x0); //off_H_reg to 0.
}

void PCA9685_Sleep(int fd)
{
	int current_settings = wiringPiI2CReadReg8(fd, PCA9685_MODE_1_ADDR);
	current_settings = current_settings | 0x10;

	wiringPiI2CWriteReg8(fd, PCA9685_MODE_1_ADDR, current_settings);
}
void PCA9685_Wake_Up(int fd)
{
	int current_settings = wiringPiI2CReadReg8(fd, PCA9685_MODE_1_ADDR);
	current_settings = current_settings & 0xEF;

	wiringPiI2CWriteReg8(fd, PCA9685_MODE_1_ADDR, current_settings);
}

/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static void PCA9685_set_PWM_Freq(int fd, float freq)
{
	freq = (freq > 1000 ? 1000 : (freq < 40 ? 40 : freq));

	//Formula found onto the chip datasheet :
	//prescale = round(osc_clock/ (4096 * 200) - 1.
	// with osc_clock = 25MHz
	int prescale = (int)(25000000.0f / (4096 * freq) - 1.0f);

	int settings = wiringPiI2CReadReg8(fd, PCA9685_MODE_1_ADDR);
	int sleep_mode = settings | 0x10; //put the chip in sleep mode.
	int wake_mode = settings & 0xEF; //put the chip in non-sleep mode before restart.
	int restart = wake_mode | 0x80; //restart mode.

	wiringPiI2CWriteReg8(fd, PCA9685_MODE_1_ADDR, sleep_mode);
	wiringPiI2CWriteReg8(fd, PCA9685_PRESCALE_ADDR, prescale);
	wiringPiI2CWriteReg8(fd, PCA9685_MODE_1_ADDR, wake_mode);

	sleep(0.001);
	wiringPiI2CWriteReg8(fd, PCA9685_MODE_1_ADDR, restart);
}

static int PCA9685_calc_ON_Time(float impulse_Ms, int freq)
{
	int pwm_period_Ms = (int) 1000.0f / freq;
	return (int) (impulse_Ms*100) / pwm_period_Ms;
}
