/**
 * \file  HomeScreenRobotConnectedFragment.java
 * \version  0.1
 * \author Matéo R
 * \date May 18, 2023
 * \brief The HomeScreenRobotConnectedFragment class corresponds to the fragment
 * that displays the list of robots and their connection status with respect to
 * the application, as well as a message indicating that you should press the screen
 * to continue to the CommandScreenActivity activity.
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

import android.content.Intent;
import android.os.Bundle;

import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;

import java.util.List;

import project.src.R;
import project.src.model.core.GUI;
import project.src.model.core.Robot;
import project.src.view.activities.CommandScreenActivity;
import project.src.view.ressources.RobotAdapter;
import project.src.view.viewmodel.HomeScreenViewModel;


public class HomeScreenRobotConnectedFragment extends Fragment {
    private static final String TAG = "H";

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief ListView of the scrollView
     */
    private ListView listView;

    /**
     * @brief View of the fragment
     */
    private View view;

    /**
     * @brief ViewModel to know data about the RobotList
     */
    private HomeScreenViewModel model;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Construtor of the class
     */
    public HomeScreenRobotConnectedFragment() {
        // Required empty public constructor
    }

    /* ----------------------  ON CREATE VIEW ---------------------------- */

    /**
     * @brief Creation on the view of the fragment.
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

        this.view = inflater.inflate(R.layout.fragment_home_screen_robot_connected, container, false);
        this.initModelView();
        this.connectToCommandScreen();
        return this.view;
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /* ----------------------  PROTECTED FUNCTIONS  ------------------------------- */

    /**
     * @brief Allow to switch the screen, HomeScreen to CommandScreen after pressing
     * the screen HomeScreen.
     */
    protected void connectToCommandScreen(){
        TextView touch = view.findViewById(R.id.toucher_ici);
        touch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(), CommandScreenActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
                GUI.INSTANCE.commandRobot();
                startActivity(intent);
            }
        });
    }

    /**
     * @brief Insert informations about connected and disconnected robot on the fragment.
     */
    protected void initModelView(){
        this.listView = this.view.findViewById(R.id.list_robot);
        this.model = new ViewModelProvider(requireActivity()).get(HomeScreenViewModel.class);
        List<Robot> listRobot = this.model.getRobots();
        //Robot.ConnectionState state = listRobot.get(0).getConnectionState();
        this.listView.setAdapter(new RobotAdapter(getActivity(), listRobot));
    }

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

}