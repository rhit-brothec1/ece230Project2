/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: Cooper Brotherton
 *******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "rgbLED.h"
#include "Switches.h"

#define blinkRate 10135

int activeLED = 0;

/*
 * Sets up I/O
 */
void setup()
{
    RGBLED_init();
    Switch_init();
}

/*
 * Function for debouncing a switch through a delay
 *
 * Delay is ~3.337 ms
 */
void debounce()
{
    int delay = 1000;
    while (delay)
    {
        delay--;
    }
}

/*
 * State where the LED blinks at 1 Hz while waiting for switch input
 */
void ledBlink()
{
    int counter = blinkRate;
    RGBLED_togglePin(activeLED);
    while (1)
    {
        // Turn off LED is S1 pressed
        if (Switch_pressed(1))
        {
            RGBLED_turnOff();
            debounce();
            return;
        }
        // Change active LED is S2 pressed
        if (Switch_pressed(4))
        {
            activeLED = (activeLED + 1) % 3;
            RGBLED_turnOnPortOnly(activeLED);
            debounce();
            while (Switch_pressed(4))
                ;
        }
        // if counter == 0, toggle LED, reset counter
        if (!counter)
        {
            RGBLED_togglePin(activeLED);
            counter = blinkRate;
        }
        counter--;
    }
}

/*
 * Basic flow state
 */
void loop()
{
    // Wait for S1 press and release
    while (!Switch_pressed(1))
        ;
    debounce();
    while (Switch_pressed(1))
        ;
    debounce();
    // Start LED cycle
    ledBlink();
    // Wait until S1 is released before looping
    while (Switch_pressed(1))
        ;
    debounce();
}

/*
 * Start of the program
 */
int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    setup();

    while (1)
    {
        loop();
    }
}
