#include "radio_nrf24l01.h"

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

#include <chrono>

using namespace RobotRemoteController::Hal;

static constexpr std::chrono::milliseconds IO_SETUP_DELAY {3000};
static constexpr std::chrono::milliseconds TASK_LOOP_SLEEP_DURATION {50};

int main()
{
    stdio_init_all();
    sleep_ms(static_cast<uint32_t>(IO_SETUP_DELAY.count()));

    if (cyw43_arch_init() != 0) {
        printf("Failed to initialize cyw43\n");
        return 1;
    }

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    RadioNrf24l01 radio;

    if(not radio.InitializeRadio())
    {
        printf("Failed to initialize radio\n");
        return 1;
    }

    printf("Radio initialization complete\n");

    while(true)
    {
        radio.Run(std::chrono::steady_clock::now());
        sleep_ms(static_cast<uint32_t>(TASK_LOOP_SLEEP_DURATION.count()));
    }

    return 0;
}
