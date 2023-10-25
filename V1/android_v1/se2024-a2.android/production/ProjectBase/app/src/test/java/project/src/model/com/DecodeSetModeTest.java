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
public class DecodeSetModeTest {

    /**
     * @brief Camera state expected.
     */
    private PeriphState cameraExp;

    /**
     * @brief Radar state expected.
     */
    private PeriphState radarExp;

    /**
     * @brief Buzzer state expected.
     */
    private PeriphState buzzerExp;

    /**
     * @brief LEDs state expected.
     */
    private PeriphState ledsExp;

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
                { PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED,
                        new byte[]{0x06, 0x0, 0x00,  0x00,  0x00,  0x00} },
                { PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.ENABLED,
                        new byte[]{0x06, 0x0,  0x01,  0x01,  0x01,  0x01} },

                { PeriphState.ENABLED, PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED,
                        new byte[]{0x06, 0x0,  0x01,  0x00,  0x00,  0x00} },
                { PeriphState.DISABLED, PeriphState.ENABLED, PeriphState.DISABLED, PeriphState.DISABLED,
                        new byte[]{0x06, 0x0,  0x00,  0x01,  0x00,  0x00} },
                { PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.ENABLED, PeriphState.DISABLED,
                        new byte[]{0x06, 0x0,  0x00,  0x00,  0x01,  0x00} },
                { PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.ENABLED,
                        new byte[]{0x06, 0x0,  0x00,  0x00,  0x00,  0x01} },


                { PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.DISABLED, PeriphState.DISABLED,
                        new byte[]{0x06, 0x0,  0x01,  0x01,  0x00,  0x00} },
                { PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.ENABLED, PeriphState.ENABLED,
                        new byte[]{0x06, 0x0,  0x00,  0x00,  0x01,  0x01} },
                { PeriphState.ENABLED, PeriphState.DISABLED, PeriphState.ENABLED, PeriphState.DISABLED,
                        new byte[]{0x06, 0x0,  0x01,  0x00,  0x01,  0x00} },
                { PeriphState.DISABLED, PeriphState.ENABLED, PeriphState.DISABLED, PeriphState.ENABLED,
                        new byte[]{0x06, 0x0,  0x00,  0x01,  0x00,  0x01} },
                { PeriphState.DISABLED, PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.DISABLED,
                        new byte[]{0x06, 0x0,  0x00,  0x01,  0x01,  0x00} },
                { PeriphState.ENABLED, PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.ENABLED,
                        new byte[]{0x06, 0x0,  0x01,  0x00,  0x00,  0x01} },
        });
    }

    /**
     * @brief Contructor of the test. It is invoke for each test case.
     */
    public DecodeSetModeTest(PeriphState camera, PeriphState radar, PeriphState buzzer, PeriphState leds, byte[] frame) {
        cameraExp = camera;
        radarExp = radar;
        buzzerExp = buzzer;
        ledsExp = leds;
        frameGiven = frame;
    }

    /**
     * @brief Execution of the test with the asserts.
     */
    @Test
    public void decodeSetModeTest() {

        /*
        @brief Event expected.
         */
        Event eventExp = new Event(Event.EventID.SET_MODE);
        eventExp.setOperatingMode(new OperatingMode(cameraExp, radarExp, buzzerExp, ledsExp));

        /*
        @brief Test execution.
         */
        Event eventRes = Protocol.decode(1,frameGiven);

        /*
        @brief Asserts types.
         */
        assertNotNull(eventRes);
        assertEquals(eventExp.getClass(), eventRes.getClass());

        /*
        @brief Asserts values.
         */
        assertEquals(eventExp.getEventID(), eventRes.getEventID());
        assertEquals(eventExp.getOperatingMode().getCamera(), eventRes.getOperatingMode().getCamera());
        assertEquals(eventExp.getOperatingMode().getRadar(), eventRes.getOperatingMode().getRadar());
        assertEquals(eventExp.getOperatingMode().getBuzzer(), eventRes.getOperatingMode().getBuzzer());
        assertEquals(eventExp.getOperatingMode().getLeds(), eventRes.getOperatingMode().getLeds());
    }
}