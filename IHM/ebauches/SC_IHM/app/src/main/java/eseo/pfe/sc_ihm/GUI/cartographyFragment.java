package eseo.pfe.sc_ihm.GUI;


import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.fragment.app.Fragment;
import eseo.pfe.sc_ihm.R;

public class cartographyFragment extends Fragment { //implements DopplerSensorManager.DopplerSensorDataListener {
    /*
     * VARIABLES LOCALES
     */

    private View root;

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

