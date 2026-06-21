#pragma once

namespace RobotRemoteController::Hal
{

struct JoystickState
{
    float x_axis = 0;
    float y_axis = 0;
};

class IJoystickDriver
{
public:
    virtual ~IJoystickDriver() = default;

    virtual const JoystickState& GetJoystickState() const = 0;
};
} // namespace RobotRemoteController::Hal