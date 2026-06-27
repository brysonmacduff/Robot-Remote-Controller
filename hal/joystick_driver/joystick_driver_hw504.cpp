#include "joystick_driver_hw504.h"

namespace RobotRemoteController::Hal
{

JoystickDriverHw504::JoystickDriverHw504(AdcGpio x_axis_gpio, AdcGpio y_axis_gpio, uint8_t button_gpio)
: m_x_axis_gpio(x_axis_gpio)
, m_y_axis_gpio(y_axis_gpio)
, m_button_gpio(button_gpio)
{
    m_is_button_enabled = true;
}

JoystickDriverHw504::JoystickDriverHw504(AdcGpio x_axis_gpio, AdcGpio y_axis_gpio)
: m_x_axis_gpio(x_axis_gpio)
, m_y_axis_gpio(y_axis_gpio)
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

void JoystickDriverHw504::InitializeGpioForSampling()
{
    adc_init();

    adc_gpio_init(static_cast<uint8_t>(m_x_axis_gpio)); // x-axis
    adc_gpio_init(static_cast<uint8_t>(m_y_axis_gpio)); // y-axis

    if(m_is_button_enabled)
    {
        gpio_init(m_button_gpio); // button
        gpio_set_dir(m_button_gpio, GPIO_IN);
        gpio_pull_up(m_button_gpio);
    }
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

    // The y-axis normalized voltage must be reverse (multiple by -1) because the joystick's y-axis is backwards (-_-)
    m_joystick_state.y_axis = NormalizeVoltage(input_voltage) * -1;
}

void JoystickDriverHw504::SampleButton()
{
    if(not m_is_button_enabled)
    {
        return;
    }

    m_joystick_state.is_button_pressed = not gpio_get(m_button_gpio);
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