package project.src.model.core;

import org.junit.Assert;
import org.junit.Test;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import project.src.model.PeriphState;

public class OperatingModeTest {
    /**
     * @brief Testing the default OpertaingMode construction.
     * @throws InvocationTargetException
     * @throws IllegalAccessException
     * @throws NoSuchMethodException
     */
    @Test
    public void OperatingModeEmptyConstructorTest() throws InvocationTargetException, IllegalAccessException, NoSuchMethodException {

        //----------------------- Expected values -------------------------------------

        int expectedCamera = PeriphState.ENABLED.ordinal();
        int expectedRadar = PeriphState.ENABLED.ordinal();
        int expectedBuzzer = PeriphState.ENABLED.ordinal();
        int expectedLeds = PeriphState.ENABLED.ordinal();

        //----------------------- Constructor -----------------------------------------

        OperatingMode operatingMode = new OperatingMode();

        //----------------------- getCamera -------------------------------------------

        Method getCamera = OperatingMode.class.getMethod("getCamera");

        int camera = (int) getCamera.invoke(operatingMode);

        //----------------------- getRadar --------------------------------------------

        Method getRadar = OperatingMode.class.getMethod("getRadar");

        int radar = (int) getRadar.invoke(operatingMode);

        //----------------------- getBuzzer -------------------------------------------

        Method getBuzzer = OperatingMode.class.getMethod("getBuzzer");

        int buzzer = (int) getBuzzer.invoke(operatingMode);

        //----------------------- getLeds ---------------------------------------------

        Method getLeds = OperatingMode.class.getMethod("getLeds");

        int leds = (int) getLeds.invoke(operatingMode);

        // ---------------------- Check results ---------------------------------------

        Assert.assertEquals(expectedCamera, camera);
        Assert.assertEquals(expectedRadar, radar);
        Assert.assertEquals(expectedBuzzer, buzzer);
        Assert.assertEquals(expectedLeds, leds);

    }

    /**
     * @brief Testing the OperatingMode construction with all parameters ENABLED.
     * @throws InvocationTargetException
     * @throws IllegalAccessException
     * @throws NoSuchMethodException
     */
    @Test
    public void OperatingModeConstructorEnabledTest() throws InvocationTargetException, IllegalAccessException, NoSuchMethodException {

        //----------------------- Expected values -------------------------------------

        int expectedCamera = PeriphState.ENABLED.ordinal();
        int expectedRadar = PeriphState.ENABLED.ordinal();
        int expectedBuzzer = PeriphState.ENABLED.ordinal();
        int expectedLeds = PeriphState.ENABLED.ordinal();

        //----------------------- Constructor -----------------------------------------

        OperatingMode operatingMode = new OperatingMode(PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.ENABLED);

        //----------------------- getCamera -------------------------------------------

        Method getCamera = OperatingMode.class.getMethod("getCamera");

        int camera = (int) getCamera.invoke(operatingMode);

        //----------------------- getRadar --------------------------------------------

        Method getRadar = OperatingMode.class.getMethod("getRadar");

        int radar = (int) getRadar.invoke(operatingMode);

        //----------------------- getBuzzer -------------------------------------------

        Method getBuzzer = OperatingMode.class.getMethod("getBuzzer");

        int buzzer = (int) getBuzzer.invoke(operatingMode);

        //----------------------- getLeds ---------------------------------------------

        Method getLeds = OperatingMode.class.getMethod("getLeds");

        int leds = (int) getLeds.invoke(operatingMode);

        // ---------------------- Check results ---------------------------------------

        Assert.assertEquals(expectedCamera, camera);
        Assert.assertEquals(expectedRadar, radar);
        Assert.assertEquals(expectedBuzzer, buzzer);
        Assert.assertEquals(expectedLeds, leds);

    }

    /**
     * @brief Testing the OperatingMode construction with all parameters DISABLED.
     * @throws InvocationTargetException
     * @throws IllegalAccessException
     * @throws NoSuchMethodException
     */
    @Test
    public void OperatingModeConstructorDisabledTest() throws InvocationTargetException, IllegalAccessException, NoSuchMethodException {

        //----------------------- Expected values -------------------------------------

        int expectedCamera = PeriphState.DISABLED.ordinal();
        int expectedRadar = PeriphState.DISABLED.ordinal();
        int expectedBuzzer = PeriphState.DISABLED.ordinal();
        int expectedLeds = PeriphState.DISABLED.ordinal();

        //----------------------- Constructor -----------------------------------------

        OperatingMode operatingMode = new OperatingMode(PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED);

        //----------------------- getCamera -------------------------------------------

        Method getCamera = OperatingMode.class.getMethod("getCamera");

        int camera = (int) getCamera.invoke(operatingMode);

        //----------------------- getRadar --------------------------------------------

        Method getRadar = OperatingMode.class.getMethod("getRadar");

        int radar = (int) getRadar.invoke(operatingMode);

        //----------------------- getBuzzer -------------------------------------------

        Method getBuzzer = OperatingMode.class.getMethod("getBuzzer");

        int buzzer = (int) getBuzzer.invoke(operatingMode);

        //----------------------- getLeds ---------------------------------------------

        Method getLeds = OperatingMode.class.getMethod("getLeds");

        int leds = (int) getLeds.invoke(operatingMode);

        // ---------------------- Check results ---------------------------------------

        Assert.assertEquals(expectedCamera, camera);
        Assert.assertEquals(expectedRadar, radar);
        Assert.assertEquals(expectedBuzzer, buzzer);
        Assert.assertEquals(expectedLeds, leds);

    }

