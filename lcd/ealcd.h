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

#ifndef __EALCD_H
#define __EALCD_H

/* command "bases", commands before any params set */
#define EALCD_CLEAR_BASE	(1 << 0)
#define EALCD_HOME_BASE		(1 << 1)
#define EALCD_DC_BASE		(1 << 3)
#define EALCD_FS_BASE		(1 << 5)

/* non data pins */
#define EALCD_P_EN		(1 << 0)
#define EALCD_P_RW		(1 << 1)
#define EALCD_P_RS		(1 << 2)
#define EALCD_P_PO		(1 << 4)

/* function set stuff */
#define EALCD_P_FS_DL		(1 << 4)
#define EALCD_P_FS_N		(1 << 3)
#define EALCD_P_FS_F		(1 << 2)

/* display control stuff */
#define EALCD_P_DC_D		(1 << 2)
#define EALCD_P_DC_C		(1 << 1)
#define EALCD_P_DC_B		(1 << 0)

/* bit bang time allowances */
#define EALCD_DELAY_INIT	15
#define EALCD_DELAY_CMD		5
#define EALCD_DELAY_DATA	1

void			ealcd_init();
void			ealcd_write4(uint8_t rs, uint8_t rw, uint8_t data);
void			ealcd_write8(uint8_t rs, uint8_t rw, uint8_t data);
void			ealcd_put_char(char c);
void			ealcd_put_string(char *s);
void			ealcd_function_set(uint8_t num_lines);
void			ealcd_display_ctrl(uint8_t on, uint8_t cursor,
				uint8_t blink);
void			ealcd_clear();
void			ealcd_home();

#endif /* ealcd.h */
