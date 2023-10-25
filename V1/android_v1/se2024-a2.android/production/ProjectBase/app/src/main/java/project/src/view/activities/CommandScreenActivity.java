/**
 * \file  CommandScreenActivity.java
 * \version  0.1
 * \author Matéo R
 * \date May 18, 2023
 * \brief The CommandScreenActivity class corresponds to the control page for the various robots
 * and their camera feeds.
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

package project.src.view.activities;

/* ----------------------  INCLUDES  ---------------------------------------- */

import androidx.appcompat.app.AlertDialog;
/* ----------------------  INCLUDES  ---------------------------------------- */

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;
import androidx.navigation.NavController;
import androidx.navigation.fragment.NavHostFragment;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

import project.src.R;
import project.src.model.Command;
import project.src.model.PeriphState;
import project.src.model.core.GUI;
import project.src.model.core.GUISecretary;
import project.src.model.core.Starter;
import project.src.model.core.Robot;
import project.src.view.ressources.JoystickView;
import project.src.view.viewmodel.CommandScreenViewModel;

public class CommandScreenActivity extends AppCompatActivity implements View.OnSystemUiVisibilityChangeListener{

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief ViewModel of the Activity.
     */
    private CommandScreenViewModel viewModel;

    /**
     * @brief RelativeLayout to indicate the selection of the first robot.
     */
    private RelativeLayout selectScreenFirstRobot;

    /**
     * @brief RelativeLayout to indicate the selection of the second robot.
     */
    private RelativeLayout selectScreenSecondRobot;

    /**
     * @brief RelativeLayout of the buttons to control the robot movement.
     */
    private RelativeLayout commands;

    /**
     * @brief Navigation controller for switchs.
     */
    private NavController navControllerSwitch;

    /**
     * @brief List of navcontroller for camera
     */
    private List<NavController> cameraListController = new ArrayList<>();

    /**
     * @brief List of navcontroller for radar
     */
    private List<NavController> radarListController = new ArrayList<>();

    /**
     * @brief List of navcontroller for information about robots
     */
    private List<View> viewListInformation = new ArrayList<>();

    /**
     * @brief Buttons to pilot the selected robot.
     */
    private Button btnForward, btnBackward, btnRight, btnLeft;

    /**
     * @brief Robot list from the ViewModel.
     */
    private List<Robot> robotList;

    /**
     * @brief Integer to know which robot is disconnected
     */
    private int idDisconnected = 0;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /* ----------------------  ON CREATE  ---------------------------- */

    /**
     * @brief Creation of the activity CommandScreenActivity.
     * @param savedInstanceState Life cycle parameter of the activity.
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_command_screen);

        this.viewModel = new ViewModelProvider(this).get(CommandScreenViewModel.class);

        this.enterFullScreen();
        this.hideNavBar();
        this.initFragments();
        this.initCmdBtns();
    }

    /**
     * @brief Notify that the CommandScreen have started.
     */
    @Override
    protected void onStart() {
        super.onStart();
        Starter.INSTANCE.notifyCommandActivity(Starter.ScreenState.STARTED);
        this.robotList = this.viewModel.getRobots();
    }

    /**
     * @brief Notify that the CommandScreen have stopped.
     */
    @Override
    protected void onStop() {
        super.onStop();
        Starter.INSTANCE.notifyCommandActivity(Starter.ScreenState.STOPPED);
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief To hide the navigation bar when she appears.
     * @param visibility state of the navigation bar.
     */
    @Override
    public void onSystemUiVisibilityChange(int visibility) {
        // Masquer la barre de navigation immédiatement lorsque la visibilité change
        if ((visibility & View.SYSTEM_UI_FLAG_HIDE_NAVIGATION) == 0) {
            View decorView = getWindow().getDecorView();
            decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
        }
    }

    /* ----------------------  PROTECTED FUNCTIONS  ------------------------------- */

    /**
     * @brief Hide the navigation bar when we enter in the activity
     */
    protected void hideNavBar() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
        // Ajouter un listener pour détecter lorsque la barre de navigation est réaffichée
        decorView.setOnSystemUiVisibilityChangeListener(this);
    }

    /**
     * @brief Show the screen of the app in fullscreen
     */
    protected void enterFullScreen() {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
    }

