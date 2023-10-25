/**
 * \file  HomeScreenViewModel.java
 * \version  0.1
 * \author Matéo R
 * \date May 18, 2023
 * \brief The purpose of this class is to evolve Views data.
 * It enables communication between GuiSecretary and UI.
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
import java.util.List;

import project.src.model.core.GUISecretary;
import project.src.model.core.Robot;

public class HomeScreenViewModel extends ViewModel {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief List of Robots.
     */
    private MutableLiveData<ArrayList<Robot>> robots;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Constructor of the class.
     */
    public HomeScreenViewModel() {
        this.robots = (MutableLiveData<ArrayList<Robot>>) GUISecretary.INSTANCE.getRobots();
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Get the robot list from the MutableLiveData robots.
     * @return robot
     */
    public ArrayList<Robot> getRobots() {
        ArrayList<Robot> robot = robots.getValue();
        return robot;
    }

    /**
     * @brief Get the size of the robot list from the MutableLiveData robots.
     * @return size of robot
     */
    public int getListSize() {
        ArrayList<Robot> robot = robots.getValue();
        if (robots != null) {
            return robot.size();
        }
        return 0;
    }

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

}
