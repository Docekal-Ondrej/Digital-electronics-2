/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed. Use 
 * functions from GPIO library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_5   PB5    // AVR pin where green LED is connected
#define LED_1   PB1
#define LED_2   PB2
#define LED_3   PB3
#define LED_4   PB4
#define BTN   PD4
#define BLINK_DELAY 250
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/sfr_defs.h>
#include <stdbool.h>
#include "gpio.h"           // GPIO library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed. Functions 
 *           from user-defined GPIO library is used.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    GPIO_config_output(&DDRB, LED_5);
    GPIO_write_low(&PORTB, LED_5);

	GPIO_config_output(&DDRB, LED_1);
	GPIO_write_low(&PORTB, LED_1);

	GPIO_config_output(&DDRB, LED_2);
	GPIO_write_low(&PORTB, LED_2);

	GPIO_config_output(&DDRB, LED_3);
	GPIO_write_low(&PORTB, LED_3);

	GPIO_config_output(&DDRB, LED_4);
	GPIO_write_low(&PORTB, LED_4);

    GPIO_config_input_nopull(&DDRD, BTN);
    GPIO_write_high(&PORTD, BTN);
    
	int dir = 0;
	bool loop = false;
	while (1){
		if(GPIO_read(&PIND, BTN))
		{
			loop = true;
		}

	while (loop)
		{
			if(dir == 1) {
				GPIO_toggle(&PORTB, LED_1);
				_delay_ms(BLINK_DELAY);
				GPIO_toggle(&PORTB, LED_1);
				GPIO_toggle(&PORTB, LED_2);
				_delay_ms(BLINK_DELAY);
				GPIO_toggle(&PORTB, LED_2);
				GPIO_toggle(&PORTB, LED_3);
				_delay_ms(BLINK_DELAY);
				GPIO_toggle(&PORTB, LED_3);
				GPIO_toggle(&PORTB, LED_4);
				_delay_ms(BLINK_DELAY);
				GPIO_toggle(&PORTB, LED_4);
				dir = 0;
			}
			else
			{
				GPIO_toggle(&PORTB, LED_5);
				_delay_ms(BLINK_DELAY);
				GPIO_toggle(&PORTB, LED_5);
				GPIO_toggle(&PORTB, LED_4);
				_delay_ms(BLINK_DELAY);
				GPIO_toggle(&PORTB, LED_4);
				GPIO_toggle(&PORTB, LED_3);
				_delay_ms(BLINK_DELAY);
				GPIO_toggle(&PORTB, LED_3);
				GPIO_toggle(&PORTB, LED_2);
				_delay_ms(BLINK_DELAY);
				GPIO_toggle(&PORTB, LED_2);
				dir = 1;
			}
		}
    }

    // Will never reach this
    return 0;
}

