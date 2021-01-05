/*
 * Author:      Cooper Brotherton
 * Date:        January 5, 2021
 * Libraries:   SysTick and GPIO from DriverLib
 */
/************************************************************************
 * MSP432 Project 2 ECE230 Winter 2020-2021
 *
 * Description: S1 toggles whether the LED is blinking.
 *              S2 toggles which LED is blinking in order of R->G->B.
 *              Active LED blinks at for 500ms, off for 500ms.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *       S1-->|P1.1        P2.0  |---> LED2 red
 *            |            P2.1  |---> LED2 green
 *       S2-->|P1.4        P2.2  |---> LED2 blue
 *            |                  |
 *
*************************************************************************/
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
    RGBLED_turnOnOnlyPin(activeLED);
    while (1)
    {
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
        RGBLED_togglePin(activeLED);
    }
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
    MAP_SysTick_enableModule();
    ledBlink();
    MAP_SysTick_disableModule();
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
