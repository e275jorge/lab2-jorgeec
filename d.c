/*
 * Lab_2.d
 *
 * Created: 9/29/2020 6:06:58 PM
 * Author : e275j
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <uart.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

int e = 0;				// Edge counter
int c = 0;				// Cycles of pulse(both high and low) width
int o = 0;				// Overflows counter
char String[10];		// Print

void Initialize()
{
	cli();						//Disable global interrupts
	DDRB &= ~(1<<DDB0);			//Port B0 set as input pin
	DDRB |= (1<<DDB5);			//Port B5 set as output port
	PORTD |= (1<<PORTD);		//Enable pull up resistor
	
	// set timer to 1 clock div by 256
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	// set timer 1 to normal
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	TCCR1B &= ~(1<<ICES1);		// Look for a falling edge
	TIFR1 |= (1<<ICF1);			// Clear flag
	TIMSK1 |= (1<<ICIE1);		// Enable input capture
	
	TIMSK1 |= (1<<TOIE1);		//Enable Timer1 overflow interrupt
	
	sei();						//Enable global interrupt
	}
	
ISR (TIMER1_CAPT_vect);
{
	if (((TCCR1B & (1<<ICES1)) && (PINB & (1<<PINB0))) || (!(TCCR1B & (1<<ICES1)) && !(PINB & (1<<PINB0))))
	{
		c = ICR1 - e + o * 65536;
		e = ICR1;
		PORTB ^= (1<<PORTB5);								// Toggle LED
		if (TCCR1B & (1<<ICES1))							// Find "dot" or "dash"
		{
			if ((c > 1875) && (c < 12500))					// Print "Dot"
			{
				sprintf(String,"Dot\n");
				UART_putstring(String);
			}
			else if ((c > 12500) && (c < 25000))			// Print "Dash"
			{
				sprintf(String,"Dash\n");
				UART_putstring(String);
			}
		}
		else if (e > 25000)									// Find "space" 
		{
			sprintf(String,"Space\n");
			UART_putstring(String);
		}
		o = 0;												//Reset overflow 
		TCCR1B ^= (1<<ICES1);
}


int main(void)
{
	Initialize();
	UART_init(BAUD_PRESCALER); 
	while (1)
	{
	}
}