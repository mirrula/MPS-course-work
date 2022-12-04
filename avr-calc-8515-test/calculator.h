#ifndef CALCULATOR_H

	#define CALCULATOR_H

	#include <stdbool.h>

	void init_calculator(void);
	void run(void);
	void reset(void);
	double calculate(float, char, float);
	void show_result(void);
	void decide(unsigned char, bool*, int*);
	void send_digit(unsigned char);
	void on_off(void);
	void uart(void);

#endif
