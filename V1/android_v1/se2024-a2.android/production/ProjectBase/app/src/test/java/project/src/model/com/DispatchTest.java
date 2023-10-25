package project.src.model.com;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.contains;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.View;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.mockito.Mockito;

import java.io.IOException;
import java.lang.reflect.Field;
import java.util.ArrayList;

import project.src.model.PeriphState;
import project.src.model.core.AddressReader;
import project.src.model.core.Event;
import project.src.model.core.GUI;
import project.src.model.core.GUIRinger;
import project.src.model.core.GUISecretary;
import project.src.model.core.LogsManager;
import project.src.model.core.OperatingMode;
import project.src.model.core.Viewer;

public class DispatchTest {

    private Viewer viewerMock;
    static GUISecretary guiSecretaryMock;
    static GUIRinger guiRingerMock;
    static LogsManager logsManagerMock;
    static GUI guiMock;
    static Postman postmanMock;
    private Dispatcher dispatcher;
    private AddressReader addressReaderMock;




    @Before
    public void setUp() throws IOException, IllegalAccessException, NoSuchFieldException {
        // Mock initialization.
        // Viewer mock.
        viewerMock = mock(Viewer.class);
        Field viewerInstance = Viewer.class.getDeclaredField("INSTANCE");
        viewerInstance.setAccessible(true);
        viewerInstance.set(viewerInstance, viewerMock);
        Mockito.doNothing().when(viewerMock);

        /*
        addressReaderMock = mock(AddressReader.class);
        when(addressReaderMock.getIPAddress()).thenReturn("address");
        doNothing().when(AddressReader.setContext(new Context()));
         */

        guiRingerMock = mock(GUIRinger.class);
        guiSecretaryMock = mock(GUISecretary.class);
        logsManagerMock = mock(LogsManager.class);
        postmanMock = mock(Postman.class);
        guiMock = mock(GUI.class);

        // Dispatcher we want to test.
        dispatcher = new Dispatcher(postmanMock, 1);

        // Instance of singleton objects.

        // GUIRinger mock.
        Field guiRingerInstance = GUIRinger.class.getDeclaredField("INSTANCE");
        guiRingerInstance.setAccessible(true);
        guiRingerInstance.set(guiRingerInstance, guiRingerMock);

        // GUISecretary mock.
        Field guiSecretaryInstance = GUISecretary.class.getDeclaredField("INSTANCE");
        guiSecretaryInstance.setAccessible(true);
        guiSecretaryInstance.set(guiSecretaryInstance, guiSecretaryMock);

        // LogsManager mock.
        Field logsManagerInstance = LogsManager.class.getDeclaredField("INSTANCE");
        logsManagerInstance.setAccessible(true);
        logsManagerInstance.set(logsManagerInstance, logsManagerMock);

        // GUI mock.
        Field GUIInstance = GUI.class.getDeclaredField("INSTANCE");
        GUIInstance.setAccessible(true);
        GUIInstance.set(GUIInstance, guiMock);
    }

    @Test
    public void dispatchTest() {

        /*
        @brief Test data given.
         */
        Event eventAckDisconnection = new Event(Event.EventID.DISCONNECTED_OK);
        Event eventAlert = new Event(Event.EventID.ALERT);
        Event eventSetAvailability = new Event(Event.EventID.SET_AVAILABILITY);
        Event eventSetLogs = new Event(Event.EventID.SET_LOGS);
        Event eventSetMode = new Event(Event.EventID.SET_MODE);
        Event eventSetRadar = new Event(Event.EventID.SET_RADAR);


        int idRobot = 1;
        eventAckDisconnection.setIdRobot(idRobot);
        eventAlert.setIdRobot(idRobot);
        eventSetAvailability.setIdRobot(idRobot);
        eventSetLogs.setIdRobot(idRobot);
        eventSetMode.setIdRobot(idRobot);
        eventSetRadar.setIdRobot(idRobot);

        int idAlert = 1;
        eventAlert.setIdAlert(idAlert);

        ArrayList<String> logs = new ArrayList<>();
        logs.add("String list");
        eventSetLogs.setLogs(logs);

        eventSetMode.setOperatingMode(new OperatingMode(PeriphState.ENABLED, PeriphState.DISABLED,
                PeriphState.ENABLED, PeriphState.DISABLED));

        Event.RadarState radarState = Event.RadarState.OBJECT_DETECTED;
        eventSetRadar.setRadarState(radarState);

        /*
        @brief Test execution.
         */
        dispatcher.dispatch(eventAckDisconnection); // Associated to the function disconnectedOk
        dispatcher.dispatch(eventAlert);
        dispatcher.dispatch(eventSetAvailability);
        dispatcher.dispatch(eventSetLogs);
        dispatcher.dispatch(eventSetMode);
        dispatcher.dispatch(eventSetRadar);

        /*
        @brief Expectation calls.
         */
        verify(guiRingerMock).setAvailability(eventAlert.getIdRobot());
        verify(guiSecretaryMock).setMode(eventSetMode.getIdRobot(), eventSetMode.getOperatingMode());
        verify(logsManagerMock).setLogs(eventSetLogs.getIdRobot(), eventSetLogs.getLogs());
        verify(guiMock).raiseMemoryAlert(eventAlert.getIdRobot(), eventAlert.getIdAlert());
        verify(guiSecretaryMock).disconnectedOk(eventAckDisconnection.getIdRobot());
        verify(guiSecretaryMock).setRadar(anyInt(), any());
    }

    @After
    public void tearDown() {
        /*
        Nothing to do.
         */
    }
}
