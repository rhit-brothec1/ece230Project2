/*
 * Switches.h
 *
 *  Created on: Dec 18, 2020
 *      Author: Cooper
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

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

#define SWITCH_PORT                                                 GPIO_PORT_P1
#define S1_PIN                                                      GPIO_PIN1
#define S2_PIN                                                      GPIO_PIN4
#define SWITCH_PINS                                                 (0x0012)

/*!
 * \brief This function configures the switch pins as input pins
 *
 * This function configures P1.1 and P1.4 as input pins with pull-up resistors
 *
 * \return None
 */
extern void Switch_init(void);

/*!
 * \brief Get whether a switch is pressed
 *
 * This function gets the input of Switch s
 *
 * \param port is the port to check the input (S1 = 1, S2 = 4)
 *
 * \return true if the switch is pressed, false otherwise
 */
extern bool Switch_pressed(int port);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* SWITCHES_H_ */
