/**
 * \file  GUISecretary.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief GUISecretary object is responsible for the main data model of the application. It contains
 * the robot list, the selected robot, and other important data. It also responsible for the
 * connection of the robots, and change of their operating modes.
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
 */

/* ----------------------  PACKAGE  ---------------------------------------- */
package project.src.model.core;

/* ----------------------  INCLUDES  ---------------------------------------- */

import androidx.annotation.Nullable;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.ArrayBlockingQueue;

import project.src.model.LogLevel;
import project.src.model.PeriphState;
import project.src.model.com.Postman;
import project.src.model.com.ProxyControllerCore;
import project.src.view.viewmodel.CommandScreenViewModel;

public class GUISecretary extends Thread {

    /* ----------------------  PRIVATE CONSTANT  ------------------------------ */

    /**
     * @brief Number of event accepted by the GUISecretary queue.
     */
    private static final int MAX_CAPACITY_QUEUE = 20;

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */


    /**
     * @brief The mutable live data of the robot list.
     */
    private final MutableLiveData<ArrayList<Robot>> robots = new MutableLiveData<>();

    /**
     * @brief The mutable live data of the id robot which is disconnected
     */
    //private final MutableLiveData<Integer> idRobotConnection = new MutableLiveData<Integer>();

    /**
     * @brief GUISecretary queue to send messages.
     */
    private final ArrayBlockingQueue<Event> guiSecretaryQueue;

    /**
     * @brief List of Postman and responsible to send frames to the server.
     */
    private final ArrayList<Postman> postmanList = new ArrayList<Postman>(){};

    /**
     * @brief All state that GUISecretary can be at.
     */
    private enum State{
        IDLE,
        WAITING_FOR_CONNECTION,
        /**
         * The state below isn't right to the design because we wanted to wait the var ready and not
         * the flushed logs file. So it corresponds to the WAITING_FOR_FLUSHED_FILE in the design.
         */
        WAITING_FOR_VAR_READY,
        AVAILABLE,
        FINISH_FOR_EMPTY_LIST,
        WAITING_FOR_MODE,
        WAITING_FOR_DISCONNECTION,
        WAITING_POP_UP_VALIDATION,
        STOP
    }

    /* ----------------------  PROTECTED OBJECTS  ------------------------------ */

    /**
     * @brief The robot list of all robot with their configurations.
     */
    protected ArrayList<Robot> robotList = new ArrayList<Robot>(){};

    /**
     * @brief Selected robots by the user. The user can ask movements and change the operating modes of
     * this robot.
     * @note We can think about change this to an index in the robotList.
     */
    protected Robot selectedRobot;

    /**
     * @brief Current robot to take into consideration while manipulating robotList.
     * For instance : the connection to the server.
     */
    protected Robot currentRobot;

    /**
     * @brief Proxy of the controllerCore of the robot.
     */
    protected ProxyControllerCore proxyControllerCore;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief Actual state of GUISecretary.
     */
    protected State state;

    /* ----------------------  SINGLETON INSTANCE AND CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Instance of the singleton object of GUISecretary.
     */
    public static GUISecretary INSTANCE;

