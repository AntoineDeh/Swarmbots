/**
 * \file  ProxyCamera.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Proxy of the camera of the robots. Its role is to expose the public functions of the
 * object for this application to call them.
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

public class ProxyCamera
{
	/* ----------------------  PRIVATE OBJECTS  ------------------------------ */

	/**
	 * @brief List of postman used to send frames to the server.
	 */
	private ArrayList<Postman> postmanList;

	/* ----------------------  PRIVATE VARIABLES  ---------------------------- */

	/**
	 * @brief Tag of the proxy camera object.
	 */
	private static final String TAG = "ProxyCamera";

	/* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

	/**
	 * @brief Consctructor of the proxy of the controller pilot of the robots.
	 */
	public ProxyCamera() {
	}

	/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

	/**
	 * @brief Set the postman list in the proxy. Initialized empty at the beginning.
	 */
	public void setPostManList(ArrayList<Postman> postmanList) {
		this.postmanList = postmanList;
	}

	/**
	 * @brief Ask to setup the gui info to the robot.
	 */
	public void setupGUIInfo(int selectedRobot, String ipAddress, int port) {
		// Encode the message
		byte[] frame = Protocol.encodeSetupGUIInfo(ipAddress, port);

		// Send to the server with the postman
		this.postmanList.get(selectedRobot-1).sendFrame(frame);
	}

	/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
}

