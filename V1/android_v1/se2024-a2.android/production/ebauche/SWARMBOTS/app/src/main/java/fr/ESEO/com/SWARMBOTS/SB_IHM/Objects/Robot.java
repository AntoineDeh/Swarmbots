package fr.ESEO.com.SWARMBOTS.SB_IHM.Objects;

public class Robot {
    //-----------------Attributs-----------------------------
    private int idRobot;
    private int operatingMode;

    public enum{
        CAMERA_DISABLE,
        RADAR,
        LED,
        BUZZER
    }
}
