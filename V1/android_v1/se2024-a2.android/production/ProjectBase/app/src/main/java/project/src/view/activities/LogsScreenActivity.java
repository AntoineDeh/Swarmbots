/**
 * \file  LogsScreenActivity.java
 * \version  0.1
 * \author julien
 * \date May 26, 2023
 * \brief The LogsScreenActivity class manages user interaction on the log screen.
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

package project.src.view.activities;

/* ----------------------  INCLUDES  ---------------------------------------- */

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ListView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

import java.util.ArrayList;

import project.src.R;
import project.src.model.core.GUI;
import project.src.model.core.LogsManager;
import project.src.model.core.Starter;
import project.src.view.ressources.LogsAdapter;
import project.src.view.viewmodel.LogsScreenViewModel;

public class LogsScreenActivity extends AppCompatActivity implements View.OnSystemUiVisibilityChangeListener {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */
    /**
     * @brief Tag of the ModelView.
     */
    private static final String TAG = "LogsScreen";

    /**
     * @brief Controller of the ModelView.
     */
    private LogsScreenViewModel model;

    /**
     * @brief List view for logs.
     */
    private ListView listView;

    /**
     * @brief List of logs to show on screen.
     */
    private ArrayList<String> listLogs = new ArrayList<String>(){};

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Creation of the Activity LogsScreenActivity
     * @param savedInstanceState Life cycle parameter of the activity.
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_logs_screen);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        this.model = new ViewModelProvider(this).get(LogsScreenViewModel.class);

        this.listView = findViewById(R.id.logs_list);
        this.enterFullScreen();
        this.hideNavBar();

    }

    /**
     * @brief Notify that the LogsScreen have started.
     */
    @Override
    protected void onStart() {
        super.onStart();
        Starter.INSTANCE.notifyLogsActivity(Starter.ScreenState.STARTED);
    }

    /**
     * @brief Notify that the LogsScreen have stopped.
     */
    @Override
    protected void onStop() {
        super.onStop();
        Starter.INSTANCE.notifyLogsActivity(Starter.ScreenState.STOPPED);
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
     * @brief To hide the navigation bar when she appears
     * @param visibility state of the navigation bar
     */
    @Override
    public void onSystemUiVisibilityChange(int visibility) {
        // Hide navigation bar immediately when visibility changes
        if ((visibility & View.SYSTEM_UI_FLAG_HIDE_NAVIGATION) == 0) {
            View decorView = getWindow().getDecorView();
            decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
        }
    }

    /**
     * @brief Function to come back on the CommandScreen.
     * @param view
     */
    public void onClickCommandScreenActivity(View view){
        Intent intent = new Intent(this, CommandScreenActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        GUI.INSTANCE.commandRobot();
        startActivity(intent);
    }

    /**
     * @brief Function to show the logs of IHM.
     * @param view
     */
    public void onClickButtonIHM(View view) {
        // Call the selection of gui logs.
        GUI.INSTANCE.selectLogs(0);

        Drawable drawable = getResources().getDrawable(R.drawable.logs_button_select);
        view.setBackground(drawable);

        //Set other buttons on unselected
        Button buttonRobot1 = findViewById(R.id.button2);
        buttonRobot1.setBackground(getResources().getDrawable(R.drawable.logs_button_unselect));
        Button buttonRobot2 = findViewById(R.id.button3);
        buttonRobot2.setBackground(getResources().getDrawable(R.drawable.logs_button_unselect));
        if (this.listLogs != null) this.listLogs.clear();
        this.listLogs = this.model.getLogs();
        LogsAdapter logsAdap = new LogsAdapter(this, this.listLogs);
        this.listView.setAdapter(logsAdap);
    }

    /**
     * @brief Function to show the logs of Robot1.
     * @param view
     */
    public void onClickButtonRobot1(View view) {
        // Call the selection of robot 1 logs.
        GUI.INSTANCE.selectLogs(1);

        Drawable drawable = getResources().getDrawable(R.drawable.logs_button_select);
        view.setBackground(drawable);

        //Set other buttons on unselected
        Button ihm = findViewById(R.id.button);
        ihm.setBackground(getResources().getDrawable(R.drawable.logs_button_unselect));
        Button buttonRobot2 = findViewById(R.id.button3);
        buttonRobot2.setBackground(getResources().getDrawable(R.drawable.logs_button_unselect));
        if (this.listLogs != null) this.listLogs.clear();
        this.listLogs = this.model.getLogs();
        LogsAdapter logsAdap = new LogsAdapter(this, this.listLogs);
        this.listView.setAdapter(logsAdap);
    }

    /**
     * @brief Function to show the logs of Robot2.
     * @param view
     */
    public void onClickButtonRobot2(View view) {
        // Call the selection of robot 2 logs.
        GUI.INSTANCE.selectLogs(2);

        Drawable drawable = getResources().getDrawable(R.drawable.logs_button_select);
        view.setBackground(drawable);

        //Set other buttons on unselected
        Button ihm = findViewById(R.id.button);
        ihm.setBackground(getResources().getDrawable(R.drawable.logs_button_unselect));
        Button buttonRobot1 = findViewById(R.id.button2);
        buttonRobot1.setBackground(getResources().getDrawable(R.drawable.logs_button_unselect));
        if (this.listLogs != null) this.listLogs.clear();
        this.listLogs = this.model.getLogs();
        LogsAdapter logsAdap = new LogsAdapter(this, this.listLogs);
        this.listView.setAdapter(logsAdap);
    }

    private void showPopup() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setCancelable(false);
        LayoutInflater inflater = getLayoutInflater();
        View customLayout = inflater.inflate(R.layout.pop_up, null);
        builder.setView(customLayout);

        AlertDialog dialog = builder.create();

        Button customButton = customLayout.findViewById(R.id.frame_25);
        customButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Perform any action you want when the custom button is clicked
                dialog.dismiss();
                hideNavBar();
            }
        });

        // Create and show the dialog
        dialog.show();
    }

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

}