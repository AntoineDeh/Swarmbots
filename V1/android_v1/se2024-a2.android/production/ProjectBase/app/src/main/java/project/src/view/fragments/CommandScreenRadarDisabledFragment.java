/**
 * \file  CommandScreenRadarDisabledFragment.java
 * \version  0.1
 * \author Matéo R
 * \date Jun 5, 2023
 * \brief The CommandScreenRadarDisabledFragment class corresponds to the fragment
 * used to hide the radar indicator when the radar device is disabled or the robot
 * is disconnected.
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

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import project.src.R;

public class CommandScreenRadarDisabledFragment extends Fragment {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /* ----------------------  CONSTRUCTOR  ---------------------------- */

    /**
     * @brief Constructor of the class.
     */
    public CommandScreenRadarDisabledFragment() {
        // Required empty public constructor
    }

    /* ----------------------  ON CREATE VIEW  ---------------------------- */

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
     * @return view the fragment.
     */
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_command_screen_radar_disabled, container, false);
    }

    /* ----------------------  PUBLIC FUNCTIONS  ------------------------------- */

    /* ----------------------  PROTECTED FUNCTIONS  ------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
}