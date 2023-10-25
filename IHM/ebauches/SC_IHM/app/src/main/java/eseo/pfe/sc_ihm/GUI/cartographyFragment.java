package eseo.pfe.sc_ihm.GUI;


import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.Fragment;
import eseo.pfe.sc_ihm.R;

public class cartographyFragment extends Fragment { //implements DopplerSensorManager.DopplerSensorDataListener {
    /*
     * VARIABLES LOCALES
     */

    private View root;
    private AlertDialog alertDialog;

    /*
     * Functions
     */

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        root = inflater.inflate(R.layout.fragment_cartography, container, false);


        return root;
    }






}

