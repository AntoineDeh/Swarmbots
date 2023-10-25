/**
 * \file  CommandScreenSwitchFragment.java
 * \version  0.1
 * \author Mahery FONG
 * \date May 15, 2023
 * \brief The CommandScreenSwitchFragment class corresponds to the fragment
 * displaying the switches used to manage the robot's peripherals.
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

package project.src.view.fragments;

/* ----------------------  INCLUDES  ---------------------------------------- */

import android.os.Bundle;

import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;

import android.os.CountDownTimer;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import java.util.List;

import project.src.R;
import project.src.model.PeriphState;
import project.src.model.core.GUI;
import project.src.model.core.OperatingMode;
import project.src.model.core.Robot;
import project.src.view.viewmodel.CommandScreenViewModel;

public class CommandScreenSwitchFragment extends Fragment {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    private static final String TAG = "CommandScreenActivity";

    /**
     * @brief View model of the command screen
     */
    private CommandScreenViewModel model;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief Robot list
     */
    private List<Robot> robotList;

    /**
     * Selected robot
     */
    private Robot selectedRobot;

    /* ----------------------  CONSTRUCTOR  ---------------------------- */

    /**
     * @brief Constructor of the class.
     */
    public CommandScreenSwitchFragment() {
        // Required empty public constructor
    }

    /* ----------------------  ON CREATE VIEW  ---------------------------- */

    /**
     * @brief Creation on the view of the fragment, and listeners of switchs.
     * @param inflater The LayoutInflater object that can be used to inflate
     * any views in the fragment,
     * @param container If non-null, this is the parent view that the fragment's
     * UI should be attached to.  The fragment should not add the view itself,
     * but this can be used to generate the LayoutParams of the view.
     * @param savedInstanceState If non-null, this fragment is being re-constructed
     * from a previous saved state as given here.
     *
     * @return view of the fragment.
     */
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        this.model = new ViewModelProvider(getActivity()).get(CommandScreenViewModel.class);

        this.robotList = this.model.getRobots();

        View view = inflater.inflate(R.layout.fragment_command_screen_switch, container, false);

        //NavController for the fragment of camera
        NavController controllerCamera1 = Navigation.findNavController(getActivity(), R.id.fragmentContainerView);
        NavController controllerCamera2 = Navigation.findNavController(getActivity(), R.id.fragmentContainerView5);

        // Switch of camera and change it in the stats of the robot.
        Switch switch_camera = view.findViewById(R.id.switch_camera);
        // Switch of radar and change it in the stats of the robot.
        Switch switch_radar = view.findViewById(R.id.switch_radar);
        // Switch of buzzer and change it in the stats of the robot.
        Switch switch_buzzer = view.findViewById(R.id.switch_buzzer);
        // Switch of led and change it in the stats of the robot.
        Switch switch_led = view.findViewById(R.id.switch_led);

        this.robotList = this.model.getRobots();

        //To set the display of switchs
        this.model.getSelectedRobot().observe(getActivity(),
                robot -> {
                    this.selectedRobot = this.model.getSelectedRobot().getValue();

                    if (this.selectedRobot.getOperatingMode().getCamera() == PeriphState.ENABLED.ordinal()){
                        switch_camera.setChecked(true);
                    } else {
                        switch_camera.setChecked(false);
                    }
                    if (this.selectedRobot.getOperatingMode().getRadar() == PeriphState.ENABLED.ordinal()){
                        switch_radar.setChecked(true);
                    } else {
                        switch_radar.setChecked(false);
                    }
                    if (this.selectedRobot.getOperatingMode().getBuzzer() == PeriphState.ENABLED.ordinal()){
                        switch_buzzer.setChecked(true);
                    } else {
                        switch_buzzer.setChecked(false);
                    }
                    if (this.selectedRobot.getOperatingMode().getLeds() == PeriphState.ENABLED.ordinal()){
                        switch_led.setChecked(true);
                    } else {
                        switch_led.setChecked(false);
                    }
        });

        switch_camera.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(switch_camera.isChecked()){
                    GUI.INSTANCE.editRobotMode(OperatingMode.PeriphToChange.CAMERA, PeriphState.ENABLED);
                    robotList = model.getRobots();
                    if (robotList.get(0).getSelectionState() == Robot.SelectionState.SELECTED){
                        controllerCamera1.navigate(R.id.commandScreenBlankCameraFragment);
                    } else {
                        controllerCamera2.navigate(R.id.commandScreenCameraRobot2Fragment);
                    }
                } else {
                    GUI.INSTANCE.editRobotMode(OperatingMode.PeriphToChange.CAMERA, PeriphState.DISABLED);
                    robotList = model.getRobots();
                    if (robotList.get(0).getSelectionState() == Robot.SelectionState.SELECTED){
                        controllerCamera1.navigate(R.id.commandScreenCameraOFFFragment);
                    } else {
                        controllerCamera2.navigate(R.id.commandScreenCameraOFFFragment);
                    }
                }
            }
        });

        switch_radar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(switch_radar.isChecked()){
                    GUI.INSTANCE.editRobotMode(OperatingMode.PeriphToChange.RADAR, PeriphState.ENABLED);
                } else {
                    GUI.INSTANCE.editRobotMode(OperatingMode.PeriphToChange.RADAR, PeriphState.DISABLED);
                }
            }
        });

        switch_buzzer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(switch_buzzer.isChecked()){
                    GUI.INSTANCE.editRobotMode(OperatingMode.PeriphToChange.BUZZER, PeriphState.ENABLED);
                } else {
                    GUI.INSTANCE.editRobotMode(OperatingMode.PeriphToChange.BUZZER, PeriphState.DISABLED);
                }
            }
        });

        switch_led.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                if(switch_led.isChecked()){
                    GUI.INSTANCE.editRobotMode(OperatingMode.PeriphToChange.LEDS, PeriphState.ENABLED);
                } else {
                    GUI.INSTANCE.editRobotMode(OperatingMode.PeriphToChange.LEDS, PeriphState.DISABLED);
                }
            }
        });

        return view;
    }

    /* ----------------------  PUBLIC FUNCTIONS  ------------------------------- */

    /* ----------------------  PROTECTED FUNCTIONS  ------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
}