/***********************************************************************
 * 
 * Decimal counter with 7-segment output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC
#include "gpio.h"
#include <util/delay.h>

uint8_t counter = 0;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Display decimal counter values on SSD (Seven-segment 
 *           display) when 16-bit Timer/Counter1 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{

    // Configure SSD signals
    SEG_init();
	SEG_clear();
    // Test of SSD: display number '3' at position 0
	// Configure 16-bit Timer/Counter1 for Decimal counter
		TIM1_overflow_1s();
		TIM0_overflow_4ms();
    // Set the overflow prescaler to 262 ms and enable interrupt
		TIM1_overflow_interrupt_enable();
		TIM0_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
	sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment decimal counter value and display it on SSD.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    if(counter >= 59)
    {
	    counter = 0;
    }
    else
    {
	    counter++;
    }
}

ISR(TIMER0_OVF_vect)
{
	static uint8_t pos = 0;	// This line will only run the first time
	static uint8_t digit;
	static uint8_t power;
	power = pow(10, pos + 1);
	digit = ((counter % power) / pow(10, pos));
	
	SEG_update_shift_regs(digit, pos);
	
	pos++;
	
	if (pos == 4)
	{
		pos = 0;
	}
}