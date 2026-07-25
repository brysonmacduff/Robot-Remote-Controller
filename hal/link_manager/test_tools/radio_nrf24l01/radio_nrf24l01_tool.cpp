#include "radio_nrf24l01.h"
 
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

#include <chrono>

using namespace RobotRemoteController::Hal;

constexpr uint32_t TASK_SCHEDULE_SLEEP_INTERVAL_MS = 25;
constexpr uint32_t IO_INITIALIZATION_DELAY_MS = 3000;

int main()
{
    // Must be static due to garbage pointer deletion bug in the RF24::begin() function
    static RF24 rf24_radio;

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

    // Wait for serial I/O initialization
    sleep_ms(IO_INITIALIZATION_DELAY_MS);

    // Turn on the light so it is visually obvious that the firmware has started
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    RadioNrf24l01 link_manager(rf24_radio);

    link_manager.SetRxPacketCallback([](const RobotMiddleware::Packet& rx_packet)
    {
        RobotMiddleware::Packet rx_packet_copy = rx_packet;

        printf("RadioNrf24l01::RxCallback() -> RX: {%s}\n", rx_packet_copy.ToString().c_str());
    });

    if(not link_manager.InitializeRadio())
    {
        printf("Failed to initialize RF24 radio\n");
        return 1;
    }

    RobotMiddleware::Packet packet {};
    packet.payload[0] = 0xff; // Put something in the payload so none of the outbound packet payloads look empty

    uint8_t tx_counter = 0;
    const uint8_t tx_counter_limit = 10;

    std::chrono::steady_clock::time_point time_point_now = std::chrono::steady_clock::now();

    while(true)
    {
        if(tx_counter >= tx_counter_limit)
        {
            link_manager.EnqueueTxPacket(packet);
            ++packet.sequence_number;
            tx_counter = 0;
        }

        ++tx_counter;

        sleep_ms(TASK_SCHEDULE_SLEEP_INTERVAL_MS);

        time_point_now = std::chrono::steady_clock::now();

        link_manager.Run(time_point_now);
    }

    return 0;
}