/* Author: Thia Wyrod
 * License: GPLv3
 */
#ifndef _STRUCT_ALIASES_H_
#define _STRUCT_ALIASES_H_

#ifdef __BOARD_GINGER__

#define DEFAULT_POLARITY Polar_ActiveLow
#define BUTTON_POLARITY Polar_ActiveHigh
#define LED_POLARITY DEFAULT_POLARITY

#define DEFAULT_IO_CFG IO_FAST_SLEW|IO_PULL_UP|IO_PULL_ENABLE|IO_DRIVE_4mA
#define DEFAULT_LED_CFG DEFAULT_IO_CFG
#define DEFAULT_BUTTON_CFG IO_FAST_SLEW|IO_PULL_DOWN|IO_PULL_ENABLE|IO_DRIVE_4mA

#define SW0 { GPIO_A, io_PA, 0x1, BUTTON_POLARITY }
#define SW1 { GPIO_A, io_PA, 0x2, BUTTON_POLARITY }
#define SW2 { GPIO_A, io_PA, 0x4, BUTTON_POLARITY }
#define SW3 { GPIO_A, io_PA, 0x8, BUTTON_POLARITY }

#define LED1 { GPIO_D, io_PD4, 0x10, LED_POLARITY }
#define LED2 { GPIO_D, io_PD5, 0x20, LED_POLARITY }
#define LED3 { GPIO_D, io_PD6, 0x40, LED_POLARITY }
#define LED4 { GPIO_D, io_PD7, 0x80, LED_POLARITY }

#elif defined(__BOARD_XINC2_DEV__)

#define DEFAULT_POLARITY Polar_ActiveLow
#define BUTTON_POLARITY DEFAULT_POLARITY
#define LED_POLARITY DEFAULT_POLARITY

#define DEFAULT_IO_CFG IO_FAST_SLEW|IO_PULL_UP|IO_PULL_ENABLE|IO_DRIVE_4mA
#define DEFAULT_LED_CFG DEFAULT_IO_CFG
#define DEFAULT_BUTTON_CFG DEFAULT_IO_CFG

#define SW0 { GPIO_D, io_PD6, 0x40, BUTTON_POLARITY }
#define SW1 { GPIO_D, io_PD7, 0x80, BUTTON_POLARITY }
#define SW2 { GPIO_A, io_PA, 0x40, BUTTON_POLARITY }
#define SW3 { GPIO_A, io_PA, 0x80, BUTTON_POLARITY }

#define LED1 { GPIO_C, io_PC0, 0x1, LED_POLARITY }
#define LED2 { GPIO_C, io_PC1, 0x2, LED_POLARITY }
#define LED3 { GPIO_D, io_PD4, 0x10, LED_POLARITY }
#define LED4 { GPIO_D, io_PD5, 0x20, LED_POLARITY }
#define LED5 { GPIO_A, io_PA, 0x1, LED_POLARITY }
#define LED6 { GPIO_A, io_PA, 0x2, LED_POLARITY }
#define LED7 { GPIO_A, io_PA, 0x4, LED_POLARITY }
#define LED8 { GPIO_A, io_PA, 0x8, LED_POLARITY }

#endif

#endif //_STRUCT_ALIASES_H_
