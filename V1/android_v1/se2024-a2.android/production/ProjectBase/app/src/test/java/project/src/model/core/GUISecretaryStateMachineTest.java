package project.src.model.core;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.verify;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.lang.reflect.Field;

import project.src.model.PeriphState;
import project.src.model.com.ProxyControllerCore;

public class GUISecretaryStateMachineTest {

    private static final int STEP_DELAY_MS = 750;

    static GUIRinger guiRingerMock;
    static LogsManager logsManagerMock;
    static GUI guiMock;

    static ProxyControllerCore proxyControllerCoreMock;

    //@InjectMocks
    //private Postman postmanMock;

    @Before
    public void setUp() throws IOException, IllegalAccessException, NoSuchFieldException {

        // Initialize the singleton instance with the mock object

        // Mock initialization.
        guiRingerMock = mock(GUIRinger.class);
        //guiMock = mock(GUI.class);
        proxyControllerCoreMock = mock(ProxyControllerCore.class);

        // Instance of singleton objects.
        // ConfigReader mock
        /*
        ArrayList<Robot> robotArrayList = new ArrayList<>();
        robotArrayList.add(new Robot(1, "192.168.1.16", ""));
        robotArrayList.add(new Robot(2, "192.168.1.17", ""));

        when(ConfigReader.getRobotListFromConfig()).thenReturn(robotArrayList);

         */

        // GUIRinger mock.
        /*
        Field guiRingerInstance = GUIRinger.class.getDeclaredField("INSTANCE");
        guiRingerInstance.setAccessible(true);
        guiRingerInstance.set(guiRingerInstance, guiRingerMock);

        // LogsManager mock.
        Field logsManagerInstance = LogsManager.class.getDeclaredField("INSTANCE");
        logsManagerInstance.setAccessible(true);
        logsManagerInstance.set(logsManagerInstance, logsManagerMock);

         */

        // GUI mock.
        GUI.INSTANCE.create();
        Field GUIInstance = GUI.class.getDeclaredField("INSTANCE");
        GUIInstance.setAccessible(true);
        GUIInstance.set(GUIInstance, guiMock);

        doNothing().when(GUI.INSTANCE).initReady();
        doNothing().when(GUI.INSTANCE).modeReady();

        // ProxyControllerCore mock
        doNothing().when(proxyControllerCoreMock).askMode(anyInt());
    }

