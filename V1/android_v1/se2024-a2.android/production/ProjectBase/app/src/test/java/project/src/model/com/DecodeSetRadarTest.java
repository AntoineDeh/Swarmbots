package project.src.model.com;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import project.src.model.core.Event;

public class DecodeSetRadarTest {

    @Before
    public void setUp() {
        /*
        Nothing to do.
         */
    }

    @Test
    public void decodeSetRadar() {

        /*
        @brief Event expected.
         */
        Event eventExpNotDetected = new Event(Event.EventID.SET_RADAR);
        Event eventExpDetected = new Event(Event.EventID.SET_RADAR);

        /*
        @brief Test data given.
         */
        byte[] frameGivenNotDetected = new byte[]{0x12, 0x0,  0x00};
        byte[] frameGivenDetected = new byte[]{0x12, 0x0,  0x01};

        /*
        @brief Test execution.
         */
        Event eventResNotDetected = Protocol.decode(1,frameGivenNotDetected);
        Event eventResDetected = Protocol.decode(1,frameGivenDetected);

        /*
        @brief Asserts types.
         */
        assertNotNull(eventResNotDetected);
        assertNotNull(eventResDetected);
        assertEquals(eventExpNotDetected.getClass(), eventResNotDetected.getClass());
        assertEquals(eventExpNotDetected.getClass(), eventResDetected.getClass());

        /*
        @brief Asserts values.
         */
        assertEquals(eventExpNotDetected.getEventID(), eventResNotDetected.getEventID());
        assertEquals(eventExpDetected.getEventID(), eventResDetected.getEventID());
    }


    @After
    public void tearDown() {
        /*
        Nothing to do.
         */
    }
}
