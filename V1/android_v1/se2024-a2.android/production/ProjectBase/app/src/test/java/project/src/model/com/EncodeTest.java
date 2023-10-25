package project.src.model.com;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertNotNull;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;

import java.util.Arrays;

import project.src.model.Command;
import project.src.model.core.Robot;

import project.src.model.com.Protocol;

@RunWith(Parameterized.class)
public class EncodeTest {

    /**
     * @brief Protocol message we want to test.
     */
    private enum ProtocolMsg {
        ASK_AVAILABILITY,
        ASK_CMD,
        SET_STATE,
        ASK_MODE,
        SET_MODE,
        ASK_LOGS,
        ALERT,
        ASK_TO_DISCONNECT,
        SET_CURRENT_TIME,
        SET_IP_PORT,
        LOGS_RECEIVED
    };

    /**
     * @brief Protocol message to test.
     */
    private final ProtocolMsg protocolMsg;

    /**
     * @brief Commande given for the test.
     */
    private final Integer[] cmdGiven;

    /**
     * @brief Frame expected for the test.
     */
    private final byte[] frameExp;

    /**
     * @brief Parameters of the test. Each line represents a test case to be executed.
     * @serialData data Elements to be tested.
     */
    @Parameterized.Parameters(name= "index: {index} / msg: {0} / given: {1} / expected: {2}")
    public static Iterable<Object[]> data() {
        return Arrays.asList(new Object[][] {
                /*
                AskAvailability test
                 */
                { ProtocolMsg.ASK_AVAILABILITY, null, new byte[]{0x00, 0x02, 0x00, 0x01} },

                /*
                AskCmd test
                 */
                { ProtocolMsg.ASK_CMD, new Integer[]{1}, new byte[]{0x00, 0x03, 0x00, 0x03, 0x01} },
                { ProtocolMsg.ASK_CMD, new Integer[]{2}, new byte[]{0x00, 0x03, 0x00, 0x03, 0x02} },
                { ProtocolMsg.ASK_CMD, new Integer[]{3}, new byte[]{0x00, 0x03, 0x00, 0x03, 0x03} },
                { ProtocolMsg.ASK_CMD, new Integer[]{4}, new byte[]{0x00, 0x03, 0x00, 0x03, 0x04} },

                /*
                SetState test
                 */
                // Selected
                { ProtocolMsg.SET_STATE, new Integer[]{2}, new byte[]{0x00, 0x03, 0x00, 0x04, 0x02} },
                // Not selected
                { ProtocolMsg.SET_STATE, new Integer[]{3}, new byte[]{0x00, 0x03, 0x00, 0x04, 0x03} },

                /*
                AskMode test
                 */
                { ProtocolMsg.ASK_MODE, null, new byte[]{0x00, 0x02, 0x00, 0x05} },

                /*
                SetMode test
                 */
                { ProtocolMsg.SET_MODE, new Integer[]{0, 0, 0, 0}, new byte[]{0x00, 0x06, 0x00, 0x06,
                        0x00, 0x00, 0x00, 0x00} },
                { ProtocolMsg.SET_MODE, new Integer[]{1, 1, 1, 1}, new byte[]{0x00, 0x06, 0x00, 0x06,
                        0x01, 0x01, 0x01, 0x01} },
                { ProtocolMsg.SET_MODE, new Integer[]{1, 0, 1, 0}, new byte[]{0x00, 0x06, 0x00, 0x06,
                        0x01, 0x00, 0x01, 0x00} },
                { ProtocolMsg.SET_MODE, new Integer[]{0, 1, 0, 1}, new byte[]{0x0, 0x06, 0x0, 0x06,
                        0x00, 0x01, 0x00, 0x01} },

                /*
                AskLogs test
                 */
                { ProtocolMsg.ASK_LOGS, null, new byte[]{0x0, 0x02, 0x0, 0x07} },

                /*
                Alert test
                 */
                { ProtocolMsg.ALERT, new Integer[]{1}, new byte[]{0x0, 0x03, 0x0, 0x09, 0x01} },

                /*
                AskToDisconnect test
                 */
                { ProtocolMsg.ASK_TO_DISCONNECT, null, new byte[]{0x0, 0x02, 0x0, 0x10} },

                /*
                SetCurrentTime test
                 */
                { ProtocolMsg.SET_CURRENT_TIME, new Integer[]{2023, 5, 13, 17, 5, 1}, new byte[]{0x0, 0x09, 0x0,
                        (byte) 0x13, (byte) 20, (byte) 23, (byte) 5, (byte) 13, (byte) 17, (byte) 5, (byte) 1}},

                { ProtocolMsg.SET_CURRENT_TIME, new Integer[]{2370, 81, 14, 22, 20, 54}, new byte[]{0x0, 0x09, 0x0,
                        (byte) 0x13, (byte) 23, (byte) 70, (byte) 81, (byte) 14, (byte) 22, (byte) 20, (byte) 54}},

                /*
                SetIpPort test
                 */
                { ProtocolMsg.SET_IP_PORT, new Integer[]{12345},
                        new byte[]{0x0, 0x08, 0x0, 0x14,
                                (byte) 127, (byte) 23, (byte) 1, (byte) 52, 0x30, 0x39} },

                /*
                LogsReceived test
                 */
                { ProtocolMsg.LOGS_RECEIVED, null, new byte[]{0x0, 0x02, 0x0, 0x15} },
        });
    }

