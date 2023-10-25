/**
 * \file  UDPReceiver.java
 * \version  0.1
 * \author Matéo R
 * \date May 18, 2023
 * \brief Thread to receive packet video from a UDP socket.
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

package project.src.model.com;

/* ----------------------  INCLUDES  ---------------------------------------- */

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import project.src.model.core.Viewer;

public class UDPReceiver extends Thread {

    /* ----------------------  PRIVATE OBJECTS  ---------------------------------------- */

    /**
     * @brief Socket of the localhost to receive packet
     */
    private DatagramSocket socket;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------------------- */

    /**
     * @brief Référence for a port.
     */
    private static final int PORT = 5000;

    /**
     * @brief Port from Viewer.
     */
    private int port;

    /**
     * @brief IP address from Viewer.
     */
    private String ipAddress;

    /**
     * @brief Table of byte received from the packet received by UDP.
     */
    private byte[] data;

    /**
     * @brief Size of the table of byte received from the packet received by UDP.
     */
    private int length;

    /**
     * @brief State of UDPReceiver to loop catching the camera stream.
     */
    private enum State {
        RUN,
        STOP
    };

    /**
     * @brief Current state of the UDPReceiver object.
     */
    private State state = State.RUN;

    /* ----------------------  CONSTRUCTOR  ---------------------------------------- */

    /**
     * @brief Constructor of the class.
     */
    public UDPReceiver(int port, String iPaddress){
        this.port = port;
        this.ipAddress = iPaddress;
    }

    /* ----------------------  PUBLIC FUNCTIONS  ---------------------------------------- */

    /**
     * @brief Thread associated to create the socket to receive camera UDP packet
     */
    @Override
    public void run() {
        this.initSocket();
    }

    /* ----------------------  PROTECTED FUNCTIONS  ---------------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ---------------------------------------- */

    /**
     * @brief Creation of the socket and listen this socket to receive packet,
     * post the data and length to viewer in a CameraPacket.
     */
    private void initSocket(){
        try {
            this.socket = new DatagramSocket(this.port, InetAddress.getByName(this.ipAddress)); // Port d'écoute UDP
            byte[] buffer = new byte[65535];

            while(this.state != State.STOP){

                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                this.socket.receive(packet);
                this.data = packet.getData();
                this.length = packet.getLength();

                //Send camerapacket to the message queue viewer
                Viewer.setCamera(this.port, this.data, this.length);

            }
        } catch (IOException e) {

        } finally {
            if (this.socket != null) {
                this.socket.close();
            }
        }
    }

    /**
     * @brief Command to stop to receive datagram packet.
     */
    public void stopCatchingPacket() {
        this.state = State.STOP;
    }
}