    /**
     * @brief Initialization of the screen :
     * Hide cache on the screen and turn every navigation fragment in the right configuration
     */
    protected void initFragments() {
        // Hide selection and commands
        this.selectScreenFirstRobot = findViewById(R.id.cache_frame5);
        this.selectScreenFirstRobot.setVisibility(View.GONE);
        this.selectScreenSecondRobot = findViewById(R.id.cache_frame6);
        this.selectScreenSecondRobot.setVisibility(View.GONE);
        this.commands = findViewById(R.id.commandBtns_circle);
        this.commands.setVisibility(View.GONE);

        //Search and set all fragments
        NavHostFragment navHostSwitch =
                (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.fragmentContainerView2);
        this.navControllerSwitch = navHostSwitch.getNavController();

        NavHostFragment navHostFragmentRobot1 =
                (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.fragmentContainerView);
        NavController navControllerRobot1 = navHostFragmentRobot1.getNavController();

        NavHostFragment navHostFragmentRobot2 =
                (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.fragmentContainerView5);
        NavController navControllerRobot2 = navHostFragmentRobot2.getNavController();

        NavHostFragment navHostFragmentRadar1 =
                (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.fragmentContainerView3);
        NavController navControllerRadar1 = navHostFragmentRadar1.getNavController();

        NavHostFragment navHostFragmentRadar2 =
                (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.fragmentContainerView4);
        NavController navControllerRadar2 = navHostFragmentRadar2.getNavController();

        View robot1Id = findViewById(R.id.robot1Id);
        View robot2Id = findViewById(R.id.robot2Id);

        // Add camera navcontroller in the table
        this.cameraListController.add(navControllerRobot1);
        this.cameraListController.add(navControllerRobot2);

        // Add radar navcontroller in the table
        this.radarListController.add(navControllerRadar1);
        this.radarListController.add(navControllerRadar2);

        // For robot information on the screen.
        this.viewListInformation.add(robot1Id);
        this.viewListInformation.add(robot2Id);

        this.viewModel.getRobotList().observe(this,
            robot -> {
                this.setViewInit();
        });

        //Set the fragments camera
        this.robotList = this.viewModel.getRobots();
        if(this.robotList.get(0).getConnectionState() == Robot.ConnectionState.DISCONNECTED ||
                this.robotList.get(0).getOperatingMode().getCamera() == PeriphState.DISABLED.ordinal()){
            this.cameraListController.get(0).popBackStack(this.cameraListController.get(0).getGraph().getStartDestinationId(), false);
            this.cameraListController.get(0).navigate(R.id.commandScreenCameraOFFFragment);
        } else {
            this.cameraListController.get(0).popBackStack(this.cameraListController.get(0).getGraph().getStartDestinationId(), false);

            this.cameraListController.get(0).navigate(R.id.commandScreenBlankCameraFragment);

        }
        if(this.robotList.size()>1){
            if(this.robotList.get(1).getConnectionState() == Robot.ConnectionState.DISCONNECTED ||
                    this.robotList.get(1).getOperatingMode().getCamera() == PeriphState.DISABLED.ordinal()){
                this.cameraListController.get(1).popBackStack(this.cameraListController.get(1).getGraph().getStartDestinationId(), false);
                this.cameraListController.get(1).navigate(R.id.commandScreenCameraOFFFragment);
            } else {
                this.cameraListController.get(1).popBackStack(this.cameraListController.get(1).getGraph().getStartDestinationId(), false);

                this.cameraListController.get(1).navigate(R.id.commandScreenCameraRobot2Fragment);

            }
        }

        //
        this.viewModel.getRobotIdDeconnection().observe(this,
                id -> {
                    //this.idDisconnected = this.viewModel.getIdRobotDeconnection();
                    this.idDisconnected = 0;
                    if(this.idDisconnected != 0) {
                        this.showPopup(this.idDisconnected);
                    }
        });
    }

