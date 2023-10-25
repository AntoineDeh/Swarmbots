/**
 * \file  GUIRinger.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief GUIRinger is responsible for checking the availability of robots. All robots will receive
 * a ping (askAvailability) and should respond with a pong (setAvailability). If the robot doesn't
 * respond before a delay time, a failed pongs variable of the robot increase. The robot is
 * considered disconnected when its failed pongs variable reach the max value authorized. When this
 * happend, GUIRinger inform the other objects of the robot disconnection.
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

import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ArrayBlockingQueue;

import project.src.model.LogLevel;
import project.src.model.com.ProxyControllerRinger;

public class GUIRinger extends Thread {

    /* ----------------------  PRIVATE CONSTANT  ------------------------------ */

    /**
     * @brief Time out to notify GUIRinger to ask the availability of the robots. Its value should
     * be higher than TIME_OUT_PONGS times max number of robots.
     */
    private static final int REFRESH_PING_PERIOD = 15000;

    /**
     * @brief Time out that we ask the robot to respond.
     */
    private static final int TIME_OUT_PONGS = 5000;

    /**
     * @brief Number of event accepted by the GUIRinger queue.
     */
    private static final int MAX_CAPACITY_QUEUE = 20;

    /**
     * @brief Number of failed pongs missed before considering a robot to be unavailable and disconnected.
     */
    private static final int MAX_FAILED_PONGS = 1;

    /* ----------------------  PRIVATE OBJECTS  ---------------------------------------- */

    /**
     * @brief Proxy controller ringer to simulate the controller ringer object in the robot and
     * expose its function to communicate wih it.
     */
    private ProxyControllerRinger proxyControllerRinger;

    /**
     * @brief Robot list of all robot to check their connection state.
     */
    private ArrayList<Robot> robotList = new ArrayList<>();

    /**
     * @brief Current robot we want its response for the connection state.
     */
    private Robot currentRobot;

    /**
     * @brief Possible state of the state machine object.
     */
    private enum State {
        WAITING_SECRETARY,
        INIT_VAR_ROBOT,
        INIT,
        SENDING_PING,
        PONG_RECEIVED,
        STOP
    }

    /**
     * @brief General timer of all robots to know when it should ask the availability.
     */
    private Timer timerAllRobots;

    /**
     * @brief General timer task of all robots to know when it should ask the availability. Canceled
     * when the timer raise the call function.
     */
    private TimerTask timerTaskAllRobots;

    /**
     * @brief Timer to ask the availability to know their connection state.
     */
    private Timer timerAskAvailability;

    /**
     * @brief Timer task to ask the availability to know their connection state. Canceled when the
     * timer raise the call function.
     */
    private TimerTask timerTaskAskAvailability;

    /**
     * @brief GUIRinger queue to send messages.
     */
    private final ArrayBlockingQueue<Event> guiRingerQueue;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------------------- */

    /**
     * @brief Current state of the object.
     */
    private State state;

    /* ----------------------  SINGLETON INSTANCE AND CONSTRUCTOR(S)  ---------------------------------------- */


    public static GUIRinger INSTANCE;

    static {
        INSTANCE = new GUIRinger();
    }

    private GUIRinger() {
        this.guiRingerQueue = new ArrayBlockingQueue<>(MAX_CAPACITY_QUEUE);
        this.proxyControllerRinger = new ProxyControllerRinger();
        this.state = State.WAITING_SECRETARY;
    }

    /* ----------------------  PUBLIC FUNCTIONS  ---------------------------------------- */

    /**
     * @brief Call to initialize the singleton instance.
     */
    public void create() {}

    /**
     * @brief Run method for the GUIRinger thread with its state machine. It will take event
     * from its queue and execute the event associated.
     */
    @Override
    public void run() {
        Event event = null;

        while (this.state != State.STOP) {
            // Catch the event in the queue.
            try {
                event = this.guiRingerQueue.take();
            } catch (InterruptedException ignored) {
                continue;
            }

            if (event.getEventID() == Event.EventID.QUIT) {
                // Cancel the timers
                if (this.timerAllRobots != null) {
                    this.timerAllRobots.cancel();
                    this.timerAllRobots = null;
                }
                if (this.timerAskAvailability != null) {
                    this.timerAskAvailability.cancel();
                    this.timerAskAvailability = null;
                }
                this.state = State.STOP;
                continue;
            }

            switch (this.state) {

                case WAITING_SECRETARY :
                    if (event.getEventID() == Event.EventID.CHECK_FAILED_PONG_VAR) {
                        this.createFailedPongVar(event.getIdRobot());
                        // We don't call the update failed pongs function because it uses the robot
                        // list we don't have yet.
                        GUISecretary.INSTANCE.notifyVarReady(event.getIdRobot());
                        this.state = State.INIT_VAR_ROBOT;
                    }
                    break;

                case INIT_VAR_ROBOT :
                    if (event.getEventID() == Event.EventID.CHECK_FAILED_PONG_VAR) {
                        this.createFailedPongVar(event.getIdRobot());
                        // We don't call the update failed pongs function because it uses the robot
                        // list we don't have yet.
                        GUISecretary.INSTANCE.notifyVarReady(event.getIdRobot());
                    }

                    else if (event.getEventID() == Event.EventID.INIT_OK) {
                        this.robotList = GUISecretary.INSTANCE.getRobotList();
                        this.proxyControllerRinger.setPostManList(GUISecretary.INSTANCE.getPostmanList());
                        // Start the timer to ask the availability of the robots.
                        this.startTimerAllRobots();
                        this.state = State.INIT;
                    }

                    break;

                case INIT :
                    if (event.getEventID() == Event.EventID.SEND_PING) {
                        this.timerTaskAllRobots.cancel();

                        // Ask availability to the current robot.
                        this.pickFirstRobot();
                        // Loop until the robot is connected or we have finish to ask robots.
                        while (this.currentRobot != null) {

                            // We ask the availability only for the connected robot. So if the current
                            // robot isn't connected we skip to the next robot.
                            if (this.currentRobot.getConnectionState() != Robot.ConnectionState.CONNECTED) {
                                this.pickNextRobot();
                            } else {
                                // The current robot is connected, we ask its availability.
                                this.proxyControllerRinger.askAvailability(this.currentRobot.getId());
                                LogsManager.INSTANCE.log(LogLevel.DEBUG, "Ask availability to the robot"+this.currentRobot.getId());
                                // Restart the timer for the next robot.
                                this.startTimerAskAvailability();
                                this.state = State.SENDING_PING;
                                break;
                            }
                        }
                    }

                    break;

                case SENDING_PING :

                    // The current robot responded available.
                    if (event.getEventID() == Event.EventID.SET_AVAILABILITY) {
                        this.timerAskAvailability.cancel();
                        this.updateFailedPongs(0, this.currentRobot.getId());
                        LogsManager.INSTANCE.log(LogLevel.INFO, "Set availability by the robot"+this.currentRobot.getId());
                    }

                    // The current robot isn't available.
                    else if (event.getEventID() == Event.EventID.TIME_OUT_PING) {
                        LogsManager.INSTANCE.log(LogLevel.DEBUG, "Robot"+this.currentRobot.getId()+" didn't responded.");
                        if (this.checkFailedPongs(this.currentRobot.getId())) {
                            // The current robot didn't respond but has a number of failed pongs lower than the max authorized.
                            this.updateFailedPongs(this.currentRobot.getFailedPongsVar()+1, this.currentRobot.getId());
                        } else {
                            // The current robot is considered disconnected.
                            GUISecretary.INSTANCE.notifyDisconnectedRobot(this.currentRobot.getId(), false);
                        }
                    }

                    // Do whatever the response of the previous robot has responded.
                    this.pickNextRobot();

                    // Loop until the robot is connected or we have finish to ask robots.
                    while (this.currentRobot != null) {

                        // We ask the availability only for the connected robot. So if the current
                        // robot isn't connected we skip to the next robot.
                        if (this.currentRobot.getConnectionState() != Robot.ConnectionState.CONNECTED) {
                            this.pickNextRobot();
                        } else {
                            // The current robot is connected, we ask its availability.
                            this.proxyControllerRinger.askAvailability(this.currentRobot.getId());
                            LogsManager.INSTANCE.log(LogLevel.INFO, "Ask the availability to the robot"+this.currentRobot.getId());
                            // Restart the timer for the next robot.
                            this.startTimerAskAvailability();
                            break;
                        }
                    }

                    if (this.currentRobot == null) {
                        this.state = State.INIT;
                        this.startTimerAllRobots();
                    }

                    break;

                default :
                    break;
            }
        }
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Notify the reception of the robot.
     * @param idRobot id of the robot we want to check.
     */
    public void setAvailability(int idRobot) {
        Event event = new Event(Event.EventID.SET_AVAILABILITY);
        event.setIdRobot(idRobot);
        this.guiRingerQueue.add(event);
    }

    /**
     * @brief Ask to check that failed pongs var is initialized.
     * @param idRobot id of the robot we want to check.
     */
    public void checkFailedPongVar(int idRobot) {
        Event event = new Event(Event.EventID.CHECK_FAILED_PONG_VAR);
        event.setIdRobot(idRobot);
        this.guiRingerQueue.add(event);
    }

    /**
     * @brief Check if the given robot has reached the limit authorized number of failed pongs.
     * @param idRobot Id of the robot we want to check its failed pongs value.
     * @return boolean related to the value of failed pongs if it's lower than the limit
     * authorized.
     */
    public boolean checkFailedPongs(int idRobot) {
        LogsManager.INSTANCE.log(LogLevel.DEBUG, "Robot"+idRobot+" didn't respond. " +
                "Failed pongs equals to "+this.robotList.get(idRobot-1).getFailedPongsVar()+".");
        return (this.robotList.get(idRobot-1).getFailedPongsVar() < MAX_FAILED_PONGS);
    }

    /**
     * @brief Signal the end of the application and ask to stop the thread.
     */
    public void stopRing() {
        this.guiRingerQueue.add(new Event(Event.EventID.QUIT));
    }


    /**
     * @brief Signal the end of initialization of all connection robots.
     */
    public void initOk() {
        this.guiRingerQueue.add(new Event(Event.EventID.INIT_OK));
    }

    /* ----------------------  PRIVATE FUNCTIONS  ---------------------------------------- */

    /**
     * @brief Start the overall timer to ask the availability of all robots.
     */
    private void startTimerAllRobots() {
        // Initialize the timer
        timerAllRobots = new Timer();
        // Set a new timer task.
        timerTaskAllRobots = new TimerTask() {
            @Override
            public void run() {
                // At the end of the timer, call pingAvailability function to restart the pings of all robots.
                timeOutAllRobots();
            }
        };
        // Schedule the timer task to ring after TIME_OUT_ASK_ROBOTS milliseconds.
        timerAllRobots.schedule(timerTaskAllRobots, REFRESH_PING_PERIOD);
    }

    /**
     * @brief Time out
     */
    private void timeOutAllRobots() {
        // Cancel the timer to reset it.
        timerTaskAllRobots.cancel();

        // Post the event to send pings to all connected robots.
        this.guiRingerQueue.add(new Event(Event.EventID.SEND_PING));
    }

    /**
     * @brief Start the overall timer to ask availability to the current robot.
     */
    private void startTimerAskAvailability() {
        // Initialize the timer.
        timerAskAvailability = new Timer();

        // Set a new timer task.
        timerTaskAskAvailability = new TimerTask() {
            @Override
            public void run() {
                // At the end of the timer, call pingAvailability function to restart the pings of all robots.
                timeOutAskAvailability();
            }
        };
        // Schedule the timer task to ring after TIME_OUT_ASK_ROBOTS milliseconds.
        timerAskAvailability.schedule(timerTaskAskAvailability, TIME_OUT_PONGS);
    }

    /**
     * @brief Time out while asking the availability of the current robot.
     */
    private void timeOutAskAvailability() {
        // Cancel the timer to reset it.
        timerTaskAskAvailability.cancel();

        // Post the event to send pings to all connected robots.
        this.guiRingerQueue.add(new Event(Event.EventID.TIME_OUT_PING));
    }

    /**
     * @brief Change the currentRobot variable to the first robot in the robot list.
     */
    private void pickFirstRobot() {
        if(this.robotList != null) {
            this.currentRobot = this.robotList.get(0);
        } else {
            this.state = State.STOP;
        }
    }

    /**
     * @brief Change the currentRobot variable to the next robot in the robot list. If it was the
     * last robot in the list, sets null value into current robot.
     */
    private void pickNextRobot() {
        // Take the index of the new robot in the robot list (index in the list = id robot + 1)
        int idRobot = this.currentRobot.getId();
        if (idRobot == this.robotList.size()) {
            // Max list reached.
            this.currentRobot = null;
        } else {
            // Not max reached, so indexing the current robot.
            LogsManager.INSTANCE.log(LogLevel.DEBUG, "Cannot ping due to an empty list of robot");
            this.currentRobot = this.robotList.get(idRobot);
        }
    }

    /**
     * @brief Update the failed pong value of the robot of the idRobot.
     * @param value New value the failed pong value of the robot will take.
     * @param idRobot Id of the robot.
     */
    private void updateFailedPongs(int value, int idRobot) {
        this.robotList.get(idRobot-1).setFailedPongsVar(value);
    }

    /**
     * @brief Create the failed pong variable of the robot of the idRobot, if it doesn't exist yet.
     * @param idRobot Id of the robot we want to create the failed pongs var.
     */
    private void createFailedPongVar(int idRobot) {
        /*
        Empty function because it use the robot list with failed pongs var already initialized.
         */
    }
}
