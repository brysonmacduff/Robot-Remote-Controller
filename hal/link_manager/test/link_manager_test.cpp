#include "radio_nrf24l01.h"

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <hardware/watchdog.h>

#include <chrono>

using namespace RobotRemoteController::Hal;

static constexpr std::chrono::milliseconds TASK_LOOP_SLEEP_DURATION {100};

int main()
{
    if(not stdio_init_all())
    {
        printf("Failed to initialize stdio.\n");
        return 1;
    }

    if (cyw43_arch_init() != 0) 
    {
        printf("Failed to initialize cyw43\n");
        return 1;
    }

    bool is_led_active = true;

    // Wait for serial I/O initialization
    sleep_ms(3000);

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, is_led_active);

    printf("Before rf24 radio instantiation\n");

    RF24 rf24_radio;

    RadioNrf24l01 radio(rf24_radio);

    sleep_ms(100);

    printf("after radio instantiation\n");

    while(not radio.InitializeRadio())
    {
        printf("Failed to initialize radio\n");
        sleep_ms(100);
    }

    sleep_ms(100);

    printf("After rf24 radio initialization\n");

    //watchdog_enable(10000, false);

    //printf("Initialization complete\n");

    uint8_t loop_counter = 0;

    printf("loop checkpoint E\n");

    /*
    RobotMiddleware::Packet packet {};
    for(int i = 0; i < 10; ++i)
    {
        packet.sequence_number++;
        radio.EnqueueTxPacket(packet);
        sleep_ms(1000);
    }*/

    /*
    RobotMiddleware::Packet packet {};
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    packet.sequence_number++;
    radio.EnqueueTxPacket(packet);
    sleep_ms(100);
    */
    
    while(true)
    {
        //printf("loop checkpoint 1\n");

        //packet.sequence_number++;
        //radio.EnqueueTxPacket(packet);

        const auto current_time = std::chrono::steady_clock::time_point{std::chrono::microseconds {time_us_64()}};
        
        //radio.Run(current_time);

        //printf("loop checkpoint 2\n");

        sleep_ms(static_cast<uint32_t>(TASK_LOOP_SLEEP_DURATION.count()));

        //printf("loop checkpoint 3\n");

        //printf("loop checkpoint 4\n");

        //printf("loop checkpoint 5\n");

        //printf("loop checkpoint 6\n");

        //++loop_counter;

        //printf("current time: {%llu}\n",current_time.time_since_epoch().count());

        // THIS WILL CAUSE A FIRMWARE CRASH

        is_led_active = not is_led_active;
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, is_led_active);
    }
    

    return 0;
}
