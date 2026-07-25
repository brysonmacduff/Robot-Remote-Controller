#include "RF24.h"
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <array>

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

    // Wait for serial I/O initialization
    sleep_ms(1000);

    // Turn on the light so it is visually obvious that the firmware has started
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    static RF24 radio;

    spi_init(spi0, 4000000); // RF24 can only use SPI0 on the pico

    gpio_set_function(2, GPIO_FUNC_SPI);  // SCK
    gpio_set_function(3, GPIO_FUNC_SPI);  // MOSI
    gpio_set_function(4, GPIO_FUNC_SPI);  // MISO

    gpio_init(22); 
    gpio_set_dir(22, GPIO_OUT);  // CE
    gpio_init(17); 
    gpio_set_dir(17, GPIO_OUT);  // CSN

    // Initialize the radio hardware

    if (not radio.begin(22, 17)) 
    {
        printf("Failed to initialize radio hardware. This is a fatal error.\n");
        return 1;
    }

    // These could be made configurable but it is unclear at this time whether they should be.
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_1MBPS);

    // Open pipes for both TX and RX
    radio.openWritingPipe(0xE8E8F0F0E1LL);
    radio.openReadingPipe(1, 0xE8E8F0F0E1LL);

    // Enter RX mode to start
    radio.startListening();

    std::array<uint8_t, 32> tx_packet = { 1 };

    while(true)
    {
        sleep_ms(50);

        radio.stopListening(); // Enter TX
        
        radio.write(tx_packet.data(), tx_packet.size());

        radio.startListening(); // Exit TX

        radio.ce(0);

        const bool is_radio_available = radio.available();

        radio.ce(1);

        if(not is_radio_available)
        {
            continue;
        }

        std::array<uint8_t, 32> rx_packet = { 0 };

        radio.read(rx_packet.data(), rx_packet.size());

    }

    return 1;
}