/*!
 *	@file		LCD.h
 *	@brief
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_LCD_H)
	#define JAQ_LCD_H 1

	#include <common/common.h>

	/*!
	 *	@function	LCD__init
	 *	@brief
	 *	Initialisert das LC-Display.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	UNUSED void LCD__init(void);

	/*!
	 *	@function	LCD__clearScreen
	 *	@brief
	 *	Löscht den gesamten Inhalt auf dem LC-Display.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	UNUSED void LCD__clearScreen(void);

	/*!
	 *	@function	LCD__clearLine
	 *	@brief
	 *	Löscht die Zeile `nY` komplett.
	 *
	 *	@param		nY
	 *	Zeile die gelöscht werden soll.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	UNUSED void LCD__clearLine(u8 nY);

	/*!
	 *	@function	LCD__gotoXY
	 *	@brief
	 *	Springt zum Punkt X = `nX` , Y = `nY`.
	 *
	 *	@param		nX
	 *	X-Koordinate zur der gesprungen werden soll.
	 *	@param		nY
	 *	Y-Koordinate zur der gesprungen werden soll.
	 */
	UNUSED void LCD__gotoXY(u8 nX, u8 nY);

	/*!
	 *	@function	LCD__print
	 *	@brief
	 *	Gibt `sStr` am LC-Display aus.
	 *
	 *	@param		sStr
	 *	Zeichenkette die ausgegeben werden soll.
	 *	@param		...
	 *	Allfällige Variablen.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	UNUSED void LCD__print(const char *sStr, ...) FORMAT(1, 2);

	/*!
	 *	@function	LCD__putc
	 *	@brief
	 *	Schreibt einen einzelnen Buchstaben auf
	 *	das LC-Display.
	 *
	 *	@param		cCHar
	 *	Buchstabe der geschrieben werden soll.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	UNUSED void LCD__putc(u8 cChar);

#endif // !defined(JAQ_LCD_H)
