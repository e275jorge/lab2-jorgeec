/*
 * Lab_2.e
 *
 * Created: 11/09/2020 12:23:12 PM
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
char Length[2];
char Dash_Dot[2];
char String[10];		//Print

char ASCII;

char Morse[5];

int Morse_length = 0;

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
		PORTB ^= (1<<PORTB5);				// Toggle LED
		if (TCCR1B & (1<<ICES1))			// Find "dot" or "dash"
		{
			if (Morse_length < 5)
			{
				
				if ((width > 1875) && (width < 12500))
				{
					Morse_length += 1;
					PORTB |= (1<<PORTB1);
					_delay_ms(50);
					PORTB &= ~(1<<PORTB1);
					Morse[Morse_length - 1] = '.';
					sprintf(Dash_Dot, ". ");
					UART_putstring(Dash_Dot);
				}
				else if ((width > 12500) && (width < 25000))
				{
					Morse_length += 1;
					PORTB |= (1<<PORTB2);
					_delay_ms(50);
					PORTB &= ~(1<<PORTB2);
					Morse[Morse_length - 1] = '-';
					sprintf(Dash_Dot, "- ");
					UART_putstring(Dash_Dot);
				}
			}
		}
		else if (width > 25000)				// Find "space"
		{
			sprintf(Length, "%d", Morse_length);
			UART_putstring(Length);
			Find_ASCII();
			Morse_length = 0;
			for (int i = 0; i < 5; i++)
			{
				Morse[i] = NULL;
			}
		}
		o = 0;				//Reset overflow times to 0
		TCCR1B ^= (1<<ICES1);
	}
}

ISR(TIMER1_OVF_vect)
{
	o += 1;					//Counter overflow
}

void Find_ASCII()
{
	if (Morse_length < 5)
	{
		if (Morse[0] == '.')
		{
			if (Morse_length == 1)
			{
				sprintf(String, "\n%c\n\n", 'E');
				UART_putstring(String);
			}
			else if (Morse[1] == '.')
			{
				if (Morse_length == 2)
				{
					sprintf(String, "\n%c\n\n", 'I');
					UART_putstring(String);
				}
				else if (Morse[2] == '.')
				{
					if (Morse_length == 3)
					{
						sprintf(String, "\n%c\n\n", 'S');
						UART_putstring(String);
					}
					else if ((Morse[3] == '.') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'H');
						UART_putstring(String);
					}
					else if ((Morse[3] == '-') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'V');
						UART_putstring(String);
					}
				}
				else if (Morse[2] == '-')
				{
					if (Morse_length == 3)
					{
						sprintf(String, "\n%c\n\n", 'U');
						UART_putstring(String);
					}
					else if ((Morse[3] == '.') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'F');
						UART_putstring(String);
					}
					else
					{
						sprintf(String, "\n%c\n\n", '?');
						UART_putstring(String);
					}
				}
			}
			else if (Morse[1] == '-')
			{
				if (Morse_length == 2)
				{
					sprintf(String, "\n%c\n\n", 'A');
					UART_putstring(String);
				}
				else if (Morse[2] == '.')
				{
					if (Morse_length == 3)
					{
						sprintf(String, "\n%c\n\n", 'R');
						UART_putstring(String);
					}
					else if ((Morse[3] == '.') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'L');
						UART_putstring(String);
					}
					else
					{
						sprintf(String, "\n%c\n\n", '?');
						UART_putstring(String);
					}
				}
				else if (Morse[2] == '-')
				{
					if (Morse_length == 3)
					{
						sprintf(String, "\n%c\n\n", 'W');
						UART_putstring(String);
					}
					else if ((Morse[3] == '.') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'P');
						UART_putstring(String);
					}
					else if ((Morse[3] == '-') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'J');
						UART_putstring(String);
					}
				}
			}
		}
		else if (Morse[0] == '-')
		{
			if (Morse_length == 1)
			{
				sprintf(String, "\n%c\n\n", 'T');
				UART_putstring(String);
			}
			else if (Morse[1] == '.')
			{
				if (Morse_length == 2)
				{
					sprintf(String, "\n%c\n\n", 'N');
					UART_putstring(String);
				}
				else if (Morse[2] == '.')
				{
					if (Morse_length == 3)
					{
						sprintf(String, "\n%c\n\n", 'D');
						UART_putstring(String);
					}
					else if ((Morse[3] == '.') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'B');
						UART_putstring(String);
					}
					else if ((Morse[3] == '-') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'X');
						UART_putstring(String);
					}
				}
				else if (Morse[2] == '-')
				{
					if (Morse_length == 3)
					{
						sprintf(String, "\n%c\n\n", 'K');
						UART_putstring(String);
					}
					else if ((Morse[3] == '.') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'C');
						UART_putstring(String);
					}
					else if ((Morse[3] == '-') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'Y');
						UART_putstring(String);
					}
				}
			}
			else if (Morse[1] == '-')
			{
				if (Morse_length == 2)
				{
					sprintf(String, "\n%c\n\n", 'M');
					UART_putstring(String);
				}
				else if (Morse[2] == '.')
				{
					if (Morse_length == 3)
					{
						sprintf(String, "\n%c\n\n", 'G');
						UART_putstring(String);
					}
					else if ((Morse[3] == '.') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'Z');
						UART_putstring(String);
					}
					else if ((Morse[3] == '-') && (Morse_length == 4))
					{
						sprintf(String, "\n%c\n\n", 'Q');
						UART_putstring(String);
					}
				}
				else if ((Morse[2] == '-') && (Morse_length == 3))
				{
					sprintf(String, "\n%c\n\n", 'O');
					UART_putstring(String);
				}
				else
				{
					sprintf(String, "\n%c\n\n", '?');
					UART_putstring(String);
				}
			}
		}
	}
	else if (Morse_length == 5)
	{
		if (Morse[0] == '.')
		{
			if (Morse[1] == '.')
			{
				if (Morse[2] == '.')
				{
					if (Morse[3] == '.')
					{
						if (Morse[4] == '.')
						{
							sprintf(String, "\n%c\n\n", '5');
							UART_putstring(String);
						}
						else if (Morse[4] == '-')
						{
							sprintf(String, "\n%c\n\n", '4');
							UART_putstring(String);
						}
						else
						{
							sprintf(String, "\n%c\n\n", '?');
							UART_putstring(String);
						}
					}
					else if ((Morse[3] == '-') && (Morse[4] == '-'))
					{
						sprintf(String, "\n%c\n\n", '3');
						UART_putstring(String);
					}
					else
					{
						sprintf(String, "\n%c\n\n", '?');
						UART_putstring(String);
					}
				}
				else if ((Morse[2] == '-') && (Morse[3] == '-') && (Morse[4] == '-'))
				{
					sprintf(String, "\n%c\n\n", '2');
					UART_putstring(String);
				}
				else
				{
					sprintf(String, "\n%c\n\n", '?');
					UART_putstring(String);
				}
			}
			else if ((Morse[1] == '-') && (Morse[2] == '-') && (Morse[3] == '-') && (Morse[4] == '-'))
			{
				sprintf(String, "\n%c\n\n", '1');
				UART_putstring(String);
			}
			else
			{
				sprintf(String, "\n%c\n\n", '?');
				UART_putstring(String);
			}
		}
		else if (Morse[0] == '-')
		{
			if (Morse[1] == '-')
			{
				if (Morse[2] == '-')
				{
					if (Morse[3] == '-')
					{
						if (Morse[4] == '-')
						{
							sprintf(String, "\n%c\n\n", '0');
							UART_putstring(String);
						}
						else if (Morse[4] == '.')
						{
							sprintf(String, "\n%c\n\n", '9');
							UART_putstring(String);
						}
						else
						{
							sprintf(String, "\n%c\n\n", '?');
							UART_putstring(String);
						}
					}
					else if ((Morse[3] == '.') && (Morse[4] == '.'))
					{
						sprintf(String, "\n%c\n\n", '8');
						UART_putstring(String);
					}
					else
					{
						sprintf(String, "\n%c\n\n", '?');
						UART_putstring(String);
					}
				}
				else if ((Morse[2] == '.') && (Morse[3] == '.') && (Morse[4] == '.'))
				{
					sprintf(String, "\n%c\n\n", '7');
					UART_putstring(String);
				}
				else
				{
					sprintf(String, "\n%c\n\n", '?');
					UART_putstring(String);
				}
			}
			else if ((Morse[1] == '.') && (Morse[2] == '.') && (Morse[3] == '.') && (Morse[4] == '.'))
			{
				sprintf(String, "\n%c\n\n", '6');
				UART_putstring(String);
			}
			else
			{
				sprintf(String, "\n%c\n\n", '?');
				UART_putstring(String);
			}
		}
	}
	else
	{
		sprintf(String, "\n%c\n\n", '?');
		UART_putstring(String);
	}
}

int main(void)
{
	Initialize();
	UART_init(BAUD_PRESCALER);
	while (1)
	{
    }
}
