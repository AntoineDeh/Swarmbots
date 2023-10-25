/**
 * \file  Event.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Event that represent the messages between active objects. It contains information related
 * to their state machine.
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

import java.util.ArrayList;

import project.src.model.Command;
import project.src.model.LogLevel;
import project.src.model.PeriphState;

public class Event {

    /* ----------------------  PUBLIC VARIABLES  ------------------------------ */

    /**
     * @brief Id of the event.
     */
    public enum EventID {
        /**
         * GUISecretary event.
         */
        NOTHING,
        /**
         * GUISecretary event.
         */
        IDLE,
        /**
         * GUISecretary event.
         */
        ASK_INIT,
        /**
         * GUISecretary event.
         */
        ACK_CONNECTION,
        /**
         * GUISecretary event.
         */
        CONNECTION_FAILED,
        /**
         * GUISecretary event.
         */
        NOTIFY_VAR_READY,
        /**
         * GUISecretary event.
         */
        NOTIFY_DISCONNECTED_ROBOT,
        /**
         * GUISecretary event.
         */
        SET_MODE_INIT,
        /**
         * GUISecretary event.
         */
        SET_MODE,
        /**
         * GUISecretary event.
         */
        DISCONNECT_ROBOTS,
        /**
         * GUISecretary event.
         */
        DISCONNECTED_OK,
        /**
         * GUISecretary event.
         */
        UNSELECT_ROBOT,

        /**
         * GUI and GUISecretary event.
         */
        SELECT_ROBOT,

        /**
         * GUI event.
         */
        INIT_READY,
        /**
         * GUI event.
         */
        COMMAND_ROBOT,
        /**
         * GUI event.
         */
        MODE_READY,
        /**
         * GUI event.
         */
        MOVE_ROBOT,
        /**
         * GUI event.
         */
        CONSULT_LOGS,
        /**
         * GUI event.
         */
        EDIT_ROBOT_MODE,
        /**
         * GUI event.
         */
        SELECT_LOGS,
        /**
         * GUI event.
         */
        LOGS_READY,
        /**
         * GUI event.
         */
        EXPORT_LOGS,
        /**
         * GUI event.
         */
        RETURN_HOME,
        /**
         * GUI event.
         */
        SET_RADAR,
        /**
         * GUI event. Has the role of the validate and flush action in the specifications.
         */
        VALIDATE,
        /**
         * Event shared by all singleton objects to stop the application.
         */
        QUIT,
        /**
         * GUI event.
         */
        STOP,

        /**
         * Viewer event
         */
        SETUP_ROBOT_PORT,

        /**
         * LogsManager event.
         */
        SET_LOGS,

        /**
         * GUIRinger event.
         */
        CHECK_FAILED_PONG_VAR,
        /**
         * GUIRinger event.
         */
        INIT_OK,
        /**
         * GUIRinger event.
         */
        SET_AVAILABILITY,
        /**
         * GUIRinger event related to the timer.
         */
        SEND_PING,
        /**
         * GUIRinger event related to the timer.
         */
        TIME_OUT_PING,

        /**
         * LogsManager event.
         */
        CHECK_FLUSHED_SAVED_LOGS_FILE,
        /**
         * LogsManager event.
         */
        INIT_FILE_LIST,
        /**
         * LogsManager event.
         */
        LOG,
        /**
         * LogsManager event.
         */
        TRANSMIT_RTC,
        /**
         * LogsManager event.
         */
        SET_SELECTED_LOGS,
        /**
         * LogsManager event.
         */
        ASK_LOGS_EXPORT,

        /**
         * SB event.
         */
        ALERT,
        /**
         * Starter event.
         */
        SCREEN_STOPPED,
        /**
         * Starter event.
         */
        SCREEN_START
    }

    /**
     * @brief Possible state of the radar related to the obstacle detection.
     */
    public enum RadarState {
        OBJECT_NOT_DETECTED,
        OBJECT_DETECTED
    }

    /**
     * @brief Activity screen that takes one of the 3 possible screens.
     */
    public enum ScreenActivity {
        HOME_SCREEN,
        COMMAND_SCREEN,
        LOGS_SCREEN,
    }

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief Current id event given in the event.
     */
    private EventID eventID;

    /**
     * @brief Current state radar given in the event.
     */
    private RadarState radarState;

    /**
     * @brief Current screen activity given in the event.
     */
    private ScreenActivity screenActivity;

    /**
     * @brief Current given command given in the event.
     */
    private Command command;

    /**
     * @brief Current robot given in the event.
     */
    private Robot robot = null;

    /**
     * @brief Current operating mode given in the event.
     */
    private OperatingMode operatingMode;

    /**
     * @brief Peripheral that change and given in the event. It is used to inform the operating mode
     * of the given peripheral that change at a specific event.
     */
    private OperatingMode.PeriphToChange periphToChange;

    /**
     * @brief State peripheral that change and given in the event.
     */
    private PeriphState periphState;

    /**
     * @brief Selection state given in the event.
     */
    private Robot.SelectionState selectionState;

    /**
     * @brief Log level given in the event.
     */
    private LogLevel logLevel;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief Boolean value that indicates the event source given in the event. True for the
     * dispatcher source, and false for else object.
     */
    private boolean fromDispatcher;

    /**
     * @brief Id of the robot given in the event.
     */
    private int idRobot = 0;

    /**
     * @brief Id of the alert given in the event.
     */
    private int idAlert = 0;

    /**
     * @brief Boolean value that indicates that gui logs is selected. True if gui logs is selected
     * and false if not.
     */
    private boolean isGUILogsSelected;

    /**
     * @brief Id of the file given in the event.
     */
    private String idFile;

    /**
     * @brief String to indicate the selected logs.
     */
    private String selectedLogs;

    /**
     * @brief String message given in the event. Used to log an event in LogsManager.
     */
    private String msg;

    /**
     * @brief List of logs given in the event.
     */
    private ArrayList<String> logs = null;

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Constructor of the object.
     * @param eventID Id event given at each event initialization.
     */
    public Event(EventID eventID){
        this.eventID = eventID;
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Set the boolean condition of the dispatcher source.
     * @param fromDispatcher Boolean value to know if the event source is dispatcher object.
     */
    public void setFromDispatcher(boolean fromDispatcher) {
        this.fromDispatcher = fromDispatcher;
    }

    /**
     * @brief Get the from dispatcher boolean condition.
     * @return Boolean value to inform if the event source is dispatcher object.
     */
    public boolean getFromDispatcher() {
        return this.fromDispatcher;
    }

    /**
     * @brief Set the log level to the event.
     * @param logLevel Log level to set.
     */
    public void setLogLevel(LogLevel logLevel) {
        this.logLevel = logLevel;
    }

    /**
     * @brief Get the log level of the event.
     * @return Log level of the event.
     */
    public LogLevel getLogLevel() {
        return this.logLevel;
    }

    /**
     * @brief Get the message of the event.
     * @return Message of the event.
     */
    public String getMsg() {
        return this.msg;
    }

    /**
     * @brief Set the message to the event.
     * @param msg Message to set.
     */
    public void setMsg(String msg) {
        this.msg = msg;
    }

    /**
     * @brief Set the selected logs to the event.
     * @param selectedLogs Selected logs to set.
     */
    public void setSelectedLogs(String selectedLogs) {
        this.selectedLogs = selectedLogs;
    }

    /**
     * @brief Get the selected logs of the event.
     * @return Selected logs of the event.
     */
    public String getSelectedLogs() {
        return this.selectedLogs;
    }

    /**
     * @brief Set the id file to the event.
     * @param idFile Id file to set.
     */
    public void setIdFile(String idFile) {
        this.idFile = idFile;
    }

    /**
     * @brief Get the id file of the event.
     * @return Id file of the event.
     */
    public String getIdFile() {
        return this.idFile;
    }

    /**
     * @brief Set the screen activity to the event.
     * @param screenActivity Screen activity to set.
     */
    public void setScreenActivity(ScreenActivity screenActivity) {
        this.screenActivity = screenActivity;
    }

    /**
     * @brief Get the screen activity of the event.
     * @return Screen activity of the event.
     */
    public ScreenActivity getScreenActivity() {
        return this.screenActivity;
    }

    /**
     * @brief Get the boolean condition if gui logs is selected in the event.
     * @return Boolean condition to verify is gui logs is selected in the event.
     */
    public boolean getIsGUISelectedLogs() {
        return isGUILogsSelected;
    }

    /**
     * @brief Set the gui selected logs condition to the event.
     * @param isGUILogsSelected Boolean condition to set if gui logs is selected to the event.
     */
    public void setGUISelectedLogs(boolean isGUILogsSelected) {
        this.isGUILogsSelected = isGUILogsSelected;
    }

    /**
     * @brief Set the command to the event.
     * @param command Command set to the event.
     */
    public void setCommand(Command command) {
        this.command = command;
    }

    /**
     * @brief Get the command of the event.
     * @return Command of the event.
     */
    public Command getCommand() {
        return this.command;
    }

    /**
     * @brief Set the selection state to the event.
     * @param selectionState Selection state to the event.
     */
    public void setSelectionState(Robot.SelectionState selectionState) {
        this.selectionState = selectionState;
    }

    /**
     * @brief Get the selectionstate of the event.
     * @return Selection state of the event.
     */
    public Robot.SelectionState getSelectionState() {
        return this.selectionState;
    }

    /**
     * @brief Set the peripheral to change  to the event.
     * @param periphToChange Peripheral that change to the event.
     * @param periphState New state peripheral that change to the peripheral of the event.
     */
    public void setPeriphToChangeAndState(OperatingMode.PeriphToChange periphToChange, PeriphState periphState) {
        this.periphToChange = periphToChange;
        this.periphState = periphState;
    }

    /**
     * @brief Get the peripheral to change of the event.
     * @return Peripheral to change of the event.
     */
    public OperatingMode.PeriphToChange getPeriphToChange() {
        return this.periphToChange;
    }

    /**
     * @brief Get the peripheral state of the event.
     * @return Peripheral state of the event.
     */
    public PeriphState getPeriphState() {
        return this.periphState;
    }

    /**
     * @brief Set the id event to the event.
     * @param eventID Id event of the event.
     */
    public void setEventID(EventID eventID)
    {
        this.eventID = eventID;
    }

    /**
     * @brief Get the id event of the event.
     * @return Id event of the event.
     */
    public EventID getEventID()
    {
        return this.eventID;
    }

    /**
     * @brief Set the id robot to the event.
     * @param idRobot Id robot of the event.
     */
    public void setIdRobot(int idRobot)
    {
        this.idRobot = idRobot;
    }

    /**
     * @brief Get the id robot of the event.
     * @return Id robot of the event.
     */
    public int getIdRobot()
    {
        return idRobot;
    }

    /**
     * @brief Set the operating mode to the event.
     * @param operatingMode Operating mode of the event.
     */
    public void setOperatingMode(OperatingMode operatingMode)
    {
        this.operatingMode = operatingMode;
    }

    /**
     * @brief Get the operating mode of the event.
     * @return Operating mode of the event.
     */
    public OperatingMode getOperatingMode()
    {
        return this.operatingMode;
    }

    /**
     * @brief Set the id alert to the event.
     * @param idAlert Id alert of the event.
     */
    public void setIdAlert(int idAlert)
    {
        this.idAlert = idAlert;
    }

    /**
     * @brief Get the id alert of the event.
     * @return Id alert of the event.
     */
    public int getIdAlert() {
        return this.idAlert;
    }

    /**
     * @brief Set the list of logs to the event.
     * @param logs List of logs of the event.
     */
    public void setLogs(ArrayList<String> logs)
    {
        this.logs = logs;
    }

    /**
     * @brief Get the logs of the event.
     * @return Logs of the event.
     */
    public ArrayList<String> getLogs() {
        return this.logs;
    }

    /**
     * @brief Set the robot to the event.
     * @param robot Robot of the event.
     */
    public void setRobot(Robot robot)
    {
        this.robot = robot;
    }

    /**
     * @brief Get the robot of the event.
     * @return Robot of the event.
     */
    public Robot getRobot()
    {
        return this.robot;
    }

    /**
     * @brief Set the state radar to the event.
     * @param radarState State radar of the event.
     */
    public void setRadarState(RadarState radarState) {
        this.radarState = radarState;
    }

    /**
     * @brief Get the state of the radar of the event.
     * @return State of the radar of the event.
     */
    public RadarState getRadarState() {
        return this.radarState;
    }

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
}
