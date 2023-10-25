/**
 * \file  config.h
 * \version  0.1
 * \author Florentin LEPELTIER
 * \author Joshua MONTREUIL
 * \date May 3, 2023
 * \brief config file for the program.
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


#ifndef CONFIG_H_
#define CONFIG_H_
/************CONFIG MACROS************/
/* GENERAL */
/**
 * \def ID_ROBOT
 * Robot identifier.
 */
#define ID_ROBOT                   2

/* LOGGER */
/**
 * \def CONFIG_LOGGER_LOG_LEVEL
 * Type of log level printed.
 * 0 prints DEBUG, INFO, WARNING and ERROR.
 * 1 prints INFO, WARNING and ERROR.
 * 2 prints WARINING and ERROR.
 * 3 only prints ERROR.
 * ( 0:DEBUG | 1:INFO | 2:WARNING | 3:ERROR )
 */
#define CONFIG_LOGGER_LOG_LEVEL    0
/**
 * \def CONFIG_LOGGER_PRINT_MODE
 * Logger print mode. ( 0:TERMINAL ONLY | 1:FILE ONLY | 2:BOTH )
 */
#define CONFIG_LOGGER_PRINT_MODE   2
/**
 * \def CONFIG_LOGGER_LOG_SIZE
 * Log size.
 */
#define CONFIG_LOGGER_LOG_SIZE     2048
/**
 * \def CONFIG_LOG_FILE_PATH
 * File path of the log file.
 */
#define CONFIG_LOG_FILE_PATH       "/home/pi/logs.txt"
/**
 * \def CONFIG_TEMP_LOG_FILE_PATH
 * File path of the temporary log file. (before the rtc is given).
 */
#define CONFIG_TEMP_LOG_FILE_PATH  "/home/pi/temp_logs.txt"

/* DISPATCHER */
/**
 * \def MAX_RECEIVED_BYTES
 * Maximum of possible received bytes.
 */
#define MAX_RECEIVED_BYTES         20

#endif /* CONFIG_H_ */
