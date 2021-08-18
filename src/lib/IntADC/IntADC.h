/*!
 *	@file		IntADC.h
 *	@brief
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_INTADC_H)
	#define JAQ_INTADC_H 1

	#include <common/common.h>

	enum IntADC__channelSetting {
		IntADCCH0 = 0b000,
		IntADCCH1 = 0b001,
		IntADCCH2 = 0b010,
		IntADCCH3 = 0b011,
		IntADCCH4 = 0b100,
		IntADCCH5 = 0b101,
		IntADCCH6 = 0b110,
		IntADCCH7 = 0b111
	};

	typedef		enum IntADC__channelSetting		IntADC_channel_t;

	void IntADC__enable(void);

	/*!
	 *	@function	IntADC__startMeasurement
	 *	@brief
	 *	Startet eine Messung mit dem internen ADC.
	 *
	 *	@param		nCH			Kanalselektion (siehe IntADC_channel_t)
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void IntADC__startMeasurement(IntADC_channel_t nCH);

	/*!
	 *	@function	IntADC__isDone
	 *	@brief
	 *	Prüft ob der interne ADC mit der Messung fertig ist.
	 *
	 *	@param		dResult		Wenn dResult nicht 'NULL' ist, wird das
	 *	Ergebnis dort abgelegt. (Einheit: V)
	 *
	 *	@return
	 *	'TRUE' wenn Messung fertig, ansonsten 'FALSE'.
	 *
	 *	@warning
	 *		- IntADC__startMeasurement muss vorher aufgerufen worden sein!
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	bool IntADC__isDone(ldbl *dResult);

	/*!
	 *	@function	IntADC__disable
	 *	@brief
	 *	Deaktiviert den internen ADC.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void IntADC__disable(void);

#endif // !defined(JAQ_INTADC_H)
