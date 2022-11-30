#include <avr/io.h>
#include "lcd.h"


int main( void )
{
  
  lcd_init();
  lcd_goto(0,0);
  lcd_puts("mainloop.ru/avr-" );
  lcd_goto(0,1);
  lcd_puts("atmega/lcd.html" );
  for(;;) {
  }
  return 0;
}
















