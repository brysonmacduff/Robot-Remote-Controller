#pragma once

#include "i_joystick_driver.h"
#include <cstdint>

namespace RobotRemoteController::Hal
{
/**
 * @brief This class functions as driver for a two-axis joystick.
 */
class JoystickDriver : public IJoystickDriver
{
public:
    JoystickDriver(uint8_t gpio);

    const JoystickState& GetJoystickState() const override;

private:
    uint8_t m_gpio { 0 };
    JoystickState m_joystick_state { .x_axis = 0, .y_axis = 0};
};
} // namespace RobotRemoteController::Hal