/*
 * Copyright (c) 2010, Edd Barrett <vext01@gmail.com>
 * $Id$
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * A 4-bit HD44780 interface for an AVR board.
 * Currently does not check busy status of the controller, only bitbangs.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

/* non data pins */
#define LCD_P_EN		(1 << 0)
#define LCD_P_RW		(1 << 1)
#define LCD_P_RS		(1 << 2)

/* function set stuff */
#define LCD_FS_BASE		(1 << 5)

#define LCD_P_FS_DL		(1 << 4)
#define LCD_P_FS_N		(1 << 3)
#define LCD_P_FS_F		(1 << 2)

/* display control stuff */
#define LCD_DC_BASE		(1 << 3)

#define LCD_P_DC_D		(1 << 2)
#define LCD_P_DC_C		(1 << 1)
#define LCD_P_DC_B		(1 << 0)

#define LCD_CLEAR_BASE		(1)
#define LCD_HOME_BASE		(2)

/* which ports is your shit on? */
#define	LCD_CTRL		PORTB
#define LCD_DATA		PORTD

/* bit bang time allowances */
#define LCD_DELAY_INIT		15
#define LCD_DELAY_CMD		6
#define LCD_DELAY_DATA		1

uint8_t				lcd_bits = 4;

/*
 * XXX make operations non-blatting, ie use the current values of the
 * AVR ports and just ammend what we need to change. Will prevent
 * peripherals on other pins from being nuked.
 */ 

/* write in 4 byte mode (LSb) */
/* don't call this yourself */
void
lcd_write4(uint8_t rs, uint8_t rw, uint8_t data)
{
	uint8_t			ctrl = 0;

	if (rw)
		ctrl += LCD_P_RW;

	if (rs)
		ctrl += LCD_P_RS;

	/* data bus on 4 most sig bits of PORTD */
	LCD_DATA = data << 4;

	/* bring EN pin up and down again */
	LCD_CTRL = ctrl & ((~LCD_P_EN) & 0x0f);
	_delay_ms(LCD_DELAY_CMD);
	LCD_CTRL = ctrl | LCD_P_EN;
	_delay_ms(LCD_DELAY_CMD);
	LCD_CTRL = ctrl & ((~LCD_P_EN) & 0x0f);

}

void
lcd_write8(uint8_t rs, uint8_t rw, uint8_t data)
{
	uint8_t			low, high;
	uint16_t		delay = LCD_DELAY_CMD;

	/* data bus is much faster to write */
	if (rs)
		delay = LCD_DELAY_DATA;

	low = data & 0x0f;
	high = (data & 0xf0) >> 4;

	lcd_write4(rs, rw, high);
	lcd_write4(rs, rw, low);

	/* a delay after each 2 nibbles */
	_delay_ms(delay);
}

void
lcd_put_char(char c)
{
	lcd_write8(1, 0, c);
}

void
lcd_put_string(char *s)
{
	for (; *s != 0; s ++)
		lcd_put_char(*s);
}

/*
 * set number of bits, number of lines and font
 */
void
lcd_function_set(uint8_t num_lines)
{
	uint8_t			ctrl = LCD_FS_BASE;

	switch (lcd_bits) {
	case 8:
		ctrl = ctrl + LCD_P_FS_N;
		break;
	case 4:
		break;
	default:
		exit (1); /* bad, should not happen */
		break;
	};

	switch (num_lines) {
	case 2:
		ctrl = ctrl + LCD_P_FS_N;
		break;
	case 1:
		break;
	default:
		exit (0); /* cant do this */
	}

	/* XXX other font */

	lcd_write8(0, 0, ctrl);
	if (lcd_bits == 4)
		lcd_write4(0, 0, 0);
}

void
lcd_display_ctrl(uint8_t on, uint8_t cursor, uint8_t blink)
{
	uint8_t			ctrl = LCD_DC_BASE;

	if (on)
		ctrl += LCD_P_DC_D;

	if (cursor)
		ctrl += LCD_P_DC_C;

	if (blink)
		ctrl += LCD_P_DC_B;

	lcd_write8(0, 0, ctrl);
}

void
lcd_clear()
{
	lcd_write8(0, 0, LCD_CLEAR_BASE);
}

void
lcd_home()
{
	lcd_write8(0, 0, LCD_HOME_BASE);
}

void
debug_blink()
{
	return;
	PORTC = 0xff;
	_delay_ms(500);
	PORTC = 0x0;
}


int
main(void)
{
	/* set PORTD for LCD output */
	DDRB = 0xff;
	DDRD = 0xf0;
	DDRC = 0xff;

	LCD_DATA = 0;
	LCD_CTRL = 0;

	_delay_ms(LCD_DELAY_INIT);

	/*
	 * the following is the example from the data sheet
	 */

	/* 4 bit operation */
	//debug_blink();
	//lcd_write4(0, 0, 0b0010);

	/* 4 bit operation (again?!) + set font */
	//debug_blink();
	//lcd_write4(0, 0, 0b0010);
	//lcd_write4(0, 0, 0b0000);

	lcd_function_set(2);

	/* turn on display and cursor */
	//debug_blink();
	//lcd_write4(0, 0, 0b0000);
	//lcd_write4(0, 0, 0b1110);

	lcd_display_ctrl(1, 1, 1);


	//lcd_put_string("T");
	//

	/* home */
	//debug_blink();
	//lcd_write4(0, 0, 0b0);
	//lcd_write4(0, 0, 0b10);

	lcd_home();

	lcd_clear();

	/* put a 'H' */
	//debug_blink();
	//lcd_write4(1, 0, 0b100);
	//lcd_write4(1, 0, 0b1000);

	lcd_put_string("FUCKSHIT!!!!!");

	while(1);

	return (0);
}