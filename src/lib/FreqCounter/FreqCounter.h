/*!
 *	@file		FreqCounter.h
 *	@brief
 *	Dieses Modul übernimmt die Frequenzmessung des
 *	Openkollektorausgangs. Für die Messung werden
 *	den externen Interrupt (INT2) sowie den Zähler
 *	Timer/Counter0 verwendet.
 *	Die `FreqCounter__startMeasurement` Funktion
 *	startet ein Messung. Solange die Frequenz
 *	nicht bestimmt wurde gibt `FreqCounter__isDone`
 *	'false' zurück. Sobald die Frequenz bestimmt
 *	werden konnte gibt sie 'true' zurück.
 *	Eine Messung zu starten obwohl bereits eine
 *	im Gange ist, ist ungültig und hat einen
 *	Programmabruch zur Folge.
 *
 *	@warning
 *		- Interrupte müssen aktiviert sein!
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_FREQCOUNTER_H)
	#define JAQ_FREQCOUNTER_H 1

	#include <common/common.h>

	/*!
	 *	@function	FreqCounter__enable
	 *	@brief
	 *	Aktiviert den Frequenzzähler.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void FreqCounter__enable(void);

	/*!
	 *	@function	FreqCounter__startMeasurement
	 *	@brief
	 *	Startet eine neue Frequenzmessung.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void FreqCounter__startMeasurement(void);

	/*!
	 *	@function	FreqCounter__isDone
	 *	@brief
	 *	Prüft ob die Frequenzmessung fertiggestellt wurde.
	 *
	 *	@param		dResult		Wenn dResult nicht 'NULL' ist, wird das
	 *	Ergebnis dort abgelegt. (Einheit: Hz)
	 *
	 *	@return		bool
	 *	'TRUE' wenn Messung fertig, ansonsten 'FALSE'.
	 *
	 *	@warning
	 *		- FreqCounter__startMeasurement muss vorher aufgerufen worden sein!
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	bool FreqCounter__isDone(ldbl *dResult);

	/*!
	 *	@function	FreqCounter__disable
	 *	Deaktiviert den Frequenzzähler.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void FreqCounter__disable(void);

#endif // !defined(JAQ_FREQCOUNTER_H)
