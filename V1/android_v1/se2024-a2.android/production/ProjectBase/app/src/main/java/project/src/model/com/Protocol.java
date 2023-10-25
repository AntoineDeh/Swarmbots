/**
 * \file  Protocol.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Protocol is responsible for encoding frames to send to the server, and decoding frames
 * received from the server.
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

import project.src.model.Command;
import project.src.model.PeriphState;
import project.src.model.core.Event;
import project.src.model.core.OperatingMode;
import project.src.model.core.Robot;

public final class Protocol
{
	/* ----------------------  PRIVATE OBJECTS  ------------------------------ */

	/* ----------------------  PRIVATE & PUBLIC VARIABLES  ---------------------------- */

	/**
	 * Id message of askAvailability frame.
	 */
	public static final byte ID_ASK_AVAILABILITY = 0x01;

	/**
	 * Size of askAvailability frame.
	 */
	private static final byte SIZE_ASK_AVAILABILITY = 0x02;

	/**
	 * Id message of setAvailability frame.
	 */
	public static final byte ID_SET_AVAILABILITY = 0x02;

	/**
	 * Size of setAvailability frame.
	 */
	private static final byte SIZE_SET_AVAILABILITY = 0x02;

	/**
	 * Id message of askCmd frame.
	 */
	public static final byte ID_ASK_CMD = 0x03;

	/**
	 * Size of askCmd frame.
	 */
	private static final byte SIZE_ASK_CMD = 0x03;

	/**
	 * Forward command frame.
	 */
	public static final byte FORWARD_COMMAND = 0x00;

	/**
	 * Right command frame.
	 */
	public static final byte RIGHT_COMMAND = 0x01;

	/**
	 * Left command frame.
	 */
	public static final byte LEFT_COMMAND = 0x02;

	/**
	 * Backward command frame.
	 */
	public static final byte BACKWARD_COMMAND = 0x03;

	/**
	 * Stop command frame.
	 */
	public static final byte STOP_COMMAND = 0x04;

	/**
	 * Id message of setState frame.
	 */
	public static final byte ID_SET_STATE = 0x04;

	/**
	 * Size of setState frame.
	 */
	private static final byte SIZE_SET_STATE = 0x03;

	/**
	 * Id message of askMode frame.
	 */
	public static final byte ID_ASK_MODE = 0x05;

	/**
	 * Size of askMode frame.
	 */
	private static final byte SIZE_ASK_MODE = 0x02;

	/**
	 * Id message of setMode frame.
	 */
	public static final byte ID_SET_MODE = 0x06;

	/**
	 * Size of setMode frame.
	 */
	private static final byte SIZE_SET_MODE = 0x06;

	/**
	 * Id message of askLogs frame.
	 */
	public static final byte ID_ASK_LOGS = 0x07;

	/**
	 * Size of askLogs frame.
	 */
	private static final byte SIZE_ASK_LOGS = 0x02;

	/**
	 * Id message of setLogs frame.
	 */
	public static final byte ID_SET_LOGS = 0x08;

	/**
	 * Size of setLogs frame.
	 */
	private static final byte SIZE_SET_LOGS = 0x02;

	/**
	 * Id message of alert frame.
	 */
	public static final byte ID_ALERT = 0x09;

	/**
	 * Size of alert frame.
	 */
	private static final byte SIZE_ALERT = 0x03;

	/**
	 * Id message of askToDisconnect frame.
	 */
	public static final byte ID_ASK_TO_DISCONNECT = 0x10;

	/**
	 * Size of askToDisconnect frame.
	 */
	private static final byte SIZE_ASK_TO_DISCONNECT = 0x02;

	/**
	 * Id message of ackDisconnection frame.
	 */
	public static final byte ID_ACK_DISCONNECTION = 0x11;

	/**
	 * Size of ackDisconnection frame.
	 */
	private static final byte SIZE_ACK_DISCONNECTION = 0x02;

	/**
	 * Id message of setRadar frame.
	 */
	public static final byte ID_SET_RADAR = 0x12;

	/**
	 * Size of setRadar frame.
	 */
	private static final byte SIZE_SET_RADAR = 0x02;

	/**
	 * Id message of setCurrentTime frame.
	 */
	public static final byte ID_SET_CURRENT_TIME = 0x13;

	/**
	 * Size of setCurrentTime frame.
	 */
	private static final byte SIZE_SET_CURRENT_TIME = 0x09;

	/**
	 * Id message of setIpPort frame.
	 */
	public static final byte ID_SET_IP_PORT = 0x14;

	/**
	 * Size of setIpPort frame.
	 */
	private static final byte SIZE_SET_IP_PORT = 0x08;

	/**
	 * Id message of logsReceived frame.
	 */
	public static final byte ID_LOGS_RECEIVED = 0x15;

	/**
	 * Size of logsReceived frame.
	 */
	private static final byte SIZE_LOGS_RECEIVED = 0x02;

	/* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

	/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

	/**
	 * Encode the frame to send askAvailability to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeAskAvailability(){
		return new byte[]{0x0, Protocol.SIZE_ASK_AVAILABILITY,
				0x0, Protocol.ID_ASK_AVAILABILITY};
	}

	/**
	 * Encode the frame to send askAvailability to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeAskCmd(Command cmd){
		return new byte[]{0x0, Protocol.SIZE_ASK_CMD, 0x0, Protocol.ID_ASK_CMD, cmd.getValue()};
	}

	/**
	 * Encode the frame to send setState to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeSetState(Robot.SelectionState state){
		return new byte[]{0x0, Protocol.SIZE_SET_STATE, 0x0,
				Protocol.ID_SET_STATE, (byte) ((state == Robot.SelectionState.SELECTED)?0x02:0x03)};
	}

	/**
	 * Encode the frame to send askMode to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeAskMode(){
		return new byte[]{0x0, Protocol.SIZE_ASK_MODE, 0x0, Protocol.ID_ASK_MODE};
	}

	/**
	 * Encode the frame to send setMode to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeSetMode(int cameraMode, int radarMode, int buzzerMode, int ledMode){
		return new byte[]{0x0, Protocol.SIZE_SET_MODE, 0x0, Protocol.ID_SET_MODE,
				(byte) cameraMode, (byte) radarMode, (byte) buzzerMode, (byte) ledMode};
	}

	/**
	 * Encode the frame to send askLogs to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeAskLogs(){
		return new byte[]{0x0, Protocol.SIZE_ASK_LOGS, 0x0, Protocol.ID_ASK_LOGS};
	}

	/**
	 * Encode the frame to send askLogs to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeAlert(int alert){
		return new byte[]{0x0, Protocol.SIZE_ALERT, 0x0, Protocol.ID_ALERT, (byte) alert};
	}

	/**
	 * Encode the frame to send askToDisconnect to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeAskToDisconnect(){
		return new byte[]{0x0, Protocol.SIZE_ASK_TO_DISCONNECT, 0x0, Protocol.ID_ASK_TO_DISCONNECT};
	}

	/**
	 * Encode the frame to send askLogs to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeSetCurrentTime(int year, int month, int day, int hour, int minutes, int seconds){
		// Divide the year in 2
		String strYear = String.valueOf(year);
		byte firstHalfYear = Byte.parseByte(strYear.substring(0,2));
		byte secondHalfYear = Byte.parseByte(strYear.substring(2));

		return new byte[]{0x0, Protocol.SIZE_SET_CURRENT_TIME, 0x0, Protocol.ID_SET_CURRENT_TIME,
				firstHalfYear, secondHalfYear, (byte) month, (byte) day,
				(byte) hour, (byte) minutes, (byte) seconds};
	}

	/**
	 * @brief Decompose rtc to year, month, day, hour, minutes and seconds in a array list of integer.
	 */
	public static ArrayList<Integer> decomposeRtc(String rtc) {
		ArrayList<Integer> decompositionRtc = new ArrayList<>();
		decompositionRtc.add(2023);
		decompositionRtc.add(5);
		decompositionRtc.add(30);
		return decompositionRtc;
	}

	/**
	 * Encode the frame to send askToDisconnect to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeSetupGUIInfo(String ipAddress, int port){
		// Ip address encoding
		String[] numbers = ipAddress.split("\\.");
		// Port encoding
		String strHex = decimalToStrHex(port);
		int firstPartPort = Integer.parseInt(strHex.substring(0, 2), 16);
		int secondPartPort = Integer.parseInt(strHex.substring(2, 4), 16);

		return new byte[]{0x0, Protocol.SIZE_SET_IP_PORT, 0x0, Protocol.ID_SET_IP_PORT,
				// Ip address
				(byte) Integer.parseInt(numbers[0]), (byte) Integer.parseInt(numbers[1]),
				(byte) Integer.parseInt(numbers[2]), (byte) Integer.parseInt(numbers[3]),
				// Port
				(byte) firstPartPort, (byte) secondPartPort};
	}

	/**
	 * Encode the frame to send askToDisconnect to the server
	 * @return frame to send to the server
	 */
	public static byte[] encodeLogsReceived(){
		return new byte[]{(byte)0x00, Protocol.SIZE_LOGS_RECEIVED,
				(byte)0x00, Protocol.ID_LOGS_RECEIVED};
	}

	/**
	 * @brief Decode message depending on the protocol shared by the robot. From the frames
	 * received, this function change it into an event ready to be dispatched.
	 * @param frame Frame we received ready to be analyzed and change into event.
	 * @return event we want to dispatch depending on the frame received.
	 */
	public static Event decode(int idRobot, byte[] frame)
	{
		Event event = new Event(Event.EventID.NOTHING);
		switch (frame[0]) {
			case ID_SET_AVAILABILITY :
				event.setEventID(Event.EventID.SET_AVAILABILITY);
				event.setIdRobot(idRobot);
				break;
			case ID_SET_MODE :
				event.setEventID(Event.EventID.SET_MODE);
				// Affecting the operating modes of the event to send it to GUISecretary
				OperatingMode operatingMode = new OperatingMode();
				operatingMode.setOperatingMode(
						frame[2] == 0 ? PeriphState.DISABLED : PeriphState.ENABLED,
						frame[3] == 0 ? PeriphState.DISABLED : PeriphState.ENABLED,
						frame[4] == 0 ? PeriphState.DISABLED : PeriphState.ENABLED,
						frame[5] == 0 ? PeriphState.DISABLED : PeriphState.ENABLED
				);
				event.setIdRobot(idRobot);
				event.setOperatingMode(operatingMode);
				break;
			case ID_SET_LOGS :
				event.setEventID(Event.EventID.SET_LOGS);
				ArrayList<String> logsList = new ArrayList<>();
				event.setLogs(logsList);
				event.setIdRobot(idRobot);
				break;
			case ID_ALERT :
				event.setEventID(Event.EventID.ALERT);
				event.setIdRobot(idRobot);
				int idAlert = frame[2];
				if (idAlert < 0 ) idAlert = 0;
				else if (idAlert > 99) idAlert = 99;
				event.setIdAlert(idAlert);
				break;
			case ID_ACK_DISCONNECTION :
				// The message from the protocol shared with the server is ack disconnection but
				// the event we want to received for the concerned objects is disconnected ok.
				event.setEventID(Event.EventID.DISCONNECTED_OK);
				event.setIdRobot(idRobot);
				break;
			case ID_SET_RADAR :
				event.setEventID(Event.EventID.SET_RADAR);
				event.setIdRobot(idRobot);
				event.setRadarState((frame[2]==1)? Event.RadarState.OBJECT_DETECTED: Event.RadarState.OBJECT_NOT_DETECTED);
				break;
			default :
				break;
		}
		return event;
	}

	/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

	/**
	 * @brief Convert a decimal number to hexadecimal number.
	 * Source : <a href="https://stackoverflow.com/questions/62387078/convert-decimal-base-10-to-hexadecimal-base-16-in-java">...</a>.
	 * Reformat by Mahery FONG.
	 * @param decimal Decimal number we want to convert.
	 * @return String of hexadecimal number converted.
	 */
	private static String decimalToStrHex(int decimal) {
		String digits = "0123456789ABCDEF";
		if (decimal <= 0) return "0";
		// flexible to change in any base under 16
		int base = 16;
		String hex = "";
		while (decimal > 0) {

			// rightmost digit
			int digit = decimal % base;

			// string concatenation
			hex = digits.charAt(digit) + hex;
			decimal = decimal / base;
		}
		return hex;
	}
}

