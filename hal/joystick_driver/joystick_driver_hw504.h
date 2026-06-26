#pragma once

#include "i_joystick_driver.h"
#include "adc_gpio_utility.h"

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/adc.h>

#include <cstdint>
#include <cmath>

namespace RobotRemoteController::Hal
{
/**
 * @brief This class functions as a driver for a two-axis joystick known as HW-504.
 * @note The pico supports ADC sampling for gpio pins 26 to 29 only.
 * @warning This class does not initialize pico hardware. It is assumed that ADC/gpio hardware is managed elsewhere. This class will not function correctly if the ADC
 * hardware and corresponding GPIO pins are not initialized first.
 */
class JoystickDriverHw504 : public IJoystickDriver
{
public:
    JoystickDriverHw504(AdcGpio x_axis_gpio, AdcGpio y_axis_gpio, AdcGpio button_gpio);

    const JoystickState& GetJoystickState() const override;

    const JoystickState& Sample() override;

private:
    // The ADC read value is meant to be interpreted as a 12 bit number with an upper limit voltage of 3.3 V
    static constexpr float ADC_SAMPLE_TO_VOLTAGE_FACTOR = 3.3f / (1 << 12);
    static constexpr float VOLTAGE_HIGH = 3.3f;
    static constexpr float VOLTAGE_LOW = 0;
    static constexpr float FLOAT_COMPARISON_TOLERANCE = 0.1f;
    static constexpr float NORMALIZED_RANGE = 2;
    static constexpr float NORMALIZED_LOWER_BOUND = -1;
    static constexpr float NORMALIZED_UPPER_BOUND = 1;
    
    AdcGpio m_x_axis_gpio;
    AdcGpio m_y_axis_gpio;
    AdcGpio m_button_gpio;
    JoystickState m_joystick_state { .x_axis = 0, .y_axis = 0, .is_button_pressed = false};

    float SampleInputVoltage(AdcGpio gpio);
    void SampleAxisX();
    void SampleAxisY();
    void SampleButton();
    static float NormalizeVoltage(float voltage);
    static bool IsFloatEqual(float first, float second, float tolerance);
};
} // namespace RobotRemoteController::Hal