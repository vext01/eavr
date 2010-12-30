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
#include <util/delay.h>
#include <stdio.h>

#include "ealcd_params.h"
#include "ealcd.h"

/*
 * We will do a fresh reset of the lcd power, so that the arduino
 * initialisation will not screw up the lcd configuration
 */
#define LCD_P_POWER		(1 << 3)
#define LCD_POWER_PORT		PORTB

void
demo_title()
{
	char			rev[20] = "$Revision: 11 $", *c;

	/* clear screen and start at far left */
	ealcd_display_ctrl(1, 1, 1);
	ealcd_home();
	ealcd_clear();

	ealcd_set_ddram_addr(4);
	ealcd_put_string("EAVR LCD");

	ealcd_set_ddram_addr(67);
	ealcd_put_string("Version");
	for (c = rev; *c != ':'; c ++);
	for (; *c != '$'; c ++)
		ealcd_put_char(*c);

	_delay_ms(2000);
}

void
demo_free()
{
	ealcd_display_ctrl(1, 1, 1);
	ealcd_clear();

	ealcd_set_ddram_addr(3);
	ealcd_put_string("Free Code");
	ealcd_set_ddram_addr(65);
	ealcd_put_string("<ISC Licensed>");

	_delay_ms(3000);
}

void
demo_chevrons()
{
	int			i, j;

	/* no cursor for this one */
	ealcd_display_ctrl(1, 0, 0);

	for (j = 0; j < 4; j ++) {
		ealcd_clear();
		ealcd_put_string(">>");
		ealcd_set_ddram_addr(64);
		ealcd_put_string(">>");
		for (i = 0; i < 16; i++) {
			ealcd_shift(EALCD_SHIFT_DISP, EALCD_SHIFT_RIGHT);
			if (i == 8) {
				ealcd_put_char('>');
			}
			_delay_ms(50);
		}
	}
}

int
main(void)
{

	/* you still have to do data dirctions manually */
	DDRC = 0b00001111;
	DDRB = 0b00001111;

	/* power lcd logic */
	LCD_POWER_PORT = LCD_POWER_PORT | LCD_P_POWER;

	/* set up lcd */
	ealcd_init();

	/* set number of lines and font */
	ealcd_function_set(2, 0);

	/* cursor on, blink, font 1 */
	ealcd_display_ctrl(1, 1, 1);

	while(1) {
		demo_title();
		demo_chevrons();
		demo_free();
		demo_chevrons();
	}

	return (0);
}
