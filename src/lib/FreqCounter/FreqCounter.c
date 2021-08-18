/*!
 *	@file		FreqCounter.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <FreqCounter/FreqCounter.h>

// Statische Definitionen --------------------------------
static volatile u32 __nOverflows	= 0;
static volatile bool __bIsDone		= TRUE;
static volatile bool __bStarted		= FALSE;

static INLINE void __ExtINT2_enable(void) {
	// Aufruf von ISR verhindern
	GIFR |= _BV(INTF2);
	GICR |= _BV(INT2);
}

static INLINE void __ExtINT2_disable(void) {
	GICR &= ~_BV(INT2);
	// Aufruf von ISR verhindern
	GIFR |= _BV(INTF2);
}

static INLINE bool __ExtINT2_isEnabled(void) {
	return BIT_ISSET(GICR, INT2);
}

static INLINE void __Timer0_start(void) {
	// Timer/Counter0 mit Prescaler 1 starten
	TCCR0 = _BV(CS00);
}

static INLINE void __Timer0_stop(void) {
	// Zähler stoppen
	TCCR0 = 0x00;
	// Aufruf von ISR verhindern
	TIFR |= _BV(TOV0);
}

static INLINE bool __Timer0_isRunning(void) {
	return TCCR0 != 0x00;
}
// Statische Definitionen --------------------------------

/*!
 *	ISR Prioritätenliste:
 *
 *	0 - TIMER0_OVF_vect
 *	1 - INT2_vect
 */

/*!
 **********************************************************
 * TIMER 0 OVERFLOW INTERRUPT
 **********************************************************
 *	Wird beim Überlaufen von Zähler Timer/Counter0
 *	aufgerufen.
 *	Zeit bis erster Überlauf	= 15.9375 Mikrosekunden
 *	Zeit bis Überlauf			= 1.0485759375 Sekunden
 */
ISR(TIMER0_OVF_vect, ISR_BLOCK) {
	ASSERT(__bIsDone == FALSE);

	++__nOverflows;

	/*!
	 *	Stoppen der Messung bevor
	 *	der Zähler überläuft.
	 */
	if (__nOverflows == 0xFFFFul) {
		__ExtINT2_disable();
		__Timer0_stop();
		// Zählerwerte zurücksetzen
		TCNT0			= 0x00;
		__nOverflows	= 0;
		// Messung künstlich beenden
		__bIsDone		= TRUE;
	}
}

/*!
 **********************************************************
 *	EXTERNAL INTERRUPT REQUEST 2
 **********************************************************
 *	Diese Interruptserviceroutine wird bei jeder positiven Flanke
 *	an INT2 (PD5) aufgerufen.
 */
ISR(INT2_vect, ISR_BLOCK) {
	ASSERT(__bIsDone == FALSE);

	if (__Timer0_isRunning()) {
		// Bei der zweiten Flanke muss der Zähler gestoppt werden
		__Timer0_stop();
		__ExtINT2_disable();
		// Flag setzen
		__bIsDone = TRUE;
	} else {
		// Zähler muss zurückgesetzt worden sein
		ASSERT(TCNT0 == 0x00);
		ASSERT(__nOverflows == 0);

		__Timer0_start();
	}
}

/*!
 *	@function	FreqCounter__enable
 */
void FreqCounter__enable(void) {
	INTERRUPTS_REQUIRED();

	// Vorerst alles deaktivieren
	FreqCounter__disable();

	// Überlaufinterrupt für Timer/Counter0 aktivieren
	TIMSK |= _BV(TOIE0);
	// Der externe Interrupt soll bei positive Flanken ausgelöst werden
	MCUCSR |= _BV(ISC2);
}

/*!
 *	@function	FreqCounter__startMeasurement
 */
void FreqCounter__startMeasurement(void) {
	INTERRUPTS_REQUIRED();
	ASSERT(__bIsDone == TRUE && __bStarted == FALSE);
	ASSERT(!__Timer0_isRunning());
	ASSERT(!__ExtINT2_isEnabled());

	/*!
	 *	Initialisieren der Variablen VOR Aufruf
	 *	des Interruptes um Zeit zu sparen.
	 */
	TCNT0			= 0x00;	// Zählerwert auf 0 setzen
	__nOverflows	= 0;	// Überlaufzähler zurücksetzen

	// Überlaufinterrupt muss aktiviert sein
	ASSERT(BIT_ISSET(TIMSK, TOIE0));

	__bIsDone		= FALSE;// Flag zurücksetzen
	__bStarted		= TRUE;

	__ExtINT2_enable();
}

/*!
 *	@function	FreqCounter__isDone
 */
bool FreqCounter__isDone(ldbl *dResult) {
	INTERRUPTS_REQUIRED();
	// Es muss eine Messung gestartet worden sein
	ASSERT(__bStarted == TRUE);

	// Flag sofort kopieren
	bool bIsDone = __bIsDone;

	{
		if (bIsDone == TRUE) {
			/*!
			 *	Hier sind keine Synchronisationstechniken
			 *	notwendig, da in diesem Abschnitt weder der
			 *	externe noch der Überlauf-Interrupt aktiviert sein
			 *	dürfen.
			 */
			ASSERT(!__ExtINT2_isEnabled());
			ASSERT(!__Timer0_isRunning());

			if (dResult != NULL) {
				ldbl _dResult;

				// Zählerwert lesen
				u32 nTimerValue = TCNT0;
	
				// Überläufe dazuaddieren
				// << 8 ist gleichwertig mit Multiplikation von 256
				nTimerValue += ((u32)__nOverflows << 8ul);

				// Frequenz aus Zählerwert errechnen:
				// f = (1 / (nTimerValue * 62.5ns))
				_dResult = 1.0L / (((ldbl)nTimerValue * 62.5E-9L));

				if (!isfinite(_dResult)) {
					_dResult = 0.0;
				}

				*dResult = _dResult;
			}

			__bStarted = FALSE;
		}
	}

	return bIsDone;
}

/*!
 *	@function	FreqCounter__disable
 */
void FreqCounter__disable(void) {
	__ExtINT2_disable();
	// Überlaufinterrupt deaktivieren
	TIMSK &= ~_BV(TOIE0);
	__Timer0_stop();
	// Flag setzen
	__bIsDone	= TRUE;
	__bStarted	= FALSE;
}
