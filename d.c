/*
 * Lab_2.c
 *
 * Created: 9/29/2020 6:06:58 PM
 * Author : e275j
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

int main(void)
{	
		
	cli();						//Disable global interrupts
	DDRB &= ~(1<<DDB0);			//Port B0 set as input pin
	DDRB |= (1<<DDB5);			//Port B5 set as output port
	PORTD |= (1<<PORTD);		//Enable pull up resistor
	
	// set timer to 1 clock div by 8
	TCCR1B &= ~(1<<CS10); 
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS12);
	
	// set timer 1 to normal
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	TCCR1B |= (1<<ICES1);		// Look for a rising edge
	TIFR1 |= (1<<ICF1);			// Clear flag
	TIMSK1 |= (1<<ICIE1);		// Enable input capture
	
	sei();
	
	ISR (TIMER1_CAPT_vect);
		{
			PORTB ^= (1<<PORTB5);	    //PB1 port HIGH
		}
	
	while (1)
	{
		while (!(TIFR1 & (1<<ICF1))); // Waiting for press
		PORTB ^= (1<<PORTB5);	      //PB5 port HIGH
		TIFR1 |= (1<<ICF1);			  // Clear flag		
	}
}