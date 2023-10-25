package eseo.pfe.sc_ihm;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ImageView;


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
    private Button cartographyButton;
    private Button trajectoryButton;
    private Button resetButton;
    private Button stopButton;
    private ImageView quitButton;
    private AlertDialog alertDialog;
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

        cartographyButton = findViewById(R.id.CartographyID);
        cartographyButton.setOnClickListener(v -> {
            showPopup("Cartographie en cours");
        });

        trajectoryButton = findViewById(R.id.trajectoireID);
        trajectoryButton.setOnClickListener(v -> {
            showPopup("Calcul trajectoire en cours");
        });

        resetButton = findViewById(R.id.resetID);
        resetButton.setOnClickListener(v -> {
            showPopup("RESET en cours");
        });

        stopButton = findViewById(R.id.stopID);
        stopButton.setOnClickListener(v -> {
            showPopup("STOP !!!");
        });

        quitButton = findViewById(R.id.quitID);
        quitButton.setOnClickListener(v -> {
            showPopup("quitter l'application");
        });
    }

    private void showPopup(String message1) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage(message1)
                .setCancelable(false)
                .setPositiveButton("OK", (dialog, id) -> dialog.dismiss());

        alertDialog = builder.create();
        alertDialog.show();
    }
}