#pragma once
#include "i_link_manager.h"
#include "i_joystick_driver.h"

#include <robot_middleware/command_sender.h>
#include <robot_middleware/status_receiver.h>
#include <robot_middleware/motion_control_protocol.h>

#include <chrono>
#include <cmath>
#include <numbers>

namespace RobotRemoteController::App
{

namespace RM = RobotMiddleware;

/**
 * @brief This class functions as a hub of command and status data between the radio-frequency link layer and peripheral
 * input devices.
 * @note This class assumes that the worker tasks which govern peripheral input devices and the RF layer are being driven
 * elsewhere such as in a task schedule. This class does not take responsibility for executing such tasks.
 */
class RemoteControlAgent
{
public:

    RemoteControlAgent(Hal::ILinkManager& link_manager, Hal::IJoystickDriver& joystick_driver);

    /**
     * @brief Runs the main task of this class. Drives the exchange of information between link manager and the joystick driver.
     * @note This function does not "drive" the sampling of the joystick state.
     * @note This function does not "run" the link manager state machine.
     */
    void Run();

private:
    Hal::ILinkManager& m_link_manager;
    Hal::IJoystickDriver& m_joystick_driver;

    RM::CommandSender m_command_sender;
    RM::StatusReceiver m_status_receiver;

    void SetRobotMiddlewareCallbacks();
    
    void UpdateMotionCommand();
    static RM::MotionControlProtocol::MotionState ConvertJoystickStateToMotionState(const Hal::JoystickState& joystick_state);
};
} // namespace App::RobotRemoteController