/**
 * \file  ProxyControllerCore.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Proxy of the controller core of the robots. Its role is to expose the public functions of
 * the object for this application to call them.
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

import project.src.model.core.OperatingMode;
import project.src.model.core.Robot;

public class ProxyControllerCore
{

	/* ----------------------  PRIVATE OBJECTS  ------------------------------ */

	/**
	 * @brief List of postman used to send frames to the server.
	 */
	private ArrayList<Postman> postmanList = new ArrayList<>();

	/* ----------------------  PRIVATE VARIABLES  ---------------------------- */

	/* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

	/**
	 * @brief Constructor of the proxy of the controller core of the robots.
	 */
	public ProxyControllerCore() {
	}

	/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

	/**
	 * @brief Set the postman list in the proxy. Initialized empty at the beginning.
	 */
	public void setPostmanList(ArrayList<Postman> postmanList) {
		this.postmanList = postmanList;
	}

	/**
	 * @brief Send to the robot to set new operating mode.
	 *
	 */
	public void setState(int selectedRobot, Robot.SelectionState state) {
		// Encode frames
		byte[] frame = Protocol.encodeSetState(state);

		// Send it to the server.
		this.postmanList.get(selectedRobot-1).sendFrame(frame);
	}

	/**
	 * @brief Ask the operating mode of the robot chosen.
	 * @param idRobot id of the robot we ask the operating mode.
	 */
	public void askMode(int idRobot)
	{
		// Encodage du message
		byte[] frame = Protocol.encodeAskMode();

		// Send it to the server.
		this.postmanList.get(idRobot-1).sendFrame(frame);
	}

	/**
	 * @brief Set the operating mode of the given robot.
	 * @param selectedRobot robot we want to set the new operating mode.
	 * @param operatingMode new operating mode to set.
	 */
	public void askSetMode(int selectedRobot, OperatingMode operatingMode)
	{
		// Encode frames.
		byte[] frame = Protocol.encodeSetMode(operatingMode.getCamera(), operatingMode.getRadar(),
				operatingMode.getBuzzer(), operatingMode.getLeds());

		// Send it to the server.
		this.postmanList.get(selectedRobot-1).sendFrame(frame);
	}

	/**
	 * @brief Ask the logs of the given robot.
	 * @param selectedRobot id of the robot we ask the logs.
	 */
	public void askLogs(int selectedRobot)
	{
		// Encode the frame
		byte[] frame = Protocol.encodeAskLogs();

		// Send it to the server.
		this.postmanList.get(selectedRobot-1).sendFrame(frame);
	}

	/**
	 * @brief Ask to disconnect the connection to the robot.
	 * @param selectedRobot idRobot of the robot.
	 */
    public void askToDisconnect(int selectedRobot) {
		// Encode the frame
		byte[] frame = Protocol.encodeAskToDisconnect();

		// Send it to the server.
		this.postmanList.get(selectedRobot-1).sendFrame(frame);
    }

	/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

}

