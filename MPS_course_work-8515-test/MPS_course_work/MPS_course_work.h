#ifndef LCD__H
#define LCD__H

#include <avr/io.h>

//����� ������
#define LCD_DATA PORTD
#define LCD_DATA_DDR DDRD
 
//����������� �����
#define LCD_CTRL PORTC
#define LCD_CTRL_DDR DDRC
 
#define LCD_RS  _BV( 0 )  /*0 - �������, 1 - ������*/
#define LCD_RW  _BV( 1 )  /*0 - ������, 1 - ������*/
#define LCD_E   _BV( 2 )  
 
#define lcd_set_e()   LCD_CTRL |= LCD_E  /*��������� � 1 ����� E*/
#define lcd_set_rs()  LCD_CTRL |= LCD_RS /*��������� � 1 ����� RS*/
#define lcd_set_rw()  LCD_CTRL |= LCD_RW /*��������� � 1 ����� RW*/
 
#define lcd_clear_e()  LCD_CTRL &= ~LCD_E  /*��������� � 0 ����� E*/
#define lcd_clear_rs() LCD_CTRL &= ~LCD_RS /*��������� � 0 ����� RS*/
#define lcd_clear_rw() LCD_CTRL &= ~LCD_RW /*��������� � 0 ����� RW*/




#define LCD_CLR              _BV( 0 ) /*�������� �������*/
 
#define LCD_HOME             _BV( 1 ) /*������� � ������ ������*/
 
#define LCD_MODE             _BV( 2 ) /*����� ������� ��� ������ ��� ������ �������*/
#define LCD_MODE_INC         _BV( 1 ) 
#define LCD_MODE_SHIFT       _BV( 0 ) 
 
#define LCD_ON               _BV( 3 ) /*���/���� �������, ������, �������*/
#define LCD_ON_DISPLAY       _BV( 2 ) 
#define LCD_ON_CURSOR        _BV( 1 ) 
#define LCD_ON_BLINK         _BV( 0 )
 
#define LCD_SHIFT            _BV( 4 ) /*������� ������ �������/������*/
#define LCD_SHIFT_DISPLAY    _BV( 3 )
#define LCD_SHIFT_RIGHT      _BV( 2 )
 
#define LCD_FUNCTION         _BV( 5 ) /*����������� ���� ������, ���-�� �����, ������ ������*/
#define LCD_FUNCTION_8BIT    _BV( 4 )
#define LCD_FUNCTION_2LINES  _BV( 3 )
#define LCD_FUNCTION_10DOTS  _BV( 2 )
 
#define LCD_SET_CGRAM_ADDR   _BV( 6 ) /*��������� ������ CGRAM*/
 
#define LCD_SET_DDRAM_ADDR   _BV( 7 ) /*��������� ������ DDRAM*/


void lcd_init( void );
void lcd_putc( unsigned char c );
void lcd_puts( const char *str);
void lcd_goto( unsigned char x, unsigned char y );

#endif
