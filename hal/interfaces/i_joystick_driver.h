#pragma once
#include <string>
#include <sstream>

namespace RobotRemoteController::Hal
{

struct JoystickState
{
    float x_axis = 0;
    float y_axis = 0;
    bool is_button_pressed = false;

    std::string ToString() const
    {
        std::stringstream output;

        output << "{'x_axis': " << x_axis << ", 'y_axis': " << y_axis << ", 'button': ";

        if(is_button_pressed)
        {
            output << "true";
        }
        else
        {
            output << "false";
        }

        output << "}";

        return output.str();
    }
};

class IJoystickDriver
{
public:
    virtual ~IJoystickDriver() = default;

    /**
     * @brief Returns the last known joystick state.
     */
    virtual const JoystickState& GetJoystickState() const = 0;

    /**
     * @brief Samples the joystick state.
     */
    virtual const JoystickState& Sample() = 0;
};
} // namespace RobotRemoteController::Hal