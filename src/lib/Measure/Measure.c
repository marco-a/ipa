/*!
 *	@file		Measure.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <Measure/Measure.h>
#include <Timer/Timer.h>				// Timer_*
#include <IntADC/IntADC.h>				// IntADC_*
#include <ExtADC/ExtADC.h>				// ExtADC_*
#include <FreqCounter/FreqCounter.h>	// FreqCounter_*
#include <SigGen/SigGen.h>				// SigGen_*
#include <Watchdog/Watchdog.h>			// Watchdog_*

// Statische Definitionen --------------------------------
struct __acquisition {
	// Funktion zum starten der Messung
	Measure__startMeasurementFNC_t		startMeasurementFNC;
	// Kontext bzw. Argumente für die Funktion zum starten
	void								*startMeasurementFNCCTX;
	// Funktion zum Abfragen ob die Messung fertig ist
	Measure__isDoneFNC_t				isDoneFNC;
	// Funktion zum Umwandeln des Wertes (optional)
	Measure__cnvResultFNC_t				cnvResultFNC;

	// Flag ob Messung gestartet wurde
	bool								__bStarted;
	// Aktueller Messwert
	ldbl								__dReading;
	// Messwerte aufsummiert
	ldbl								__dReadings;
	// Anzahl gemessene Messwerte während Zeitperiode
	u16									__nReadings;
	// Flag ob Messung zwingend beendet werden muss
	bool								bMustFinish;
	// Internes Flag
	bool								bShouldFinish;
	// Zeitperiode in der die Messung gemacht wird
	u8									nTimeSlice;
};

typedef struct __acquisition __acquisition_t;
typedef Measure__MeasurementID_t __id_t;

#define __MAX_ACQUISITIONS	6u

// ID für die aktuelle Messung
static __id_t __nAcquisitionID		= 0u;
static __id_t __nAcquisitionLastID	= 0u;

static ldbl __dResults[__MAX_ACQUISITIONS];
static __acquisition_t __acquisitions[__MAX_ACQUISITIONS];

static bool __bTaskStarted = FALSE;
static bool __bTaskStatus[__MAX_ACQUISITIONS];

static bool __doMeasurement(__id_t nID, ldbl *dReading) {
	ldbl dResult					= NAN;
	bool bDone						= FALSE;
	__acquisition_t *acquisition	= &__acquisitions[nID];

	ASSERT(acquisition != NULL);

	/*!
	 *	Falls die Zeitperiode abgelaufen ist
	 *	und die Messung nicht ZWINGEND fertig sein
	 *	muss setzen wir das `bDone` Flag um die
	 *	Messung zu beenden.
	 *	Falls bShouldFinish auf 'TRUE' gesetzt wurde
	 *	wird beim Nächsten Aufruf von 'isDone' die
	 *	Messung beendet.
	 */
	if (Timer__hasExpired() == TRUE && acquisition->bShouldFinish == FALSE) {
		if (acquisition->bMustFinish == FALSE) {
			bDone = TRUE;
		} else if (acquisition->bMustFinish == TRUE) {
			if (acquisition->__bStarted == FALSE) {
				bDone = TRUE;
			} else {
				acquisition->bShouldFinish = TRUE;
			}
		}
	} else
	/*!
	 *	Starten einer neuen Messung.
	 */
		if (acquisition->__bStarted == FALSE) {
			// bShouldFinish darf hier nicht auf 'TRUE' gesetzt sein!
			ASSERT(acquisition->bShouldFinish == FALSE);

			acquisition->startMeasurementFNC(acquisition->startMeasurementFNCCTX);

			acquisition->__bStarted	= TRUE;
		} else
		/*!
		 *	Prüfen ob die Messung vorbei ist.
		 *	Falls die Messung beendet wurde
		 *	speichern wir das Messergebnis in
		 *	__dReadings ab.
		 *	Wenn bShouldFinish auf 'TRUE' gesetzt ist
		 *	beenden wir an diesem Zeitpunkt die Messung.
		 */
			if (acquisition->isDoneFNC(&dResult) == TRUE) {
				acquisition->__dReadings	+= dResult;
				acquisition->__nReadings	+= 1;
				acquisition->__bStarted		 = FALSE;

				if (acquisition->bShouldFinish == TRUE) {
					bDone = TRUE;
				}
			}

	/*!
	 *	Messung soll beendet werden.
	 *	Flags werden zurückgesetzt sowie
	 *	das Messergebnis berechnet.
	 */
	if (bDone == TRUE) {
		if (acquisition->bShouldFinish == TRUE) {
			ASSERT(acquisition->__bStarted == FALSE);
		}

		acquisition->bShouldFinish = FALSE;

		if (acquisition->__nReadings == 0) {
			acquisition->__dReading /= 2;
		} else {
			acquisition->__dReading = (acquisition->__dReadings / (ldbl)acquisition->__nReadings);
		}

		*dReading = acquisition->__dReading;

		acquisition->__dReadings	= 0.0L;
		acquisition->__nReadings	= 0;
	}
	
	return bDone;
}
// Statische Definitionen --------------------------------

