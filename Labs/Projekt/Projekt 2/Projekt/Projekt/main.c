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
#define BTN	PC0
#define servo1 PB1
#define servo2 PB2
#define SW PD5

#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include <stdbool.h>		// Add bool data type
#include "gpio.h"			// GPIO library
#include "twi.h"			// I2C library
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/sfr_defs.h>	// For testing bit values in control registers

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

uint16_t photoresistorValue;

/* Functions ---------------------------------------------------------*/
//defining ADC-read function
uint16_t readADC (uint8_t channel)
{
    ADMUX = (0xf0 & ADMUX) | channel;	//Set input channel to read
    ADCSRA |= (1 << ADSC);	//Start an ADC conversion by setting ADSC bit
    loop_until_bit_is_clear(ADCSRA, ADSC);	//wait until AD convertion is finished
    return (ADC);
}
/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  
 * Returns:  none
 **********************************************************************/
int main(void)
{
	DDRB |= (1 << servo1);	//OC1A
	DDRB |= (1 << servo2);	//OC1B
	DDRD |= ~(1 << SW);  // Arduino PIN5
	PORTD |= (1 << SW);
	
	//PWM is set to non-inverting mode
	//Setting Fast PWM mode 14: set WGM11, WGM12, WGM13 to 1, set prescaler to 8
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);

	ICR1 = 19999;	//Set ICR1 register: PWM period
	
	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //ADC clock prescaler to 8
	ADCSRA |= (1 << ADEN); //enables the ADC
	
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
        if(bit_is_clear(PIND,5)) //hold the SW bu
        {
            OCR1A = (readADC(ADC1D)*5); //reading analog value from A_pin A1
            
            OCR1B = (readADC(ADC2D)*5); //reading analog value from A_pin A2
        }
        if(bit_is_set(PIND,5))
        {
            ADCSRA |= (1 << ADSC); //start ADC conversion
            loop_until_bit_is_clear(ADCSRA, ADSC); //wait until ADC conversion is done
            photoresistorValue = ADC; //read ADC value from photo-resistor
            
            OCR1A = 500 + photoresistorValue*5.5;  //Servo reads values from photo-resistor,where 500 is 0 deg

            OCR1B = 500 + photoresistorValue*5.5;
        }
        
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