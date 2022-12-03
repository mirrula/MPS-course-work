#include <stdlib.h>
#include <util/delay.h>
#include "calculator.h"
#include "keyboard.h"
#include "lcd.h"
#include <stdbool.h>

float r, a, b; 								// a and b are operands, r is the result.
float ans;
int count; 									// Number of characters displaying on first line.
unsigned short state; 						// Calculator and Monitor state.
unsigned char op; 							// Operator: [/, *, +, -, ...].

static char line[] = "                "; 	// Single line, 16 characters long.
static char buffer[16];
static char overflow[] = "   Overflow   ";

void init_calculator() {
    _delay_ms(500);
    init_display();
    init_keyboard();

    reset();
    run();
}

double calculate(float m, char operator, float n) {
    switch (operator) {
    case '+':
        return r = m + n;
    case '-':
        return r = m - n;
    case '*':
        return r = m * n;
    case '/':
        return r = m / n;

    case 'p':
        return r = m * n / 100;
    case 'm':
        return r = (int)m % (int)n;
    case 'v':
        return r = (int)m / (int)n;
    case 'r':
		r = 1;
		for (int i=0; i<n; i++) r = r * m;
        return r;

    }
    return r = m;
}

void show_result() {
    move_to(0, 1);			//начало нижней строки
    send_string("A=");
    char *data = buffer;

    if (r <= 999999999999)
        dtostrf(r, 9, 5, buffer);	//данные с плавающей точкой -> массив символов
    else 
		data = overflow;

    send_string(data);
    state = 4;
}

void send_digit(unsigned char digit) {
    send_data(digit + '0');
}

void run() {
	bool *is_dec_p;
	bool is_decimal = 0;		//флаг (0 - целая часть, 1 - дробная)
	is_dec_p = &is_decimal;

	int *paw_p;
	int paw = 1;				//разряд после запятой
	paw_p = &paw;

    while (1) {
        decide(scan_key(), is_dec_p, paw_p);
    }
}

void reset() {
    clear();
    move_to(0, 0);		//начало верхней строки
    r = a = b = op = state = count = 0;
}

void on_off(){ disp_on_off(); }

/* States:
0 - Displaying banner.
1 - Input first operand, a.
2 - Middle Operator pressed, op.
3 - Input second operand, b.
4 - Result displayed.
*/

void decide(unsigned char key, bool* is_dec_p, int* paw_p) {
    
	// Check if it is a digit.
    if (key >= '0' && key <= '9')
    {
		unsigned char digit = key - '0'; // '2' --> 2

        switch (state) {

        case 0:
            if (digit) {
                //clear(); 						// Clear screen if input is non-zero.
                state = 1;
            }

        case 1:
            if (count == 14) return; 			// You cannot fillup the screen with a single operand.

			if (*is_dec_p == 0) {
            	a = a * 10 + digit;				// append to a
			} 

			if (*is_dec_p == 1) {				

				int d = 1;
				for (int i=0; i<*paw_p; i++) {
					d = d * 10;
				}

				a = (a * d + digit) / d;

				*paw_p = *paw_p + 1;
			}

            send_data(key);
            count++;
            break;

        case 2:
            if (digit) {
				state = 3;
			}

        case 3:
			if (*is_dec_p == 0) {
            	b = b * 10 + digit;				// append to b
			} 

			if (*is_dec_p == 1) {
				int d = 1;
				for (int i=0; i<*paw_p; i++) {
					d = d * 10;
				}

				b = (b * d + digit) / d;

				*paw_p = *paw_p + 1;
			}

            send_data(key);
            count++;
            break;

        case 4:
            if (digit) {
                line[0] = ' ';
                line[1] = ' ';

                move_to(0, 0);					//начало верхней строки
                send_string(line); 				// Clear 1st line.

                move_to(0, 0);
                a = b = op = count = 0;
                state = 1;

                decide(key, is_dec_p, paw_p); 	// Recursively capture digit.
                return;
            }
            break;

        }
    } 

	else if (key == '.') {
		*is_dec_p = 1;
		send_data(key);
	}

	else {
		
		*is_dec_p = 0;
		*paw_p = 1;		

        switch (key) {
        case '/':
        case '*':
        case '+':
        case '-':
        case 'p':
        case 'm':
		case 'v':
		case 'r':
            switch (state) {

            case 2:
                move_to(0,0); 		// Modify the operand displayed.

            case 1:
                send_data(key);
                count++;
                state = 2;
                break;

            case 3:
                calculate(a, op, b);
                show_result();
				ans = calculate(a, op, b);

            case 4:
                move_to(0, 0);
                line[0] = 'A'; 		// 'A' represents current result.
                line[1] = key; 		// Operator of the operation.
                send_string(line);

                move_to(2,0);
                count = 2;
                a = r; 				// Put result into a.
                b = 0; 				// Clear b.
                state = 2;

                break;
            }

            op = key;

            break;

        case '=':
            if(state == 3){
                calculate(a, op, b);
                show_result();
				ans = calculate(a, op, b);
            }
            break;

        case 'C': // "C" button.
            //if (state != 0) 
			reset();
            break;

        case 'A': // "ANS" button.
			send_data('A');
			b = ans;
			state = 3;
            break;

        case 'D': // "DEL" button.
			move_to(-1,0);
			send_data(' ');
            break;

		case 'O':
			on_off();
			break;
        }
    }
}

