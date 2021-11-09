// LIQUIDCRYSTAL_I2C_PCF8574 V1.0 adapted for Atmel Studio by AD-SDU. 2019

#ifndef LCD_H
#define LCD_H

#ifdef	__cplusplus
extern "C" {
	#endif

	void LCD_init(void);
	void LCD_write_char(char message);
	void LCD_write_str(const char *message);

	void LCD_clear(void);
	void LCD_home(void);

	void LCD_display_off(void);
	void LCD_display_on(void);
	void LCD_blink_off(void);
	void LCD_blink_on(void);
	void LCD_cursor_off(void);
	void LCD_cursor_on(void);
	void LCD_scroll_display_left(void);
	void LCD_scroll_display_right(void);
	void LCD_left_to_right(void);
	void LCD_right_to_Left(void);
	void LCD_no_backlight(void);
	void LCD_backlight(void);
	void LCD_autoscroll(void);
	void LCD_no_autoscroll(void);
	void LCD_create_char(unsigned char location, unsigned char charmap[]);
	void LCD_set_cursor(unsigned char col, unsigned char row);

    extern void LCD_command_write(unsigned char value);
	extern unsigned char LCD_command_read(void);
	extern void LCD_data_write(unsigned char value);
	extern unsigned char LCD_data_read(void);
	unsigned char LCD_busy(void);
	unsigned char LCD_address_counter(void);
	unsigned char LCD_read_DDRam(unsigned char address);
	unsigned char LCD_read_CGRam(unsigned char address);
	#ifdef	__cplusplus
}
#endif

#endif // LCD_H
