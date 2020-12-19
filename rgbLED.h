/*
 * rgbLED.h
 *
 *  Created on: Dec 18, 2020
 *      Author: Cooper Brotherton
 */

#ifndef RGBLED_H_
#define RGBLED_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define RGB_PORT                                                    GPIO_PORT_P2
#define RGB_RED_PIN                                                 GPIO_PIN0
#define RGB_GREEN_PIN                                               GPIO_PIN1
#define RGB_BLUE_PIN                                                GPIO_PIN2
#define RGB_ALL_PINS                                                (0x0007)

/*!
 * \brief This function configures LED2 pins as output pins
 *
 * This function configures P2.0, P2.1, and P2.2 as output pins
 *  for the RGB LED2, and initializes LED to 'off'
 *
 * Modified bits 0 to 2 of \b P2DIR register and \b P2SEL registers.
 *
 * \return None
 */
extern void RGBLED_init(void);

/*!
 * \brief This function toggles the LED at the giv en pin
 *
 * This function changes the state of the LED at the pin.
 *
 * \param port is the port to have its LED toggled
 *
 * \ return None
 */
extern void RGBLED_togglePin(int port);

/*!
 * \brief This function turns off LED2
 *
 * \return None
 */
extern void RGBLED_turnOff();

/*!
 * \brief This function turns on only on LED
 *
 * This function turns on only one LED and turns off the rest.
 *
 * \param port is the port which will have its LED on. All others will be off.
 *
 * \return None
 */
extern void RGBLED_turnOnPortOnly(int port);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* RGBLED_H_ */
