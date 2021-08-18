/*!
 *	@file		common.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <common/common.h>
#include <LCD/LCD.h>		// LCD__*
#include <stdarg.h>			// va_list

/*!
 *	@function	throw
 */
void throw(const char *sFile, u32 nLine, const char *sMsg, ...) {
	DISABLE_INTERRUPTS();

	char sMessage[16];
	va_list ap;

	memset(sMessage, 0, sizeof(sMessage));
	va_start(ap, sMsg);
	vsnprintf(sMessage, sizeof(sMessage), sMsg, ap);
	va_end(ap);
	
	LCD__init();
	LCD__clearScreen();
	LCD__print("%-10s\nLine: %4" PRIu32, sFile, nLine);

	for (;;);
}
