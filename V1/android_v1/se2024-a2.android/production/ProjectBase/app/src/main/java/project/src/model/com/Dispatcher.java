/**
 * \file  Dispatcher.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Dispatcher is responsible for the frames reception and dispatch to the other objects.
 * <p>
 * \section License
 * <p>
 * The MIT License
 * <p>
 * Copyright (c) 2023, Prose A2 2023
 * <p>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p>
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * <p>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * <p>
 * \copyright Prose A2 2023
 *
 */

/* ----------------------  PACKAGE  ---------------------------------------- */
package project.src.model.com;

/* ----------------------  INCLUDES  ---------------------------------------- */

import java.io.IOException;
import java.util.concurrent.ArrayBlockingQueue;

import project.src.model.core.Event;
import project.src.model.core.GUI;
import project.src.model.core.GUIRinger;
import project.src.model.core.GUISecretary;
import project.src.model.core.LogsManager;

public class Dispatcher extends Thread implements Postman.ConnectionObserver
{
	/* ----------------------  PRIVATE CONSTANT  ------------------------------ */

	/**
	 * @brief Number of messages accepted by the Dispatcher queue.
	 */
	private static final int MAX_CAPACITY_QUEUE = 20;

	/* ----------------------  PRIVATE OBJECTS  ------------------------------ */
	/**
	 * @brief Postman to send message to the robot.
	 */
	private final Postman postman;

	/**
	 * @brief Id robot we are responsible to.
	 */
	private final int idRobot;

	/**
	 * @brief Connection state between the robot and the tablet.
	 */
	private enum ConnectedState {
		NOT_CONNECTED,
		CONNECTED
	}

	/**
	 * @brief Current state of the connection.
	 */
	private ConnectedState connectedState;

	/* ----------------------  PRIVATE VARIABLES  ---------------------------- */

	/**
	 * @brief Dispatcher queue to start the socket read.
	 */
	private final ArrayBlockingQueue<ConnectedState> dispatcherQueue;

	/* ----------------------  CONSTRUCTOR(S)  -------------------------------- */
	/**
	 * @brief Constructor of the dispatcher object.
	 */
	public Dispatcher(Postman postman, int idRobot) {
		this.postman = postman;
		this.idRobot = idRobot;

		this.dispatcherQueue = new ArrayBlockingQueue<>(MAX_CAPACITY_QUEUE);
		this.connectedState = ConnectedState.NOT_CONNECTED;

		this.postman.addObserver(this);
	}

	/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
	/**
	 * @brief Main function of Dispatcher thread. It receives the connection state of the robot to
	 * begin reading or not the socket.
	 */
	@Override
	public void run() {
		try {
			this.connectedState = this.dispatcherQueue.take();
			Event event;
			while (this.connectedState == ConnectedState.CONNECTED) {
				// Read and take the message size
				byte[] frameSize = this.postman.readService(2);

				if (frameSize[1] != 0) {
					// Read and take the message depending on its size (4th place in the frame)
					byte[] frame = this.postman.readService(frameSize[1]);

					event = Protocol.decode(this.idRobot, frame);
					this.dispatch(event);
				} else {
					this.connectedState = ConnectedState.NOT_CONNECTED;
				}
			}
		} catch (InterruptedException | IOException e) {
			this.connectedState = ConnectedState.NOT_CONNECTED;
		}
	}

	/**
	 * @brief Distributes message to concerned objects.
	 */
	void dispatch(Event event)
	{
		switch (event.getEventID()) {
			case SET_AVAILABILITY :
				GUIRinger.INSTANCE.setAvailability(event.getIdRobot());
				break;
			case SET_MODE :
				GUISecretary.INSTANCE.setMode(event.getIdRobot(), event.getOperatingMode());
				break;
			case SET_LOGS :
				LogsManager.INSTANCE.setLogs(event.getIdRobot(), event.getLogs());
				break;
			case ALERT :
				GUI.INSTANCE.raiseMemoryAlert(event.getIdRobot(), event.getIdAlert());
				break;
			case DISCONNECTED_OK:
				GUISecretary.INSTANCE.disconnectedOk(event.getIdRobot());
				break;
			case SET_RADAR :
				GUISecretary.INSTANCE.setRadar(event.getIdRobot(), event.getRadarState());
				break;
			default:
				break;
		}
	}

	/**
	 * @brief Notify that we are connected to the robot.
	 */
	@Override
	public void onConnectionEstablished() {
		this.dispatcherQueue.add(ConnectedState.CONNECTED);
		GUISecretary.INSTANCE.ackConnection(this.idRobot);
	}

	/**
	 * @brief Notify that the connection with the robot is lost.
	 */
	@Override
	public void onConnectionLost() {
		this.dispatcherQueue.add(ConnectedState.NOT_CONNECTED);
		GUISecretary.INSTANCE.notifyDisconnectedRobot(this.idRobot, true);
	}

	/**
	 * @brief Notify that the connection has failed.
	 */
	@Override
	public void onConnectionFailed() {
		this.dispatcherQueue.add(ConnectedState.NOT_CONNECTED);
		GUISecretary.INSTANCE.connectionFailed(this.idRobot);
	}
	/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
}

