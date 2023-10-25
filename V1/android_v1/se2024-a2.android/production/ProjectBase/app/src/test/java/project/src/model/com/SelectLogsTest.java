package project.src.model.com;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import project.src.model.core.Event;

public class SelectLogsTest {

    @Before
    public void setUp(){
        /**
         * Nothing to do.
         */
    }

    @Test
    public void selectLogsTest(){
        /**
         * @brief Events expected.
         */
        Event eventDetected = new Event(Event.EventID.SELECT_LOGS);

        /**
         * @brief Assert not null.
         */
        assertNotNull(eventDetected);

        /**
         * @brief Assert types.
         */
        assertEquals(eventDetected.getClass(), new Event(Event.EventID.SELECT_LOGS).getClass());

        /**
         * @brief Assert values.
         */
        assertEquals(eventDetected.getEventID(), new Event(Event.EventID.SELECT_LOGS).getEventID());
    }

    @After
    public void tearDown(){
        /**
         * Nothing to do.
         */
    }
}
