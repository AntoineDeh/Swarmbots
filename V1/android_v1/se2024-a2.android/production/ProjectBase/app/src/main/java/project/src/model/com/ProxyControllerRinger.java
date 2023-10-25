/**
 * \file  ProxyControllerRinger.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Proxy of the controller ringer of the robots. Its role is to expose the public functions
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
 *
 * \copyright Prose A2 2023
 *
 */

/* ----------------------  PACKAGE  ---------------------------------------- */
package project.src.model.com;

/* ----------------------  INCLUDES  ---------------------------------------- */

import java.util.ArrayList;

public class ProxyControllerRinger
{
	/* ----------------------  PRIVATE OBJECTS  ------------------------------ */

	/**
	 * @brief List of postman used to send frames to the server.
	 */
	private ArrayList<Postman> postmanList;

	/* ----------------------  PRIVATE VARIABLES  ---------------------------- */

	/* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

	/**
	 * @brief Consctructor of the proxy of the controller ringer of the robots.
	 */
	public ProxyControllerRinger() {
	}

	/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

	/**
	 * @brief Set the postman list in the proxy. Initialized empty at the beginning.
	 */
	public void setPostManList(ArrayList<Postman> postmanList) {
		this.postmanList = postmanList;
	}

	/**
	 * @brief Ask the availability of the given robot.
	 * @param idRobot id of the robot we want the availability.
	 */
	public void askAvailability(int idRobot)
	{
		// Encode frames.
		byte[] frame = Protocol.encodeAskAvailability();

		// Send frames to the server.
		this.postmanList.get(idRobot-1).sendFrame(frame);

	}

	/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

}

