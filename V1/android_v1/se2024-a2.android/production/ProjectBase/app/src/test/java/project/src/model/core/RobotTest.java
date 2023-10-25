package project.src.model.core;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;

import org.junit.Assert;
import org.junit.Test;

import project.src.model.PeriphState;

public class RobotTest {

    /**
     * @brief Testing the Robot construction.
     */
    @Test
    public void constructorRobotTest(){

        //----------------------- Expected values -------------------------------------

        int expectedIdRobot = 1;
        String expectedIpAddress = "127.0.0.1";
        String expectedMacAddress = "xxxxxx";
        int expectedPort = 5000;

        //----------------------- Constructor -----------------------------------------

        Robot robot1 = new Robot(expectedIdRobot, expectedIpAddress, expectedMacAddress, expectedPort);

        //----------------------- getters -------------------------------------------

        int idRobot = robot1.getId();
        String ipAddress = robot1.getIpAddress();
        String macAddress = robot1.getMacAddress();
        int port = robot1.getPort();

        // ---------------------- Check results ---------------------------------------

        Assert.assertEquals(expectedIdRobot, idRobot);
        Assert.assertEquals(expectedIpAddress, ipAddress);
        Assert.assertEquals(expectedMacAddress, macAddress);
        Assert.assertEquals(expectedPort, port);
    }

    /**
     * @brief Test the failedPongVar manipulation.
     */
    @Test
    public void failedPongVarTest(){

        //----------------------- Expected value -------------------------------------

        int expectedValue = 1;

        //----------------------- Constructor -----------------------------------------

        int idRobot = 1;
        String ipAddress = "127.0.0.1";
        String macAddress = "xxxxxx";
        int port = 5000;

        Robot robot1 = new Robot(idRobot, ipAddress, macAddress, port);

        //----------------------- setter & getter -------------------------------------
        robot1.setFailedPongsVar(expectedValue);

        int value = robot1.getFailedPongsVar();

        // ---------------------- Check results ---------------------------------------
        Assert.assertEquals(expectedValue, value);
    }

    @Test
    public void operatingModeTest(){

        //----------------------- Constructors -----------------------------------------

        int idRobot = 1;
        String ipAddress = "127.0.0.1";
        String macAddress = "xxxxxx";
        int port = 5000;

        Robot robot1 = new Robot(idRobot, ipAddress, macAddress, port);
        Robot robot2 = new Robot(idRobot, ipAddress, macAddress, port);
        OperatingMode operatingMode = new OperatingMode();

        //----------------------- setter & getter -------------------------------------

        OperatingMode opMode1 = robot1.getOperatingMode();
        robot2.setOperatingMode(new OperatingMode(PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED));
        OperatingMode opMode2 = robot2.getOperatingMode();

        // ---------------------- Check results ---------------------------------------
        Assert.assertEquals(operatingMode.get().getCamera(), opMode1.getCamera());
        Assert.assertEquals(operatingMode.get().getRadar(), opMode1.getRadar());
        Assert.assertEquals(operatingMode.get().getBuzzer(), opMode1.getBuzzer());
        Assert.assertEquals(operatingMode.get().getLeds(), opMode1.getLeds());
        //Assert.assertEquals(new OperatingMode(PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED), opMode2);
    }
}
