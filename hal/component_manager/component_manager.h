#pragma once
#include "remote_control_agent.h"
#include "joystick_driver_hw504.h"
#include "radio_nrf24l01.h"
#include "rf24_provider.h"

#include <chrono>

namespace RobotRemoteController::Hal
{
class ComponentManager
{
public:
    ComponentManager();

    /**
     * @brief Starts the task schedule. Runs the main tasks of every component in a loop.
     * @warning This is a blocking call that will hold the main thread.
     * @returns False if the task schedule could not be started. This can happen if any component fails to start. Retruns true after the task schedule is stopped.
     */
    bool StartTaskSchedule();

    /**
     * @brief Orders the task schedule to stop.
     */
    void StopTaskSchedule();

    /**
     * @brief Indicates whether the task schedule is running.
     */
    bool IsTaskScheduleActive() const { return m_is_task_schedule_active; }

private:

    bool m_is_task_schedule_active { false };
    JoystickDriverHw504 m_joystick_driver;
    RadioNrf24l01 m_link_manager;
    Rf24Provider m_rf24_provider;
    App::RemoteControlAgent m_remote_control_agent;
};
} // namespace RobotRemoteController::Hal