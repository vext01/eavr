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
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "ealcd_params.h"
#include "ealcd.h"

/*
 * XXX make operations non-blatting, ie use the current values of the
 * AVR ports and just ammend what we need to change. Will prevent
 * peripherals on other pins from being nuked.
 *
 * Also currently assumed that the 4th bit of the control bus is the
 * LCD logic power. We hold this low and bring it up after so that the
 * arduino initialisation does not interfere with the LCD configuration.
 */

void
ealcd_init()
{
	/* power lcd logic */
	EALCD_CTRL = EALCD_P_PO;
	_delay_ms(EALCD_DELAY_INIT);
}

/* write in 4 byte mode (LSb) */
void
ealcd_write4(uint8_t rs, uint8_t rw, uint8_t data)
{
	uint8_t			ctrl = 8; /* EALCD_CTRL << 4 == lcd power */

	if (rw)
		ctrl += EALCD_P_RW;

	if (rs)
		ctrl += EALCD_P_RS;

	/* data bus on 4 most sig bits of PORTD */
	EALCD_DATA = data << EALCD_DBUS_SHIFT;

	/* bring EN pin up and down again */
	EALCD_CTRL = ctrl & ((~EALCD_P_EN) & 0x0f);
	_delay_ms(EALCD_DELAY_CMD);
	EALCD_CTRL = ctrl | EALCD_P_EN;
	_delay_ms(EALCD_DELAY_CMD);
	EALCD_CTRL = ctrl & ((~EALCD_P_EN) & 0x0f);
}

void
ealcd_write8(uint8_t rs, uint8_t rw, uint8_t data)
{
	uint8_t			low, high;
	uint16_t		delay = EALCD_DELAY_CMD;

	/* data bus is much faster to write */
	if (rs)
		delay = EALCD_DELAY_DATA;

	/* we must do the 8 bit write in 2 small 4 bits ones */
	low = data & 0x0f;
	high = (data & 0xf0) >> 4;

	ealcd_write4(rs, rw, high);
	ealcd_write4(rs, rw, low);

	/* a delay after each 2 nibbles */
	_delay_ms(delay);
}

void
ealcd_put_char(char c)
{
	ealcd_write8(1, 0, c);
}

void
ealcd_put_string(char *s)
{
	for (; *s != 0; s ++)
		ealcd_put_char(*s);
}

/*
 * set number of bits to 4, number of lines and font
 */
void
ealcd_function_set(uint8_t num_lines)
{
	uint8_t			ctrl = EALCD_FS_BASE;

	switch (num_lines) {
	case 2:
		ctrl = ctrl + EALCD_P_FS_N;
		break;
	case 1:
		break;
	default:
		exit (0); /* cant do this */
	}

	/* XXX other font */

	ealcd_write8(0, 0, ctrl);
	ealcd_write4(0, 0, 0);
}

void
ealcd_display_ctrl(uint8_t on, uint8_t cursor, uint8_t blink)
{
	uint8_t			ctrl = EALCD_DC_BASE;

	if (on)
		ctrl += EALCD_P_DC_D;

	if (cursor)
		ctrl += EALCD_P_DC_C;

	if (blink)
		ctrl += EALCD_P_DC_B;

	ealcd_write8(0, 0, ctrl);
}

void
ealcd_clear()
{
	ealcd_write8(0, 0, EALCD_CLEAR_BASE);
}

void
ealcd_home()
{
	ealcd_write8(0, 0, EALCD_HOME_BASE);
}