    /**
     * @brief Contructor of the test. It is invoke for each test case.
     */
    public EncodeTest(ProtocolMsg msg, Integer[] input, byte[] expected) {
        protocolMsg = msg;
        cmdGiven = input;
        frameExp = expected;
    }

    /**
     * @brief Execution of the test with the asserts.
     */
    @Test
    public void encodeTest() {
        switch (protocolMsg) {
            case ASK_AVAILABILITY :
                // Not null assert
                assertNotNull(Protocol.encodeAskAvailability());
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeAskAvailability());
                break;
            case ASK_CMD :
                // Integer to Command
                Command command = Command.FORWARD;
                switch (cmdGiven[0]) {
                    case 1 :
                        command = Command.RIGHT;
                        break;
                    case 2 :
                        command = Command.LEFT;
                        break;
                    case 3 :
                        command = Command.BACKWARD;
                        break;
                    case 4 :
                        command = Command.STOP;
                        break;
                }

                // Not null assert
                assertNotNull(Protocol.encodeAskCmd(command));
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeAskCmd(command));
                break;

            case SET_STATE :
                // Integer to SelectedState
                Robot.SelectionState state = (cmdGiven[0]==2) ?
                        Robot.SelectionState.SELECTED: Robot.SelectionState.UNSELECTED;

                // Not null assert
                assertNotNull(Protocol.encodeSetState(state));
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeSetState(state));
                break;
            case ASK_MODE :
                // Not null assert
                assertNotNull(Protocol.encodeAskMode());
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeAskMode());
                break;
            case SET_MODE :
                // Not null assert
                assertNotNull(Protocol.encodeSetMode(cmdGiven[0], cmdGiven[1], cmdGiven[2], cmdGiven[3]));
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeSetMode(cmdGiven[0], cmdGiven[1], cmdGiven[2], cmdGiven[3]));
                break;
            case ASK_LOGS :
                // Not null assert
                assertNotNull(Protocol.encodeAskLogs());
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeAskLogs());
                break;
            case ALERT :
                // Not null assert
                assertNotNull(Protocol.encodeAlert(cmdGiven[0]));
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeAlert(cmdGiven[0]));
                break;
            case ASK_TO_DISCONNECT :
                // Not null assert
                assertNotNull(Protocol.encodeAskToDisconnect());
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeAskToDisconnect());
                break;
            case SET_CURRENT_TIME :
                // Not null assert
                assertNotNull(Protocol.encodeSetCurrentTime(cmdGiven[0], cmdGiven[1], cmdGiven[2],
                        cmdGiven[3], cmdGiven[4], cmdGiven[5]));
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeSetCurrentTime(cmdGiven[0], cmdGiven[1],
                        cmdGiven[2], cmdGiven[3], cmdGiven[4], cmdGiven[5]));
                break;
            case SET_IP_PORT :
                // Not null assert
                assertNotNull(Protocol.encodeSetupGUIInfo("127.23.1.52", cmdGiven[0]));
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeSetupGUIInfo("127.23.1.52", cmdGiven[0]));
                break;
            case LOGS_RECEIVED :
                // Not null assert
                assertNotNull(Protocol.encodeLogsReceived());
                // Array equals assert
                assertArrayEquals(frameExp, Protocol.encodeLogsReceived());
                break;
            default :
                break;
        }
    }
}