/**
 * \file  ConfigReader.java
 * \version  0.7
 * \author julien
 * \date May 26, 2023
 * \brief The configReader class takes care of creating the list of robots from a configuration file.
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
 *
 * Copyright (c) 2002, JSON.org
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * \copyright JSON.org
 */

/* ----------------------  PACKAGE  ---------------------------------------- */

package project.src.model.core;

/* ----------------------  INCLUDES  ---------------------------------------- */

import android.content.Context;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

import project.src.model.LogLevel;

public class ConfigReader {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief Context of the class.
     */
    private static Context context;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief Config of the robot sorted by FileConfig.json order.
     */
    private static final ArrayList<String> config = new ArrayList<>();

    /**
     * @brief List of the robot extracted from FileConfig.json.
     */
    private static final ArrayList<Robot> robotList = new ArrayList<>();

    /* ----------------------  PUBLIC VARIABLES  ---------------------------- */

    /**
     * @brief Transformation format for the config file reading.
     */
    public static final String transformationFormat ="UTF-8";

    /**
     * @brief Reading the field "robots".
     */
    public static final String targetField = "robots";

    /**
     * @brief Reading the line "idRobot".
     */
    public static String idRobot = "idRobot";

    /**
     * @brief Reading the line "ipAddress".
     */
    public static final String ipAddress = "ipAddress";

    /**
     * @brief Reading the line "port".
     */
    public static final String port = "port";

    /**
     * @brief Reading the line "macAddress".
     */
    public static final String macAddress = "macAddress";

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Reading FileConfig.json in order to build the robotList.
     * @return The robotList list from FileConfig.json.
     * @param jsonFile Name of JSON file.
     */
    public static ArrayList<Robot> getRobotListFromConfig(String jsonFile) {
        try {
            //File reading.
            InputStream robotsConfigFile = context.getAssets().open(jsonFile);
            int size = robotsConfigFile.available();
            byte[] buffer = new byte[size];
            robotsConfigFile.read(buffer);
            robotsConfigFile.close();
            String json = new String(buffer, ConfigReader.transformationFormat);

            //Asking infos.
            JSONObject jsonObject = new JSONObject(json);
            JSONArray robotsArray = jsonObject.getJSONArray(ConfigReader.targetField);
            for (int i = 0; i < robotsArray.length(); i++) {
                JSONObject configRobots = robotsArray.getJSONObject(i);

                //Extracting infos.
                int idRobotRead = configRobots.getInt(ConfigReader.idRobot);
                String ipAddressRead = configRobots.getString(ConfigReader.ipAddress);
                int portRead = configRobots.getInt(ConfigReader.port);
                String macAddressRead = configRobots.getString(ConfigReader.macAddress);

                //Config creation.
                ConfigReader.config.add(ConfigReader.idRobot + ": " + idRobotRead + "\n" +
                        ConfigReader.ipAddress + ": " + ipAddressRead + "\n" +
                        ConfigReader.port + ": " + portRead + "\n" +
                        ConfigReader.macAddress + ": " + macAddressRead);
                parseConfig(ConfigReader.config);
            }

        } catch (IOException | JSONException e) {
            //Errors management.
            e.printStackTrace();
        }
        return robotList;
    }

    /**
     * @brief Constructor used to retrieve the context of the class.
     */
    public static void setContext(Context context) {
        ConfigReader.context = context;
    }

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

    /**
     * @brief robotList building
     * @param config : Configuration to set for robots
     */
    private static void parseConfig(ArrayList<String> config){
        if(config.size()>0){
            //Extracting robot.
            String lastRobot = config.get(config.size()-1);
            String[] robotElements = lastRobot.split("\n");

            //Extracting idRobot.
            String nameIdRobot = robotElements[0];
            String[] idRobotSplitter = nameIdRobot.split(": ");
            int idRobot = Integer.parseInt(idRobotSplitter[1]);

            //Extracting ipAddress.
            String nameIpAddress = robotElements[1];
            String[] ipAddressSplitter = nameIpAddress.split(": ");
            String ipAddress = ipAddressSplitter[1];

            //Extracting port.
            String namePort = robotElements[2];
            String[] portSplitter = namePort.split(": ");
            int port = Integer.parseInt(portSplitter[1]);

            //Extracting macAddress.
            String nameMacAddress = robotElements[3];
            String[] macAddressSplitter = nameMacAddress.split(": ");
            String macAddress = macAddressSplitter[1];

            //robotList construction.
            Robot newRobot = new Robot(idRobot, ipAddress, macAddress, port);
            robotList.add(newRobot);

        }else{
            //Error Management
            LogsManager.INSTANCE.log(LogLevel.DEBUG,"Empty config.");
        }
    }
}
