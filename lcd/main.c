/*
 * Copyright (c) 2010, Edd Barrett <vext01@gmail.com>
 * $Id: lcd.c 7 2010-12-29 17:22:43Z eb771 $
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

#include "lcdparams.h"
#include "ealcd.h"

int
main(void)
{
	char			rev[20] = "$Revision: 11 $", *c;

	/* set PORTB for LCD databus */
	DDRB = 0x0f;

	/* PORTC for LCD control bus */
	DDRC = 0x0f;

	LCD_DATA = 0;
	LCD_CTRL = 0;

	/* set up lcd */
	lcd_init();

	/* set number of lines */
	lcd_function_set(2);

	/* cursor on, blink, font 1 */
	lcd_display_ctrl(1, 1, 1);

	/* clear screen and start at far left */
	lcd_home();
	lcd_clear();

	lcd_put_string("EAVR LCD                                ");

	lcd_put_string("Version");
	for (c = rev; *c != ':'; c ++);
	for (; *c != '$'; c ++)
		lcd_put_char(*c);

	while(1);

	return (0);
}
