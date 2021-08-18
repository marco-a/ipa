/*!
 *	@file		Measure.h
 *	@brief
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_MEASURE_H)
	#define JAQ_MEASURE_H 1

	#include <common/common.h>

	typedef void (*Measure__startMeasurementFNC_t)(void *ctx);
	typedef bool (*Measure__isDoneFNC_t)(ldbl *dResult);
	typedef ldbl (*Measure__cnvResultFNC_t)(ldbl dResult);

	typedef u8 Measure__MeasurementID_t;

	/*!
	 *	@function	Measure__addMeasurement
	 *	@brief
	 *	Fügt eine Messaufgabe hinzu.
	 *	
	 *	@param		startFNC
	 *	Funktion zum starten der Messung.
	 *	@param		startFNCCTX
	 *	Allfällige Parameter für die `startFNC` Funktion
	 *	können hier übergeben werden.
	 *	@param		isDoneFNC
	 *	Funktion zum Prüfen ob die Messung abgeschlossen wurde.
	 *	@param		bMustFinish
	 *	Wenn auf 'TRUE' gesetzt muss die Messung
	 *	auf jeden Fall abgeschlossen sein.
	 *	@param		cnvResultFNC
	 *	Optionaler Parameter: falls angegeben kann der gemessene
	 *	Wert mit der Funktion umgerechnet werden.
	 *	@param		nTimeSlice
	 *	Zeitperiode in denen die Messungen erfolgen sollen.
	 *
	 *	@return		Measure__MeasurementID_t
	 *	ID für die Messaufgabe.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	Measure__MeasurementID_t Measure__addMeasurement(Measure__startMeasurementFNC_t startFNC, void *startFNCCTX,  Measure__isDoneFNC_t isDoneFNC, bool bMustFinish, Measure__cnvResultFNC_t cnvResultFNC, u8 nTimeSlice);

	/*!
	 *	@function	Measure__getMeasuredValue
	 *	@brief
	 *	Prüft ob die Messaufgabe mit der ID `nID` erledigt wurde.
	 *	Das Ergebnis wird in `dResult` abgelegt.
	 *
	 *	@param		nID
	 *	ID der Messaufgabe.
	 *	@param		dResult
	 *	Pointer zur Variable in der das Messergebnis abgelegt werden soll.
	 *
	 *	@return		bool
	 *	'TRUE' wenn neuer Wert verfügbar war, ansonsten 'FALSE'.
	 *
	 *	@warning
	 *		- Die Funktion gibt einen neuen Wert nur **einmal**
	 *		  zurück.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	bool Measure__getMeasuredValue(Measure__MeasurementID_t nID, ldbl *dResult);

	/*!
	 *	@function	Measure__acquire
	 *	@brief
	 *	Führt die Messaufgaben nacheinander aus.
	 *
	 *	@warning
	 *		- Measure__addMeasurements muss vorher aufgerufen worden sein!
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void Measure__acquire(void);

#endif // !defined(JAQ_MEASURE_H)
