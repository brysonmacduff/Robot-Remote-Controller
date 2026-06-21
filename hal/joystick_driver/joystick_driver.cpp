#include "joystick_driver.h"

namespace RobotRemoteController::Hal
{

JoystickDriver::JoystickDriver(uint8_t gpio)
: m_gpio(gpio)
{
}

const JoystickState &JoystickDriver::GetJoystickState() const
{
    return m_joystick_state;
}

} // namespace RobotRemoteController::Hal