    static {
        try {
            // Instanciate the singleton object of GUISecretary.
            INSTANCE = new GUISecretary();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * @brief GUISecretary constructor.
     * @throws IOException les exceptions d'entrée/sortie.
     */
    private GUISecretary() throws IOException {
        this.guiSecretaryQueue = new ArrayBlockingQueue<>(MAX_CAPACITY_QUEUE);
        this.proxyControllerCore = new ProxyControllerCore();
        Viewer.INSTANCE.create();
        GUIRinger.INSTANCE.create();
        this.state = State.IDLE;
        //this.idRobotConnection.postValue(0);
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Empty method for the creation of the singleton object.
     */
    public void create() {

    }

    /**
     * @brief Run method for the GUISecretary thread with its state machine. It will take event
     * from its queue and execute the event associated.
     */
    @Override
    public void run() {
        // Called by the start() function of GUISecretary
        Viewer.INSTANCE.start();
        GUIRinger.INSTANCE.start();
        Event event = null;
        while(this.state != State.STOP) {
            // Catch the event in the queue.
            try {
                // Catch the oldest message event in the queue.
                event = this.guiSecretaryQueue.take();
            } catch (InterruptedException ignored) {
                continue;
            }

            if (event.getEventID() == Event.EventID.DISCONNECT_ROBOTS) {
                // Stop the active singleton objects.
                Viewer.INSTANCE.stopReceivePacket();
                GUIRinger.INSTANCE.stopRing();

                // Disconnect all robots.
                this.pickFirstRobot();
                while (this.currentRobot != null) {
                    if (this.currentRobot.getConnectionState() != Robot.ConnectionState.CONNECTED) {
                        this.pickNextRobot();
                    } else {
                        this.proxyControllerCore.askToDisconnect(this.currentRobot.getId());
                        LogsManager.INSTANCE.log(LogLevel.DEBUG, "Disconnecting robot"+this.currentRobot.getId());
                        this.state = State.WAITING_FOR_DISCONNECTION;
                        break;
                    }
                }
                // If we sent askToDisconnect to the current robot, we skip the code below.
                // Else, no robot was connected.
                if (this.currentRobot == null) {
                    GUI.INSTANCE.stopDisconnection();
                    this.state = State.STOP;
                }
                continue;
            }

            switch (this.state)
            {
                case IDLE:
                    if (event.getEventID() == Event.EventID.ASK_INIT) {
                        this.robotList = ConfigReader.getRobotListFromConfig("FileConfig.json");

                        if (this.robotList == null) {
                            this.state = State.FINISH_FOR_EMPTY_LIST;
                            // Notify GUI to have finished the initialization of the robots.
                            GUI.INSTANCE.initReady();
                        } else {
                            if (this.robotList.size() == 0){
                                this.state = State.FINISH_FOR_EMPTY_LIST;
                                // Notify GUI to have finished the initialization of the robots.
                                GUI.INSTANCE.initReady();
                            } else {
                                // Pick the first robot in the robot list and launch its connection.
                                this.pickFirstRobot();
                                this.initConnection();
                                this.state = State.WAITING_FOR_CONNECTION;
                            }
                        }
                    }
                    break;
                case WAITING_FOR_CONNECTION:
                    if (event.getEventID() == Event.EventID.ACK_CONNECTION) {
                        // The current robot is successfully connected. We wait the var to be ready.
                        this.updateConnectedRobot(event.getIdRobot());
                        LogsManager.INSTANCE.checkFlushedSavedLogsFile(this.currentRobot.getId());
                        GUIRinger.INSTANCE.checkFailedPongVar(this.currentRobot.getId());
                        this.state = State.WAITING_FOR_VAR_READY;
                    }

                    else if (event.getEventID() == Event.EventID.CONNECTION_FAILED) {
                        // The current robot isn't available.
                        // We don't initialize the flushed file and connection var.
                        this.updateDisconnectedRobot(event.getIdRobot());
                        this.pickNextRobot();

                        if (this.currentRobot != null) {
                            // There is still robot to initialize. So launch the connection to the
                            // current robot.
                            this.initConnection();
                        } else {
                            // End of the initialization of robots.
                            this.endInitConnection();
                            this.state = State.AVAILABLE;
                        }
                    }
                    break;
                case WAITING_FOR_VAR_READY:
                    if (event.getEventID() == Event.EventID.NOTIFY_VAR_READY)
                    {
                        LogsManager.INSTANCE.transmitRtc(this.currentRobot.getId());

                        this.pickNextRobot();

                        if (this.currentRobot != null) {
                            // There is still robot to initialize. So launch the connection to the
                            // current robot.
                            this.initConnection();
                            this.state = State.WAITING_FOR_CONNECTION;
                        } else {
                            // End of the initialization of robots.
                            this.endInitConnection();
                            this.state = State.AVAILABLE;
                        }
                    }
                    break;

                case AVAILABLE:

                    // Disconnection of a robot.
                    if (event.getEventID() == Event.EventID.NOTIFY_DISCONNECTED_ROBOT){
                        this.updateDisconnectedRobot(event.getIdRobot());
                    }
                    else if (event.getEventID() == Event.EventID.SET_MODE) {
                        // User click on the screen to change the operating mode of a robot.
                        // Change the operating mode of the selected robot.
                        this.selectedRobot.getOperatingMode().setOneOperatingMode(
                                event.getPeriphToChange(), event.getPeriphState());
                        // Change the operating mode in the robot list.
                        this.changeMode(this.selectedRobot.getId(), this.selectedRobot.getOperatingMode());
                        // User has clicked on the screen to change operating mode and we share it to the robot.
                        this.proxyControllerCore.askSetMode(this.selectedRobot.getId(), this.selectedRobot.getOperatingMode());
                    }

                    else if (event.getEventID() == Event.EventID.SET_MODE_INIT) {
                        this.pickFirstRobot();
                        while (this.currentRobot != null) {
                            if (this.currentRobot.getConnectionState() != Robot.ConnectionState.CONNECTED) {
                                this.pickNextRobot();
                            } else {
                                this.proxyControllerCore.askMode(this.currentRobot.getId());
                                this.state = State.WAITING_FOR_MODE;
                                break;
                            }
                        }
                        // If we sent askMode to the current robot, we skip the code below.
                        // Else, no robot was connected.
                        if (this.currentRobot == null) {
                            // Signal other objects GUISecretary has finished.
                            // Improvement idea : Notify other objects and block the access to the command screen.
                            GUI.INSTANCE.modeReady();
                            Viewer.INSTANCE.setupRobotPort();
                        }
                    }

                    else if (event.getEventID() == Event.EventID.SELECT_ROBOT) {
                        this.updateSelectedRobot(event.getIdRobot());
                    }

                    // Unselect robots because we change screen.
                    else if (event.getEventID() == Event.EventID.UNSELECT_ROBOT) {
                        this.updateUnselectedRobot();
                    }

                    else if (event.getEventID() == Event.EventID.SET_RADAR) {

                        if (event.getRadarState() == Event.RadarState.OBJECT_DETECTED) {
                            // If an obstacle is detected, change in the selected robot and robot list.
                            if (this.selectedRobot != null) {
                                if (this.selectedRobot.getId() == event.getIdRobot())
                                    // It change the selected robot obstacle detection if the robot
                                    // that its obstacle detection changed is the selected one.
                                    this.selectedRobot.setObstacleDetectionTrue();
                            }
                            // Update the obstacle detection state of the concerned robot in the robot list.
                            this.robotList.get(event.getIdRobot()-1).setObstacleDetectionTrue();
                        } else {
                            // If no obstacle is detected, change in the selected robot and robot list.
                            if (this.selectedRobot != null) {
                                if (this.selectedRobot.getId() == event.getIdRobot())
                                    // It change the selected robot obstacle detection if the robot
                                    // that its obstacle detection changed is the selected one.
                                    this.selectedRobot.setObstacleDetectionFalse();
                            }
                            // Update the obstacle detection state of the concerned robot in the robot list.
                            this.robotList.get(event.getIdRobot()-1).setObstacleDetectionFalse();
                        }
                        this.robots.postValue(this.robotList);
                    }
                    break;

                case WAITING_FOR_MODE:
                    if (event.getEventID() == Event.EventID.SET_MODE)
                    {
                        this.changeMode(event.getIdRobot(), event.getOperatingMode());
                        this.pickNextRobot();
                        while (this.currentRobot != null) {
                            if (this.currentRobot.getConnectionState() != Robot.ConnectionState.CONNECTED) {
                                this.pickNextRobot();
                            } else {
                                this.proxyControllerCore.askMode(this.currentRobot.getId());
                                break;
                            }
                        }

                        if (this.currentRobot == null) {
                            // End to take the operating Mode of all robots.
                            GUI.INSTANCE.modeReady();
                            Viewer.INSTANCE.setupRobotPort();
                            this.state = State.AVAILABLE;
                        }

                    }
                    break;

                case WAITING_POP_UP_VALIDATION:

                    // Validation of the pop up button to close the pop up.
                    if (event.getEventID() == Event.EventID.VALIDATE) {
                        //this.idRobotConnection.postValue(0);
                        this.state = State.IDLE;
                    }
                    break;

                case WAITING_FOR_DISCONNECTION:
                    if (event.getEventID() == Event.EventID.DISCONNECTED_OK) {
                        this.updateDisconnectedRobot(event.getIdRobot());
                        this.pickNextRobot();
                        while (this.currentRobot != null) {
                            if (this.currentRobot.getConnectionState() != Robot.ConnectionState.CONNECTED) {
                                this.pickNextRobot();
                            } else {
                                this.proxyControllerCore.askToDisconnect(this.currentRobot.getId());
                                LogsManager.INSTANCE.log(LogLevel.DEBUG, "Disconnecting robot"+this.currentRobot.getId());
                                break;
                            }
                        }
                        // If we sent askToDisconnect to the current robot, we skip the code below.
                        // Else, no robot was connected.
                        if (this.currentRobot == null) {
                            // TODO : Implement a observer connected to Viewer to notify
                            GUI.INSTANCE.stopDisconnection();
                            this.state = State.STOP;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    /**
     * @brief Validate the pop up.
     */
    public void validate(){
        this.guiSecretaryQueue.add(new Event(Event.EventID.VALIDATE));
    }

    /**
     * IdRecup : Identifiant du robot récupéré lors d'une demande de connexion.
     */
    public void askInit() {
        this.guiSecretaryQueue.add(new Event(Event.EventID.ASK_INIT));
    }

    /**
     * @brief Give the ProxyControllerCore.
     * @return ProxyControllerCore asked.
     */
    public ProxyControllerCore getProxyControllerCore()
    {
        return this.proxyControllerCore;
    }

    /**
     * @brief Give the robot list.
     * @return robot list asked.
     */
    public ArrayList<Robot> getRobotList(){
        return this.robotList;
    }

    /**
     * @brief To get the robot list in live data.
     * @return robots
     */
    public LiveData<ArrayList<Robot>> getRobots(){
        return this.robots;
    }

    /**
     * @brief To get the id robot connection in live data.
     * @return idRobotConnection
     */
    public LiveData<Integer> getIdRobotConnection(){
        //return this.idRobotConnection;
        LiveData<Integer> liveData = new LiveData<Integer>(){};
        return liveData;
    }

    /**
     * @brief Give the selected robot.
     * @return selectedRobot Selected robot.
     */
    public Robot getSelectedRobot() {
        return this.selectedRobot;
    }

    /**
     * @brief Unselect robot because we want to change the screen
     */
    public void unselectRobot() {
        this.guiSecretaryQueue.add(new Event(Event.EventID.UNSELECT_ROBOT));
    }

    /**
     * @brief Change the state of the selected robot and send the state to the robot.
     */
    public void setSelectedRobot(int selectedRobot, Robot.SelectionState robotState) {
        Event event = new Event(Event.EventID.SELECT_ROBOT);
        event.setIdRobot(selectedRobot);
        event.setSelectionState(robotState);
        this.guiSecretaryQueue.add(event);
    }

    /**
     * @brief Call to change the operating mode for a robot with the given idRobot. Its an event
     * from user that click on the screen.
     * @param periphToChange peripheral concerned by the change to the new periph state.
     * @param periphState state of the peripheral that change to.
     */
    public void askSetMode(OperatingMode.PeriphToChange periphToChange, PeriphState periphState){
        Event event = new Event(Event.EventID.SET_MODE);
        event.setPeriphToChangeAndState(periphToChange, periphState);
        this.guiSecretaryQueue.add(event);
    }

    /**
     * @brief Set the update operating mode of the robot.
     */
    public void setMode(int idRobot, OperatingMode operatingMode) {
        Event event = new Event(Event.EventID.SET_MODE);
        event.setIdRobot(idRobot);
        event.setOperatingMode(operatingMode);
        this.guiSecretaryQueue.add(event);
    }

    /**
     * @brief Change the operating mode for a selectedRobot in the robotList.
     * @param idRobot id of the robot to change its operating Mode.
     * @param operatingMode new operating mode to change.
     */
    public void changeMode(int idRobot, OperatingMode operatingMode){
        this.robotList.get(idRobot-1).setOperatingMode(operatingMode);
        this.robots.postValue(this.robotList);
    }

    /**
     * @brief Acknowledge for a robot disconnection.
     * @param idRobot of the concerned robot.
     */
    public void disconnectedOk(int idRobot){
        Event event = new Event(Event.EventID.DISCONNECTED_OK);
        event.setIdRobot(idRobot);
        this.guiSecretaryQueue.add(event);
    }

    /**
     * @brief Acknowledge the connection of a robot.
     * @param idRobot of the concerned robot.
     */
    public void ackConnection(int idRobot){
        Event event = new Event(Event.EventID.ACK_CONNECTION);
        event.setIdRobot(idRobot);
        this.guiSecretaryQueue.add(event);
    }

    /**
     * @brief Alert that the connection has failed for a specific robot.
     * @param idRobot of the concerned robot.
     */
    public void connectionFailed(int idRobot){
        Event event = new Event(Event.EventID.CONNECTION_FAILED);
        event.setIdRobot(idRobot);
        this.guiSecretaryQueue.add(event);
    }

    /**
     * @brief Responsible for the disconnection of all robots for the end of the application.
     * Go through all robot in the robotList to disconnect them.
     */
    public void disconnectRobots(){
        this.guiSecretaryQueue.add(new Event(Event.EventID.DISCONNECT_ROBOTS));
    }

    /**
     * @brief Call to ask the operating mode to the robots.
     */
    public void setModeInit(){
        this.guiSecretaryQueue.add(new Event(Event.EventID.SET_MODE_INIT));
    }

    /**
     * @brief Notifying that the var is ready.
     * @param idRobot of the robot concerned.
     */
    public void notifyVarReady(int idRobot){
        this.guiSecretaryQueue.add(new Event(Event.EventID.NOTIFY_VAR_READY));
    }

    /**
     * @brief Notifying the disconnection (not voluntary) of a robot.
     * @param idRobot Id of the robot disconnected.
     * @param fromDispatcher Boolean that inform the source of the invoke.
     *                       True : from dispatcher - False : from else.
     */
    public void notifyDisconnectedRobot(int idRobot, boolean fromDispatcher){
        Event event = new Event(Event.EventID.NOTIFY_DISCONNECTED_ROBOT);
        event.setIdRobot(idRobot);
        event.setFromDispatcher(fromDispatcher);
        this.guiSecretaryQueue.add(event);
    }

    /**
     * @brief Set the new state of the radar of the robot.
     * @param idRobot Id robot that the radar has changed.
     * @param radarState New state of the radar.
     */
    public void setRadar(int idRobot, Event.RadarState radarState) {
        Event event = new Event(Event.EventID.SET_RADAR);
        event.setIdRobot(idRobot);
        event.setRadarState(radarState);
        this.guiSecretaryQueue.add(event);
    }

    /**
     * @brief Give the postman list. There is one postman instance for each robot. The postman is
     * identified by its index in the list to the id robot -1.
     * @return Postman list of the robots.
     */
    public ArrayList<Postman> getPostmanList() {
        return this.postmanList;
    }

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

    /**
     * @brief Initialize the connection with all robots in the robotList.
     */
    private void initConnection() {

        // Creation of postman for the current robot.
        Postman postman = new Postman(this.currentRobot.getId(), this.currentRobot.getIpAddress(), this.currentRobot.getPort());
        // Starting the thread so the connection to the server (robot).
        postman.start();

        // Adding the postman in the postmanList.
        this.postmanList.add(postman);

    }

    /**
     * @brief Change the selected robot to the one given in parameter identify by its idRobot. It
     * also unselect the current selected robot.
     * @param idRobotToSelect id of the robot we want to select.
     * @note This function has been created by the developers to separate the functionalities.
     */
    private void updateSelectedRobot(int idRobotToSelect) {
        if (this.robotList.get(idRobotToSelect-1).getConnectionState() == Robot.ConnectionState.DISCONNECTED) {
            // The robot given in parameter isn't connected so leaves this function.
            return;
        }

        // A robot is already selected.
        if (this.selectedRobot != null) {
            // If this robot is the one called in parameter, we don't do anything, else, unselect it
            if (this.selectedRobot.getId() == idRobotToSelect) return;

            // Update the unselect state to the right robot
            this.robotList.get(this.selectedRobot.getId()-1).setUnselectedState();

            // Send the selection state to unselected to the concerned robot.
            this.proxyControllerCore.setState(this.selectedRobot.getId(), Robot.SelectionState.UNSELECTED);
        }

        // Selecting the new robot.
        this.robotList.get(idRobotToSelect-1).setSelectedState();
        this.selectedRobot = this.robotList.get(idRobotToSelect-1);

        // Send the selection state to selected to the concerned robot.
        this.proxyControllerCore.setState(this.selectedRobot.getId(), Robot.SelectionState.SELECTED);

        // Post the robot list into the live data list.
        this.robots.postValue(this.robotList);
        // Post the selected robot into the live data robot.
        CommandScreenViewModel.setSelectedRobot(this.selectedRobot);
    }

    /**
     * @brief Unselect a selected robot if one has been selected. Also change the operating mode of
     * camera of all robots.
     * @note This function has been created by the developers to separate the functionalities of GUISecretary.
     */
    private void updateUnselectedRobot() {
        // Change the operating mode of the camera of the robots if they are enable.
        this.pickFirstRobot();
        while (this.currentRobot != null) {
            if (this.currentRobot.getOperatingMode().getCamera() == PeriphState.ENABLED.ordinal()) {
                this.robotList.get(this.currentRobot.getId()-1).getOperatingMode().
                        setOneOperatingMode(OperatingMode.PeriphToChange.CAMERA, PeriphState.DISABLED);
                this.proxyControllerCore.askSetMode(this.currentRobot.getId(), this.currentRobot.getOperatingMode());
            }
            this.pickNextRobot();
        }
        // If a robot is currently selected, unselect it.
        if (this.selectedRobot != null) {
            this.robotList.get(this.selectedRobot.getId()-1).setUnselectedState();
            // Send the selection state to unselected to the concerned robot.
            this.proxyControllerCore.setState(this.selectedRobot.getId(), Robot.SelectionState.UNSELECTED);
            this.selectedRobot = null;
        }
        this.robots.postValue(this.robotList);
    }

    /**
     * @brief End of the connection to the robots, signal all other objects.
     */
    private void endInitConnection() {
        // Create the proxys (frame senders)
        this.proxyControllerCore.setPostmanList(this.postmanList);
        this.robots.postValue(this.robotList);

        // Notify GUIRinger to have finished the initialization of the robots.
        GUIRinger.INSTANCE.initOk();

        // Notify GUI to have finished the initialization of the robots.
        GUI.INSTANCE.initReady();
    }

    /**
     * @brief Update the connection state of the robot in the robotList to be disconnected. Also
     * change the disconnected robot list by adding the id robot and connected robot list by
     * removing the id robot of the given robot.
     * @param idRobot Id of the robot that just disconnected.
     */
    private void updateDisconnectedRobot(int idRobot){
        this.robotList.get(idRobot-1).setDisconnectedState();
        boolean showPopUp = false;
        if (showPopUp) {
            //this.idRobotConnection.postValue(idRobot);
            this.state = State.WAITING_POP_UP_VALIDATION;
            LogsManager.INSTANCE.log(LogLevel.DEBUG, "Robot"+idRobot+" is disconnected, display pop-up");
        }
        this.robots.postValue(this.robotList);
    }

    /**
     * @brief Update the connection state of the robot in the robotList to be connected. Also change
     * the disconnected robot list by removing the id robot and connected robot list by adding the
     * id robot of the given robot.
     */
    private void updateConnectedRobot(int idRobot){
        this.robotList.get(idRobot-1).setConnectedState();
        this.robots.postValue(this.robotList);
    }

    /**
     * @brief Update the robot list with the given robot list.
     * @param newRobotList to replace the actual one.
     */
    private void updateRobotList(ArrayList<Robot> newRobotList){
        this.robotList = newRobotList;
    }

    /* ----------------------  PROTECTED FUNCTIONS  ------------------------------- */

    /**
     * @brief Change the currentRobot variable to the first robot in the robotList.
     */
    protected void pickFirstRobot() {
        if (this.robotList != null) {
            this.currentRobot = this.robotList.get(0);
        } else {
            // TODO : If we want to catch the problem of empty robot list, we can add notifiers here.
            this.currentRobot = null;
        }
    }

    /**
     * @brief Change the currentRobot variable to the next robot in the robotList. If it was the
     * last robot in the list, it returns a error.
     */
    protected void pickNextRobot() {
        if (this.currentRobot != null) {
            // Take the index of the new robot in the robot list (index in the list = id robot + 1)
            int idRobot = this.currentRobot.getId();
            if (idRobot == this.robotList.size()) {
                // Max list reached.
                this.currentRobot = null;
            } else {
                // Not max reached, so indexing the current robot.
                this.currentRobot = this.robotList.get(idRobot);
            }
        }
    }
}
