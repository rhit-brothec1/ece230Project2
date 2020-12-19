/*
 * Switches.c
 *
 *  Created on: Dec 18, 2020
 *      Author: Cooper
 */
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Switches.h"

void Switch_init(void)
{
    // set S1 and S2 pins as input using GPIO driver functions
    GPIO_setAsInputPinWithPullUpResistor(SWITCH_PORT, SWITCH_PINS);
}

extern bool Switch_pressed(int port)
{
    // GPIO_INPUT_PIN_HIGH
    return GPIO_getInputPinValue(SWITCH_PORT,
                                 1 << port) == GPIO_INPUT_PIN_HIGH ?
            false : true;
}
