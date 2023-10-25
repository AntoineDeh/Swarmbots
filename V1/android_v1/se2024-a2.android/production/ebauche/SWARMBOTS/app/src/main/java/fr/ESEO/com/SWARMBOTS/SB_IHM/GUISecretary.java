package fr.ESEO.com.SWARMBOTS.SB_IHM;

import android.system.ErrnoException;

import fr.ESEO.com.SWARMBOTS.SB_IHM.Objects.Robot;
/*
    A besoin du bon fonctionnement de la communication TCP/IP pour écrire les fonctions restantes
    Peut écrire la MAE
 */
public class GUISecretary {
    //-----------------Attributs-----------------------------
    private java.util.List<Robot> robotList;
    private Robot selectedRobot;
    private Robot currentRobot;
    private int timeOutConnection;
    private int getTimeOutDisconnection;
    private java.util.List<Robot> connectedRobot;
    private java.util.List<Robot> disconnectedRobot;

    //-----------------Méthodes-----------------------------
    //-------Public----------
    public void askInit() {
        return;
    }
    public java.util.List<Robot> getRobotList(){
        return robotList;
    }

    public void setSelectedRobot(Robot selectedRobot){
        this.selectedRobot = selectedRobot;
    }

    public void setMode(int idRobot, int operatingMode){
        return;
    }

    public void changeMode(Robot selectedRobot, int operatingMode){
        return;
    }

    public void disconnectedOk(int idRobot){
        return;
    }

    public void ackConnection(int idRobot){
        return;
    }

    public void disconnectRobots(){
        //TODO : insérer la partie déconnexion réelle
        for(int i = 0 ; i < connectedRobot.size() ; i++){
            updateDisconnectedRobot(connectedRobot.get(i), this.robotList);
        }
    }

    public void setModeInit(){
        return;
    }

    public void notifyVarReady(int idRobot){
        return;
    }

    public void notifyDisconnectedRobot(int idRobot){
        return;
    }

    //-------Private----------
    private void first(java.util.List<Robot> robotList){
        if(robotList != null) {
            this.currentRobot = robotList.get(0);
        }else{
            throw new NullPointerException("robotList is null.");
        }
    }

    private void next(java.util.List<Robot> robotList){
        if (this.currentRobot == null){
            throw new NullPointerException("currentRobot is null.");
        }
        for(int i = 0 ; i<robotList.size() ; i++){
            if(this.currentRobot == robotList.get(i) && i++ < robotList.size()){
                this.currentRobot = robotList.get(i++);
            }else if(i++ >= robotList.size()){
                throw new Error("robotList size exceeded.");
            }
        }
    }

    private void updateDisconnectedRobot(Robot currentRobot, java.util.List<Robot> robotList){
        for(int i = 0 ; i < connectedRobot.size() ; i++){
            if(currentRobot == connectedRobot.get(i)){
                this.connectedRobot.remove(currentRobot);
            }
        }
        this.disconnectedRobot.add(currentRobot);
    }

    private void updateConnectedRobot(Robot currentRobot, java.util.List<Robot> robotList){
        if (disconnectedRobot != null){
            for(int i = 0 ; i < disconnectedRobot.size() ; i++){
                if(currentRobot == disconnectedRobot.get(i)){
                    this.disconnectedRobot.remove(currentRobot);
                }
            }
        }
        this.connectedRobot.add(currentRobot);
    }

    private void updateRobotList(java.util.List<Robot> robotList){
        this.robotList = robotList;
    }
}