    @Test
    public void guiSecretaryTest() throws InterruptedException {

        /*
        @brief Initialize and start GUISecretary instance.
         */
        GUISecretary.INSTANCE.create();
        GUISecretary.INSTANCE.proxyControllerCore = proxyControllerCoreMock;
        GUISecretary.INSTANCE.start();

        /*
        Prepare the robot list properties.
         */
        Robot robot1 = new Robot(1, "192.168.1.14", "", 5000);
        Robot robot2 = new Robot(2, "142.251.84.13", "", 5001);
        OperatingMode operatingMode1 = new OperatingMode(
                PeriphState.ENABLED, PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.ENABLED);
        OperatingMode operatingMode2 = new OperatingMode(
                PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.ENABLED, PeriphState.ENABLED);

        /*
        @brief State machine test execution of all transitions.
         */
        // START -> INIT.
        /*
        System.out.println("State should be IDLE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.INSTANCE.state, GUISecretary.State.IDLE);
        GUISecretary.INSTANCE.askInit();
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be WAITING_FOR_CONNECTION : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.WAITING_FOR_CONNECTION, GUISecretary.INSTANCE.state);

        // Empty the list and add the robots for the test. This event should appear after getting
        // the robot list from the config reader.
        GUISecretary.INSTANCE.robotList = new ArrayList<>();
        GUISecretary.INSTANCE.robotList.add(robot1);
        GUISecretary.INSTANCE.robotList.add(robot2);

        // IDLE -> WAITING_FOR_CONNECTION (robot1).
        GUISecretary.INSTANCE.ackConnection(robot1.getId());
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be WAITING_FOR_FLUSHED_FILE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.WAITING_FOR_FLUSHED_FILE, GUISecretary.INSTANCE.state);

        // WAITING_FOR_CONNECTION -> WAITING_FOR_FLUSHED_FILE.
        GUISecretary.INSTANCE.notifyVarReady(robot1.getId());
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be WAITING_FOR_CONNECTION : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.WAITING_FOR_CONNECTION, GUISecretary.INSTANCE.state);

        // WAITING_FOR_FLUSHED_FILE -> WAITING_FOR_CONNECTION (robot2).
        GUISecretary.INSTANCE.ackConnection(robot2.getId());
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be WAITING_FOR_FLUSHED_FILE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.WAITING_FOR_FLUSHED_FILE, GUISecretary.INSTANCE.state);

        // WAITING_FOR_FLUSHED_FILE -> AVAILABLE.
        GUISecretary.INSTANCE.notifyVarReady(robot2.getId());
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be AVAILABLE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.AVAILABLE, GUISecretary.INSTANCE.state);

         */

        /* ----------------------  GETTING OPERATING MODES  -------------------------------- */

        // AVAILABLE -> WAITING_FOR_MODE (waiting robot1).
        /*
        GUISecretary.INSTANCE.setModeInit();
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be WAITING_FOR_MODE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.WAITING_FOR_MODE, GUISecretary.INSTANCE.state);

        // WAITING_FOR_MODE -> WAITING_FOR_MODE (waiting robot2).
        GUISecretary.INSTANCE.changeMode(robot1.getId(), operatingMode1);
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be WAITING_FOR_MODE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.WAITING_FOR_MODE, GUISecretary.INSTANCE.state);

        // TODO : test the value of operating mode of robot.

        // WAITING_FOR_MODE -> AVAILABLE (response of robot2).
        GUISecretary.INSTANCE.changeMode(robot2.getId(), operatingMode2);
        Thread.sleep(STEP_DELAY_MS);
        verify(guiMock).modeReady();
        System.out.println("State should be AVAILABLE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.AVAILABLE, GUISecretary.INSTANCE.state);

        // TODO : test the value of operating mode of robot.

         */

        /* ----------------------  SELECTING ROBOTS  -------------------------------- */
        /*

        // No selected robot assert
        assertNull(GUISecretary.INSTANCE.selectedRobot);

        // AVAILABLE -> AVAILABLE (selecting robot1).
        GUISecretary.INSTANCE.setSelectedRobot(robot1.getId(), Robot.SelectionState.SELECTED);
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be AVAILABLE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.AVAILABLE, GUISecretary.INSTANCE.state);

        // Selected robot assert.
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getId(), robot1.getId());

        // Operating mode assert
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getOperatingMode().getCamera(),
                robot1.getOperatingMode().getCamera());
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getOperatingMode().getRadar(),
                robot1.getOperatingMode().getRadar());
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getOperatingMode().getBuzzer(),
                robot1.getOperatingMode().getBuzzer());
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getOperatingMode().getLeds(),
                robot1.getOperatingMode().getLeds());

        // AVAILABLE -> AVAILABLE (select robot2 and unselect robot1).
        GUISecretary.INSTANCE.setSelectedRobot(robot2.getId(), Robot.SelectionState.SELECTED);
        Thread.sleep(STEP_DELAY_MS);
        System.out.println("State should be AVAILABLE : " + GUISecretary.INSTANCE.state);
        assertEquals(GUISecretary.State.AVAILABLE, GUISecretary.INSTANCE.state);

        // Selected robot assert.
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getId(), robot2.getId());
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getSelectionState(), Robot.SelectionState.SELECTED);

        // Operating mode assert
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getOperatingMode().getCamera(),
                robot1.getOperatingMode().getCamera());
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getOperatingMode().getRadar(),
                robot1.getOperatingMode().getRadar());
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getOperatingMode().getBuzzer(),
                robot1.getOperatingMode().getBuzzer());
        assertEquals(GUISecretary.INSTANCE.selectedRobot.getOperatingMode().getLeds(),
                robot1.getOperatingMode().getLeds());

         */

        /* ----------------------  DISCONNECTING ROBOTS  -------------------------------- */

        /*
        @brief Expectation calls.
         */
    }

    @After
    public void tearDown() {
        /*
        Nothing to do.
         */
    }
}
