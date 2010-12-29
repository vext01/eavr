/* XXX, i don't like this file */

#ifndef __EALCD_PARAMS_H
#define __EALCD_PARAMS_H

/* which ports is your shit on? */
#define	EALCD_CTRL			PORTB
#define EALCD_DATA			PORTC

/*
 * where on the ports is your shit?
 *
 * Eg, if ctrl pins are on pins 0, 1, 2, 3 of a port:
 *   EALCD_CBUS_SHIFT 0
 *
 * Eg, if data pins are on pins 2, 3, 4, 5 of a port:
 *   EALCD_CBUS_SHIFT 2
 */
#define EALCD_DBUS_SHIFT		0
#define EALCD_CBUS_SHIFT		0

#endif
