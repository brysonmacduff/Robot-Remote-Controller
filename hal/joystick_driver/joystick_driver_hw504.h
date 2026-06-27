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
 * @warning This class does not validate that the constructor's gpio pin configuration is going to work.
 */
class JoystickDriverHw504 : public IJoystickDriver
{
public:
    /**
     * @brief Use this constructor if the joystick driver should sample joystick button input.
     * @note The "x" and "y" axis inputs must use ADC-compatible gpio pins, but the button can be assigned to any GPIO pin.
     * @example A stable confuguration is: JoystickDriverHw504(AdcGpio::GPIO_26, AdcGpio::GPIO_27, 22)
     */
    JoystickDriverHw504(AdcGpio x_axis_gpio, AdcGpio y_axis_gpio, uint8_t button_gpio);

    /**
     * @brief Use this constructor if the joystick driver should ignore joystick button input.
     */
    JoystickDriverHw504(AdcGpio x_axis_gpio, AdcGpio y_axis_gpio);
    
    /**
     * @brief Returns the last known joystick state.
     */
    const JoystickState& GetJoystickState() const override;

    /**
     * @brief Reads the input voltages from the joystick to determine the joystick state.
     */
    const JoystickState& Sample() override;

    /**
     * @brief Initializes the GPIOs for reading input voltage from the joystick.
     * @warning This must be called before sampling begins. Otherwise, sampling the input voltages will fail.
     */
    void InitializeGpioForSampling();

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
    uint8_t m_button_gpio {0};
    bool m_is_button_enabled { false };
    JoystickState m_joystick_state { .x_axis = 0, .y_axis = 0, .is_button_pressed = false};

    float SampleInputVoltage(AdcGpio gpio);
    void SampleAxisX();
    void SampleAxisY();
    void SampleButton();
    static float NormalizeVoltage(float voltage);
    static bool IsFloatEqual(float first, float second, float tolerance);
};
} // namespace RobotRemoteController::Hal