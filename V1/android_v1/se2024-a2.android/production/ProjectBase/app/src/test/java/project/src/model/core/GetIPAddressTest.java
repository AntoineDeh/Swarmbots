package project.src.model.core;

import static org.junit.Assert.assertEquals;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class GetIPAddressTest {

    /**
     * @brief Expected values.
     */
    String ipStringExpected;
    AddressReader addressReader;

    @Before
    public void setUp(){
        /**
         * @brief Constructor and the targeted ip address.
         */
        this.addressReader = new AddressReader();
        ipStringExpected = "0.0.0.0";

    }

    @Test
    public void getIPAddressTest(){
        /**
         * @brief Testing the ipAddress call.
         */
        String ipString = addressReader.getIPAddress();

        /**
         * @brief Testing values.
         */
        assertEquals(ipStringExpected,ipString);
    }


    @After
    public void tearDown(){
        /**
         * Nothing to do.
         */
    }
}
