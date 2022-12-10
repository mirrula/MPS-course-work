#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "uart.h"

void UsartInit()
{
	UBRRL=25;                  				//1 000 000 / (2400 * 16) - 1 = 25
  	UCSRB=(1<<TXEN);              			
  	UCSRC=(1<<URSEL)|(1<<UCSZ0|(1<<UCSZ1));   
}

void Transmit(char* data)
{
  	for (int i=0; i<strlen(data); i++)
    {
        while(!(UCSRA&(1<<UDRE))) {}    // wait ready of port
      	UDR = data[i];
  	}
}
