/**
 * \file  LogsManager.java
 * \version  0.2
 * \author Mahery FONG, julien NICOT
 * \date Jun 7, 2023
 * \brief entry of the program.
 * \note According to the design, in the function saveGUILogs(loglevel, message),
 * logList should have been a log array. However, given the progress of the project
 * and the non-priority of flushing logs, logList becomes a String to facilitate the task.
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
 */
/* ----------------------  PACKAGE  ---------------------------------------- */
package project.src.model.core;

/* ----------------------  INCLUDES  ---------------------------------------- */

import android.annotation.SuppressLint;
import android.content.Context;

import androidx.lifecycle.MutableLiveData;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;

import java.io.InputStreamReader;
import java.time.DayOfWeek;
import java.time.LocalDateTime;
import java.time.Month;
import java.time.format.DateTimeFormatter;

import java.util.ArrayList;
import java.util.Objects;
import java.util.TimeZone;
import java.util.concurrent.ArrayBlockingQueue;

import project.src.model.LogLevel;
import project.src.model.com.ProxyControllerLogger;
import project.src.view.activities.HomeScreenActivity;

public class LogsManager extends Thread{

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief Time zone.
     */
    private static final String TIME_ZONE = "Europe/Paris";

    /**
     * @brief Tag of the object.
     */
    private static final String TAG = "LogsManager";

    /**
     * @brief Current day to log event. It's defined at the start of the object and doesn't change
     * until the thread restart.
     */
    private String currentDayMonth = "";

    /**
     * @brief Current year to log event. It's defined at the start of the object and doesn't change
     * until the thread restart.
     */
    private String currentYear = "";

    /**
     * @brief Proxy for ControllerLogger.
     */
    private final ProxyControllerLogger proxyControllerLogger;

    private enum State {
        IDLE,
        FLUSHING,
        EXPORTING,
        STOP
    }

    /**
     * @brief State of the robot.
     */
    private State state;

    /**
     * @brief GUIRinger queue to send messages.
     */
    private final ArrayBlockingQueue<Event> logsManagerQueue;

    /**
     * @brief HomeScreenActivity's context.
     */
    @SuppressLint("StaticFieldLeak")
    private static Context context;

    /**
     * @brief Mutable live data to display the selected logs saved in the logs file.
     */
    private final MutableLiveData<ArrayList<String>> selectedLogsMutableLiveData = new MutableLiveData<>();


    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief String that indicates the current selected logs.
     */
    private final String selectedLogsList = "";

    /**
     * @brief Logs of GUI. Should be deleted after implementing logs file (database)
     */
    private final ArrayList<String> guiLogs = new ArrayList<>();

    /**
     * @brief Logs of the robot. Shared by the 2 existing robot and should be deleted after implementing
     * logs in tcp communication and logs file (database)
     */
    private final ArrayList<String> robotLogs = new ArrayList<>();

    /**
     * @brief Id files of the GUI and robots logs.
     */
    public ArrayList<String> idFileList;

    /* ----------------------  SINGLETON INSTANCE AND CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief LogsManager's Instance.
     */
    @SuppressLint("StaticFieldLeak")
    public static LogsManager INSTANCE;

    /**
     * @brief Number of event accepted by the GUISecretary queue.
     */
    private static final int MAX_CAPACITY_QUEUE = 20;

    static {
        INSTANCE = new LogsManager();
    }
    
