/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define BTN		PC0

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include <stdbool.h>		// Add bool data type
#include "gpio.h"			// GPIO library
#include "twi.h"			// I2C library
#include "bme280.h"			// BME280 sensor library

/* Variables ---------------------------------------------------------*/
char lcd_value[8];
static uint8_t btnpressed = 0;
typedef enum {              // FSM declaration
	STATE_START,
	STATE_TEMP,
	STATE_HUM,
	STATE_PRES,
} state_t;
state_t state = STATE_START;
static uint8_t next_state = 0;
/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD display when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
	// Initialize I2C
	
	// Configure buttons
	GPIO_config_input_pullup(&DDRC, BTN);
	GPIO_write_low(&PORTC, BTN);
    
	// Initialize LCD display	
	lcd_init(LCD_DISP_ON);
	
	lcd_gotoxy(0, 0);
	lcd_puts("Weather ");
	lcd_gotoxy(8, 0);
	lcd_puts("station ");
	
    // Configure 8-bit Timer/Counter2
	TIM2_overflow_128us();
	TIM2_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
		//Endless loop
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD display every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER2_OVF_vect)
{
	static uint8_t counter = 0;
	static uint8_t tempMSB;
	static uint8_t tempLSB;
	static uint16_t temp;
	static uint8_t hum;
	static uint8_t press;
	static lcd_string[3];
	if(counter <= 250){
		counter++;
	}
	else{
		
		counter = 0;
	}
	
	if(!GPIO_read(&PINC, BTN)){
		if(btnpressed == 0){
			next_state++;
			btnpressed = 1;
		}
	}
	else if(GPIO_read(&PINC, BTN))
	{
		btnpressed = 0;
	};
	
	switch(state)
	{
	case STATE_START:
		if(next_state == 1){
			state = STATE_TEMP;
		}
		break;
	case STATE_TEMP:
		if(next_state == 2){
			state = STATE_HUM;
		}
		lcd_gotoxy(0, 0);
		lcd_puts("  TEMP: ");
		lcd_gotoxy(7, 0);
		itoa(tempMSB, lcd_string, 10);
		lcd_puts(lcd_string);
		lcd_gotoxy(13, 0);
		//itoa(tempLSB, lcd_string, 10);
		//lcd_puts(lcd_string);
		break;
	case STATE_HUM:
		if(next_state == 3){
			state = STATE_PRES;
		}
		lcd_gotoxy(0, 0);
		lcd_puts("   HUM: ");
		lcd_gotoxy(8, 0);
		lcd_puts("value  ");
		break;
	case STATE_PRES:
		if(next_state > 3){
			state = STATE_TEMP;
			next_state = 1;
		}
		lcd_gotoxy(0, 0);
		lcd_puts(" PRESS: ");
		lcd_gotoxy(8, 0);
		lcd_puts("value  ");
		break;
	}	

}