/*!
 *	@function	Measure__addMeasurement
 */
__id_t Measure__addMeasurement(
	Measure__startMeasurementFNC_t startFNC,
	void *startFNCCTX,
	Measure__isDoneFNC_t isDoneFNC,
	bool bMustFinish,
	Measure__cnvResultFNC_t cnvResultFNC,
	u8 nTimeSlice
) {
	__id_t nNewID = __nAcquisitionLastID;
	__acquisition_t *acquisition = &__acquisitions[nNewID];

	ASSERT(acquisition != NULL);
	ASSERT(startFNC != NULL);
	ASSERT(isDoneFNC != NULL);

	// Variablen kopieren
	acquisition	->	startMeasurementFNC		= startFNC;
	acquisition	->	startMeasurementFNCCTX	= startFNCCTX;
	acquisition	->	isDoneFNC				= isDoneFNC;
	acquisition	->	cnvResultFNC			= cnvResultFNC;
	acquisition	->	__bStarted				= FALSE;
	acquisition	->	__dReading				= 0.0L;
	acquisition	->	__dReadings				= 0.0L;
	acquisition	->	__nReadings				= 0;
	acquisition	->	bMustFinish				= bMustFinish;
	acquisition	->	bShouldFinish			= FALSE;
	acquisition	->	nTimeSlice				= nTimeSlice;

	++__nAcquisitionLastID;

	ASSERT(__MAX_ACQUISITIONS > __nAcquisitionLastID);

	return nNewID;
}

/*!
 *	@function	Measure__getMeasuredValue
 */
bool Measure__getMeasuredValue(__id_t nID, ldbl *dResult) {
	ASSERT(dResult != NULL);

	if (__bTaskStatus[nID] == TRUE) {
		ldbl _dResult;

		_dResult = __acquisitions[nID].__dReading;

		/*!
		 *	Falls Funktion zum Umrechnung des Wertes
		 *	angegeben wurde, wird diese nun
		 *	aufgerufen.
		 */
		if (__acquisitions[nID].cnvResultFNC != NULL) {
			_dResult = __acquisitions[nID].cnvResultFNC(_dResult);
		}

		*dResult = _dResult;

		__bTaskStatus[nID] = FALSE;

		return TRUE;
	} else {
		return FALSE;
	}
}

/*!
 *	@function	Measure__acquire
 */
void Measure__acquire(void) {
	INTERRUPTS_REQUIRED();
	ASSERT(__nAcquisitionLastID > 0);

	if (__bTaskStarted == FALSE) {
		// Starten des Zählers mit der angegeben Zeitperiode
		Timer__start(__acquisitions[__nAcquisitionID].nTimeSlice);

		__bTaskStarted = TRUE;
	} else {
		// Schauen ob die Messung bzw. die Aufgabe abgearbeitet wurde
		__bTaskStatus[__nAcquisitionID] = __doMeasurement(__nAcquisitionID, &__dResults[__nAcquisitionID]);

		if (__bTaskStatus[__nAcquisitionID] == TRUE) {
			// Nächste Aufgabe auswählen
			__bTaskStarted = FALSE;
			__nAcquisitionID += 1;

			if (__nAcquisitionID == __nAcquisitionLastID) {
				__nAcquisitionID = 0;
			}
		}
	}
}

