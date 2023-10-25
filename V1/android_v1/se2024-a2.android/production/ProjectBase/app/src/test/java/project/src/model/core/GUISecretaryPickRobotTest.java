package project.src.model.core;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;

public class GUISecretaryPickRobotTest {

    @Before
    public void setUp() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException, InstantiationException {
        // If there is a instance of GUISecretary already exist, it initializes a new one.
        if (GUISecretary.INSTANCE != null) {
            Constructor<GUISecretary> constructor = GUISecretary.class.getDeclaredConstructor();
            constructor.setAccessible(true);

            GUISecretary.INSTANCE = constructor.newInstance();
        }
    }

    @Test
    public void pickRobotTest() throws NoSuchFieldException, IllegalAccessException {

        // Setup the robot list.
        // Access the private variable
        Field privateVar = GUISecretary.INSTANCE.getClass().getDeclaredField("robotList");
        privateVar.setAccessible(true);

        ArrayList<Robot> robotListGiven = new ArrayList<Robot>(){};
        Robot robot1 = new Robot(1, "123.12.3.2", "macAddress", 12345);
        Robot robot2 = new Robot(2, "123.12.3.3", "macAddress", 12345);

        robotListGiven.add(robot1);
        robotListGiven.add(robot2);

        // Modify the value of the private variable.
        privateVar.set(GUISecretary.INSTANCE, robotListGiven);

        // Test execution.
        GUISecretary.INSTANCE.pickFirstRobot();

        // Access the private variable.
        Field pvCurrentFirstRobot = GUISecretary.INSTANCE.getClass().getDeclaredField("currentRobot");
        privateVar.setAccessible(true);
        Robot currentFirstRobotRes = (Robot) pvCurrentFirstRobot.get(GUISecretary.INSTANCE);

        /*
        @brief Asserts of the first robot.
         */
        assertNotNull(currentFirstRobotRes);
        assertEquals(currentFirstRobotRes.getClass(), robot1.getClass());
        assertEquals(currentFirstRobotRes.getId(), robot1.getId());
        assertEquals(currentFirstRobotRes.getIpAddress(), robot1.getIpAddress());

        /*
        @brief Test execution for the first robot.
         */
        GUISecretary.INSTANCE.pickNextRobot();

        // Access the private variable.
        Field pvCurrentNextRobot = GUISecretary.INSTANCE.getClass().getDeclaredField("currentRobot");
        privateVar.setAccessible(true);
        Robot currentNextRobotRes = (Robot) pvCurrentNextRobot.get(GUISecretary.INSTANCE);

        /*
        @brief Asserts of the next robot.
         */
        assertNotNull(currentNextRobotRes);
        assertEquals(currentNextRobotRes.getClass(), robot2.getClass());
        assertEquals(currentNextRobotRes.getId(), robot2.getId());
        assertEquals(currentNextRobotRes.getIpAddress(), robot2.getIpAddress());

        /*
        @brief Test execution to test the limit of the pickNextRobot that should make the currentRobot null.
         */
        GUISecretary.INSTANCE.pickNextRobot();

        // Access the private variable.
        Field pvCurrentNullRobot = GUISecretary.INSTANCE.getClass().getDeclaredField("currentRobot");
        privateVar.setAccessible(true);
        Robot currentNullRobotRes = (Robot) pvCurrentNullRobot.get(GUISecretary.INSTANCE);

        /*
        @brief Asserts of outbound next robot.
         */
        assertNull(currentNullRobotRes);
    }


    @After
    public void tearDown() {
        // Freeing the instance of GUISecretary
        GUISecretary.INSTANCE = null;
    }
}
