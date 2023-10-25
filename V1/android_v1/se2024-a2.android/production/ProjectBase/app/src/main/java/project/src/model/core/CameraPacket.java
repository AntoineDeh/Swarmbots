/**
 * \file  CameraPacket.java
 * \version  0.1
 * \author Matéo R
 * \date Jun 5, 2023
 * \brief Class to create a packet to be send to the UDPReceiver class to the
 * Viewer class. This packet contain the data, the length of the data and the port of
 * the UDPReceiver.
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

/* ----------------------  INCLUDES  ---------------------------------------- */

public class CameraPacket {

    /* ----------------------  PRIVATE OBJECTS  ---------------------------------------- */

    /* ----------------------  PRIVATE VARIABLES  ---------------------------------------- */

    /**
     * @brief Table of bytes from a thread UDPReceiver.
     */
    private byte[] data;

    /**
     * @brief Port from a thread UDPReceiver.
     */
    private int port;

    /**
     * @brief Length of the table of bytes from a thread UDPReceiver.
     */
    private int length;

    /* ----------------------  CONSTRUCTOR  ---------------------------------------- */

    /**
     * @brief Constructor of the class.
     * @param port Integer for a port.
     * @param data Table of bytes for data.
     * @param length Integer for length of the the table of bytes.
     */
    public CameraPacket(int port, byte[] data, int length){
        this.port = port;
        this.data = data;
        this.length = length;
    }

    /**
     * @brief Empty constructor used by the function calling the application stop.
     * @param port Integer for a port.
     */
    public CameraPacket(int port){
        this.port = port;
    }

    /* ----------------------  PUBLIC FUNCTIONS  ---------------------------------------- */

    /**
     * @brief Get the port.
     * @return port
     */
    public int getPort(){
        return this.port;
    }

    /**
     * @brief Get the data.
     * @return data
     */
    public byte[] getData(){
        return this.data;
    }

    /**
     * @brief Get the length.
     * @return length
     */
    public int getLength(){
        return this.length;
    }

    /* ----------------------  PROTECTED FUNCTIONS  ---------------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ---------------------------------------- */

}
