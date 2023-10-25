package project.src;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;

import project.src.model.com.*;

@RunWith(Suite.class)
@Suite.SuiteClasses({EncodeTest.class, DecodeSetAvailabilityTest.class, DecodeAckDisconnectionTest.class
        , DecodeAlertTest.class, DecodeSetLogsTest.class, DecodeSetModeTest.class
        , DecodeSetRadarTest.class})
public class ProtocolTest {
}
