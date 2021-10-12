# Lab 3: ONDŘEJ DOČEKAL

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/Docekal-Ondrej/Digital-electronics-2](https://github.com/Docekal-Ondrej/Digital-electronics-2)



### Data types in C

1. Complete table.

| **Data type** | **Number of bits** | **Range** | **Description** |
| :-: | :-: | :-: | :-- | 
| `uint8_t`  | 8 | 0, 1, ..., 255 | Unsigned 8-bit integer |
| `int8_t`   | 8 | -128, ..., 127 | Signed 8-bit integer |
| `uint16_t` | 16 | 0, 1, ..., 65 535 | Unsigned 16-bit integer |
| `int16_t`  | 16 | -32 768, ..., 32 767 | Signed 16-bit integer |
| `float`    | 32 | -3.4e+38, ..., 3.4e+38 | Single-precision floating-point |
| `void`     | - | - | Has no value |


### GPIO library

1. In your words, describe the difference between the declaration and the definition of the function in C.

   Declaration tells the compiler about the name, return type and parameters of the function while definition contains the algorithm inside of the function. 

2. Part of the C code listing with syntax highlighting, which toggles LEDs only if push button is pressed. Otherwise, the value of the LEDs does not change. Use function from your GPIO library. Let the push button is connected to port D:

```c
    // Configure Push button at port D and enable internal pull-up resistor
    // WRITE YOUR CODE HERE

    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(BLINK_DELAY);

        if(GPIO_read(&PIND, BTN))
        {
            _delay_ms(BLINK_DELAY);
            GPIO_toggle(&PORTC, LED_BLUE);
            GPIO_toggle(&PORTB, LED_GREEN);
        }
    }
```


### Knight rider

```c
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

    GPIO_config_input_pullup(&DDRD, BTN);
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
```