    /**
     * @brief Initialize fragments radars and information about robot.
     */
    protected void setViewInit(){
        this.robotList = this.viewModel.getRobots();

        // Loop to initialize fragment.
        for(int i = 0; i < this.robotList.size(); i++){

            // Radar
            if(this.robotList.get(i).getOperatingMode().getRadar() == PeriphState.DISABLED.ordinal() ||
                    this.robotList.get(i).getConnectionState() == Robot.ConnectionState.DISCONNECTED){
                this.radarListController.get(i).popBackStack(this.cameraListController.get(i).getGraph().getStartDestinationId(), false);
                this.radarListController.get(i).navigate(R.id.commandScreenRadarDisabledFragment);
            } else if (this.robotList.get(i).getObstacleDetection() == Robot.ObstacleDetection.DETECTED){
                this.radarListController.get(i).popBackStack(this.cameraListController.get(i).getGraph().getStartDestinationId(), false);
                this.radarListController.get(i).navigate((R.id.commandScreenRadarOffFragment));
            } else {
                this.radarListController.get(i).popBackStack(this.cameraListController.get(i).getGraph().getStartDestinationId(), false);
                this.radarListController.get(i).navigate((R.id.commandScreenRadarOnFragment));
            }
            // Information
            TextView textIdRobot = this.viewListInformation.get(i).findViewById(R.id.textViewId2);
            TextView connectionRobot = this.viewListInformation.get(i).findViewById(R.id.robot_status2);
            ImageView imageConnectionRobot= this.viewListInformation.get(i).findViewById(R.id.robot_circle2);

            if(this.robotList.get(i).getConnectionState() == Robot.ConnectionState.CONNECTED){
                textIdRobot.setText("Robot" + this.robotList.get(i).getId());
                connectionRobot.setText(R.string.connected);
                imageConnectionRobot.setImageResource(R.drawable.home_circle_connected);
            } else {
                textIdRobot.setText("Robot" + this.robotList.get(i).getId());
                connectionRobot.setText(R.string.disconnected);
                imageConnectionRobot.setImageResource(R.drawable.home_circle_disconnected);
            }
        }
    }

    /**********************************************LISTENERS*****************************************************/

