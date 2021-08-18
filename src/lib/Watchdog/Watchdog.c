/*!
 *	@file		Watchdog.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <Watchdog/Watchdog.h>
#include <avr/wdt.h>			// wdt_*

/*!
 *	MCUCSR vor dem main() Aufruf lesen.
 *	Siehe:
 *	http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
 */
u8 nMCUSRCopy __attribute__((__section__(".noinit")));
UNUSED void getMCUSR(void) __attribute__((__naked__)) __attribute__((__section__(".init3")));

void getMCUSR(void) {
	nMCUSRCopy	= MCUCSR;
	MCUCSR		= 0x00;
	wdt_disable();
}

/*!
 *	@function	Watchdog__init
 */
void Watchdog__init(void) {
	wdt_enable(WDTO_250MS);
}

/*!
 *	@function	Watchdog__reset
 */
void Watchdog__reset(void) {
	wdt_reset();
}

/*!
 *	@function	Watchdog__wasResetted
 */
bool Watchdog__wasResetted(void) {
	return BIT_ISSET(nMCUSRCopy, WDRF);
}
