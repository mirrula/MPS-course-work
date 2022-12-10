#include <stdlib.h>
#include <util/delay.h>

#include "calculator.h"
#include "keyboard.h"
#include "lcd.h"
#include "uart.h"

enum exp_states
{
    start_disp,		//0 Displaying banner.
	a_input,		//1 Input first operand, a.
	op_input,		//2 Middle Operator pressed, op.
	b_input,		//3 Input second operand, b.
	result_disp,	//4 Result displayed
};

float r, a, b; 								// a and b are operands, r is the result.
float ans;									// for "ANS" operator
int count; 									// Number of characters displaying on first line.
enum exp_states state; 						// Calculator and Monitor state.
unsigned char op; 							// Operator: [/, *, +, -, ...].

static char line[] = "                "; 	// Single line, 16 characters long.
static char buffer[16];
static char overflow[] = "   Overflow   ";

/////////////////////////////////////////////////////////////////////////////////////////////

void init_calculator() {
    _delay_ms(500);

    init_display();
    init_keyboard();
	UsartInit();

    reset();
    run();
}

void reset() {
    clear();
    move_to(0, 0);		//начало верхней строки
    r = a = b = op = count = 0;
	state = start_disp;
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

void send_digit(unsigned char digit) {
    send_data(digit + '0');
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
    case 'p'://%
        return r = m * n / 100;
    case 'm'://mod
        return r = (int)m % (int)n;
    case 'v'://div
        return r = (int)m / (int)n;

    }
    return r = m;
}

void show_result() {
    move_to(0, 1);			//bottom line start
    send_string("A=");
    
	char* data = buffer;
    if (r <= 999999999999)
        dtostrf(r, 9, 5, buffer);	//float -> char* (r -> buffer)
    else 
		data = overflow;
    send_string(data);

    state = result_disp;
}

void uart() {
	char uart_ans[64];
	dtostrf(ans, 9, 5, uart_ans);	//float -> char* (ans -> uart_ans)
	Transmit(uart_ans);
}

void decide(unsigned char key, bool* is_dec_p, int* paw_p) {
    
	// Check if it is a digit.
    if (key >= '0' && key <= '9')
    {
		unsigned char digit = key - '0'; // '2' --> 2

        switch (state) {

        case start_disp:
            if (digit) {
                state = a_input;
            }

        case a_input:
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

        case op_input:
            if (digit) {
				state = b_input;
			}

        case b_input:
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

        case result_disp:
            if (digit) {
                line[0] = ' ';
                line[1] = ' ';

                move_to(0, 0);					// top string
                send_string(line); 				// Clear 1st line.

                move_to(0, 0);
                a = b = op = count = 0;
                state = a_input;

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
            switch (state) {

            case op_input:
                move_to(0,0); 		// Modify the operand displayed.

            case a_input:
                send_data(key);
                count++;
                state = op_input;
                break;

            case b_input:
                calculate(a, op, b);
                show_result();
				ans = calculate(a, op, b);

            case result_disp:
                move_to(0, 0);
                line[0] = 'A'; 		// 'A' represents current result.
                line[1] = key; 		// Operator of the operation.
                send_string(line);

                move_to(2,0);
                count = 2;
                a = r; 				// Put result into a.
                b = 0; 				// Clear b.
                state = op_input;

                break;
            }

            op = key;

            break;

        case '=':
            if(state == b_input){
                calculate(a, op, b);
                show_result();
				ans = calculate(a, op, b);
            }
            break;

        case 'C': // "C" button.
			reset();
            break;

        case 'A': // "ANS" button.
			send_data('A');
			b = ans;
			state = b_input;
            break;

		case 'O':// "ON/OFF" button.
			disp_on_off();
			break;

		case 'u':// "uart" button.
			uart();
			break;
        }
    }
}

