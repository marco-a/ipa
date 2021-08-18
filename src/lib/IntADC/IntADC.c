/*!
 *	@file		IntADC.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <IntADC/IntADC.h>

// Statische Definitionen --------------------------------
static bool __bStarted	= FALSE;

static INLINE ldbl __toVoltage(u16 nReading) {
	return nReading * (ldbl)4E-3L;
}
// Statische Definitionen --------------------------------

/*!
 *	@function	IntADC__enable
 */
void IntADC__enable(void) {
	// Vorerst alles deaktivieren
	IntADC__disable();

	// ADC aktivieren
	ADCSRA |= _BV(ADEN);
	// Samplefrequenz einstellen
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

/*!
 *	@function	IntADC__startMeasurement
 */
void IntADC__startMeasurement(IntADC_channel_t nCH) {
	ASSERT(BIT_ISSET(ADCSRA, ADEN));
	ASSERT(__bStarted == FALSE);
	ASSERT(nCH < 8);

	// Löschen von Kanalselektion
	ADMUX &= 0b11100000;
	// Kanalselektion setzen
	ADMUX |= nCH;
	ADCW   = 0;

	_delay_ms(1);

	// Wandelung starten
	ADCSRA |= _BV(ADSC);

	__bStarted = TRUE;
}

/*!
 *	@function	IntADC__isDone
 */
bool IntADC__isDone(ldbl *dResult) {
	ASSERT(__bStarted == TRUE);

	bool bIsDone	= !BIT_ISSET(ADCSRA, ADSC);

	if (bIsDone == TRUE) {
		if (dResult != NULL) {
			*dResult = __toVoltage(ADCW);
		}

		__bStarted = FALSE;
	}

	return bIsDone;
}

/*!
 *	@function	IntADC__disable
 */
void IntADC__disable(void) {
	// Messung muss abgeschlossen sein
	ASSERT(__bStarted == FALSE);
	// Wandelung muss fertig sein
	ASSERT(!BIT_ISSET(ADCSRA, ADSC));

	// ADC deaktivieren
	ADCSRA	= 0x00;
}
