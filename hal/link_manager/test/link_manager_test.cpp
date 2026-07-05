#include "radio_nrf24l01.h"

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <hardware/watchdog.h>

#include <chrono>

using namespace RobotRemoteController::Hal;

static constexpr std::chrono::milliseconds IO_SETUP_DELAY {3000};
static constexpr std::chrono::milliseconds TASK_LOOP_SLEEP_DURATION {50};

//RF24 radio(22, 17);

int main()
{
    if(not stdio_init_all())
    {
        printf("Failed to initialize stdio.\n");
        return 1;
    }

    if (cyw43_arch_init() != 0) {
        printf("Failed to initialize cyw43\n");
        return 1;
    }

    sleep_ms(static_cast<uint32_t>(IO_SETUP_DELAY.count()));

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    printf("Before rf24 radio instantiation\n");

    RF24 rf24_radio;

    printf("Before wrapper radio instantiation\n");

    RadioNrf24l01 radio(rf24_radio);

    printf("Before wrapper radia initialization\n");

    if(not radio.InitializeRadio())
    {
        printf("Failed to initialize radio\n");
        return 1;
    }

    printf("Initialization complete\n");

    uint32_t loop_counter = 0;

    while(true)
    {
        radio.Run(std::chrono::steady_clock::now());
        sleep_ms(static_cast<uint32_t>(TASK_LOOP_SLEEP_DURATION.count()));
        printf("Loop iteration {%d}\n",loop_counter);
        ++loop_counter;
    }

    /*
    while(true)
    {
        sleep_ms(1000);
        printf("loop count: {%d}\n",loop_counter);
        ++loop_counter;
    }
    */
    
    return 0;
}
