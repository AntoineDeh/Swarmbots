package project.src.model.com;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import project.src.model.core.Event;

public class DecodeSetAvailabilityTest {

    private Event eventExp;

    @Before
    public void setUp() {
        eventExp = new Event(Event.EventID.SET_AVAILABILITY);
    }

    @Test
    public void decodeSetAvailability() {

        /*
        @brief Test data given.
         */
        byte[] frameGiven = new byte[]{0x02, 0x0};

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
    }


    @After
    public void tearDown() {
        /*
        Nothing to do.
         */
    }
}
