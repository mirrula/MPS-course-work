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
	void uart(void);
	float add_digit(float, unsigned char, bool*, int*);

#endif
