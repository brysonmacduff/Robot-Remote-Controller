#include "joystick_driver_hw504.h"

namespace RobotRemoteController::Hal
{

JoystickDriverHw504::JoystickDriverHw504(AdcGpio x_axis_gpio, AdcGpio y_axis_gpio, AdcGpio button_gpio)
: m_x_axis_gpio(x_axis_gpio)
, m_y_axis_gpio(y_axis_gpio)
, m_button_gpio(button_gpio)
{
}

const JoystickState& JoystickDriverHw504::GetJoystickState() const
{
    return m_joystick_state;
}

const JoystickState& JoystickDriverHw504::Sample()
{
    SampleAxisX();
    SampleAxisY();
    SampleButton();

    return m_joystick_state;
}

float JoystickDriverHw504::SampleInputVoltage(AdcGpio gpio)
{
    const uint8_t adc_input = ADC_GPIO_INPUT_TABLE.at(gpio);

    // Prepeare the ADC to read from the correct GPIO
    adc_select_input(adc_input);

    const uint16_t raw_adc_sample = adc_read();

    return static_cast<float>(raw_adc_sample) * ADC_SAMPLE_TO_VOLTAGE_FACTOR;
}

void JoystickDriverHw504::SampleAxisX()
{
    const float input_voltage = SampleInputVoltage(m_x_axis_gpio);

    m_joystick_state.x_axis = NormalizeVoltage(input_voltage);
}

void JoystickDriverHw504::SampleAxisY()
{
    const float input_voltage = SampleInputVoltage(m_y_axis_gpio);

    m_joystick_state.y_axis = NormalizeVoltage(input_voltage);
}

void JoystickDriverHw504::SampleButton()
{
    const float input_voltage = SampleInputVoltage(m_button_gpio);

    m_joystick_state.is_button_pressed = IsFloatEqual(input_voltage, VOLTAGE_HIGH, FLOAT_COMPARISON_TOLERANCE);
}

float JoystickDriverHw504::NormalizeVoltage(float voltage)
{
    const float percentage = voltage / VOLTAGE_HIGH;
    const float normalized_voltage = NORMALIZED_LOWER_BOUND + percentage * NORMALIZED_RANGE;

    return normalized_voltage;
}

bool JoystickDriverHw504::IsFloatEqual(float first, float second, float tolerance)
{
    return std::abs(first - second) <= tolerance;
}

} // namespace RobotRemoteController::Hal