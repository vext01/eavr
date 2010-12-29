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

#ifndef __EALCD_PARAMS_H
#define __EALCD_PARAMS_H

/* which ports is your shit on? */
#define	EALCD_CBUS_PORT		PORTB
#define EALCD_DBUS_PORT		PORTC

/*
 * where on the ports is your shit?
 *
 * Eg, if ctrl pins are on pins 0, 1, 2, 3 of a port:
 *   EALCD_CBUS_SHIFT 0
 *
 * Eg, if data pins are on pins 2, 3, 4, 5 of a port:
 *   EALCD_CBUS_SHIFT 2
 */
#define EALCD_DBUS_SHIFT	0
#define EALCD_CBUS_SHIFT	0

#endif
