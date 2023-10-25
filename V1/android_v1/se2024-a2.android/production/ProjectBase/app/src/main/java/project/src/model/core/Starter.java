/**
 * \file  Starter.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 18, 2023
 * \brief Starter is responsible for the start of the application and active object threads.
 * It also detects the application stop and stop all active object threads.
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

import java.util.concurrent.ArrayBlockingQueue;

public class Starter extends Thread {

    /* ----------------------  PRIVATE CONSTANT  ------------------------------ */

    /**
     * @brief Number of event accepted by the GUISecretary queue.
     */
    private static final int MAX_CAPACITY_QUEUE = 10;

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief State of the application.
     */
    public enum ApplicationState {
        OPENED,
        CLOSED
    }

    /**
     * @brief State of the screens.
     */
    public enum ScreenState {
        STARTED,
        STOPPED
    }

    /**
     * @brief Starter queue to send messages.
     */
    private final ArrayBlockingQueue<Event> starterQueue;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief Current state of the application.
     */
    private ApplicationState applicationState = ApplicationState.OPENED;

    /**
     * @brief Current state of the home screen.
     */
    private ScreenState homeScreenState = ScreenState.STOPPED;

    /**
     * @brief Current state of the command screen.
     */
    private ScreenState commandScreenState = ScreenState.STOPPED;

    /**
     * @brief Current state of the logs screen.
     */
    private ScreenState logsScreenState = ScreenState.STOPPED;

    /**
     * @brief Instance of the starter basic singleton object.
     */
    public static Starter INSTANCE;

    static {
        INSTANCE = new Starter();
    }

    /**
     * @brief Private constructor for the basic singleton Starter object.
     */
    private Starter() {
        this.starterQueue = new ArrayBlockingQueue<>(MAX_CAPACITY_QUEUE);
    }

    /**
     * @brief Create and start the active objects.
     */
    public void main() {
        this.homeScreenState = ScreenState.STARTED;

        GUI.INSTANCE.create();
        GUISecretary.INSTANCE.create();

        if (!LogsManager.currentThread().isDaemon()) LogsManager.INSTANCE.start();
        if (!GUISecretary.currentThread().isDaemon()) GUISecretary.INSTANCE.start();
        if (!GUI.currentThread().isDaemon()) GUI.INSTANCE.start();
        if (!Starter.currentThread().isDaemon()) Starter.INSTANCE.start();
    }

    /**
     * @brief Main function of Starter thread. It receives the events from the other objects.
     */
    @Override
    public void run() {
        // Event store in the variable.
        Event event = null;
        while(applicationState == ApplicationState.OPENED) {

            try {
                // Catch the oldest message event in the queue.
                event = this.starterQueue.take();
            } catch (InterruptedException e) {

            }

            // Call the update function depending on the event start or stop received.
            if (event.getEventID() == Event.EventID.SCREEN_START) updateStart(event.getScreenActivity());
            else updateStopped(event.getScreenActivity());

            // Check if all screen is stopped.
            this.checkAllStopped();
        }
    }

    /**
     * @brief Notify state change of the home activity to set its new state.
     * @param screenState State of the home activity screen.
     */
    public void notifyHomeActivity(ScreenState screenState) {
        Event event = new Event( (screenState == ScreenState.STARTED) ?
                Event.EventID.SCREEN_START : Event.EventID.SCREEN_STOPPED);
        event.setScreenActivity(Event.ScreenActivity.HOME_SCREEN);
        this.starterQueue.add(event);
    }

    /**
     * @brief Notify state change of the command activity to set its new state.
     * @param screenState State of the command activity screen.
     */
    public void notifyCommandActivity(ScreenState screenState) {
        Event event = new Event( (screenState == ScreenState.STARTED) ?
                Event.EventID.SCREEN_START : Event.EventID.SCREEN_STOPPED);
        event.setScreenActivity(Event.ScreenActivity.COMMAND_SCREEN);
        this.starterQueue.add(event);
    }

    /**
     * @brief Notify state change of the logs activity to set its new state.
     * @param screenState State of the logs activity screen.
     */
    public void notifyLogsActivity(ScreenState screenState) {
        Event event = new Event( (screenState == ScreenState.STARTED) ?
                Event.EventID.SCREEN_START : Event.EventID.SCREEN_STOPPED);
        event.setScreenActivity(Event.ScreenActivity.LOGS_SCREEN);
        this.starterQueue.add(event);
    }

    /**
     * @brief Check the state of all screen if they're stopped. If yes, engages the disconnection of
     * robots, and stops active objects and application.
     */
    private void checkAllStopped() {
        if ( (this.homeScreenState == ScreenState.STOPPED)
                & (this.commandScreenState == ScreenState.STOPPED)
                & (this.logsScreenState == ScreenState.STOPPED) )
        {
            // Stop application detected.
            GUI.INSTANCE.quit();
            this.applicationState = ApplicationState.CLOSED;
        }
    }

    /**
     * @brief Set the state of the given screen to running.
     * @param screen Screen concerned by the state change.
     */
    private void updateStart(Event.ScreenActivity screen) {
        switch (screen) {
            case HOME_SCREEN :
                this.homeScreenState = ScreenState.STARTED;
                break;
            case COMMAND_SCREEN :
                this.commandScreenState = ScreenState.STARTED;
                break;
            case LOGS_SCREEN :
                this.logsScreenState = ScreenState.STARTED;
                break;
            default :
                break;
        }
    }

    /**
     * @brief Set the state of the given screen to stopped.
     * @param screen Screen concerned by the state change.
     */
    private void updateStopped(Event.ScreenActivity screen) {
        switch (screen) {
            case HOME_SCREEN :
                this.homeScreenState = ScreenState.STOPPED;
                break;
            case COMMAND_SCREEN :
                this.commandScreenState = ScreenState.STOPPED;
                break;
            case LOGS_SCREEN :
                this.logsScreenState = ScreenState.STOPPED;
                break;
            default :
                break;
        }
    }

}