    /**
     * @brief Permit to come back to the HomeScreenActivity.
     * @param view Button.
     */
    public void onClickHomeScreenActivity(View view) {
        // Hide selection and commands
        this.selectScreenFirstRobot = findViewById(R.id.cache_frame5);
        this.selectScreenFirstRobot.setVisibility(View.GONE);
        this.selectScreenSecondRobot = findViewById(R.id.cache_frame6);
        this.selectScreenSecondRobot.setVisibility(View.GONE);
        this.commands = findViewById(R.id.commandBtns_circle);
        this.commands.setVisibility(View.GONE);

        //Hide switch
        this.navControllerSwitch.popBackStack(this.navControllerSwitch.getGraph().getStartDestinationId(), false);
        this.navControllerSwitch.navigate(R.id.commandScreenBlankSwitchFragment);

        Intent intent = new Intent(this, HomeScreenActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        GUI.INSTANCE.returnHome();
        startActivity(intent);
    }

    /**
     * @brief Permit to go to the LogsScreenActivity.
     * @param view Button.
     */
    public void onClickLogsScreenActivity(View view) {
        // Hide selection and commands
        this.selectScreenFirstRobot = findViewById(R.id.cache_frame5);
        this.selectScreenFirstRobot.setVisibility(View.GONE);
        this.selectScreenSecondRobot = findViewById(R.id.cache_frame6);
        this.selectScreenSecondRobot.setVisibility(View.GONE);
        this.commands = findViewById(R.id.commandBtns_circle);
        this.commands.setVisibility(View.GONE);

        //Hide switch
        this.navControllerSwitch.popBackStack(this.navControllerSwitch.getGraph().getStartDestinationId(), false);
        this.navControllerSwitch.navigate(R.id.commandScreenBlankSwitchFragment);

        Intent intent = new Intent(this, LogsScreenActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        GUI.INSTANCE.consultLogs();
        startActivity(intent);
    }

    /**
     * @brief Permit to show the cache of the selected robot. Here to show or hide the cache and to
     * put the selected robot on the first robot.
     * @param view of the camera of the first robot.
     */
    public void onClickScreenCameraLeft(View view) {
        if(this.robotList.get(0).getConnectionState() == Robot.ConnectionState.CONNECTED){
            if(this.selectScreenFirstRobot.getVisibility() != View.VISIBLE){
                if (this.selectScreenSecondRobot.getVisibility() == View.VISIBLE){
                    this.selectScreenSecondRobot.setVisibility(View.GONE);
                }
                this.selectScreenFirstRobot.setVisibility(View.VISIBLE);
                this.commands.setVisibility(View.VISIBLE);
                this.navControllerSwitch.popBackStack(this.navControllerSwitch.getGraph().getStartDestinationId(), false);
                this.navControllerSwitch.navigate(R.id.commandScreenSwitchFragment);
                GUI.INSTANCE.selectRobot(1, Robot.SelectionState.SELECTED);
            }
        }
    }

    /**
     * @brief Permit to show the cache of the selected robot. Here to show or hide the cache and to
     * put the selected robot on the second robot.
     * @param view of the camera of the second robot.
     */
    public void onClickScreenCameraRight(View view) {
        if(this.robotList.get(1).getConnectionState() == Robot.ConnectionState.CONNECTED){
            if(this.selectScreenSecondRobot.getVisibility() != View.VISIBLE){
                if (this.selectScreenFirstRobot.getVisibility() == View.VISIBLE){
                    this.selectScreenFirstRobot.setVisibility(View.GONE);
                }
                this.selectScreenSecondRobot.setVisibility(View.VISIBLE);
                this.commands.setVisibility(View.VISIBLE);
                this.navControllerSwitch.popBackStack(this.navControllerSwitch.getGraph().getStartDestinationId(), false);
                this.navControllerSwitch.navigate(R.id.commandScreenSwitchFragment);
                GUI.INSTANCE.selectRobot(2, Robot.SelectionState.SELECTED);
            }
        }
    }

    /**
     * @brief Creation on button to pilot the selected robot.
     */
    @SuppressLint("ClickableViewAccessibility")
    public void initCmdBtns(){
        this.btnForward = findViewById(R.id.btnForward);
        this.btnBackward = findViewById(R.id.btnBackward);
        this.btnRight = findViewById(R.id.btnRight);
        this.btnLeft = findViewById(R.id.btnLeft);
        this.btnForward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GUI.INSTANCE.moveRobot(Command.FORWARD);
                    btnForward.setBackground(getResources().getDrawable(R.drawable.arrow_up_blue));
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GUI.INSTANCE.moveRobot(Command.STOP);
                    btnForward.setBackground(getResources().getDrawable(R.drawable.arrow_up));
                }
                return true;
            }
        });

        this.btnBackward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GUI.INSTANCE.moveRobot(Command.BACKWARD);
                    btnBackward.setBackground(getResources().getDrawable(R.drawable.arrow_up_blue));
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GUI.INSTANCE.moveRobot(Command.STOP);
                    btnBackward.setBackground(getResources().getDrawable(R.drawable.arrow_up));
                }
                return true;
            }
        });

        this.btnRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GUI.INSTANCE.moveRobot(Command.RIGHT);
                    btnRight.setBackground(getResources().getDrawable(R.drawable.arrow_up_blue));
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GUI.INSTANCE.moveRobot(Command.STOP);
                    btnRight.setBackground(getResources().getDrawable(R.drawable.arrow_up));
                }
                return true;
            }
        });

        this.btnLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GUI.INSTANCE.moveRobot(Command.LEFT);
                    btnLeft.setBackground(getResources().getDrawable(R.drawable.arrow_up_blue));
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GUI.INSTANCE.moveRobot(Command.STOP);
                    btnLeft.setBackground(getResources().getDrawable(R.drawable.arrow_up));
                }
                return true;
            }
        });
    }

    /**
     * @brief Display a pop-up if the application receives a memory alert.
     */
    private void showPopup(int idRobot) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        String message = "ERREUR DECONNECTION";
        builder.setCancelable(false);
        LayoutInflater inflater = getLayoutInflater();
        View customLayout = inflater.inflate(R.layout.pop_up, null);

        TextView criticite = customLayout.findViewById(R.id.criticite_);
        criticite.setText(message);
        TextView explication = customLayout.findViewById(R.id.message_err);
        explication.setText("Robot" + idRobot + " est déconnecté");
        builder.setView(customLayout);

        AlertDialog dialog = builder.create();

        Button customButton = customLayout.findViewById(R.id.frame_25);
        customButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Perform any action you want when the custom button is clicked
                GUISecretary.INSTANCE.validate();
                dialog.dismiss();
                hideNavBar();
            }
        });

        // Create and show the dialog
        dialog.show();
    }
}