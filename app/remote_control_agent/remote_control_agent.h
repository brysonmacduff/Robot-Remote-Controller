#pragma once
#include "i_link_manager.h"
#include "i_joystick_driver.h"

#include <chrono>

namespace RobotRemoteController::App
{
class RemoteControlAgent
{
public:

    RemoteControlAgent(Hal::ILinkManager& link_manager, Hal::IJoystickDriver& joystick_driver);

    /**
     * @brief Runs the main task of this class. Drives the exchange of information between link manager and the joystick driver.
     */
    void Run(std::chrono::steady_clock::time_point current_time);

private:
    Hal::ILinkManager& m_link_manager;
    Hal::IJoystickDriver& m_joystick_driver;
};
} // namespace App::RobotRemoteController