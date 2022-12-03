#ifndef LCD_H

	#define LCD_H

	void init_display(void);

	void send_command(unsigned char);
	void send_data(unsigned char);
	void send_string(const char*);

	void clear(void);
	void disp_on_off(void);

	void move_to(unsigned char, unsigned char);
	void trigger(void);


#endif
