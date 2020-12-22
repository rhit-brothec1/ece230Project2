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

int activeLED = 0;

/*!
 * \brief This function sets up the device
 *
 * This function sets P2.0, P2.1, and P2.2 as outputs,
 * P1.1 and P1.4 as inputs, and sets up the SysTick timer.
 *
 * \return None
 */
void setup()
{
    RGBLED_init();
    Switch_init();
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(1509000);
}

/*!
 * \brief This function debounces switch pressed
 *
 * This function loads a count-down value for a delay.
 * Delay is ~3.337ms
 *
 * \return None
 */
void debounce()
{
    int delay = 1000;
    while (delay)
    {
        delay--;
    }
}

/*!
 * \brief This function blinks the LED at 1Hz while waiting for input
 *
 * This function uses the SysTick to blink the LED at a rate of 1Hz with a 50%
 * duty cycle. It also checks for input.\n
 *
 * On press of S1, the LED turns off and it returns to looping.
 * On press of S2, it cycles between the RGB LEDs on P2.0.
 *
 * \return None
 */
void ledBlink()
{
    RGBLED_togglePin(activeLED);
    // Check for switch input until timer hits 0
    while (((SysTick->CTRL) & SysTick_CTRL_COUNTFLAG_Msk) == 0)
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
            RGBLED_turnOnOnlyPin(activeLED);
            debounce();
            while (Switch_pressed(4))
                ;
        }
    }
    ledBlink();
}

/*!
 * \brief This function waits for S1 input
 *
 * This function waits for S1 to be pressed and released, then goes to the
 * blinking state. After blinking, it waits for S1 to be released before
 * looping back and waiting for a new S1 press
 *
 * \return None
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

/*!
 * \brief Starting point of the program
 *
 * This function is where the program starts. It initializes I/O, then goes
 * into a loop function.
 *
 * \return An integer
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