    /**
     * @brief LogsManager's constructor.
     */
    private LogsManager() {
        this.logsManagerQueue = new ArrayBlockingQueue<>(MAX_CAPACITY_QUEUE);
        this.proxyControllerLogger = new ProxyControllerLogger();
        this.state = State.IDLE;
        this.idFileList = new ArrayList<>();
        this.idFileList.add("GUILogsFile");
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Receiving context from HomeScreenActivity during initialization.
     * @param context HomeScreenActivity's context.
     */
    public void create(HomeScreenActivity context) {
        LogsManager.context = context;
    }

    /**
     * @brief State machine of LogsManager.
     */
    @Override
    public void run() {
        // Define the day and year all along the application time.
        this.currentDayMonth = this.catchDayMonth();
        this.currentYear = this.catchYear();

        Event event = null;

        while (this.state != State.STOP) {
            try {
                event = this.logsManagerQueue.take();
            } catch (InterruptedException e) {

            }

            if (event.getEventID() == Event.EventID.QUIT) {
                this.state = State.STOP;
                continue;
            }

            switch (this.state) {
                case IDLE :
                    if (event.getEventID() == Event.EventID.TRANSMIT_RTC) {
                        //this.proxyControllerLogger.askSetRtc(event.getIdRobot(), "rtc");
                    }

                    else if (event.getEventID() == Event.EventID.LOG) {
                        this.saveGUILogs(event.getLogLevel(), event.getMsg());
                    }

                    else if (event.getEventID() == Event.EventID.CHECK_FLUSHED_SAVED_LOGS_FILE) {
                        if (!this.checkFile(event.getIdFile())) {
                            this.createFlushedLogsFile(event.getIdFile());
                        }
                    }

                    else if (event.getEventID() == Event.EventID.SET_SELECTED_LOGS) {
                        if (!event.getIsGUISelectedLogs()) {
                            this.proxyControllerLogger.askLogs(event.getIdRobot());
                            LogsManager.INSTANCE.log(LogLevel.DEBUG, "Ask logs of the robot"+event.getIdRobot());
                            this.state = State.FLUSHING;
                        } else {
                            this.loadLogs(selectedLogsList);
                            GUI.INSTANCE.logsReady();
                        }
                    }

                    else if (event.getEventID() == Event.EventID.EXPORT_LOGS) {
                        // TODO : Uncomment the following line and delete the last one after
                        // TODO : finishing the export functionality. And add here the function to
                        // TODO : load the selected logs and send it to android.
                        //this.state = State.EXPORTING;
                        continue;
                    }

                    break;

                case FLUSHING :
                    if (event.getEventID() == Event.EventID.SET_LOGS) {
                        this.saveRobotLogs("Robot"+event.getIdRobot()+"LogsFile", event.getLogs());
                        this.loadLogs("Robot"+event.getIdRobot()+"LogsFile");
                        this.proxyControllerLogger.logsReceived(event.getIdRobot());
                        GUI.INSTANCE.logsReady();
                        this.state = State.IDLE;
                    }
                    break;

                case EXPORTING :
                    /*
                    Empty because this functionality isn't yet implemented.
                     */
                    break;

                default :
                    break;
            }
        }
    }

    /**
     * @brief Writing logs in the logsManagerQueue.
     * @param idRobot for the robot targeted.
     * @param logs for the content to send.
     */
    public void setLogs(int idRobot, ArrayList<String> logs) {
        Event event = new Event(Event.EventID.SET_LOGS);
        event.setIdRobot(idRobot);
        event.setLogs(logs);
        this.logsManagerQueue.add(event);
    }

    /**
     * @brief Initialize the fileList needed.
     * @param robotList proportional to the number of robots.
     */
    public void initFileList(ArrayList<Robot> robotList) {
    }

    /**
     * @brief Set the selected logs.
     * @param selectedLogs for knowing it.
     */
    public void setSelectedLogs(String selectedLogs) {
        Event event = new Event(Event.EventID.SET_SELECTED_LOGS);
        event.setGUISelectedLogs(Objects.equals(selectedLogs, selectedLogsList));
        this.logsManagerQueue.add(event);
    }

    /**
     * @brief Quitting the application and closing LogsManager's thread.
     */
    public void quit() {
        this.logsManagerQueue.add(new Event(Event.EventID.QUIT));
    }

    /**
     * @brief Verifies that the log file is created from idFile. If it is not, it will call an internal method method to create it.
     * @param idRobot Associated to the robotFile.
     */
    public void checkFlushedSavedLogsFile(int idRobot) {
        Event event = new Event(Event.EventID.CHECK_FLUSHED_SAVED_LOGS_FILE);
        event.setIdRobot(idRobot);
        this.logsManagerQueue.add(event);
    }

    public void transmitRtc(int idRobot) {
        Event event = new Event(Event.EventID.TRANSMIT_RTC);
        event.setIdRobot(idRobot);
        this.logsManagerQueue.add(event);
    }

    /**
     * @brief Get the level of log to display.
     * @return
     */
    public String getLogLevel(){
        try {
            //File reading.
            InputStream appConfigFile = context.getAssets().open("AppConfig.json");
            int size = appConfigFile.available();
            byte[] buffer = new byte[size];
            appConfigFile.read(buffer);
            appConfigFile.close();
            String json = new String(buffer, "UTF-8");

            //Asking infos.
            JSONObject jsonObject = new JSONObject(json);
            String logLevel = jsonObject.getString("level");

            return logLevel;

        } catch (IOException | JSONException e) {
            //Errors management.
            e.printStackTrace();
            return null;
        }
    }

    /**
     * @brief Intercept and ask to save the logs printed on screen.
     * @param logLevel The level of log desired.
     * @param message The content of the log.
     */
    public void log(LogLevel logLevel, String message){
        Event event = new Event(Event.EventID.LOG);
        event.setLogLevel(logLevel);
        event.setMsg(message);
        this.logsManagerQueue.add(event);
    }

    /**
     * @brief Save the logs in GUILogsFile. This function is divided by 2 with saveRobotLogs for the
     * parameter difference.
     * @param logLevel Level of the log referring to the situation which the log has been called.
     * @param message The content of the logs.
     */
    public void saveGUILogs(LogLevel logLevel, String message) {
        try {
            String logTimeMsg = logLevel + " : "+ this.currentDayMonth + this.catchTime() + this.currentYear + " - " + message;

            // File opening in reading/writing mode.
            File file = new File(context.getFilesDir(), selectedLogsList+".txt");

            //Pencil creation.
            FileWriter writer = new FileWriter(file, true);
            writer.append(logTimeMsg);

            //Pencil posing.
            writer.close();

            // Add the log into the selectedLogsList. Should be deleted if we use the logs file (database)
            this.guiLogs.add(logTimeMsg);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * @brief Save the logs of the robots in the logs file associated.
     * @param idFile Id of the file where stock the logs. It's given like : RobotXLogsFile, with X
     *               the id robot of the concerned robot.
     * @param robotLogsList The content of the logs.
     */
    public void saveRobotLogs(String idFile, ArrayList<String> robotLogsList) {
        try {
            // TODO : Implementing a loop for which the writer instance writes each line of the
            // TODO : robot logs list  into the logs file of the robot.
            // -> Replacing the SHOULD_BE_REPLACED by the pointed line inside the loop.
            String SHOULD_BE_REPLACED = "";

            // JULIEN VERSION
            // File opening in reading/writing mode.
            File file = new File(context.getFilesDir(), idFile);

            //Pencil creation.
            FileWriter writer = new FileWriter(file, true);
            writer.write(SHOULD_BE_REPLACED);

            //Pencil posing.
            writer.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * @brief Read the logs of the selected logs and change the selected logs value to the new one.
     */
    public void loadLogs(String selectedLogs) {
        try {
            FileInputStream fileInputStream = context.openFileInput(selectedLogs+".txt");
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            StringBuilder stringBuilder = new StringBuilder();
            String ligne;

            while ((ligne = bufferedReader.readLine()) != null) {
                stringBuilder.append(ligne);
            }

            fileInputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }


    /**
     * @brief Return the mutable live data logsMutableLiveData for LogsScreenViewModel
     * @return logsMutableLiveData
     */
    public MutableLiveData<ArrayList<String>> getLogs() {
        this.selectedLogsMutableLiveData.setValue(
                (Objects.equals(this.selectedLogsList, selectedLogsList)) ? this.guiLogs : this.robotLogs);
        //this.selectedLogsMutableLiveData.setValue(this.selectedLogsList);
        return this.selectedLogsMutableLiveData;
    }

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

    /**
     * @brief Create the file for save logs of the associated robot.
     * @param idFile The file targeted.
     */
    private void createFlushedLogsFile(String idFile) {
        this.idFileList.add(idFile);
    }

    /**
     * @brief Chech the file content.
     * @param idFile The file tatgeted.
     * @return The answer.
     */
    private boolean checkFile(String idFile) {
        for (String idFileInList : this.idFileList) {
            if (Objects.equals(idFileInList, idFile)) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Give the current local day,
     * @note This function isn't define in the design and it's called at the start of the thread object.
     */
    private String catchDayMonth() {
        String currentDayMonth = "";

        // Android version higher than 8.0 (2017).
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {

            // Get the current time (hour, minute, second) in paris.
            LocalDateTime currentDateTime = LocalDateTime.now(TimeZone.getTimeZone(TIME_ZONE).toZoneId());

            // Get current year, month and day (in week and month format).
            int currentYear = currentDateTime.getYear();
            Month currentMonth = currentDateTime.getMonth();
            DayOfWeek currentDayOfWeek = currentDateTime.getDayOfWeek();
            int currentDayOfMonth = currentDateTime.getDayOfMonth();

            // Adding to the time string :
            // [DAY (week)]
            currentDayMonth += currentDayOfWeek.toString().charAt(0) + currentDayOfWeek.toString().substring(1, 3).toLowerCase();
            // [DAY (week)] [MONTH] [DAY (month)]
            currentDayMonth += " " + currentMonth.toString().charAt(0) + currentMonth.toString().substring(1).toLowerCase() + " " + currentDayOfMonth;
            // [HH]:[MM]:[SS] [YEAR]
        }
        return currentDayMonth;
    }

    /**
     * @brief Give the current local day,
     * @note This function isn't define in the design and it's called at the start of the thread object.
     */
    private String catchYear() {
        String year = " ";

        // Android version higher than 8.0 (2017).
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {

            // Get the current time (hour, minute, second) in paris.
            LocalDateTime currentDateTime = LocalDateTime.now(TimeZone.getTimeZone(TIME_ZONE).toZoneId());

            // Get current year, month and day (in week and month format).
            year += String.valueOf(currentDateTime.getYear());
        }
        return year;
    }

    /**
     * @brief Give the current local day,
     * @note This function isn't define in the design and it's called at the start of the thread object.
     */
    private String catchTime() {
        String time = " ";

        // Android version higher than 8.0 (2017).
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {

            // Get the current time (hour, minute, second) in paris.
            LocalDateTime currentDateTime = LocalDateTime.now(TimeZone.getTimeZone(TIME_ZONE).toZoneId());

            // Format the current date and time
            DateTimeFormatter formatter = DateTimeFormatter.ofPattern("HH:mm:ss");
            String currentLocalTime = currentDateTime.format(formatter);

            // [HH]:[MM]:[SS] [YEAR]
            time +=  currentLocalTime;
        }
        return time;
    }
}
