/**
 * \file  Viewer.java
 * \version  0.1
 * \author Matéo R
 * \date Jun 2, 2023
 * \brief Singleton which create a number of thread UDPReceiver to receive
 * video from robots.
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

import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import project.src.model.com.ProxyCamera;
import project.src.model.com.UDPReceiver;
import project.src.view.fragments.CommandScreenCameraRobot1Fragment;
import project.src.view.fragments.CommandScreenCameraRobot2Fragment;

public class Viewer extends Thread{
    private static final String TAG = "Viewer";

    /* ----------------------  PRIVATE OBJECTS  ---------------------------------------- */

    /**
     * @brief ProxyCamera
     */
    private final ProxyCamera proxyCamera;

    /**
     * @brief List of robot given by GUISecretary.
     */
    private ArrayList<Robot> robotList;

    /**
     * @brief Queue to post CameraPacket from thread UDPReceiver.
     */
    private static LinkedBlockingQueue<CameraPacket> viewerCameraQueue;

    /**
     * @brief Queue to post event to GUISecretary.
     */
    private final ArrayBlockingQueue<Event> viewerQueue;

    private final ArrayList<UDPReceiver> udpReceiverList = new ArrayList<UDPReceiver>();

    /**
     * @brief States of the state machine
     */
    private enum State{
        IDLE,
        RECEIVING_IMAGE,
        STOP
    }

    /**
     * @brief State to observe for the state machine.
     */
    private State state;

    /* ----------------------  PRIVATE CONSTANTS  ---------------------------------------- */

    /**
     * @brief Number of event accepted by the Viewer queue.
     */
    private static final int MAX_CAPACITY_QUEUE = 10;

    /**
     * @brief Reference of port.
     */
    private static final int PORT = 5000;

    /**
     * @brief Reference of port signaling to quit the application and Viewer thread. It used in
     * CameraPacket object gives in the viewerCameraQueue to transfer the willing to stop the
     * application.
     */
    private static final int QUITTING_PORT = 5707;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------------------- */

    /**
     * @brief IP address from the tablet.
     */
    private String ipAddress;

    /* ----------------------  CONSTRUCTOR(S)  ---------------------------------------- */

    /**
     * @brief Instance of the singleton object of Viewer.
     */
    public static Viewer INSTANCE;

    static {
        try {
            // Instanciate the singleton object of Viewer.
            INSTANCE = new Viewer();
        } catch (IOException ignored) {
        }
    }

    /**
     * @brief Constructor of the class
     * @throws IOException Error while initialize the constructor object.
     */
    private Viewer() throws IOException {
        AddressReader addressReader = new AddressReader();
        this.proxyCamera = new ProxyCamera();
        viewerCameraQueue = new LinkedBlockingQueue<>();
        this.viewerQueue = new ArrayBlockingQueue<>(MAX_CAPACITY_QUEUE);
        this.state = State.IDLE;
        this.ipAddress = addressReader.getIPAddress();
    }

    /* ----------------------  PUBLIC FUNCTIONS  ---------------------------------------- */

    /**
     * @brief To create the instance Viewer.
     */
    public void create(){
    }

    /**
     * @brief Run method for the Viewer thread with its state machine. It will take event
     * from its queue and execute the event associated.
     */
    public void run(){
        Event event;
        CameraPacket packet = null;
        int portRobot = 0;
        while (this.state != State.STOP){
            // Catch the event in the queue.
            if (this.state != State.RECEIVING_IMAGE){
                try {
                    event = this.viewerQueue.take();
                } catch (InterruptedException ignored) {
                }
            } else {

                try {
                    packet = viewerCameraQueue.take();
                    portRobot = packet.getPort();
                } catch (InterruptedException ignored) {
                }

                // Detecting the call to stop application. Stop all UDPReceiver instance.
                if (portRobot == QUITTING_PORT) {
                    for (UDPReceiver udpReceiver : this.udpReceiverList) {
                        udpReceiver.stopCatchingPacket();
                    }
                    this.state = State.STOP;
                    continue;
                }

            }
            switch (this.state){
                case IDLE:
                    this.proxyCamera.setPostManList(GUISecretary.INSTANCE.getPostmanList());
                    this.robotList = GUISecretary.INSTANCE.getRobotList();
                    this.initInfoClients();
                    this.state = State.RECEIVING_IMAGE;
                    break;
                case RECEIVING_IMAGE:
                    if(portRobot == PORT){
                        CommandScreenCameraRobot1Fragment.decodingBytes(packet.getData(), packet.getLength());
                    } else if (portRobot == PORT+1){
                        CommandScreenCameraRobot2Fragment.decodingBytes(packet.getData(), packet.getLength());
                    }
                    break;
                default:
                    break;
            }
        }
    }

    /**
     * @brief Information about the data which will bu used by the MediaCodec and the origin of which
     * robot it came from.
     * @param port of the robot where does the data come from.
     * @param data used by the Mediacodec.
     * @param length of the data.
     */
    public static void setCamera(int port, byte[] data, int length){
        viewerCameraQueue.add(new CameraPacket(port, data, length));
    }

    /**
     * @brief To receive from GUISecretary the order to start create thread UDPReceiver.
     */
    public void setupRobotPort(){
        this.viewerQueue.add(new Event(Event.EventID.SETUP_ROBOT_PORT));
    }

    /**
     * @brief Correspond to the stop of the thread. It only uses the port to catch the information
     * of stopping the application.
     */
    public void stopReceivePacket(){
        viewerCameraQueue.add(new CameraPacket(QUITTING_PORT));
    }

    /* ----------------------  PROTECTED FUNCTIONS  ---------------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ---------------------------------------- */

    /**
     * @brief Creates a number of threads equal to the number of robots connected to the application.
     */
    private void initInfoClients(){
        for(int i = 0; i < this.robotList.size(); i++){
            if(this.robotList.get(i).getConnectionState() == Robot.ConnectionState.CONNECTED){
                UDPReceiver connect = new UDPReceiver(PORT+i, this.ipAddress);
                this.proxyCamera.setupGUIInfo(this.robotList.get(i).getId(), this.ipAddress, PORT+i);
                connect.setName("UDPReceiver" + i);
                connect.start();
                this.udpReceiverList.add(connect);
            }
        }
    }

}