    /**
     * @brief Testing the capability of setOpertaingMode to set all the parameters at the same time.
     * @throws InvocationTargetException
     * @throws IllegalAccessException
     * @throws NoSuchMethodException
     */
    @Test
    public void OperatingModeSetOperatingModeTest() throws InvocationTargetException, IllegalAccessException, NoSuchMethodException {

        //----------------------- Expected values -------------------------------------

        int expectedCamera = PeriphState.DISABLED.ordinal();
        int expectedRadar = PeriphState.DISABLED.ordinal();
        int expectedBuzzer = PeriphState.DISABLED.ordinal();
        int expectedLeds = PeriphState.DISABLED.ordinal();

        //----------------------- Constructor -----------------------------------------

        OperatingMode operatingMode = new OperatingMode(PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.ENABLED);

        //----------------------- setOperatingMode ------------------------------------

        operatingMode.setOperatingMode(PeriphState.DISABLED, PeriphState.DISABLED, PeriphState.DISABLED ,PeriphState.DISABLED);

        //----------------------- getCamera -------------------------------------------

        Method getCamera = OperatingMode.class.getMethod("getCamera");

        int camera = (int) getCamera.invoke(operatingMode);

        //----------------------- getRadar --------------------------------------------

        Method getRadar = OperatingMode.class.getMethod("getRadar");

        int radar = (int) getRadar.invoke(operatingMode);

        //----------------------- getBuzzer -------------------------------------------

        Method getBuzzer = OperatingMode.class.getMethod("getBuzzer");

        int buzzer = (int) getBuzzer.invoke(operatingMode);

        //----------------------- getLeds ---------------------------------------------

        Method getLeds = OperatingMode.class.getMethod("getLeds");

        int leds = (int) getLeds.invoke(operatingMode);

        // ---------------------- Check results ---------------------------------------
        System.out.println("TOTO " + expectedCamera + camera);
        Assert.assertEquals(expectedCamera, camera);
        Assert.assertEquals(expectedCamera, operatingMode.getCamera());
        Assert.assertEquals(expectedRadar, radar);
        Assert.assertEquals(expectedRadar, operatingMode.getRadar());
        Assert.assertEquals(expectedBuzzer, buzzer);
        Assert.assertEquals(expectedBuzzer, operatingMode.getBuzzer());
        Assert.assertEquals(expectedLeds, leds);
        Assert.assertEquals(expectedLeds, operatingMode.getLeds());

    }

    /**
     * @brief Testing the capability of setOneOpertaingMode to set one parameter at once.
     * @throws InvocationTargetException
     * @throws IllegalAccessException
     * @throws NoSuchMethodException
     */
    @Test
    public void OperatingModeSetOneOperatingModeTest() throws InvocationTargetException, IllegalAccessException, NoSuchMethodException {

        //----------------------- Expected values -------------------------------------

        int expectedCamera = PeriphState.DISABLED.ordinal();
        int expectedRadar = PeriphState.DISABLED.ordinal();
        int expectedBuzzer = PeriphState.DISABLED.ordinal();
        int expectedLeds = PeriphState.DISABLED.ordinal();

        //----------------------- Constructor -----------------------------------------

        OperatingMode operatingMode = new OperatingMode(PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.ENABLED, PeriphState.ENABLED);

        //----------------------- setCamera -------------------------------------------

        Method setCamera = OperatingMode.class.getMethod("setOneOperatingMode", OperatingMode.PeriphToChange.class, PeriphState.class);

        setCamera.invoke(operatingMode, OperatingMode.PeriphToChange.CAMERA, PeriphState.DISABLED);

        //----------------------- setRadar --------------------------------------------

        Method setRadar = OperatingMode.class.getMethod("setOneOperatingMode", OperatingMode.PeriphToChange.class, PeriphState.class);

        setRadar.invoke(operatingMode, OperatingMode.PeriphToChange.RADAR, PeriphState.DISABLED);

        //----------------------- setBuzzer -------------------------------------------

        Method setBuzzer = OperatingMode.class.getMethod("setOneOperatingMode", OperatingMode.PeriphToChange.class, PeriphState.class);

        setBuzzer.invoke(operatingMode, OperatingMode.PeriphToChange.BUZZER, PeriphState.DISABLED);

        //----------------------- setLeds ---------------------------------------------

        Method setLeds = OperatingMode.class.getMethod("setOneOperatingMode", OperatingMode.PeriphToChange.class, PeriphState.class);

        setLeds.invoke(operatingMode, OperatingMode.PeriphToChange.LEDS, PeriphState.DISABLED);

        //----------------------- getCamera -------------------------------------------

        Method getCamera = OperatingMode.class.getMethod("getCamera");

        int camera = (int) getCamera.invoke(operatingMode);

        //----------------------- getRadar --------------------------------------------

        Method getRadar = OperatingMode.class.getMethod("getRadar");

        int radar = (int) getRadar.invoke(operatingMode);

        //----------------------- getBuzzer -------------------------------------------

        Method getBuzzer = OperatingMode.class.getMethod("getBuzzer");

        int buzzer = (int) getBuzzer.invoke(operatingMode);

        //----------------------- getLeds ---------------------------------------------

        Method getLeds = OperatingMode.class.getMethod("getLeds");

        int leds = (int) getLeds.invoke(operatingMode);

        // ---------------------- Check results ---------------------------------------

        Assert.assertEquals(expectedCamera, camera);
        Assert.assertEquals(expectedRadar, radar);
        Assert.assertEquals(expectedBuzzer, buzzer);
        Assert.assertEquals(expectedLeds, leds);

    }

}
