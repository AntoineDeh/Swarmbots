/**
 * \file  GUI.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief GUI object is a singleton that interfaces the input of the user and transfer them to the
 * other objects of the application.
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

import android.annotation.SuppressLint;
import android.content.Context;

import java.util.ArrayList;
import java.util.concurrent.ArrayBlockingQueue;

import project.src.model.Command;
import project.src.model.LogLevel;
import project.src.model.PeriphState;
import project.src.model.com.ProxyPilot;

public class GUI extends Thread{

    /* ----------------------  PRIVATE CONSTANT  ------------------------------ */

    /**
     * @brief Number of event accepted by the GUISecretary queue.
     */
    private static final int MAX_CAPACITY_QUEUE = 20;

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief Proxy of Pilot in the robots.
     */
    private ProxyPilot proxyPilot;

    /**
     * @brief Context of the class.
     */
    private static Context context;

    /**
     * @brief All state that GUI can be at.
     */
    private enum Screen {
        HOME,
        COMMAND,
        LOGS,
        ANDROID
    }

    /**
     * @brief All state that GUI can be at all screen.
     * @note The 2 states in ROBOT_SELECTED state don't exist because acknowledge is already manage
     * by the TCP communication. If this technology changes, it should be added in the communication
     * protocol and possible state in ROBOT_SELECTED state.
     */
    private enum State {
        /**
         * @brief State in home screen.
         */
        INIT,
        /**
         * @brief State in home screen.
         */
        NO_ROBOT_AVAILABLE,
        /**
         * @brief State in home screen.
         */
        CONNECTED_ROBOTS,
        /**
         * @brief State in home screen.
         */
        WAITING_MODE,
        /**
         * @brief State in home screen.
         */
        RECONNECTING,

        /**
         * @brief State in command screen.
         */
        NO_ROBOT_SELECTED,
        /**
         * @brief State in command screen.
         */
        ROBOT_SELECTED,

        /**
         * @brief State in logs screen.
         */
        NO_LOGS_SELECTED,
        /**
         * @brief State in command screen.
         */
        WAITING_ROBOTS_LOGS,
        /**
         * @brief State in command screen.
         */
        LOGS_SELECTED,
        /**
         * @brief State in command screen.
         */
        WAITING_GUI_LOGS,

        /**
         * @brief State waiting for the acknowledge of GUISecretary disconnecting all robots.
         */
        DISCONNECTING,
        /**
         * @brief State for the end of the application.
         */
        STOP
    }

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief GUISecretary queue to send messages.
     */
    private final ArrayBlockingQueue<Event> guiQueue;

    /**
     * @brief Actual state of GUI
     */
    private State state = State.INIT;

    /**
     * @brief Current screen of GUI
     */
    private Screen screen = Screen.HOME;

    /* ----------------------  SINGLETON INSTANCE AND CONSTRUCTOR(S)  -------------------------------- */

    @SuppressLint("StaticFieldLeak")
    public static GUI INSTANCE;

    // Initialize the singleton instance with the constructor.
    static {
        INSTANCE = new GUI();
    }

    /**
     * @brief Constructor of GUI object.
     */
    public GUI() {
        this.guiQueue = new ArrayBlockingQueue<>(MAX_CAPACITY_QUEUE);
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Empty method for the creation of the singleton object.
     */
    public void create() {
        this.proxyPilot = new ProxyPilot();
    }

    /**
     * @brief Main function of GUI thread. It receives the events from the other objects.
     */
    @Override
    public void run() {

        // Called by the start() function of GUI
        Event event = null;
        LogsManager.INSTANCE.log(LogLevel.INFO, "Start application");
        GUISecretary.INSTANCE.askInit();

        while(this.state != State.STOP) {

            // Catch the event in the queue.
            try {
                // Catch the oldest message event in the queue.
                event = this.guiQueue.take();
            } catch (InterruptedException ignored) {
                continue;
            }

            if (event.getEventID() == Event.EventID.QUIT) {
                LogsManager.INSTANCE.log(LogLevel.INFO, "Closing the application.");
                GUISecretary.INSTANCE.disconnectRobots();
                this.screen = Screen.ANDROID;
                this.state = State.DISCONNECTING;
                // Loop into the while loop without reaching the code below.
                continue;
            }

            else if (event.getEventID() == Event.EventID.ALERT) {
                if (event.getIdAlert() == 0) {
                    // Pop up memory alert
                }
                continue;
            }

            switch (this.screen) {

                case HOME:

                    switch (this.state) {
                        case INIT:
                            if (event.getEventID() == Event.EventID.INIT_READY) {
                                ArrayList<Robot> robotList = GUISecretary.INSTANCE.getRobotList();
                                this.proxyPilot.setPostManList(GUISecretary.INSTANCE.getPostmanList());
                                LogsManager.INSTANCE.initFileList(robotList);

                                if (robotList == null) {
                                    this.state = State.NO_ROBOT_AVAILABLE;
                                } else {
                                    this.state = State.CONNECTED_ROBOTS;
                                }
                            }
                            break;

                        case NO_ROBOT_AVAILABLE:
                            /*
                            Empty state because no robot is available so we quit the application.
                             */
                            break;

                        case CONNECTED_ROBOTS:
                            if (event.getEventID() == Event.EventID.COMMAND_ROBOT) {
                                LogsManager.INSTANCE.log(LogLevel.INFO, "Command robots");
                                GUISecretary.INSTANCE.setModeInit();
                                this.state = State.WAITING_MODE;
                            }
                            break;

                        case WAITING_MODE:
                            if (event.getEventID() == Event.EventID.MODE_READY) {
                                // TODO : Add getRobotList if necessary (in specifications)
                                // TODO : Prevent activity to switch to the command screen.
                                this.screen = Screen.COMMAND;
                                this.state = State.NO_ROBOT_SELECTED;
                            }
                            break;

                        default:
                            break;
                    }
                    break;

                case COMMAND:

                    if (event.getEventID() == Event.EventID.CONSULT_LOGS) {
                        LogsManager.INSTANCE.log(LogLevel.INFO, "Consult logs");
                        GUISecretary.INSTANCE.unselectRobot();

                        this.screen = Screen.LOGS;
                        this.state = State.NO_LOGS_SELECTED;
                        continue;
                    }
                    else if (event.getEventID() == Event.EventID.RETURN_HOME) {
                        LogsManager.INSTANCE.log(LogLevel.INFO, "Return home");
                        GUISecretary.INSTANCE.unselectRobot();

                        this.screen = Screen.HOME;
                        // TODO : Replace CONNECTED_ROBOTS state by RECONNECTING and call
                        // TODO : GUISecretary to reconnect the unavailable robots.
                        this.state = State.CONNECTED_ROBOTS;
                        continue;
                    }

                    switch (this.state) {

                        case NO_ROBOT_SELECTED:
                            if (event.getEventID() == Event.EventID.SELECT_ROBOT) {
                                LogsManager.INSTANCE.log(LogLevel.INFO, "Select robot");
                                GUISecretary.INSTANCE.setSelectedRobot(event.getIdRobot(), Robot.SelectionState.SELECTED);
                                this.autorizeCommand();
                                LogsManager.INSTANCE.log(LogLevel.INFO, "Authorize command");
                                this.state = State.ROBOT_SELECTED;
                            }
                            break;

                        case ROBOT_SELECTED:
                            if (event.getEventID() == Event.EventID.MOVE_ROBOT) {
                                LogsManager.INSTANCE.log(LogLevel.INFO, "Move robot direction "+event.getCommand().toString().toLowerCase());
                                this.proxyPilot.askCmd(GUISecretary.INSTANCE.getSelectedRobot().getId(), event.getCommand());
                            }
                            else if (event.getEventID() == Event.EventID.SELECT_ROBOT) {
                                LogsManager.INSTANCE.log(LogLevel.INFO, "Select robot");
                                GUISecretary.INSTANCE.setSelectedRobot(event.getIdRobot(), Robot.SelectionState.SELECTED);
                            }

                            else if (event.getEventID() == Event.EventID.EDIT_ROBOT_MODE) {
                                LogsManager.INSTANCE.log(LogLevel.INFO,
                                        "Operating mode of "+event.getPeriphToChange()+" is "+event.getPeriphState());
                                GUISecretary.INSTANCE.askSetMode(event.getPeriphToChange(), event.getPeriphState());
                            }
                            break;

                        default:
                            break;
                    }
                    break;

                case LOGS:

                    if (event.getEventID() == Event.EventID.SELECT_LOGS) {
                        LogsManager.INSTANCE.log(LogLevel.INFO, "Select logs of " +
                                ((event.getIsGUISelectedLogs()) ? "GUI":"robot"+event.getIdRobot()));

                        if (event.getIsGUISelectedLogs()) {
                            // GUI's logs selected.
                            LogsManager.INSTANCE.setSelectedLogs("GUILogsFile");
                            this.state = State.WAITING_GUI_LOGS;
                        } else {
                            // One of robots' logs selected.
                            LogsManager.INSTANCE.setSelectedLogs("Robot"+event.getIdRobot()+"LogsFile");
                            this.state = State.WAITING_ROBOTS_LOGS;
                        }
                        continue;
                    }

                    else if (event.getEventID() == Event.EventID.COMMAND_ROBOT) {
                        LogsManager.INSTANCE.log(LogLevel.INFO, "Command robots");
                        this.screen = Screen.COMMAND;
                        this.state = State.NO_ROBOT_SELECTED;
                        continue;
                    }

                    switch (this.state) {

                        case NO_LOGS_SELECTED:
                        case LOGS_SELECTED:
                        case WAITING_GUI_LOGS:
                            /*
                            Empty because the possible actions are caught earlier. Those states
                            define in the design can be all merged to one state.
                             */
                            break;

                        case WAITING_ROBOTS_LOGS:
                            /*
                            Empty because this functionality isn't yet implemented.
                             */
                            break;

                        default:
                            break;
                    }
                    break;

                case ANDROID:
                    if (event.getEventID() == Event.EventID.STOP) {
                        LogsManager.INSTANCE.log(LogLevel.INFO, "Stop application");
                        this.terminate();
                    }
                    break;

                default:
                    break;
            }
        }
    }

    /**
     * @brief Call from views to command the direction of the selected robot.
     */
    public void commandRobot(){
        //Change activity
        this.guiQueue.add(new Event(Event.EventID.COMMAND_ROBOT));
    }

    /**
     * @brief Call from views to select the robot to command its peripheral and direction.
     * @param idRobot Id of the robot to command.
     * @param robotState State of the robot to set.
     */
    public void selectRobot(int idRobot, Robot.SelectionState robotState){
        Event event = new Event(Event.EventID.SELECT_ROBOT);
        event.setIdRobot(idRobot);
        event.setSelectionState(robotState);
        this.guiQueue.add(event);
    }

    /**
     * Move robot to the cmd direction.
     * @param cmd direction to go.
     */
    public void moveRobot(Command cmd) {
        //Send request to robot
        Event event = new Event(Event.EventID.MOVE_ROBOT);
        event.setCommand(cmd);
        this.guiQueue.add(event);
    }

    /**
     * @brief Call from views to change the operating mode of the selected robot.
     * @param periphToChange Peripheral that operating mode change.
     * @param periphState New state of the peripheral.
     */
    public void editRobotMode(OperatingMode.PeriphToChange periphToChange, PeriphState periphState) {
        // Change the operating mode of the selected robot.
        Event event = new Event(Event.EventID.EDIT_ROBOT_MODE);
        event.setPeriphToChangeAndState(periphToChange, periphState);
        this.guiQueue.add(event);
    }

    /**
     * @brief Notify GUI that the operating mode of the robots are ready.
     */
    public void modeReady(){
        this.guiQueue.add(new Event(Event.EventID.MODE_READY));
    }

    /**
     * @brief Signal GUI to have finished the disconnection of all robots.
     */
    public void stopDisconnection() {
        this.guiQueue.add(new Event(Event.EventID.STOP));
    }

    /**
     * @brief Call from views to change the screen to home screen.
     */
    public void returnHome(){
        this.guiQueue.add(new Event(Event.EventID.RETURN_HOME));
    }

    /**
     * @brief Call from views to change the screen to logs screen.
     */
    public void consultLogs(){
        this.guiQueue.add(new Event(Event.EventID.CONSULT_LOGS));
    }

    /**
     * @brief Select the logs asked to be display on the screen. This selection is made by the
     * id file corresponding to the id robot and equals to 0 for the GUI logs. Sets the boolean
     * function setGUISelectedLogs at true if GUI logs is asked.
     * @param idFile id file asked. Equals to 0 for the GUI logs.
     */
    public void selectLogs(int idFile){
        Event event = new Event(Event.EventID.SELECT_LOGS);
        event.setGUISelectedLogs(idFile == 0);
        this.guiQueue.add(event);
    }

    /**
     * @brief Call from viewss to export the selected logs.
     */
    public void exportLogs(){
        this.guiQueue.add(new Event(Event.EventID.EXPORT_LOGS));
    }

    /**
     * @brief Notify an error and ask to show pop up.
     */
    public void notifyError(){

    }

    /**
     * @brief Validation popup from views to close the popup.
     */
    public void validate(){
        this.guiQueue.add(new Event(Event.EventID.VALIDATE));
    }

    /**
     * @brief Call to show the popup of the disconnection alert of the robots. Used when the pings
     * reach the limit accepted.
     * @param idRobot Id robot of the robot disconnected.
     */
    public void raiseDisconnectionAlert(int idRobot) {
        Event event = new Event(Event.EventID.ALERT);
        event.setIdRobot(idRobot);
        event.setIdRobot(1);
        this.guiQueue.add(event);
    }

    /**
     * @brief Call to show the popup of memory alert of the robots.
     * @param idRobot Id robot that reach the limit of memory.
     */
    public void raiseMemoryAlert(int idRobot, int idAlert){
        Event event = new Event(Event.EventID.ALERT);
        event.setIdRobot(idRobot);
        event.setIdAlert(idAlert);
        this.guiQueue.add(event);
    }

    /**
     * @brief Validation popup from views to close the popup flush.
     */
    public void flush(){
        this.guiQueue.add(new Event(Event.EventID.VALIDATE));
    }

    /**
     * @brief Notify GUI object that the initialization is finished.
     */
    public void initReady(){
        this.guiQueue.add(new Event(Event.EventID.INIT_READY));
    }

    /**
     * @brief Notify GUI object that the logs are ready.
     */
    public void logsReady(){
        this.guiQueue.add(new Event(Event.EventID.LOGS_READY));
    }

    /**
     * @brief Call from views to quit application.
     */
    public void quit(){
        this.guiQueue.add(new Event(Event.EventID.QUIT));
    }

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

    /**
     * @brief Display the command on the screen.
     */
    private void autorizeCommand() {
    }

    /**
     * @brief Terminate thread object and application.
     */
    private void terminate() {
        LogsManager.INSTANCE.quit();
        this.state = State.STOP;
    }
}


