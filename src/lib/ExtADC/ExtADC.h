/*!
 *	@file		ExtADC.h
 *	@brief
 *	Dieses Modul übernimmt die Ansteuerung des 
 *	externen ADCs über das I2C Protokoll.
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_EXTADC_H)
	#define JAQ_EXTADC_H 1

	#include <common/common.h>

	enum ExtADC__gainSetting {
		ExtADCGain1 = 0b00,
		ExtADCGain2 = 0b01,
		ExtADCGain4 = 0b10,
		ExtADCGain8 = 0b11
	};

	enum ExtADC__channelSetting {
		ExtADCCH1 = 0b00,
		ExtADCCH2 = 0b01,
		ExtADCCH3 = 0b10,
		ExtADCCH4 = 0b11
	};

	enum ExtADC__resolutionSetting {
		ExtADC12Bit = 0b00,
		ExtADC14Bit = 0b01,
		ExtADC16Bit = 0b10,
		ExtADC18Bit = 0b11
	};

	typedef		enum ExtADC__gainSetting		ExtADC_gain_t;
	typedef		enum ExtADC__channelSetting		ExtADC_channel_t;
	typedef		enum ExtADC__resolutionSetting	ExtADC_resolution_t;

	/*!
	 *	@function	ExtADC__enable
	 *	@brief
	 *	Aktiviert das ExtADC Modul.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void ExtADC__enable(void);

	/*!
	 *	@function	ExtADC__startMeasurement
	 *	@brief
	 *	Startet eine neue Messung mit dem externen ADC.
	 *
	 *	@param		nGain		Gaineinstellung. (1x, 2x, 4x oder 8x)
	 *	@param		nCH			Kanalselektion (Kanal 1 bis 4)
 	 *	@param		nRes		Auflösung (12, 14, 16 oder 18 Bits)
	 *	12 Bit	~5ms
	 *	14 Bit	~20ms
	 *	16 Bit	~70ms
	 *	18 Bit	~270ms
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void ExtADC__startMeasurement(ExtADC_gain_t nGain, ExtADC_channel_t nCH, ExtADC_resolution_t nRes);

	/*!
	 *	@function	ExtADC__isDone
	 *	@brief
	 *	Prüft ob der externe ADC mit der Messung fertig ist.
	 *
	 *	@param		dResult		Wenn `dResult` nicht 'NULL' ist, wird das
	 *	Ergebnis dort abgelegt. (Einheit: V)
	 *
	 *	@return		bool
	 *	'TRUE' wenn Messung fertig, ansonsten 'FALSE'.
	 *
	 *	@warning
	 *		- ExtADC__startMeasurement muss vorher aufgerufen worden sein!
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	bool ExtADC__isDone(ldbl *res);

	/*!
	 *	@function	ExtADC__disable
	 *	@brief
	 *	Deaktiviert das ExtADC Modul.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void ExtADC__disable(void);

#endif // !defined(JAQ_EXTADC_H)
