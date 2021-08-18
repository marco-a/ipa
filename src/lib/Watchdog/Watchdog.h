/*!
 *	@file		Watchdog.h
 *	@brief
 *	Watchdoghilfsfunktionen.
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_WATCHDOG_H)
	#define JAQ_WATCHDOG_H 1

	#include <common/common.h>

	/*!
	 *	@function	Watchdog__init
	 *	@brief
	 *	Startet den Watchdog.
	 *	Der Watchdog muss danach alle 250 Millisekunden
	 *	zurückgesetzt werden.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void Watchdog__init(void);

	/*!
	 *	@function	Watchdog__reset
	 *	@brief
	 *	Setzt den Watchdogzähler zurück.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void Watchdog__reset(void);

	/*!
	 *	@function	Watchdog__wasResetted
	 *	@brief
	 *	Gibt an ob der Mikrokontroller vom
	 *	Watchdog zurückgesetzt wurde.
	 *
	 *	@return		bool
	 *	'TRUE' wenn von Watchdog zurückgesetzt, ansonsten 'FALSE'.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	bool Watchdog__wasResetted(void);

#endif // !defined(JAQ_WATCHDOG_H)
