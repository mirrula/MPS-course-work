#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "lcd.h"

bool on_flag;

//Функция инициализации lcd контроллера hd44780
void init_display(){
    DDRC |= (1<<PC4)|(1<<PC6); 	// PC 4 is output for RS, 6 is output for Enable.
    DDRA = 0xFF; 				// PA 0-7 is 8 bit data output bus.

	PORTC &=~(1<<PC6); // Reset PC6 (E).

	_delay_ms(15); 

	send_command(0x38); // 2 line mode.
	_delay_ms(5); 
	send_command(0x38); // 2 line mode.
	_delay_ms(100); 
	send_command(0x38); // 2 line mode.	

	send_command(0x0F);	// LCD ON, cursor ON
	_delay_ms(10);
	send_command(0x01); // Clear display screen
	_delay_ms(10);
	send_command(0x81); 
	_delay_ms(10);

	PORTC |= (1<<PC6); // Set PC6 (E).

	on_flag = 1;
}

//Функция записи байта (символа) команды в lcd контроллер
void send_command(unsigned char command){
    PORTA = command;
    trigger();
	_delay_us( 40 );
}

//Функция записи байта (символа) данных в lcd контроллер
void send_data(unsigned char data){
    PORTA = data;
    trigger();
	_delay_us( 40 );
}

//Функция генерирующая строб на линии E.
//По этому стробу производится запись команды/данных или чтение.
void trigger(){
    PORTC |= (1<<PC6); // Set PC6 (E).
    _delay_us(1);
    PORTC &=~(1<<PC6); // Reset PC6 (E).
	_delay_us(1);
}

//Функция вывода строки
void send_string(const char *str){
    while(*str) send_data(*str++); // Send characters one by one.
}

//Функция изменения текущей позиции курсора,
//x — номер символа в строке, y — номер строки
void move_to(unsigned char x, unsigned char y){
	unsigned char address = x;
	if ( y ) {
    	address += 0x40;
  	}
	PORTC &=~(1<<PC4); // Reset PC4 (RS).
    send_command(1<<7 | address);
	PORTC |=(1<<PC4); // Set PC4 (RS).
}

void clear(){
	PORTC &=~(1<<PC4); // Reset PC4 (RS).
    send_command(0x01);
	PORTC |=(1<<PC4); // Set PC4 (RS).
}

void disp_on_off(){
	if (on_flag == 1) {
		PORTC &=~(1<<PC4); // Reset PC4 (RS).
   		send_command(0x08);
		PORTC |=(1<<PC4); // Set PC4 (RS).
		on_flag = 0;
	}
	else {
		PORTC &=~(1<<PC4); // Reset PC4 (RS).

		send_command(0x0F);	// LCD ON, cursor ON
		_delay_ms(10);
		send_command(0x01); // Clear display screen
		_delay_ms(10);
		send_command(0x81); 
		_delay_ms(10);

		PORTC |=(1<<PC4); // Set PC4 (RS).
		on_flag = 1;
	}
}


