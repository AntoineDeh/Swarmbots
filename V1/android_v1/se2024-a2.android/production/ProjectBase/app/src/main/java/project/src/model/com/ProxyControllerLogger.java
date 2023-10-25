/**
 * \file  ProxyControllerLogger.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Proxy of the controller logger of the robots. Its role is to expose the public functions
 * of the object for this application to call them.
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

import java.util.ArrayList;

import project.src.model.core.LogsManager;

public class ProxyControllerLogger
{

	/* ----------------------  PRIVATE OBJECTS  ------------------------------ */

	/**
	 * @brief List of postman used to send frames to the server.
	 */
	private ArrayList<Postman> postmanList;

	/* ----------------------  PRIVATE VARIABLES  ---------------------------- */

	/* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

	/**
	 * @brief Consctructor of the proxy of the controller pilot of the robots.
	 */
	public ProxyControllerLogger() {
	}

	/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

	/**
	 * @brief Set the postman list in the proxy. Initialized empty at the beginning.
	 */
	public void setPostManList(ArrayList<Postman> postmanList) {
		this.postmanList = postmanList;
	}

	/**
	 * @brief Ask the logs to the robot.
	 */
	public void askLogs(int selectedRobot) {
		// Encode the message
		byte[] frame = Protocol.encodeAskLogs();

		// TODO : Should be deleted after implementing the logs send in the tcp communication.
		ArrayList<String> arrayList = new ArrayList<String>(){};
		arrayList.add("Logs sent !");
		LogsManager.INSTANCE.setLogs(selectedRobot, arrayList);

		// Send to the server with the postman
		// TODO : Uncomment after implementing the logs send in the tcp communication.
		//this.postmanList.get(selectedRobot-1).sendFrame(frame);
	}

	/**
	 * @brief Ask the logs to the robot.
	 */
	public void askSetRtc(int selectedRobot, String rtc) {
		// Encode the message
		ArrayList<Integer> rtcDecomposed = Protocol.decomposeRtc(rtc);
		byte[] frame = Protocol.encodeSetCurrentTime(rtcDecomposed.get(0), rtcDecomposed.get(1),
				rtcDecomposed.get(2), rtcDecomposed.get(3), rtcDecomposed.get(4), rtcDecomposed.get(5));

		// Send to the server with the postman
		//This function is not implemented now.
	}

	public void logsReceived(int selectedRobot) {
		// Encode the message
		byte[] frame = Protocol.encodeLogsReceived();

		// TODO : Get the end of init from GUISecretary to LogsManager and getPostmanList to set this object.

		// Send to the server with the postman
		//this.postmanList.get(selectedRobot-1).sendFrame(frame);
	}

	/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

}

