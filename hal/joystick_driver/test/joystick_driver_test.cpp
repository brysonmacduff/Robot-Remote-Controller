#include "joystick_driver_hw504.h"
#include "adc_gpio_utility.h"

#include <chrono>
#include <cstdio>

#include <pico/cyw43_arch.h>

using namespace RobotRemoteController::Hal;

static constexpr std::chrono::milliseconds SAMPLE_SLEEP_DURATION {50};
static constexpr std::chrono::milliseconds USB_SETUP_SLEEP_DURATION { 2000 };
static constexpr uint8_t BUTTON_GPIO = 22;

int main()
{
    if(not stdio_init_all())
    {
        printf("Failed to initialize stdio.\n");
        return 1;
    }

    sleep_ms(USB_SETUP_SLEEP_DURATION.count()); // allow USB to enumerate

    if(cyw43_arch_init() != 0)
    {
        printf("Failed to initialize cyw43.\n");
        return 1;
    }

    // Note: GPIO initialization and overall hardware initialize for driver software could be consolidated in ComponentManager

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    printf("Initializing ADC and GPIO...\n");

    JoystickDriverHw504 joystick_driver (AdcGpio::GPIO_26, AdcGpio::GPIO_27, BUTTON_GPIO);

    joystick_driver.InitializeGpioForSampling();

    printf("Done initializing hardware...\n");

    printf("Starting to sample joystick state...\n");

    while(true)
    {
        const JoystickState& joystick_state = joystick_driver.Sample();

        const std::string joystick_state_log = joystick_state.ToString() + "\n";

        printf(joystick_state_log.c_str());

        sleep_ms(static_cast<uint32_t>(SAMPLE_SLEEP_DURATION.count()));
    }

    return 0;
}