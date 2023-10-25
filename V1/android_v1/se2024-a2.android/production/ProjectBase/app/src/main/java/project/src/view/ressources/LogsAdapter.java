/**
 * \file  LogsAdapter.java
 * \version  0.1
 * \author Matéo R
 * \date Jun 10, 2023
 * \brief LogsAdapter adapt a list of Logs to a view for the IHM logs or Robots logs.
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

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.List;

import project.src.R;

public class LogsAdapter extends BaseAdapter {

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /**
     * @brief Layout to put logs information.
     */
    private LayoutInflater inflater;

    /**
     * Logs list
     */
    private List<String> logsList;

    /**
     * @brief Context from the activity
     */
    private Context context;

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Constructor of the class
     * @param context Activity where is create.
     * @param logslist Logs list to show information.
     */
    public LogsAdapter(Context context, List<String> logslist){
        this.context = context;
        this.logsList = logslist;
        this.inflater = LayoutInflater.from(context);
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief To get the size of the logs list.
     * @return The size of the logsList.
     */
    @Override
    public int getCount() {
        return (this.logsList!=null)?this.logsList.size():0;
    }

    /**
     * @brief Return a log from the logs list.
     * @param i For each log of the logs list.
     * @return an log from the index of the logs list.
     */
    @Override
    public String getItem(int i) {
        return this.logsList.get(i);
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
     * @brief Set the view with information from the log.
     * @param i For each log of the logs list.
     * @param view to implement.
     * @return view of the log information.
     */
    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        view = inflater.inflate(R.layout.logs_screen_log, null);

        TextView textViewMessage = view.findViewById(R.id.textView3);

        String log = getItem(i);

        textViewMessage.setText(log);

        return view;
    }

    /* ----------------------  PROTECTED FUNCTIONS  ------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

}
