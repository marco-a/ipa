/*!
 *	@file		SigGen.h
 *	@brief
 *	Modul zur Generierung des Sensorsignals.
 *	Mit `SigGen__setFrequency` lässt sich die
 *	Frequenz im Bereich von 0 bis 5kHz einstellen.
 *
 *	Beispiel:
 *	Modul aktivieren:
 *	SigGen__enable();
 *	Stellt Frequenz auf 50Hz ein:
 *	SigGen__setFrequency(50);
 *
 *	@warning
 *		- Interrupte müssen aktiviert sein.
 *
 *	@author		Marco Agnoli
 *	@copyright
 */
#if !defined(JAQ_SIGGEN_H)
	#define JAQ_SIGGEN_H 1

	#include <common/common.h>

	/*!
	 *	@function	SigGen__enable
	 *	@brief
	 *	Aktiviert das SigGen Modul.
	 */
	void SigGen__enable(void);

	/*!
	 *	@function	SigGen__setFrequency
	 *	@brief
	 *	Setzt die Frequenz für das Ausgangssignal im
	 *	Bereich von 0 bis 5kHz.
	 *
	 *	@param		nFrequency	Frequenz im Bereich von 0 bis 5kHz.
	 *
	 *	@warning
	 *		- SigGen__enable muss vorher aufgerufen worden sein!
	 */
	void SigGen__setFrequency(u16 nFrequency);

	/*!
	 *	@function	SigGen__disable
	 *	@brief
	 *	Deaktiviert das SigGen Modul.
	 */
	void SigGen__disable(void);

#endif // !defined(JAQ_SIGGEN_H)
