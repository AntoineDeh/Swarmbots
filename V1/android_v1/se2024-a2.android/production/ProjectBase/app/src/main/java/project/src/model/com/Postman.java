/**
 * \file  Postman.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Postman is responsible to send frames to the server and return frames sent by the server.
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

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.ArrayBlockingQueue;


public class Postman extends Thread implements Observer
{
	/* ----------------------  PRIVATE CONSTANT  ------------------------------ */

	/**
	 * @brief Max capacity of the postman queue
	 */
	private static final int MAX_CAPACITY_QUEUE = 20;

	/**
	 * @brief TimeOut to wait for the client socket connection.
	 */
	private static final int TIME_OUT_SOCKET_CONNECTION = 1500;

	/* ----------------------  PRIVATE OBJECTS  ------------------------------ */
	/**
	 * TAG of the postman object.
	 */
	private static final String TAG = "Postman";

    /**
	 * Connection observer interface to inform the other objects about the state of the connection.
	 */
	public interface ConnectionObserver {
		void onConnectionEstablished();
		void onConnectionLost();
		void onConnectionFailed();
	}

	/**
	 * @brief List of observers. Type of the interface ConnectionObserver.
	 */
	private final List<ConnectionObserver> observers = new ArrayList<>();

	/**
	 * @brief Client socket, responsible of the connection to the server.
	 */
	private Socket socket;

	/* ----------------------  PRIVATE VARIABLES  ---------------------------- */

	/**
	 * @brief Connection ip address of the server.
	 */
	private final String addresse;

	/**
	 * @brief Connection port of the server.
	 */
	private int port;

	/**
	 * @brief Stream buffer to bytes arrays to send send messages to the server.
	 */
	DataOutputStream dataOutputStream;

	/**
	 * @brief Postman queue to send messages.
	 */
	private final ArrayBlockingQueue<byte[]> postmanQueue;

	/**
	 * @brief Connection state with the server.
	 */
	private static boolean isConnected = false;

	/**
	 * @brief Id robot that this postman corresponds.
	 * @note There is one postman for every robot and it gives its idRobot to recognize it. This
	 * id robot is also used in the dispatcher associated.
	 */
	private final int idRobot;
	/* ----------------------  CONSTRUCTOR(S)  -------------------------------- */
	/**
	 * @brief Postman constructor.
	 */
	public Postman(int idRobot, String addresse, int port) {
		this.idRobot = idRobot;
		this.addresse = addresse;
		this.port = port;
		this.postmanQueue = new ArrayBlockingQueue<>(MAX_CAPACITY_QUEUE);

		// Instantiate the dispatcher
		Dispatcher dispatcher = new Dispatcher(this, this.idRobot);
		dispatcher.start();
	}

	/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

	/**
	 * @brief Adding an object as observer in the list.
	 * @param observer objects that use the notifies functions.
	 */
	public void addObserver(ConnectionObserver observer) {
		observers.add(observer);
	}

	/**
	 * @brief Removing an object in the observers' list.
	 * @param observer objects we want to remove.
	 */
	public void removeObserver(ConnectionObserver observer) {
		observers.remove(observer);
	}

	/**
	 * @brief Notifying all observers that the connection is established.
	 */
	private void notifyConnectionEstablished() {
		for (ConnectionObserver observer : observers) {
			observer.onConnectionEstablished();
		}
		isConnected = true;
	}

	/**
	 * @brief Notifying all observers that the connection has failed.
	 */
	private void notifyConnectionFailed(){
		for (ConnectionObserver observer : observers) {
			observer.onConnectionFailed();
		}
	}

	/**
	 * @brief Notifying all observers that the connection is lost.
	 */
	private void notifyConnectionLost(){
		for (ConnectionObserver observer : observers) {
			observer.onConnectionLost();
		}
	}

	/**
	 * @brief Gives to id robot that the postman is associated.
	 */
	public int getIdRobot(){
		return this.idRobot;
	}

	/**
	 * @brief Observer pattern to signal other objects of events.
	 * @param o     the observable object.
	 * @param arg   an argument passed to the {@code notifyObservers} method.
	 */
	@Override
	public void update(Observable o, Object arg) {
	}

	/**
	 * @brief Thread associated to sending messages (frames) to the server.
	 */
	@Override
	public void run() {
		// Initiate the connection to the server.
		this.connectService();

		if (isConnected) {
			try {
				this.sendLoopService();
			} catch (IOException e) {
				notifyConnectionLost();
			}

			try {
				this.socket.close();
			} catch (IOException ignored) {
			}

		}
	}

	/**
	 * Read the socket and return the frames given.
	 * @param frameSize Size of the buffer to return.
	 */
	public byte[] readService(int frameSize) throws IOException {
		byte[] buffer = new byte[frameSize];
		int bytesRead;
		try {
			// Read the socket to store in the buffer.
			bytesRead = this.socket.getInputStream().read(buffer);

			// If we received -1, throw en error
			if (bytesRead == -1) throw new IOException();

		} catch (IOException ex) {
			// The connection with the server is lost
			notifyConnectionLost();
		}
		return buffer;
	}

	/**
	 * @brief Frame sending function for all message to send to the robots. This function put a
	 * message into the postmanQueue.
	 * @param frame Frame we want to send.
	 */
	public void sendFrame(byte[] frame) { if (isConnected) this.postmanQueue.add(frame); }

	/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

	/**
	 * @brief Connect the Postman to the server. If the server didn't respond under the
	 * TIME_OUT_SOCKET_CONNECTION, notify all observers that the connection has failed.
	 */
	private void connectService()
	{
		try {
			// Initialize the socket
			this.socket = new Socket();
			InetSocketAddress inetSocketAddress = new InetSocketAddress(InetAddress.getByName(this.addresse), this.port);

			// Launch the connection with the server and start the timeout connection.
			this.socket.connect(inetSocketAddress, TIME_OUT_SOCKET_CONNECTION);

			// Notify the pattern observer : connection established
			notifyConnectionEstablished();

			// Prepare the buffers to send them to the server
			OutputStream outputStream = socket.getOutputStream();
			this.dataOutputStream = new DataOutputStream(outputStream);

		} catch (IOException e) {
			// Notify the pattern observer : connection failed
			notifyConnectionFailed();
			e.printStackTrace();
		}
	}

	/**
	 * Read postmanQueue to send message (frame) to the server.
	 */
	private void sendLoopService() throws IOException {
		while (isConnected) {
			try {
				// Wait for the frame in the postman queue and send it to the server.
				byte[] frame;
				frame = this.postmanQueue.take();

				// Sending the frame to the server.
				if (this.dataOutputStream != null) {
					this.dataOutputStream.write(frame);
				}

			} catch (InterruptedException interruptedException) {
				throw new RuntimeException(interruptedException);
			}
		}
	}
}

