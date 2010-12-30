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

void
ealcd_init()
{
	EALCD_POWERUP_WAIT
}

/* write in 4 byte mode (LSb) */
void
ealcd_write4(uint8_t rs, uint8_t rw, uint8_t data)
{
	//uint8_t			ctrl = EALCD_P_PO;
	uint8_t			cmask, c = EALCD_CBUS_PORT;
	uint8_t			dmask, d = EALCD_DBUS_PORT;

	/* zero off the ctrl bus  pins */
	cmask = 0x07 << EALCD_CBUS_SHIFT;
	c &= (~cmask);

	/* zero off the data bus pins */
	dmask = 0x0f << EALCD_DBUS_SHIFT;
	d &= (~dmask);

	if (rw)
		c += EALCD_P_RW << EALCD_CBUS_SHIFT;

	if (rs)
		c += EALCD_P_RS << EALCD_CBUS_SHIFT;

	EALCD_DBUS_PORT = (data << EALCD_DBUS_SHIFT) | d;

	/* bring EN pin down */
	EALCD_CBUS_PORT = c & ((~EALCD_P_EN) << EALCD_CBUS_SHIFT);

	/* EN up */
	EALCD_CBUS_PORT = c | (EALCD_P_EN << EALCD_CBUS_SHIFT);
	EALCD_EN_UP_DELAY

	/* EN down again */
	EALCD_CBUS_PORT = c & ((~EALCD_P_EN) << EALCD_CBUS_SHIFT);
	if (rs)
		EALCD_EN_DOWN_DELAY_DATA
	else
		EALCD_EN_DOWN_DELAY_CMD
}

void
ealcd_write8(uint8_t rs, uint8_t rw, uint8_t data)
{
	uint8_t			low, high;

	/* we must do the 8 bit write in 2 small 4 bits ones */
	low = data & 0x0f;
	high = (data & 0xf0) >> 4;

	ealcd_write4(rs, rw, high);
	ealcd_write4(rs, rw, low);
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
ealcd_function_set(uint8_t num_lines, uint8_t font)
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

	/* does not seem to work XXX */
	ctrl += (font * EALCD_P_FS_F);

	ealcd_write8(0, 0, ctrl);
	ealcd_write4(0, 0, 0);	/* as per datasheet */
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

void
ealcd_set_ddram_addr(uint8_t addr)
{
	ealcd_write8(0, 0, EALCD_SET_DD_ADDR_BASE + addr);
}

void
ealcd_shift(uint8_t screen_or_curs, uint8_t lr)
{
	uint8_t			d = EALCD_SHIFT_BASE;

	d += (lr * EALCD_P_SHIFT_LR);
	d += (screen_or_curs * EALCD_P_SHIFT_CD);

	ealcd_write8(0, 0, d);
}
