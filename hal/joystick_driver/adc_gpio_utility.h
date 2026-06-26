#pragma once
#include <map>
#include <cstdint>

namespace RobotRemoteController::Hal
{

// Only a subset of GPIO pins are capable of analog-to-digital conversion. They are defined by this enum.
enum class AdcGpio
{
    GPIO_26 = 26,
    GPIO_27 = 27,
    GPIO_28 = 28,
    GPIO_29 = 29
};

// Maps adc-capable GPIO pins to their associated ADC inputs. The ADC input value is needed to tell the pico hardware which GPIO to read voltage from.
static const std::map<AdcGpio, uint8_t> ADC_GPIO_INPUT_TABLE = { {AdcGpio::GPIO_26, 0}, {AdcGpio::GPIO_27, 1}, {AdcGpio::GPIO_28, 2}, {AdcGpio::GPIO_29, 3} };
} // namespace RobotRemoteController::Hal