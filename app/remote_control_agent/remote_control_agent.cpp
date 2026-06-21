#include "remote_control_agent.h"

namespace RobotRemoteController::App
{
RemoteControlAgent::RemoteControlAgent(Hal::ILinkManager& link_manager, Hal::IJoystickDriver& joystick_driver)
: m_link_manager(link_manager)
, m_joystick_driver(joystick_driver)
{
}

void RemoteControlAgent::Run(std::chrono::steady_clock::time_point current_time)
{
    (void)current_time;
}
} // namespace RobotRemoteController::App