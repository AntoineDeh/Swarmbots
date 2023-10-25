/**
 * \file  RobotAdapter.java
 * \version  0.1
 * \author Matéo R
 * \date May 18, 2023
 * \brief RobotAdapter adapt a list of Robot to a view for each Robot of the list.
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

package project.src.view.ressources;

/* ----------------------  INCLUDES  ---------------------------------------- */

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.core.content.ContextCompat;

import java.util.List;

import project.src.R;
import project.src.model.core.Robot;

public class RobotAdapter extends BaseAdapter {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief Layout to put robot information.
     */
    private LayoutInflater inflater;

    /**
     * @brief List of robot of the class.
     */
    private List<Robot> robotList;

    /**
     * @brief Context from an activity.
     */
    private Context context;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Constructor of the class.
     * @param context Activity where is create.
     * @param robot Robot list to show their information.
     */
    public RobotAdapter(Context context, List<Robot> robot){
        this.context = context;
        this.robotList = robot;
        this.inflater = LayoutInflater.from(context);
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Return the size of the robot list.
     * @return the size of the list.
     */
    @Override
    public int getCount() {
        return (this.robotList!=null)?this.robotList.size():0;
    }

    /**
     * @brief Return a robot from the robot list.
     * @param i For each robot of the robot list.
     * @return an robot from the index of the robot list.
     */
    @Override
    public Robot getItem(int i) {
        return this.robotList.get(i);
    }

    /**
     * @brief Not implemented.
     * @param i For each robot of the robot list.
     * @return
     */
    @Override
    public long getItemId(int i) {
        return 0;
    }

    /**
     * @brief Set the view with information from the robot.
     * @param i For each robot of the robot list.
     * @param view to implement.
     * @return view of the robot information.
     */
    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        view = this.inflater.inflate(R.layout.home_screen_robot_connected_indicator, null);

        TextView idRobot = view.findViewById(R.id.textViewId);
        TextView connectionRobot = view.findViewById(R.id.robot_status);
        ImageView imageConnectionRobot = view.findViewById(R.id.robot_circle);

        Robot robot = getItem(i);
        String id = String.valueOf(robot.getId());
        Robot.ConnectionState robotConnected = robot.getConnectionState();

        idRobot.setText("Robot"+id);
        if (robotConnected == Robot.ConnectionState.CONNECTED){
            connectionRobot.setText(R.string.connected);
            connectionRobot.setTextColor(ContextCompat.getColor(this.context, R.color.connected));
            imageConnectionRobot.setImageResource(R.drawable.home_circle_connected);
        } else {
            connectionRobot.setText(R.string.disconnected);
            connectionRobot.setTextColor(ContextCompat.getColor(this.context, R.color.disconnected));
            imageConnectionRobot.setImageResource(R.drawable.home_circle_disconnected);
        }
        return view;
    }

    /* ----------------------  PROTECTED FUNCTIONS  ------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
}
