package project.src.model.com;

import static org.junit.Assert.assertEquals;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.lang.reflect.Field;
import java.util.ArrayList;

public class SetPostManListTest {

    ProxyPilot proxyPilot;

    @Before
    public void setUp(){
        /**
         * @brief ProxyPilot Constructor.
         */
        this.proxyPilot = new ProxyPilot();
    }

    @Test
    public void setPostManTest() {
        ArrayList<Postman> postmanListExpected = new ArrayList<Postman>();
        proxyPilot.setPostManList(postmanListExpected);

        ArrayList<Postman> postmanList = null;
        try {
            Field postmanListField = ProxyPilot.class.getDeclaredField("postmanList");
            postmanListField.setAccessible(true);
            postmanList = (ArrayList<Postman>) postmanListField.get(proxyPilot);

        } catch (NoSuchFieldException | IllegalAccessException e) {
            e.printStackTrace();
        }

        assertEquals(postmanListExpected, postmanList);
    }

    @After
    public void tearDown(){
        /**
         * @brief Nothing to do.
         */
    }
}
