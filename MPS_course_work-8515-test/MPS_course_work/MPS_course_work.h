#ifndef LCD__H
#define LCD__H

#include <avr/io.h>

//линии данных
#define LCD_DATA PORTD
#define LCD_DATA_DDR DDRD
 
//управляющие линии
#define LCD_CTRL PORTC
#define LCD_CTRL_DDR DDRC
 
#define LCD_RS  _BV( 0 )  /*0 - команды, 1 - данные*/
#define LCD_RW  _BV( 1 )  /*0 - запись, 1 - чтение*/
#define LCD_E   _BV( 2 )  
 
#define lcd_set_e()   LCD_CTRL |= LCD_E  /*выставить в 1 линию E*/
#define lcd_set_rs()  LCD_CTRL |= LCD_RS /*выставить в 1 линию RS*/
#define lcd_set_rw()  LCD_CTRL |= LCD_RW /*выставить в 1 линию RW*/
 
#define lcd_clear_e()  LCD_CTRL &= ~LCD_E  /*выставить в 0 линию E*/
#define lcd_clear_rs() LCD_CTRL &= ~LCD_RS /*выставить в 0 линию RS*/
#define lcd_clear_rw() LCD_CTRL &= ~LCD_RW /*выставить в 0 линию RW*/




#define LCD_CLR              _BV( 0 ) /*Очистить дисплей*/
 
#define LCD_HOME             _BV( 1 ) /*Переход в начало экрана*/
 
#define LCD_MODE             _BV( 2 ) /*Сдвиг курсора или экрана при записи символа*/
#define LCD_MODE_INC         _BV( 1 ) 
#define LCD_MODE_SHIFT       _BV( 0 ) 
 
#define LCD_ON               _BV( 3 ) /*Вкл/выкл дисплей, курсор, мигание*/
#define LCD_ON_DISPLAY       _BV( 2 ) 
#define LCD_ON_CURSOR        _BV( 1 ) 
#define LCD_ON_BLINK         _BV( 0 )
 
#define LCD_SHIFT            _BV( 4 ) /*Команда сдвига курсора/экрана*/
#define LCD_SHIFT_DISPLAY    _BV( 3 )
#define LCD_SHIFT_RIGHT      _BV( 2 )
 
#define LCD_FUNCTION         _BV( 5 ) /*Разрядность шины данных, кол-во строк, размер шрифта*/
#define LCD_FUNCTION_8BIT    _BV( 4 )
#define LCD_FUNCTION_2LINES  _BV( 3 )
#define LCD_FUNCTION_10DOTS  _BV( 2 )
 
#define LCD_SET_CGRAM_ADDR   _BV( 6 ) /*Установка адреса CGRAM*/
 
#define LCD_SET_DDRAM_ADDR   _BV( 7 ) /*Установка адреса DDRAM*/


void lcd_init( void );
void lcd_putc( unsigned char c );
void lcd_puts( const char *str);
void lcd_goto( unsigned char x, unsigned char y );

#endif
