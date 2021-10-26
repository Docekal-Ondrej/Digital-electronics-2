# Lab 5: ONDŘEJ DOČEKAL

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/Docekal-Ondrej/Digital-electronics-2](https://github.com/Docekal-Ondrej/Digital-electronics-2)


### 7-segment library

1. In your words, describe the difference between Common Cathode and Common Anode 7-segment display.
   * CC SSD: The LEDs are on when their ports are on 1. All the LEDs share the same cathode.
   * CA SSD: The LEDs are on when their ports are on 0. All the LEDs share the same anode.

2. Code listing with syntax highlighting of two interrupt service routines (`TIMER1_OVF_vect`, `TIMER0_OVF_vect`) from counter application with at least two digits, ie. values from 00 to 59:

```c
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment counter value from 00 to 59.
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
```

```c
/**********************************************************************
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Display tens and units of a counter at SSD.
 **********************************************************************/
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
```

3. Flowchart figure for function `SEG_clk_2us()` which generates one clock period on `SEG_CLK` pin with a duration of 2&nbsp;us. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![Picture.](https://github.com/Docekal-Ondrej/Digital-electronics-2/blob/main/Labs/05-segment/Screenshot_1.png)


### Kitchen alarm

Consider a kitchen alarm with a 7-segment display, one LED and three push buttons: start, +1 minute, -1 minute. Use the +1/-1 minute buttons to increment/decrement the timer value. After pressing the Start button, the countdown starts. The countdown value is shown on the display in the form of mm.ss (minutes.seconds). At the end of the countdown, the LED will start blinking.

1. Scheme of kitchen alarm; do not forget the supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![Picture.](https://github.com/Docekal-Ondrej/Digital-electronics-2/blob/main/Labs/05-segment/Screenshot_2.png)