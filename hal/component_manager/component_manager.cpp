#include "component_manager.h"

namespace RobotRemoteController::Hal
{
ComponentManager::ComponentManager()
: m_joystick_driver(JoystickDriverHw504(AdcGpio::GPIO_26, AdcGpio::GPIO_27))
, m_link_manager(RadioNrf24l01(m_rf24_provider.GetInstance()))
, m_remote_control_agent(App::RemoteControlAgent(m_link_manager, m_joystick_driver))
{
}

bool ComponentManager::StartTaskSchedule()
{
    if(m_is_task_schedule_active)
    {
        return true;
    }

    if(not m_link_manager.InitializeRadio())
    {
        return false;
    }

    m_joystick_driver.InitializeGpioForSampling();

    m_is_task_schedule_active = true;

    while(m_is_task_schedule_active)
    {
        const auto current_time = std::chrono::steady_clock::now();

        m_joystick_driver.Sample();
        m_remote_control_agent.Run();
        m_link_manager.Run(current_time);
    }

    return m_is_task_schedule_active;
}

void ComponentManager::StopTaskSchedule()
{
    m_is_task_schedule_active = false;
}

} // namespace RobotRemoteController::Hal