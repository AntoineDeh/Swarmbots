package eseo.pfe.sc_ihm;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;


/**
 * @author Dehoux Antoine
 * @file MainActivity.java
 * @brief Activity managing the different fragments
 * @date 07/06/2023
 */
public class MainActivity extends AppCompatActivity {
    /*
     * VARIABLES LOCALES
     */

    /*
     * Functions
     */

    /**

     * @brief This method is called when the activity is starting.
     * @param[in] savedInstanceState If the activity is being re-initialized after previously being shut down then this
     *      Bundle contains the data it most recently supplied in onSaveInstanceState(Bundle). Otherwise it is null.
     **/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
}