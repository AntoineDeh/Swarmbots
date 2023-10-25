package project.src.model.core;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;

public class GUISecretaryTest {

    /*
    public static void main(String[] args) throws NoSuchFieldException, IllegalAccessException, InvocationTargetException, NoSuchMethodException {
        testConstructor();
        testInitConnection();
        testUpdateConnectedRobot();
        testUpdateDisconnectedRobot();
        testUpdateSelectedRobot();
        testUpdateUnselectedRobot();
        testChangeMode();
        testRun();
    }

     */

    @Test
    public void testConstructor() {
        try {
            GUISecretary guisecretary = GUISecretary.INSTANCE;
            System.out.println("Constructor test passed");
        } catch (Exception e) {
            System.out.println("Constructor test failed: " + e.getMessage());
        }
    }

    @Test
    public void testInitConnection() throws IllegalAccessException, NoSuchFieldException {
        GUISecretary guisecretary = GUISecretary.INSTANCE;
        guisecretary.askInit();

        //getState
        Field stateField = guisecretary.getClass().getDeclaredField("state");
        stateField.setAccessible(true);

        String state = (String) stateField.get(guisecretary);

        // Wait for initialization to complete
        while (guisecretary.getState().equals("AVAILABLE")) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        ArrayList<Robot> robotList = guisecretary.getRobotList();
        System.out.println("Robot List:");
        for (Robot robot : robotList) {
            System.out.println("Robot ID: " + robot.getId());
            System.out.println("Robot IP: " + robot.getIpAddress());
            System.out.println("Robot Port: " + robot.getPort());
            System.out.println();
        }
    }

    @Test
    public void testUpdateConnectedRobot() throws InvocationTargetException, IllegalAccessException, NoSuchMethodException, NoSuchFieldException {
        GUISecretary guisecretary = GUISecretary.INSTANCE;

        // Access the private variable
        Field privateVar = GUISecretary.INSTANCE.getClass().getDeclaredField("robotList");
        privateVar.setAccessible(true);

        // Get the value of the private variable
        ArrayList<Robot> robotList = (ArrayList<Robot>) privateVar.get(GUISecretary.INSTANCE);

        ArrayList<Robot> robotListGiven = new ArrayList<Robot>(){};
        robotListGiven.add(new Robot(1, "123.12.3.1", "macAddress", 12345));
        // Modify the value of the private variable
        privateVar.set(GUISecretary.INSTANCE, robotListGiven);

        // Assert connected state robot at the init of the robot.
        assertEquals(Robot.ConnectionState.DISCONNECTED, GUISecretary.INSTANCE.getRobotList().get(0).getConnectionState());

        //getUpdateConnectedRobot
        Method updateConnectedRobot = guisecretary.getClass().getDeclaredMethod("updateDisconnectedRobot", int.class);
        updateConnectedRobot.setAccessible(true);
        updateConnectedRobot.invoke(guisecretary,1);

        // Assert disconnected state robot by the updateDisconnectedRobot function.
        assertEquals(Robot.ConnectionState.DISCONNECTED, GUISecretary.INSTANCE.getRobotList().get(0).getConnectionState());

        //Robot selectedRobot = guisecretary.getSelectedRobot();
        //System.out.println("Selected Robot ID: " + selectedRobot.getId());
    }

    @Test
    public void testUpdateDisconnectedRobot() throws InvocationTargetException, IllegalAccessException, NoSuchMethodException, NoSuchFieldException {
        GUISecretary guisecretary = GUISecretary.INSTANCE;

        // Access the private variable
        Field privateVar = GUISecretary.INSTANCE.getClass().getDeclaredField("robotList");
        privateVar.setAccessible(true);

        // Get the value of the private variable
        ArrayList<Robot> robotList = (ArrayList<Robot>) privateVar.get(GUISecretary.INSTANCE);

        ArrayList<Robot> robotListGiven = new ArrayList<Robot>(){};
        robotListGiven.add(new Robot(1, "123.12.3.1", "macAddress", 12345));

        // Modify the value of the private variable
        privateVar.set(GUISecretary.INSTANCE, robotListGiven);

        //getUpdateDisconnectedRobot
        Method updateDisconnectedRobot = guisecretary.getClass().getDeclaredMethod("updateDisconnectedRobot", int.class);
        updateDisconnectedRobot.setAccessible(true);
        updateDisconnectedRobot.invoke(guisecretary,1);

        Robot selectedRobot = guisecretary.getSelectedRobot();
        if (selectedRobot == null) {
            System.out.println("Selected Robot is null (disconnected)");
        } else {
            System.out.println("Selected Robot ID: " + selectedRobot.getId());
        }
    }

    @Test
    public void testUpdateSelectedRobot() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        GUISecretary guisecretary = GUISecretary.INSTANCE;

        //getUpdateSelectedRobot
        Method updateSelectedRobot = guisecretary.getClass().getDeclaredMethod("updateSelectedRobot", int.class);
        updateSelectedRobot.setAccessible(true);
        updateSelectedRobot.invoke(guisecretary,1);

        Robot selectedRobot = guisecretary.getSelectedRobot();
        //System.out.println("Selected Robot ID: " + selectedRobot.getId());
    }

    @Test
    public void testUpdateUnselectedRobot() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        GUISecretary guisecretary = GUISecretary.INSTANCE;

        //getUpdateUnselectedRobot
        Method updateUnselectedRobot = guisecretary.getClass().getDeclaredMethod("updateUnselectedRobot");
        updateUnselectedRobot.setAccessible(true);
        updateUnselectedRobot.invoke(guisecretary);

        Robot selectedRobot = guisecretary.getSelectedRobot();
        if (selectedRobot == null) {
            System.out.println("Selected Robot is null (unselected)");
        } else {
            System.out.println("Selected Robot ID: " + selectedRobot.getId());
        }
    }

    @Test
    public void testChangeMode() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        GUISecretary guisecretary = GUISecretary.INSTANCE;

        //getUpdateSelectedRobot
        Method updateSelectedRobot = guisecretary.getClass().getDeclaredMethod("changeMode", int.class, OperatingMode.class);
        updateSelectedRobot.setAccessible(true);
        updateSelectedRobot.invoke(guisecretary,1, new OperatingMode());

        Robot selectedRobot = guisecretary.getSelectedRobot();
        System.out.println("Selected Robot ID: " + selectedRobot.getId());
        System.out.println("Operating Mode: " + selectedRobot.getOperatingMode().toString());
    }
}

