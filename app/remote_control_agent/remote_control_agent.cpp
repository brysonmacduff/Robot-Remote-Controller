#include "remote_control_agent.h"

namespace RobotRemoteController::App
{
RemoteControlAgent::RemoteControlAgent(Hal::ILinkManager& link_manager, Hal::IJoystickDriver& joystick_driver)
: m_link_manager(link_manager)
, m_joystick_driver(joystick_driver)
{
}

void RemoteControlAgent::Run()
{
    UpdateMotionCommand();
}

void RemoteControlAgent::SetRobotMiddlewareCallbacks()
{
    m_command_sender.SetSenderCallback([this](const RM::Packet& tx_packet)
    {
        // Forward the outbound robot middleware command packet to Link Manager
        m_link_manager.EnqueueTxPacket(tx_packet);
    });

    m_link_manager.SetRxPacketCallback([this](const RM::Packet& rx_packet)
    {
        // Forward the inbound robot middleware packet to be interpreted by the status receiver
        m_status_receiver.Receive(rx_packet);
    });

    m_status_receiver.SetMotionStatusReceiverCallback([](const RM::MotionStatus& motion_status)
    {
        (void)motion_status;
        // TODO(Bryson): Publish motion status updates to listeners
    });

}

RM::MotionControlProtocol::MotionState RemoteControlAgent::ConvertJoystickStateToMotionState(const Hal::JoystickState& joystick_state)
{
    using MotionState = RM::MotionControlProtocol::MotionState;
    constexpr float IDLE_STATE_EPSILON = 0.1;
    constexpr float DIVISOR_THRESHOLD = 0.001;

    // If the joystick is near rest position, then interpret the input command as "idle"
    if(std::abs(joystick_state.x_axis) < IDLE_STATE_EPSILON && std::abs(joystick_state.y_axis) < IDLE_STATE_EPSILON)
    {
        return MotionState::IDLE;
    }

    float polar_angle_radians = 0;
    const float cartesian_y = std::abs(joystick_state.y_axis);
    const float cartesian_x = std::abs(joystick_state.x_axis);

    if(cartesian_x < DIVISOR_THRESHOLD)
    {
        // In this case, "x" is effecively zero so the angle would be about 90 degrees (pi/2 radians)
        polar_angle_radians = std::numbers::pi_v<float> / 2;
    }
    else
    {
        // If "x" is not zero, then calculate the angle from the positive x-axis to the vector formed by coordinate [x,y] and the origin
        polar_angle_radians = std::asin(cartesian_y/cartesian_x);
    }

    // If the joystick is resting mostly in the y-axis direction, then interpret this as a "translate forward" input command
    if(polar_angle_radians > std::numbers::pi_v<float> / 4)
    {
        return MotionState::TRANSLATE_FORWARD;
    }

    // If the joystick is mostly resting in the x-axis direction, then it must be interpreted now as a rotational input command
    
    // If the joystick is pointing to the negative side x-axis, then interpet it as a "rotate anti-clockwise" input command
    if(joystick_state.x_axis < 0)
    {
        return MotionState::ROTATE_ANTICLOCKWISE;
    }
    
    // If the joystick is pointing to the positive x-axis, then interpet it as a "rotate clockwise" input command
    return MotionState::ROTATE_CLOCKWISE;
}

void RemoteControlAgent::UpdateMotionCommand()
{
    const Hal::JoystickState& joystick_state = m_joystick_driver.GetJoystickState();

    RM::MotionCommand motion_command;

    motion_command.motion_state = ConvertJoystickStateToMotionState(joystick_state);

    /* Always dispatch a motion command even if there is no change in joystick state.
     * This class does not assume that the radio-frequency layer is using a protocol that guarantees message delivery.
     * Therefore, this class should always dispatch a continuous stream of motion commands at a fixed frequency.
     */
    m_command_sender.Send(motion_command);
}

} // namespace RobotRemoteController::App