#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"

void UsartInit()
{
  UBRRL=25;                  //1 000 000 / (2400 * 16) - 1 = 25
  UCSRB=(1<<TXEN);              //?ac?aoaai ia?aaa?o
  UCSRC=(1<<URSEL)|(1<<UCSZ0|(1<<UCSZ1));    //?a?ei ?aaiou 8 aeo
}

void Transmit(char* data)
{
  for (int i=0; i<strlen(data); i++)
    {
        while(!(UCSRA&(1<<UDRE))) {}    // wait ready of port
      UDR = data[i];
    //_delay_ms(1000);
  }
}
