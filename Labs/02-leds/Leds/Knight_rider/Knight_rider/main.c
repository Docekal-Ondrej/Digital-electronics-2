
/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
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
#define BLINK_DELAY 500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>	// AVR device-specific IO definitions
#include <avr/sfr_defs.h>
#include <stdbool.h>       

/* Functions ---------------------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    DDRB = DDRB | (1<<LED_5);
    PORTB = PORTB & ~(1<<LED_5);

    DDRB = DDRB | (1<<LED_1);
    PORTB = PORTB & ~(1<<LED_1);
	
	DDRB = DDRB | (1<<LED_2);
	PORTB = PORTB & ~(1<<LED_2);
	
	DDRB = DDRB | (1<<LED_3);
	PORTB = PORTB & ~(1<<LED_3);
			   
	DDRB = DDRB | (1<<LED_4);
	PORTB = PORTB & ~(1<<LED_4);
    
    // Configure Push button at port D and enable internal pull-up resistor
    DDRD = DDRD & ~(1<<BTN);
	PORTD = PORTD | (1<<BTN);
	
	int dir = 0;
	bool loop = false;
	while (1){
	if bit_is_clear(PIND, BTN)
	{
		loop = true;
	}
	
    while (loop)
    {
		    if(dir == 1) {
		    PORTB = PORTB ^ (1<<LED_1);
			_delay_ms(BLINK_DELAY);
			PORTB = PORTB ^ (1<<LED_1);
		    PORTB = PORTB ^ (1<<LED_2);
			_delay_ms(BLINK_DELAY);
			PORTB = PORTB ^ (1<<LED_2);
		    PORTB = PORTB ^ (1<<LED_3);
			_delay_ms(BLINK_DELAY);
			PORTB = PORTB ^ (1<<LED_3);
		    PORTB = PORTB ^ (1<<LED_4);
			_delay_ms(BLINK_DELAY);
			PORTB = PORTB ^ (1<<LED_4);
			dir = 0;
			}
			else
			{
			PORTB = PORTB ^ (1<<LED_5);
			_delay_ms(BLINK_DELAY);
			PORTB = PORTB ^ (1<<LED_5);
			PORTB = PORTB ^ (1<<LED_4);
			_delay_ms(BLINK_DELAY);
			PORTB = PORTB ^ (1<<LED_4);
			PORTB = PORTB ^ (1<<LED_3);
			_delay_ms(BLINK_DELAY);
			PORTB = PORTB ^ (1<<LED_3);
			PORTB = PORTB ^ (1<<LED_2);
			_delay_ms(BLINK_DELAY);
			PORTB = PORTB ^ (1<<LED_2);
			dir = 1;
			}
    }
	
	}
    // Will never reach this
    return 0;
}

