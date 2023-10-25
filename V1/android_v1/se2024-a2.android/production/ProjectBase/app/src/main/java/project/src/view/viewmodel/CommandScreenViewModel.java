/**
 * \file  CommandScreenViewModel.java
 * \version  0.1
 * \author Matéo R
 * \date May 18, 2023
 * \brief The purpose of this class is to evolve data on Views.
 * It enables communication between GuiSecretary and UI, as well as Viewer and UI.
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

package project.src.view.viewmodel;

/* ----------------------  INCLUDES  ---------------------------------------- */

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import java.util.ArrayList;

import project.src.model.core.GUISecretary;
import project.src.model.core.Robot;

public class CommandScreenViewModel extends ViewModel {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief MutableLiveData of the robot list.
     */
    private MutableLiveData<ArrayList<Robot>> robots;

    /**
     * @brief MutableLiveData of the selected robot.
     */
    private static MutableLiveData<Robot> selectedRobot = new MutableLiveData<>();

    /**
     * @brief MutableLiveData of the robot which disconnect.
     */
    private MutableLiveData<Integer> idRobotDisconnection;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /* ----------------------  CONSTRUCTOR  ---------------------------- */

    /**
     * @brief Constructor of the class.
     */
    public CommandScreenViewModel(){
        this.robots = (MutableLiveData<ArrayList<Robot>>) GUISecretary.INSTANCE.getRobots();
        this.idRobotDisconnection = new MutableLiveData<Integer>();
        this.idRobotDisconnection.postValue(0);
    }

    /* ----------------------  PUBLIC FONCTIONS  ---------------------------- */

    /**
     * @brief Get the robots list.
     * @return robots
     */
    public ArrayList<Robot> getRobots() {
        this.robots = (MutableLiveData<ArrayList<Robot>>) GUISecretary.INSTANCE.getRobots();
        ArrayList<Robot> robot = this.robots.getValue();
        return robot;
    }

    /**
     * @brief To observe the robot list.
     * @return robots.
     */
    public LiveData<ArrayList<Robot>> getRobotList(){
        return this.robots;
    }

    /**
     * @brief To observe the selected robot.
     * @return robots.
     */
    public LiveData<Robot> getSelectedRobot(){
        return selectedRobot;
    }

    /**
     * @brief
     * @param selectedrobot to post in the mutable live data.
     */
    public static void setSelectedRobot(Robot selectedrobot){
        selectedRobot.postValue(selectedrobot);
    }

    /**
     * @brief Get the id of robot which disconnect.
     * @return id.
     */
    public Integer getIdRobotDeconnection(){
        this.idRobotDisconnection = (MutableLiveData<Integer>) GUISecretary.INSTANCE.getIdRobotConnection();
        return this.idRobotDisconnection.getValue();
    }

    /**
     * @brief To observe the robot which disconnect.
     * @return robots.
     */
    public LiveData<Integer> getRobotIdDeconnection(){
        return this.idRobotDisconnection;
    }

    /* ----------------------  PROTECTED FONCTIONS  ---------------------------- */

    /* ----------------------  PRIVATE FONCTIONS  ---------------------------- */

}
