/**
 * \file controller_logger_cmocka.c
 * \version  1.1
 * \author Joshua MONTREUIL
 * \author Fatoumata TRAORE
 * \date Jun 06, 2023
 * \brief mock file of controller logger.
 *
 * \see ../../src/logs/controller_logger.c
 * \see ../../src/logs/controller_logger.h
 * \see controller_logger_test.c
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
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"
#include "../../src/logs/controller_logger.h"

/**
 * \fn int __wrap_CONTROLLER_LOGGER_log(log_level_e log_level, const char* msg)
 * \brief Mock function of log.
 * \author Joshua MONTREUIL
 *
 * \see ../../src/logs/controller_logger.c
 */
int __wrap_CONTROLLER_LOGGER_log(log_level_e log_level, const char* msg) {
    function_called();

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_LOGGER_ask_logs(Id_Robot id_robot)
 * \brief Mock function of ask_logs.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/logs/controller_logger.c
 */
int __wrap_CONTROLLER_LOGGER_ask_logs(Id_Robot id_robot) {
    function_called();

    check_expected(id_robot);

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_LOGGER_ask_set_rtc(Id_Robot id_robot,time_t rtc)
 * \brief Mock function of ask_set_rtc.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/logs/controller_logger.c
 */
int __wrap_CONTROLLER_LOGGER_ask_set_rtc(Id_Robot id_robot,time_t rtc) {
    function_called();

    check_expected(id_robot);
    check_expected(rtc);

    return (int) mock();
}
/**
 * \fn int __wrap_CONTROLLER_LOGGER_logs_saved(Id_Robot id_robot)
 * \brief Mock function of logs_saved.
 * \author Fatoumata TRAORE
 *
 * \see ../../src/logs/controller_logger.c
 */
int __wrap_CONTROLLER_LOGGER_logs_saved(Id_Robot id_robot){
    function_called();

    check_expected(id_robot);

    return (int) mock();
}




