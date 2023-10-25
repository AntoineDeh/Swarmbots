/**
 * \file  Robot.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Robot object that contains all properties of the robots, like their connection state,
 * selection state, obstacle detection and operating mode.
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

public class Robot {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief Operating mode of the robot concerning its peripherals.
     */
    private OperatingMode operatingMode;

    /**
     * @brief Connection state of the robot.
     */
    private ConnectionState connectionState = ConnectionState.DISCONNECTED;

    /**
     * @brief Selected state of the robot.
     */
    private SelectionState selectionState = SelectionState.UNSELECTED;

    /**
     * @brief Detection of the obstacle.
     */
    private ObstacleDetection obstacleDetection = ObstacleDetection.NOT_DETECTED;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */
    /**
     * @brief Id number of the robot to identify it. Its number depends on the order in the config
     * file.
     */
    private final int id;

    /**
     * @brief Ip address of the robot to connect to it, given in the config file.
     */
    private final String ipAddress;

    /**
     * @brief Mac address of the robot, given in the config file.
     */
    private final String macAddress;

    /**
     * @brief Port of the connexion of the robot, given in the config file.
     */
    private final int port;

    /**
     * @brief Failed pongs variable to count the number of pings the robots has missed.
     */
    private int failedPongsVar = 0;

    /* ----------------------  PUBLIC OBJECTS  ------------------------------ */

    /**
     * @brief Detection state of an obstacle.
     * @value DETECTED : when the obstacle is detected.
     * @value NOT_DETECTED : when no obstacle is detected.
     */
    public enum ObstacleDetection {
        DETECTED,
        NOT_DETECTED
    }

    /**
     * @brief Connection state with the robot.
     * @value CONNECTED :
     */
    public enum ConnectionState {
        CONNECTED,
        DISCONNECTED
    }

    /**
     * @brief Selection state with the robot.
     */
    public enum SelectionState {
        SELECTED,
        UNSELECTED
    }

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Public constructor.
     * @param idRobot to identify it.
     * @param ipAddress used to be connected to the robot, given in the config file.
     * @param macAddress given in the config file.
     */
    public Robot(int idRobot, String ipAddress, String macAddress, int port){
        this.id = idRobot;
        this.ipAddress = ipAddress;
        this.macAddress = macAddress;
        this.port = port;
        this.operatingMode = new OperatingMode();
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Give the failed pongs variable of the robot.
     */
    public void setFailedPongsVar(int value) {
        this.failedPongsVar = value;
    }

    /**
     * @brief Give the failed pongs variable of the robot.
     * @return failed pongs variable integer of the robot.
     */
    public int getFailedPongsVar() {
        return this.failedPongsVar;
    }

    /**
     * @brief Give the operating mode of the robot.
     * @return operatingMode of the robot.
     */
    public OperatingMode getOperatingMode(){
        return this.operatingMode;
    }

    /**
     * @brief Parametize the operating mode of the robot.
     * @param newOpMode used to replace the actual one.
     */
    public void setOperatingMode(OperatingMode newOpMode){
        this.operatingMode = newOpMode;
    }

    /**
     * @brief Give the id of the robot.
     * @return id of the robot.
     */
    public int getId(){return this.id;}

    /**
     * @brief Give the ip address of the robot.
     * @return ipAdress of the robot.
     */
    public String getIpAddress(){
        return this.ipAddress;
    }

    /**
     * @brief Give the connection state of the robot.
     * @return connectionState of the robot.
     */
    public ConnectionState getConnectionState()
    {
        return this.connectionState;
    }

    /**
     * @brief Change the connection state of the robot to connected.
     */
    public void setConnectedState()
    {
        this.connectionState = ConnectionState.CONNECTED;
    }

    /**
     * @brief Change the connection state of the robot to disconnected.
     */
    public void setDisconnectedState()
    {
        this.connectionState = ConnectionState.DISCONNECTED;
    }

    /**
     * @brief Give the selection state of the robot.
     * @return selectedState of the robot.
     */
    public SelectionState getSelectionState(){
        return this.selectionState;
    }

    /**
     * @brief Change the selection state of the robot to selected.
     */
    public void setSelectedState(){
        this.selectionState = SelectionState.SELECTED;
    }

    /**
     * @brief Change the selection state of the robot to unselected.
     */
    public void setUnselectedState(){
        this.selectionState = SelectionState.UNSELECTED;
    }

    /**
     * @brief Give the detection state radar of the robot
     * @return obstacleDetection
     */
    public ObstacleDetection getObstacleDetection(){
        return this.obstacleDetection;
    }

    /**
     * @brief Change the detection state of the robot to detected
     */
    public void setObstacleDetectionTrue(){
        this.obstacleDetection = ObstacleDetection.DETECTED;
    }

    /**
     * @brief Change the detection state of the robot to not detected
     */
    public void setObstacleDetectionFalse(){
        this.obstacleDetection = ObstacleDetection.NOT_DETECTED;
    }

    /**
     * @brief Give the port of the robot.
     * @return port of the robot.
     */
    public int getPort(){
        return this.port;
    }

    /**
     * @brief Give the mac address of the robot.
     * @return mac address of the robot.
     */
    public String getMacAddress(){
        return this.macAddress;
    }
    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
}
