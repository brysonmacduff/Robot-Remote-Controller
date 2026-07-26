#include "remote_control_agent.h"
#include "i_link_manager.h"

#include <gtest/gtest.h>

namespace
{
using namespace RobotRemoteController;
using namespace RobotRemoteController::App;

class FakeLinkManager : public Hal::ILinkManager
{
public:
    void EnqueueTxPacket(const RobotMiddleware::Packet& packet) override
    {
        m_received_tx_packets.push_back(packet);
    }

    void SetRxPacketCallback(RxPacketCallback callback) override
    {
        m_rx_callback = std::move(callback);
    }

    void TriggerRxCallback(const RobotMiddleware::Packet& rx_packet)
    {
        m_rx_callback(rx_packet);
    }

    const std::vector<RobotMiddleware::Packet>& GetReceivedTxPackets() const
    {
        return m_received_tx_packets;
    }

private:
    RxPacketCallback m_rx_callback;
    std::vector<RobotMiddleware::Packet> m_received_tx_packets;
};

class FakeJoystickDriver : public Hal::IJoystickDriver
{
public:
    const Hal::JoystickState& GetJoystickState() const override
    {
        return m_joystick_state;
    }

    const Hal::JoystickState& Sample() override
    {
        return m_joystick_state;
    }

    void SetJoystickState(const Hal::JoystickState& joystick_state)
    {
        m_joystick_state = joystick_state;
    }

private:

