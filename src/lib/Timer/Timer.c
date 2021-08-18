/*!
 *	@file		Timer.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <Timer/Timer.h>

// Statische Definitionen --------------------------------
// Anzahl Überlaufe
static volatile u8 __nOverflows		= 0;
// Gibt an ob Stoppuhr abgelaufen ist
static volatile u8 __bHasExpired	= 1;
// Statische Definitionen --------------------------------

/*
 *	Interruptserviceroute für den Timer2.
 *	Ein LSB entspricht 64 Mikrosekunden.
 *	Ein Überlauf entspricht circa 16 Millisekunden. (64us*255)
 */
ISR(TIMER2_OVF_vect, ISR_BLOCK) {
	// Stoppuhr muss noch nicht abgelaufen sein
	ASSERT(__bHasExpired == 0);
	// Anzahl übrige Überläufe prüfen
	ASSERT(__nOverflows > 0);

	if (--__nOverflows == 0) {
		// Stoppuhr ist abgelaufen
		__bHasExpired = 1;

		// Timer2 stoppen
		TCCR2 = 0;

		// Overflow Interrupt für Timer2 deaktivieren
		TIMSK &= ~_BV(TOIE2);
	}
}

/*!
 *	@function	Timer__start
 */
void Timer__start(u8 nTime) {
	INTERRUPTS_REQUIRED();
	ASSERT(nTime > 16.32);

	// Die Stoppuhr muss abgelaufen sein
	ASSERT(__nOverflows == 0 && __bHasExpired == 1);

	// Der Timer2 muss gestoppt sein
	ASSERT(TCCR2 == 0);

	// Overflow Interrupt für Timer2 deaktivieren
	TIMSK &= ~_BV(TOIE2);

	{
		// Anzahl Überläufe berechnen
		u8 nOverflows = nTime / 16.32;

		// Da nur vielfachen von 16.32ms verwendet werden können
		// soll bei einem Überlauf von 0 das Programm abgebrochen werden.
		ASSERT(nOverflows > 0);

		// ATOMIC_BLOCK ist theoretisch nicht notwendig
		// da Variablen 8 Bit breit sind.
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			// Zähler Timer2 zurücksetzen
			TCNT2			= 0;
			// Anzahl Überläufe kopieren
			__nOverflows	= nOverflows;
		}
	}

	__bHasExpired = 0;

	// Overflow Interrupt für Timer2 aktivieren
	TIMSK |= _BV(TOIE2);

	// Timer2 mit Prescaler 1024 starten
	TCCR2 = _BV(CS22) | _BV(CS21) | _BV(CS20);
}

/*!
 *	@function	Timer__hasExpired
 */
bool Timer__hasExpired(void) {
	INTERRUPTS_REQUIRED();

	return (__bHasExpired == 1);
}
