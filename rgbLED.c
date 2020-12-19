/*
 * rgbLED.c
 *
 *  Created on: Dec 18, 2020
 *      Author: Cooper Brotherton
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rgbLED.h"

void RGBLED_init(void)
{
    // set LED2 pins as output using GPIO driver functions
    GPIO_setAsOutputPin(RGB_PORT, RGB_ALL_PINS);

    // set LED2 outputs to LOW using GPIO driver functions
    GPIO_setOutputLowOnPin(RGB_PORT, RGB_ALL_PINS);
}

void RGBLED_togglePin(int port)
{
    MAP_GPIO_toggleOutputOnPin(RGB_PORT, 1 << port);
}

void RGBLED_turnOff()
{
    MAP_GPIO_setOutputLowOnPin(RGB_PORT, RGB_ALL_PINS);
}

void RGBLED_turnOnPortOnly(int port)
{
    int mask = 1 << port;
    MAP_GPIO_setOutputLowOnPin(RGB_PORT, ~mask);
    MAP_GPIO_setOutputHighOnPin(RGB_PORT, mask);
}
