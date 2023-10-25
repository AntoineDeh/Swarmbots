package project.src.model.com;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import androidx.annotation.Nullable;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;

import java.io.Serializable;
import java.util.Arrays;

import project.src.model.PeriphState;
import project.src.model.core.Event;
import project.src.model.core.OperatingMode;

@RunWith(Parameterized.class)
public class DecodeAlertTest {

    /**
     * @brief Id alert expected for the test.
     */
    int idAlertExp;

    /**
     * @brief Frame given for the test.
     */
    private byte[] frameGiven;

    /**
     * @brief Parameters of the test. Each line represents a test case to be executed.
     * @serialData data Elements to be tested.
     */
    @Parameterized.Parameters(name= "index: {index}")
    public static Iterable<Object[]> data() {
        return Arrays.asList(new Object[][] {
                { 1, new byte[]{0x09, 0x0, (byte) 0x01} },
                { 9, new byte[]{0x09, 0x0, (byte) 0x09} },
                { 11, new byte[]{0x09, 0x0, (byte) 11} },
                { 0, new byte[]{0x09, 0x0, (byte) 0x00} },
                { 0, new byte[]{0x09, 0x0, (byte) -1} },
                { 99, new byte[]{0x09, 0x0, (byte) 99} },
                { 99, new byte[]{0x09, 0x0, (byte) 111} },
        });
    }

    /**
     * @brief Contructor of the test. It is invoke for each test case.
     */
    public DecodeAlertTest(int idAlert, byte[] frame) {
        idAlertExp = idAlert;
        frameGiven = frame;
    }

    /**
     * @brief Execution of the test with the asserts.
     */
    @Test
    public void decodeAlertTest() {

        /*
        @brief Event expected.
         */
        Event eventExp = new Event(Event.EventID.ALERT);
        eventExp.setIdAlert(idAlertExp);

        /*
        @brief Test execution.
         */
        Event eventRes = Protocol.decode(1, frameGiven);

        /*
        @brief Asserts types.
         */
        assertNotNull(eventRes);
        assertEquals(eventExp.getClass(), eventRes.getClass());

        /*
        @brief Asserts values.
         */
        assertEquals(eventExp.getIdAlert(), eventRes.getIdAlert());
    }
}