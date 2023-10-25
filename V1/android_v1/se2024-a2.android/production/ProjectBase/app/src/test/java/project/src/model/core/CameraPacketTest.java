package project.src.model.core;

import org.junit.Assert;
import org.junit.Test;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class CameraPacketTest {

    /**
     * @brief Testing the construcion of a parameterized CameraPacket with different parameters.
     */
    @Test
    public void cameraPacketConstructorTest1() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        //----------------------- Expected values ------------------------------------
        int expectedPort = 1;
        byte[] expectedArray = null;
        int expectedLength = 1;

        //----------------------- Constructor ----------------------------------------

        CameraPacket cameraPacket = new CameraPacket(1, null, 1);

        //----------------------- getPort ------------------------------------------

        // Getting getPort.
        Method getPort = CameraPacket.class.getMethod("getPort");

        // Call getterPort from CameraPocket.
        int port = (int) getPort.invoke(cameraPacket);

        //----------------------- getData ------------------------------------------
        // Getting getData.
        Method getData = CameraPacket.class.getMethod("getData");

        // Call getterPort from CameraPocket.
        byte[] array = (byte[]) getData.invoke(cameraPacket);

        //----------------------- getLength ------------------------------------------
        // Getting getData.
        Method getLength = CameraPacket.class.getMethod("getLength");

        // Call getterPort from CameraPocket.
        int length = (int) getLength.invoke(cameraPacket);

        // ---------------------- Check results -------------------------------------

        Assert.assertEquals(expectedPort, port);
        Assert.assertArrayEquals(expectedArray, array);
        Assert.assertEquals(expectedLength, length);
    }

    /**
     * @brief Testing the construcion of a parameterized CameraPacket with different parameters.
     */
    @Test
    public void cameraPacketConstructorTestNotNull() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        //----------------------- Expected values ------------------------------------
        int expectedPort = 10;
        byte[] expectedArray = new byte[]{0};
        int expectedLength = 10;

        //----------------------- Constructor ----------------------------------------

        CameraPacket cameraPacket = new CameraPacket(10, new byte[]{0}, 10);


        //----------------------- getPort ------------------------------------------

        // Getting getPort.
        Method getPort = CameraPacket.class.getMethod("getPort");

        // Call getterPort from CameraPocket.
        int port = (int) getPort.invoke(cameraPacket);

        //----------------------- getData ------------------------------------------
        // Getting getData.
        Method getData = CameraPacket.class.getMethod("getData");

        // Call getterPort from CameraPocket.
        byte[] array = (byte[]) getData.invoke(cameraPacket);

        //----------------------- getLength ------------------------------------------
        // Getting getData.
        Method getLength = CameraPacket.class.getMethod("getLength");

        // Call getterPort from CameraPocket.
        int length = (int) getLength.invoke(cameraPacket);

        // ---------------------- Check results -------------------------------------

        Assert.assertEquals(expectedPort, port);
        Assert.assertArrayEquals(expectedArray, array);
        Assert.assertEquals(expectedLength, length);
    }
}
