/**
 * \file  PCA9685.h
 * \version  1.1
 * \author Joshua MONTREUIL
 * \date Mar 14, 2023
 * \brief  SPCA9685 chip driver for the alphabot2 robot. Inspired from Reinhard
 * 		   Sprung's pca9685 software extension to wiringPi (init and set_freq).
 * 		   His software is under the GNU Lesser General Public License.
 *
 * \see PCA9685.c
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
#ifndef SRC_LIB_PCA9685_H_
#define SRC_LIB_PCA9685_H_
/* ----------------------  INCLUDES ------------------------------------------*/
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern int PCA9685_Init(const int i2c_Address, float freq)
 * \brief Init function of the chip.
 * \author Joshua MONTREUIL
 *
 * \param i2c_Address : address of the driver on the system.
 * \param freq : frequency to be computed and sent in the PRE_SCALE reg.
 *
 * \return int fd, linux file descriptor of the chip or -1 on error.
 */
extern int PCA9685_Init(const int i2c_Address, float freq);
/**
 * \fn extern int PCA9685_Write_PWM(int fd, int pin, int impulse_ms, int delay_time, int freq)
 * \brief Calculates On and off delay and times in order to compute the right PWM.
 * 		  Then sets the regs from those values computed in order to generate the PWM.
 * \brief Joshua MONTREUIL
 *
 * \param fd : Linux file descriptor, linked to the chip.
 * \param pin : reg to be written to. (!! MUST BE A LEDx_ON_L reg type)
 * \param impulse_ms : duration of the pulsation (PWM to ON).
 * \param delay_time : OFF duration before ON cycle.
 * \param freq : current frequency (Must be the same freq as the one set for the chip).
 *
 * \return '-1' if the delay_time + on_time exceeds 100% of the total period. 0 if all ok.
 */
extern int PCA9685_Write_PWM(int fd, int pin, float impulse_ms, int delay_time, int freq);
/**
 * \fn extern void PCA9685_PWM_Reset(int fd, int on_L_reg)
 * \brief Function used to set all of the registers of a PWM to 0. (LEDx_ON_L,
 * 		  LEDx_ON_H, LEDx_OFF_L, LEDx_OFF_H). This needs the first reg in parameter as
 * 		  setting the other ones to 0 is computed from the first one.
 * \author Joshua MONTREUIL
 *
 * \param fd : Linux file descriptor linked to the I2C peripheral.
 * \param on_L_reg : The first register that need to be set to 0. This register NEEDS to be LEDx_ON_L -> see datasheet..
 */
extern void PCA9685_PWM_Reset(int fd, int on_L_reg);
/**
 * \fn extern void PCA9685_Sleep(int fd)
 * \brief puts the chip into sleep mode.
 * \author Joshua MONTREUIL
 *
 * \param fd : Linux file descriptor.
 */
extern void PCA9685_Sleep(int fd);
/**
 * \fn extern void PCA9685_Wake_Up(int fd)
 * \brief puts the chip wake up mode.
 * \author Joshua MONTREUIL
 *
 * \param fd : Linux file descriptor.
 */
extern void PCA9685_Wake_Up(int fd);
#endif /* SRC_LIB_PCA9685_H_ */
