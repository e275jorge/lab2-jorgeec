/*
 * Lab_2.c
 *
 * Created: 9/29/2020 6:06:58 PM
 * Author : e275j
 */ 

#include <avr/io.h>

int main(void)
{	
	DDRB &= ~(1<<DDB0);			//Port B0 set as input pin
	DDRB |= (1<<DDB5);			//Port B5 set as output port
	PORTD |= (1<<PORTD);		//Enable pull up resistor
		
	//Set timer1 clock, divided by 8
	TCCR1B &= ~(1<<CS10);		
	TCCR1B |= (1<<CS11);		
	TCCR1B &= ~(1<<CS12);	
	
	//Set timer1 to normal
	TCCR1A &= ~(1<<WGM10);		
	TCCR1A &= ~(1<<WGM11);		
	TCCR1B &= ~(1<<WGM12);		
	TCCR1B &= ~(1<<WGM13);
		
	while (1)
	{
		while (!(TIFR1 & (1<<ICF1)));	// Waiting for press
		PORTB ^= (1<<PORTB5);	  	  	// Toggle PB1
		TIFR1 |= (1<<ICF1);			 	// Clear flag
	}
}