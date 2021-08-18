/*!
 *	@file		LCD.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <LCD/LCD.h>
#include <LCD/_lcd.h>	// lcd_*
#include <stdarg.h>		// va_list

// Statische Definitionen --------------------------------
static bool __bInited	= FALSE;
// Statische Definitionen --------------------------------

/*!
 *	@function	LCD__init
 */
void LCD__init(void) {
	if (__bInited == FALSE) {
		lcd_init(LCD_DISP_ON);

		__bInited = TRUE;
	}
}

/*!
 *	@function	LCD__init
 */
void LCD__clearScreen(void) {
	lcd_clrscr();
}

/*!
 *	@function	LCD__clearLine
 */
void LCD__clearLine(u8 nY) {
	LCD__gotoXY(0, nY);

	for (u8 nI = 0; nI < 16; ++nI) {
		LCD__putc(' ');
	}

	LCD__gotoXY(0, nY);
}

/*!
 *	@function	LCD__gotoXY
 */
void LCD__gotoXY(u8 nX, u8 nY) {
	lcd_gotoxy(nX, nY);
}

/*!
 *	@function	LCD__print
 */
void LCD__print(const char *sStr, ...) {
	char cBuffer[32];
	int nNum;

	if (!sStr) return;

	va_list ap;

	va_start(ap, sStr);
	nNum = vsnprintf(cBuffer, sizeof(cBuffer), sStr, ap);
	va_end(ap);

	if (nNum <= 0) return;

	lcd_puts(cBuffer);
	
	/*
	for (int i = 0; i < nNum; ++i) {
		lcd_putc(cBuffer[i]);
	}*/
}

/*!
 *	@function	LCD__putc
 */
void LCD__putc(u8 cChar) {
	lcd_putc(cChar);
}
