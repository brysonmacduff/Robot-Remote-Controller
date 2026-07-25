#pragma once
#include <RF24.h>

namespace RobotRemoteController::Hal
{
/**
 * @brief This class holds a single RF24 instance and manages its lifecycle.
 * @note This class may not appear to serve a purpose, but it exists to mitigate the memory management issues in the RF24 library
 * that manifest on the Pico, but not other platforms.
 * @bug Currently (July 2026), there is a bug in RF24 that will cause it to delete an uninitialized pointer, which crashes the firmware.
 * This bug can be reproduced by initializing RF24 on the stack and calling the RF24::begin() function. As a work-around, RF24
 * can be statically instantiated to prevent the illegal pointer memory deletion operation. Therefore, this class exists to
 * quarantine the instantiation procedure of RF24. This leaves open the option to instantiate RF24 differently in this class
 * while causing minimal disruption to the rest of the program that uses RF24 by reference. This will be helpful in the future
 * if Nordic Semiconductors releases a bug fix for the aforementioned issue.
 */
class Rf24Provider
{
public:

    /**
     * @brief Returns a reference to the RF24 instance that lives inside this class.
     */
    RF24& GetInstance()
    {
        // Must be static due to garbage pointer deletion bug in the RF24::begin() function
        static RF24 rf24_singleton;
        return rf24_singleton;
    }
};
} // namespace RobotRemoteController::Hal