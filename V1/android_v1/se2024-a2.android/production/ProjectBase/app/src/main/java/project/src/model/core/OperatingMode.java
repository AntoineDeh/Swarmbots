/**
 * \file  OperatingMode.java
 * \version  0.1
 * \author Florentin LEPELTIER & Julien NICOT
 * \date May 18, 2023
 * \brief Operating mode of the robots for each peripheral. It informs about the activation state
 * of the peripherals.
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

/* ----------------------  PACKAGE  ---------------------------------------- */
package project.src.model.core;

import project.src.model.PeriphState;

public class OperatingMode {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief Peripheral Camera
     */
    private int camera;

    /**
     * @brief Peripheral Radar
     */
    private int radar;

    /**
     * @brief Peripheral Leds
     */
    private int leds;

    /**
     * @brief Peripheral Buzzer
     */
    private int buzzer;

    /* ----------------------  PUBLIC OBJECTS  ------------------------------ */

    /**
     * @brief Enum of peripheral.
     */
    public enum PeriphToChange {
        CAMERA,
        RADAR,
        BUZZER,
        LEDS
    }

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Contructor of the class.
     * @param camera peripheral.
     * @param radar peripheral.
     * @param buzzer peripheral.
     * @param leds peripheral.
     */
    public OperatingMode(PeriphState camera, PeriphState radar, PeriphState buzzer, PeriphState leds) {
        this.camera = camera.ordinal();
        this.radar = radar.ordinal();
        this.buzzer = buzzer.ordinal();
        this.leds = leds.ordinal();
    }

    /**
     * @brief Default constructor.
     */
    public OperatingMode(){
        this.camera = PeriphState.ENABLED.ordinal();
        this.radar = PeriphState.ENABLED.ordinal();
        this.buzzer = PeriphState.ENABLED.ordinal();
        this.leds = PeriphState.ENABLED.ordinal();
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Get the state of the peripheral Camera.
     * @return camera.
     */
    public int getCamera(){return  this.camera;}

    /**
     * @brief Get the state of the peripheral Radar.
     * @return radar.
     */
    public int getRadar(){return this.radar;}

    /**
     * @brief Get the state of the peripheral Leds.
     * @return Leds.
     */
    public int getLeds(){return this.leds;}

    /**
     * @brief Get the state of the peripheral Buzzer.
     * @return buzzer.
     */
    public int getBuzzer(){return this.buzzer;}

    /**
     * @brief To set states of peripherals
     * @param camera peripheral.
     * @param radar peripheral.
     * @param buzzer peripheral.
     * @param leds peripheral.
     */
    public void setOperatingMode(PeriphState camera, PeriphState radar, PeriphState buzzer, PeriphState leds){
        this.camera = camera.ordinal();
        this.radar = radar.ordinal();
        this.buzzer = buzzer.ordinal();
        this.leds = leds.ordinal();
    }

    /**
     * @brief To set only one state of peripheral.
     * @param periphToChange peripheral wanted.
     * @param periphState state wanted.
     */
    public void setOneOperatingMode(PeriphToChange periphToChange, PeriphState periphState){
        switch (periphToChange) {
            case CAMERA :
                this.camera = periphState.ordinal();
                break;
            case RADAR :
                this.radar = periphState.ordinal();
                break;
            case BUZZER :
                this.buzzer = periphState.ordinal();
                break;
            case LEDS :
                this.leds = periphState.ordinal();
                break;
            default :
                break;
        }
    }

    /**
     * @brief get the object OperatingMode
     * @return object OperatingMode
     */
    public OperatingMode get(){
        return this;
    }
    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
}
