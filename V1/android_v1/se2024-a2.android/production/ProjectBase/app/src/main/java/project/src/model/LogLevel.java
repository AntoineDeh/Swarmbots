/**
 * \file  LogLevel.java
 * \version  0.1
 * \author Florentin LEPELTIER
 * \date May 18, 2023
 * \brief Log level registered in the asset file to display a specific level of logs in console.
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
package project.src.model;

/**
 * @brief Enum of the log level with its possible values.
 */
public enum LogLevel {
    DEBUG(0),
    INFO(1),
    WARNING(2),
    ERROR(3),
    NONE(4);

    /* ----------------------  ENUM VARIABLE  ---------------------------- */

    /**
     * @brief Current value of the enum.
     */
    private final int value;

    /* ----------------------  PRIVATE ENUM CONSTRUCTOR  ---------------------------- */

    /**
     * @brief Constructor of the enum command. Each enum value is associated to a value with this constructor.
     * @param value Integer value of each enum value.
     */
    private LogLevel(int value) {
        this.value = value;
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Give the value of the enum.
     * @return Value of the enum.
     */
    public int getValue() {
        return value;
    }
}
