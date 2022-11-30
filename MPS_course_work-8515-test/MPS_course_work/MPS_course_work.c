#include <util/delay.h>
#include "MPS_course_work.h"

/*
������� ������������ ����� �� ����� E. 
�� ����� ������ ������������ ������ �������/������   ��� ������.
*/
static void lcd_pulse_e(void)
{
  lcd_set_e();
  _delay_us(1);
  lcd_clear_e();
  _delay_us(1);
}

/*
������� �������� ���������� ����������� hd44780. 
����� ��������� ��������� ���, �� �� ����� ������ ������ �����, 
� ������� ������� ������� ����� �������������� ���������.
*/
static void lcd_wait(void)
{
  _delay_us( 40 );
}

/*
������� ������ ����� � lcd ����������.
������� ��� ��� ������, ������������ �������, 
������� ��� ��������� �� ����� RS ����� ������� ������ �������.
*/
static void lcd_write(unsigned char data)
{
  LCD_DATA = data;
  lcd_pulse_e();
  lcd_wait();
}

/*
������� ������������� lcd ����������� hd44780.
*/
void lcd_init( void )
{
  //����������� �� ����� ���� ������ � ����������� ����
  LCD_DATA_DDR = 0xFF;
  LCD_CTRL_DDR |= ( LCD_E | LCD_RW | LCD_RS );
 
  lcd_clear_rw();   //������������� � ����� ������ 
  lcd_clear_rs();   //���������� ����� �������
 
  _delay_ms(15); 
  //�������� 8-�� ������ ���� � 2� �������� �����
  lcd_write( LCD_FUNCTION | LCD_FUNCTION_8BIT | LCD_FUNCTION_2LINES );
  _delay_ms(5); 
  lcd_write( LCD_FUNCTION | LCD_FUNCTION_8BIT | LCD_FUNCTION_2LINES );
  _delay_us( 100 ); 
  lcd_write( LCD_FUNCTION | LCD_FUNCTION_8BIT | LCD_FUNCTION_2LINES );
 
  //��������� �������������� ���������� ������� ������� 
  lcd_write( LCD_MODE | LCD_MODE_INC ); 
 
  //�������� �������
  lcd_write( LCD_ON | LCD_ON_DISPLAY  ); 
 
  //��������� ���������� ������ 
  lcd_write( LCD_SET_DDRAM_ADDR | 0x00 ); 
 
  lcd_set_rs(); //������ ����� ���������� ������, ��������� �� ���
}

unsigned  char  lcd_codepage[]=
{
  0x41,0xa0,0x42,0xa1,0xe0,0x45,0xa3,0xa4,
  0xa5,0xa6,0x4b,0xa7,0x4d,0x48,0x4f,0xa8,
  0x50,0x43,0x54,0xa9,0xaa,0x58,0xe1,0xab,
  0xac,0xe2,0xad,0xae,0x62,0xaf,0xb0,0xb1,
  0x61,0xb2,0xb3,0xb4,0xe3,0x65,0xb6,0xb7,
  0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0x6f,0xbe,
  0x70,0x63,0xbf,0x79,0xe4,0x78,0xe5,0xc0,
  0xc1,0xe6,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
};

/*
������� ������ �������, ������ ������ ���� � ��������� win1251,
��� ��������������� � ���� lcd �����������  hd44780
������������ ������� lcd_codepage.
*/
void lcd_putc(unsigned char c) 
{
    if ( c == 0xA8) {     //����� '�'
      c = 0xA2; 
    }
    else if ( c ==0xB8) { //����� '�'
      c = 0xB5; 
    }
    else if ( c >= 0xC0 ) {
      c = lcd_codepage[ c - 0xC0 ];
    }
    lcd_write( c );
}
 
/*
������� ������ ������.
*/

void lcd_puts( const char *str)
{
  while( *str ) {
    lcd_putc( *str++ );
  }
}

/*
 ������� ��������� ������� ������� �������, 
 x - ����� ������� � ������, 
 y - ����� ������
*/
void lcd_goto( unsigned char x, unsigned char y )
{
  unsigned char address = x;
  if ( y ) {
    address += 0x40;
  }
  lcd_clear_rs();   
  lcd_write( LCD_SET_DDRAM_ADDR |  address ); 
  lcd_set_rs(); 
}
