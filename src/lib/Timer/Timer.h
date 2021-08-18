/*!
 *	@file		Timer.h
 *	@brief
 *	Hilfsmodul um Zeit abzählen zu können.
 *	`Timer__start` startet den Zähler.
 *	`Timer__hasExpired` prüft ob die angegebene Zeit
 *	verstrichen wurde.
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_TIMER_H)
	#define JAQ_TIMER_H 1

	#include <common/common.h>

	/*!
	 *	@function	Timer__start
	 *	@brief
	 *	Fängt an die Zeit `nTime` (in Millisekunden) abzuzählen.
	 *
	 *	@param		nTime		Zeitperiode in Millisekunden.
	 *
	 *	@warning
	 *		- Es sind nur vielfache von 16.35ms einstellbar!
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void Timer__start(u8 nTime);

	/*!
	 *	@function	Timer__hasExpired
	 *	@brief
	 *	Prüft ob die angebene Zeit verstrichen wurde.
	 *
	 *	@return		bool
	 *	'TRUE' falls Timer abgelaufen, ansonsten 'FALSE'.
	 *
	 *	@warning
	 *		- StopWatch__start muss vorher aufgerufen worden sein.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	bool Timer__hasExpired(void);

#endif // !defined(JAQ_TIMER_H)
