#include "joystick_driver_hw504.h"
#include "adc_gpio_utility.h"

#include <chrono>
#include <cstdio>

using namespace RobotRemoteController::Hal;

static constexpr std::chrono::milliseconds SLEEP_DURATION {50};

int main()
{
    if(not stdio_init_all())
    {
        printf("Failed to initialize. Exiting.");
        return 1;
    }

    adc_gpio_init(static_cast<uint8_t>(ADC_GPIO_INPUT_TABLE.at(AdcGpio::GPIO_26))); // x-axis
    adc_gpio_init(static_cast<uint8_t>(ADC_GPIO_INPUT_TABLE.at(AdcGpio::GPIO_27))); // y-axis
    adc_gpio_init(static_cast<uint8_t>(ADC_GPIO_INPUT_TABLE.at(AdcGpio::GPIO_28))); // button

    JoystickDriverHw504 joystick_driver (AdcGpio::GPIO_26,AdcGpio::GPIO_26,AdcGpio::GPIO_26);

    while(true)
    {
        const JoystickState& joystick_state = joystick_driver.Sample();

        printf(joystick_state.ToString().c_str());

        sleep_ms(static_cast<uint32_t>(SLEEP_DURATION.count()));
    }

    return 0;
}