/**
 * \file  HomeScreenActivity.java
 * \version  0.1
 * \author Matéo R, Julien N
 * \date Jun 7, 2023
 * \brief The HomeScreenActivity class corresponds to the application's home page controller.
 * This is the controller that will load all the elements required to use the application.
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

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.view.WindowManager;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;
import androidx.navigation.NavController;
import androidx.navigation.fragment.NavHostFragment;

import java.util.List;

import project.src.R;
import project.src.model.core.AddressReader;
import project.src.model.core.ConfigReader;
import project.src.model.core.LogsManager;
import project.src.model.core.Robot;
import project.src.model.core.Starter;
import project.src.view.viewmodel.HomeScreenViewModel;

public class HomeScreenActivity extends AppCompatActivity implements View.OnSystemUiVisibilityChangeListener {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief ViewModel of the HomeScreen.
     */
    private HomeScreenViewModel model;

    /**
     * @brief Controller to navigate between fragments in HomeScreen.
     */
    private NavController navController;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /* ----------------------  ON CREATE  ---------------------------- */

    /**
     * @brief Creation of the Activity HomeScreenActivity
     * @param savedInstanceState Life cycle parameter of the activity.
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_screen);

        ConfigReader.setContext(this);
        AddressReader.setContext(this);

        LogsManager.INSTANCE.create(this);

        Starter.INSTANCE.main();

        CountDownTimer mCountDownTimer = new CountDownTimer(5000, 1000) {
            @Override
            public void onTick(long millisUntilFinished) {
                // Ne rien faire pendant les ticks
            }
            @Override
            public void onFinish() {
                // Changer de fragment après 4 secondes
                changeFragment();
            }
        };
        mCountDownTimer.start();

        this.enterFullScreen();
        this.hideNavBar();
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Notify that the HomeScreen have started.
     */
    @Override
    protected void onStart() {
        super.onStart();
        Starter.INSTANCE.notifyHomeActivity(Starter.ScreenState.STARTED);
    }

    /**
     * @brief Notify that the HomeScreen have stopped.
     */
    @Override
    protected void onStop() {
        super.onStop();
        Starter.INSTANCE.notifyHomeActivity(Starter.ScreenState.STOPPED);
    }

    /**
     * @brief To hide the navigation bar when she appears
     * @param visibility state of the navigation bar
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

    /**
     * @brief Put the application in full screen.
     */
    protected void enterFullScreen(){
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
    }

    /**
     * @brief Hide the navigation bar when we enter in the activity
     */
    protected void hideNavBar(){
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
        // Ajouter un listener pour détecter lorsque la barre de navigation est réaffichée
        decorView.setOnSystemUiVisibilityChangeListener(this);
    }

    /**
     * @brief To navigate to the next fragment, to an error fragment if no robots are connected
     * or to a connected fragment if at least one robot is connected
     */
    protected void changeFragment(){
        NavHostFragment navHost =
                (NavHostFragment) getSupportFragmentManager().findFragmentById(R.id.fragmentContainerView2);
        this.navController = navHost.getNavController();
        this.model = new ViewModelProvider(this).get(HomeScreenViewModel.class);
        List<Robot> robotList = this.model.getRobots();
        int connectedRobot = 0;
        //If no robots connected, show the error screen.
        if (robotList == null) {
            this.navController.popBackStack(this.navController.getGraph().getStartDestinationId(), false);
            this.navController.navigate(R.id.homeScreenErrorConnectingRobots);
        } else {
            for (int i = 0; i < robotList.size(); i++) {
                if (robotList.get(i).getConnectionState() == Robot.ConnectionState.CONNECTED) {
                    connectedRobot++;
                }
            }
            if (connectedRobot > 0) {
                this.navController.popBackStack(this.navController.getGraph().getStartDestinationId(), false);
                this.navController.navigate(R.id.ecranPrincipal2Fragment);
            } else {
                this.navController.popBackStack(this.navController.getGraph().getStartDestinationId(), false);
                this.navController.navigate(R.id.homeScreenErrorConnectingRobots);
            }
        }
    }

}