    Hal::JoystickState m_joystick_state;
};

TEST(RemoteControlAgentTest, RelayMotionCommandIdle)
{
    FakeJoystickDriver joystick_driver;
    FakeLinkManager link_manager;
    RemoteControlAgent remote_control_agent (link_manager, joystick_driver);

    EXPECT_TRUE(link_manager.GetReceivedTxPackets().empty());

    // Set joystick state to be "idle"
    Hal::JoystickState joystick_state;
    joystick_state.x_axis = 0;
    joystick_state.y_axis = 0;

    joystick_driver.SetJoystickState(joystick_state);

    remote_control_agent.Run();

    ASSERT_EQ(link_manager.GetReceivedTxPackets().size(), 1);

    const RobotMiddleware::Packet rx_packet = link_manager.GetReceivedTxPackets().front();
    
    const auto motion_command_opt = RobotMiddleware::MotionControlProtocol::ExtractMotionCommand(rx_packet);

    ASSERT_TRUE(motion_command_opt.has_value());

    EXPECT_EQ(motion_command_opt.value().motion_state, RobotMiddleware::MotionControlProtocol::MotionState::IDLE);
}

TEST(RemoteControlAgentTest, RelayMotionCommandTranslateForward)
{
    FakeJoystickDriver joystick_driver;
    FakeLinkManager link_manager;
    RemoteControlAgent remote_control_agent (link_manager, joystick_driver);

    EXPECT_TRUE(link_manager.GetReceivedTxPackets().empty());

    // Set joystick state to be "translate forward"
    Hal::JoystickState joystick_state;
    joystick_state.x_axis = 0;
    joystick_state.y_axis = 0.5;

    joystick_driver.SetJoystickState(joystick_state);

    remote_control_agent.Run();

    ASSERT_EQ(link_manager.GetReceivedTxPackets().size(), 1);

    const RobotMiddleware::Packet rx_packet = link_manager.GetReceivedTxPackets().front();
    
    const auto motion_command_opt = RobotMiddleware::MotionControlProtocol::ExtractMotionCommand(rx_packet);

    ASSERT_TRUE(motion_command_opt.has_value());

    EXPECT_EQ(motion_command_opt.value().motion_state, RobotMiddleware::MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
}

TEST(RemoteControlAgentTest, RelayMotionCommandRotateAntiClockwise)
{
    FakeJoystickDriver joystick_driver;
    FakeLinkManager link_manager;
    RemoteControlAgent remote_control_agent (link_manager, joystick_driver);

    EXPECT_TRUE(link_manager.GetReceivedTxPackets().empty());

    // Set joystick state to be "rotate anti-clockwise"
    Hal::JoystickState joystick_state;
    joystick_state.x_axis = -0.5;
    joystick_state.y_axis = 0;

    joystick_driver.SetJoystickState(joystick_state);

    remote_control_agent.Run();

    ASSERT_EQ(link_manager.GetReceivedTxPackets().size(), 1);

    const RobotMiddleware::Packet rx_packet = link_manager.GetReceivedTxPackets().front();
    
    const auto motion_command_opt = RobotMiddleware::MotionControlProtocol::ExtractMotionCommand(rx_packet);

    ASSERT_TRUE(motion_command_opt.has_value());

    EXPECT_EQ(motion_command_opt.value().motion_state, RobotMiddleware::MotionControlProtocol::MotionState::ROTATE_ANTICLOCKWISE);
}

TEST(RemoteControlAgentTest, RelayMotionCommandRotateClockwise)
{
    FakeJoystickDriver joystick_driver;
    FakeLinkManager link_manager;
    RemoteControlAgent remote_control_agent (link_manager, joystick_driver);

    EXPECT_TRUE(link_manager.GetReceivedTxPackets().empty());

    // Set joystick state to be "rotate clockwise"
    Hal::JoystickState joystick_state;
    joystick_state.x_axis = 0.5;
    joystick_state.y_axis = 0;

    joystick_driver.SetJoystickState(joystick_state);

    remote_control_agent.Run();

    ASSERT_EQ(link_manager.GetReceivedTxPackets().size(), 1);

    const RobotMiddleware::Packet rx_packet = link_manager.GetReceivedTxPackets().front();
    
    const auto motion_command_opt = RobotMiddleware::MotionControlProtocol::ExtractMotionCommand(rx_packet);

    ASSERT_TRUE(motion_command_opt.has_value());

    EXPECT_EQ(motion_command_opt.value().motion_state, RobotMiddleware::MotionControlProtocol::MotionState::ROTATE_CLOCKWISE);
}

TEST(RemoteControlAgentTest, InterpretFuzzyMotionCommandTranslateForward)
{
    FakeJoystickDriver joystick_driver;
    FakeLinkManager link_manager;
    RemoteControlAgent remote_control_agent (link_manager, joystick_driver);

    EXPECT_TRUE(link_manager.GetReceivedTxPackets().empty());

    Hal::JoystickState joystick_state;

    // "Translate forward"
    joystick_state.x_axis = 0.2;
    joystick_state.y_axis = 0.7;
    joystick_driver.SetJoystickState(joystick_state);

    remote_control_agent.Run();

    ASSERT_EQ(link_manager.GetReceivedTxPackets().size(), 1);

    const RobotMiddleware::Packet rx_packet = link_manager.GetReceivedTxPackets().front();
    
    const auto motion_command_opt = RobotMiddleware::MotionControlProtocol::ExtractMotionCommand(rx_packet);

    ASSERT_TRUE(motion_command_opt.has_value());

    EXPECT_EQ(motion_command_opt.value().motion_state, RobotMiddleware::MotionControlProtocol::MotionState::TRANSLATE_FORWARD);
}

TEST(RemoteControlAgentTest, InterpretFuzzyMotionCommandRotateClockwise)
{
    FakeJoystickDriver joystick_driver;
    FakeLinkManager link_manager;
    RemoteControlAgent remote_control_agent (link_manager, joystick_driver);

    EXPECT_TRUE(link_manager.GetReceivedTxPackets().empty());

    Hal::JoystickState joystick_state;

    // "Rotate clockwise"
    joystick_state.x_axis = 0.6;
    joystick_state.y_axis = 0.3;
    joystick_driver.SetJoystickState(joystick_state);

    remote_control_agent.Run();

    ASSERT_EQ(link_manager.GetReceivedTxPackets().size(), 1);

    const RobotMiddleware::Packet rx_packet = link_manager.GetReceivedTxPackets().front();
    
    const auto motion_command_opt = RobotMiddleware::MotionControlProtocol::ExtractMotionCommand(rx_packet);

    ASSERT_TRUE(motion_command_opt.has_value());

    EXPECT_EQ(motion_command_opt.value().motion_state, RobotMiddleware::MotionControlProtocol::MotionState::ROTATE_CLOCKWISE);
}

TEST(RemoteControlAgentTest, InterpretFuzzyMotionCommandRotateAntiClockwise)
{
    FakeJoystickDriver joystick_driver;
    FakeLinkManager link_manager;
    RemoteControlAgent remote_control_agent (link_manager, joystick_driver);

    EXPECT_TRUE(link_manager.GetReceivedTxPackets().empty());

    Hal::JoystickState joystick_state;

    // "Rotate anti-clockwise"
    joystick_state.x_axis = -0.6;
    joystick_state.y_axis = 0.5;
    joystick_driver.SetJoystickState(joystick_state);

    remote_control_agent.Run();

    ASSERT_EQ(link_manager.GetReceivedTxPackets().size(), 1);

    const RobotMiddleware::Packet rx_packet = link_manager.GetReceivedTxPackets().front();
    
    const auto motion_command_opt = RobotMiddleware::MotionControlProtocol::ExtractMotionCommand(rx_packet);

    ASSERT_TRUE(motion_command_opt.has_value());

    EXPECT_EQ(motion_command_opt.value().motion_state, RobotMiddleware::MotionControlProtocol::MotionState::ROTATE_ANTICLOCKWISE);
}

TEST(RemoteControlAgentTest, InterpretFuzzyMotionCommandIdle)
{
    FakeJoystickDriver joystick_driver;
    FakeLinkManager link_manager;
    RemoteControlAgent remote_control_agent (link_manager, joystick_driver);

    EXPECT_TRUE(link_manager.GetReceivedTxPackets().empty());

    Hal::JoystickState joystick_state;

    // "Idle"
    joystick_state.x_axis = -0.04;
    joystick_state.y_axis = 0.09;
    joystick_driver.SetJoystickState(joystick_state);

    remote_control_agent.Run();

    ASSERT_EQ(link_manager.GetReceivedTxPackets().size(), 1);

    const RobotMiddleware::Packet rx_packet = link_manager.GetReceivedTxPackets().front();
    
    const auto motion_command_opt = RobotMiddleware::MotionControlProtocol::ExtractMotionCommand(rx_packet);

    ASSERT_TRUE(motion_command_opt.has_value());

    EXPECT_EQ(motion_command_opt.value().motion_state, RobotMiddleware::MotionControlProtocol::MotionState::IDLE);
}

}