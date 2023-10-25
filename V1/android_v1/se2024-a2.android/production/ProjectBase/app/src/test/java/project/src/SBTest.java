package project.src;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;

import project.src.model.com.*;
import project.src.model.core.*;

@RunWith(Suite.class)
@Suite.SuiteClasses({ProtocolTest.class, DispatchTest.class,
        OperatingModeTest.class, RobotTest.class, GUISecretaryTest.class, GUISecretaryPickRobotTest.class,
        CameraPacketTest.class})
public class SBTest {
}
