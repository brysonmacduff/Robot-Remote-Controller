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
    constexpr std::chrono::milliseconds TASK_SCHEDULE_SLEEP_DURATION {10};
    constexpr std::chrono::milliseconds LINK_MANAGER_TASK_SLEEP_DURATION {5};

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
        sleep_ms(TASK_SCHEDULE_SLEEP_DURATION.count());
        
        // Sample the joystick state
        m_joystick_driver.Sample();

        // Let remote control agent dispatch the current joystick state as a motion command
        m_remote_control_agent.Run();

        // Run the Link Manager until it has finished broadcasting the outbound packets that were enqueued during this task schedule iteration
        while(m_link_manager.GetTxMessageQueueCount() > 0)
        {
            auto current_time = std::chrono::steady_clock::now();
            m_link_manager.Run(current_time);
            sleep_ms(LINK_MANAGER_TASK_SLEEP_DURATION.count());
        }
    }

    return m_is_task_schedule_active;
}

void ComponentManager::StopTaskSchedule()
{
    m_is_task_schedule_active = false;
}

} // namespace RobotRemoteController::Hal