#include "component_manager.h"
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>

using namespace RobotRemoteController::Hal;

int main(){

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

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    ComponentManager component_manager;

    if(not component_manager.StartTaskSchedule())
    {
        return 1;
    }

    return 0;
}
