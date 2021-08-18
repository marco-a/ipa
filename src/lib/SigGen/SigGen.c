/*!
 *	@file		SigGen.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <SigGen/SigGen.h>

// Statische Definitionen --------------------------------
static volatile u8 __nOverflows		= 0;
static volatile u8 __nOverflowsCtn	= 0;
// Statische Definitionen --------------------------------

/*!
 *	ISR Prioritätenliste:
 *
 *	0 - TIMER1_COMPA_vect
 *	1 - TIMER1_OVF_vect
 */

/*!
 **********************************************************
 * TIMER 1 COMPARE MATCH INTERRUPT
 **********************************************************
 */
ISR(TIMER1_COMPA_vect, ISR_BLOCK) {
	if (__nOverflowsCtn == 0) {
		PORTD ^= _BV(PD5);

		__nOverflowsCtn = __nOverflows;

		TCNT1 = 0;
	}
}

/*!
 **********************************************************
 * TIMER 1 OVERFLOW INTERRUPT
 **********************************************************
 */
ISR(TIMER1_OVF_vect, ISR_BLOCK) {
	ASSERT(__nOverflowsCtn > 0);

	--__nOverflowsCtn;
}

/*!
 *	@function	SigGen__enable
 */
void SigGen__enable(void) {
	// Pin als Ausgang konfigurieren
	DDRD |= _BV(PD5);
}

/*!
 *	@function	SigGen__setFrequency
 */
void SigGen__setFrequency(u16 nFrequency) {
	INTERRUPTS_REQUIRED();
	ASSERT(nFrequency <= 5E3);

	ldbl 	dTemp			= (16.0E6 / ((ldbl)nFrequency * 2.0L));
	bool	bIsLowFreq		= (nFrequency < 200u);
	u8		nOverflows		= (bIsLowFreq ? dTemp / 65535u : 0);
	u16		nRemainder		= ((u32)dTemp) % 65535u;

	ASSERT(nOverflows <= 255);

	// Timer 1 stoppen
	TCCR1B = 0;
	TCCR1A = 0;

	if (bIsLowFreq == TRUE) {
		// Overflow und Compare Match Interrupte aktivieren
		TIMSK |= _BV(TOIE1);
		TIMSK |= _BV(OCIE1A);
		// Waveform Generator deaktivieren
		TCCR1A &= ~_BV(COM1A0);
		TCCR1B &= ~_BV(WGM12);
	} else {
		// Waveform Generator aktivieren
		TCCR1A |= _BV(COM1A0);
		TCCR1B |= _BV(WGM12);
		// Overflow und Compare Match Interrupte deaktivieren
		TIMSK &= ~_BV(TOIE1);
		TIMSK &= ~_BV(OCIE1A);
	}

	// Atomar neue Variablen schreiben
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		__nOverflows	= nOverflows;
		__nOverflowsCtn	= __nOverflows;
		OCR1A			= nRemainder;
		TCNT1			= 0;
	}

	// Timer mit der höchsten Auflösung starten
	TCCR1B |= _BV(CS10);
}

/*!
 *	@function	SigGen__disable
 */
void SigGen__disable(void) {
	
}
