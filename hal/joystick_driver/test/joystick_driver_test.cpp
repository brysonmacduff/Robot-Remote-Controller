#include "joystick_driver_hw504.h"
#include "adc_gpio_utility.h"

#include <chrono>
#include <cstdio>

#include <pico/cyw43_arch.h>

using namespace RobotRemoteController::Hal;

static constexpr std::chrono::milliseconds SLEEP_DURATION {50};

int main()
{
    if(not stdio_init_all())
    {
        printf("Failed to initialize. Exiting.");
        return 1;
    }

    sleep_ms(2000); // allow USB to enumerate

    printf("Start of firmware...\n");

    cyw43_arch_init();

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    printf("Initializing...\n");

    adc_init();

    adc_gpio_init(26); // x-axis
    adc_gpio_init(27); // y-axis
    adc_gpio_init(28); // button

    printf("Done initializing hardware...\n");

    JoystickDriverHw504 joystick_driver (AdcGpio::GPIO_26,AdcGpio::GPIO_27,AdcGpio::GPIO_28);

    printf("Starting to sample joystick state...\n");

    while(true)
    {
        const JoystickState& joystick_state = joystick_driver.Sample();

        const std::string joystick_state_log = joystick_state.ToString() + "\n";

        printf(joystick_state_log.c_str());

        sleep_ms(static_cast<uint32_t>(SLEEP_DURATION.count()));
    }

    return 0;
}