/**
 * \file
 *
 * \brief STK600 with the RC064X routing card board header file.
 *
 * This file contains definitions and services related to the features of the
 * STK600 board with the routing card for 64-pin TQFP AVR XMEGA devices.
 *
 * To use this board, define BOARD=STK600_RC064X.
 *
 * Copyright (c) 2010-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _STK600_RC064_H_
#define _STK600_RC064_H_

#include "compiler.h"


//! Number of LEDs.
#define LED_COUNT   8

/*! \name GPIO Connections of LEDs. To use these defines, connect the STK600
 * PORTE connector to the LEDs connector.
 */
//! @{
#if !(XMEGA_B3)
# define LED0_GPIO   IOPORT_CREATE_PIN(PORTE,0)
# define LED1_GPIO   IOPORT_CREATE_PIN(PORTE,1)
# define LED2_GPIO   IOPORT_CREATE_PIN(PORTE,2)
# define LED3_GPIO   IOPORT_CREATE_PIN(PORTE,3)
# define LED4_GPIO   IOPORT_CREATE_PIN(PORTE,4)
# define LED5_GPIO   IOPORT_CREATE_PIN(PORTE,5)
# define LED6_GPIO   IOPORT_CREATE_PIN(PORTE,6)
# define LED7_GPIO   IOPORT_CREATE_PIN(PORTE,7)
#else
# define LED0_GPIO   IOPORT_CREATE_PIN(PORTB,0)
# define LED1_GPIO   IOPORT_CREATE_PIN(PORTB,1)
# define LED2_GPIO   IOPORT_CREATE_PIN(PORTB,2)
# define LED3_GPIO   IOPORT_CREATE_PIN(PORTB,3)
# define LED4_GPIO   IOPORT_CREATE_PIN(PORTB,4)
# define LED5_GPIO   IOPORT_CREATE_PIN(PORTB,5)
# define LED6_GPIO   IOPORT_CREATE_PIN(PORTB,6)
# define LED7_GPIO   IOPORT_CREATE_PIN(PORTB,7)
#endif

//! @}


/*! \name GPIO Connections of Push Buttons. To use these defines, connect the
 * STK600 PORTF connector to the SWITCHES connector.
 */
//! @{
#if !(XMEGA_B3)
# define GPIO_PUSH_BUTTON_0	IOPORT_CREATE_PIN(PORTF,0)
# define GPIO_PUSH_BUTTON_1	IOPORT_CREATE_PIN(PORTF,1)
# define GPIO_PUSH_BUTTON_2	IOPORT_CREATE_PIN(PORTF,2)
# define GPIO_PUSH_BUTTON_3	IOPORT_CREATE_PIN(PORTF,3)
# define GPIO_PUSH_BUTTON_4	IOPORT_CREATE_PIN(PORTF,4)
#if !(XMEGA_A3B || XMEGA_A3BU)
# define GPIO_PUSH_BUTTON_5	IOPORT_CREATE_PIN(PORTF,5)
#endif
# define GPIO_PUSH_BUTTON_6	IOPORT_CREATE_PIN(PORTF,6)
# define GPIO_PUSH_BUTTON_7	IOPORT_CREATE_PIN(PORTF,7)
#else
# define GPIO_PUSH_BUTTON_0	IOPORT_CREATE_PIN(PORTD,0)
# define GPIO_PUSH_BUTTON_1	IOPORT_CREATE_PIN(PORTD,1)
# define GPIO_PUSH_BUTTON_2	IOPORT_CREATE_PIN(PORTD,2)
# define GPIO_PUSH_BUTTON_3	IOPORT_CREATE_PIN(PORTD,3)
# define GPIO_PUSH_BUTTON_4	IOPORT_CREATE_PIN(PORTD,4)
# define GPIO_PUSH_BUTTON_5	IOPORT_CREATE_PIN(PORTD,5)
# define GPIO_PUSH_BUTTON_6	IOPORT_CREATE_PIN(PORTD,6)
# define GPIO_PUSH_BUTTON_7	IOPORT_CREATE_PIN(PORTD,7)
#endif
//! @}

/*! \name SPI Connections of the AT45DBX Data Flash Memory. To use these defines,
 * connect :
 * - PC4 to /CS pin on DataFlash connector
 * - PC7 to SCK pin on DataFlash connector
 * - PC6 to SO pin on DataFlash connector
 * - PC5 to SI pin on DataFlash connector
 */
//! @{
#define AT45DBX_SPI           &SPIC
#define AT45DBX_CS            IOPORT_CREATE_PIN(PORTC,4)  // CS as output
#define AT45DBX_MASTER_SS     IOPORT_CREATE_PIN(PORTC,4)  // SS as output and overlayed with CS
#define AT45DBX_MASTER_SCK    IOPORT_CREATE_PIN(PORTC,7)  // SCK as output
#define AT45DBX_MASTER_MOSI   IOPORT_CREATE_PIN(PORTC,5)  // MOSI as output
#define AT45DBX_MASTER_MISO   IOPORT_CREATE_PIN(PORTC,6)  // MISO as input
#define AT45DBX_SPI_CLK_MASK  SYSCLK_PORT_C

//! @}

/*! \name External oscillator
 */
//@{
#define BOARD_XOSC_HZ          8000000
#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
#define BOARD_XOSC_STARTUP_US  500000
//@}


#endif  // _STK600_RC064_H_
