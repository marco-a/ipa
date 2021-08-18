/*!
 *	@file		measurements.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#define MEASUREMENTS_H_EXTERN
#include <measurements.h>

#include <Measure/Measure.h>			// Measure_*
#include <IntADC/IntADC.h>				// IntADC_*
#include <ExtADC/ExtADC.h>				// ExtADC_*
#include <FreqCounter/FreqCounter.h>	// FreqCounter_*

// Statische Definitionen --------------------------------
static void __startIntADC(void *ctx) {
	IntADC__startMeasurement(*(u8 *)ctx);
}

static void __startExtADC(void *ctx) {
	ExtADC__startMeasurement(ExtADCGain1, *(u8 *)ctx, ExtADC14Bit);
}

static void __startFreqCtr(void *ctx) {
	FreqCounter__startMeasurement();
}

/*!
 *	Definitionen, an welchen Kanälen
 *	die einzelen Messwerte gemessen werden.
 *	Variablen müssen hier definiert werden da die
 *	__addMeasurement Methode nur einen
 *	Pointer akzeptiert.
 */
static u8	__nT400CurrentCH	= IntADCCH7,	// Interner ADC Kanal 7
			__nT400VSensorCH	= IntADCCH6,	// Interner ADC Kanal 6
			__nT400AnalogCH		= ExtADCCH2;	// Externer ADC Kanal 2

UNUSED static ldbl __convertT400I(ldbl dResult) {
	// Messung der Spannung über 40.2 Ohm Widerstand R26/R25
	return (dResult / 40.2) * 1E3;
}

UNUSED static ldbl __convertT400U(ldbl dResult) {
	// Messung der Spannung über 1kOhm Widerstand R29/R28
	return (dResult / 1E3) * 10.1E3;
}

static ldbl __convertCurrent(ldbl dResult) {
	// Umwandelung des Spannungwertes ausgegeben vom INA139
	return dResult * 1E3;
}

static ldbl __convertVSensor(ldbl dResult) {
	// Umwandelung des Spannungwertes (R33/R32)
	return (dResult / 1.7E3) * (6.7E3);
}

static void __addMeasurements(void) {
	/*!
	 *	Stromaufnahme des T400 wird über den
	 *	internen ADC gemessen. (Kanal = ADC7)
	 *
	 *	Zeitperiode: 150ms
	 *
	 *	Messung muss zwingend fertig sein, da interner ADC
	 *	nur einen Kanal auf einmal messen kann.
	 */
	nMEASURE_T400_CURRENT		= Measure__addMeasurement(
									__startIntADC,
									&__nT400CurrentCH,
									IntADC__isDone,
									TRUE,
									__convertCurrent,
									150
								);

	/*!
	 *	Sensorversorgungsspannung des T400 wird über den
	 *	internen ADC gemessen. (Kanal = ADC6)
	 *
	 *	Zeitperiode: 150ms
	 *
	 *	Messung muss zwingend fertig sein, da interner ADC
	 *	nur einen Kanal auf einmal messen kann.
	 */
	nMEASURE_T400_VSENSOR		= Measure__addMeasurement(
									__startIntADC,
									&__nT400VSensorCH,
									IntADC__isDone,
									TRUE,
									__convertVSensor,
									150
								);

	/*!
	 *	Openkollektorfrequenz des T400 wird über den
	 *	externen Interrupt (INT2) gemessen.
	 *
	 *	Zeitperiode: 150ms
	 *
	 *	Da Frequenzmessung Frequenzen kleiner 10Hz messen kann
	 *	muss die Messung nicht zwingend beendet worden sein.
	 */
	nMEASURE_T400_OCFREQUENCY	= Measure__addMeasurement(
									__startFreqCtr,
									NULL,
									FreqCounter__isDone,
									FALSE,
									NULL,
									150
								);

	/*!
	 *	Der analoge Ausgang des T400 wird über den
	 *	externen ADC gemessen.
	 *
	 *	Zeitperiode: 150ms
	 *
	 *	Messung muss zwingend fertig sein, da der externe ADC
	 *	nur einen Kanal auf einmal messen kann.
	 */
	// TODO einstellbar machen über API
	// ob  Strom oder Spannungsausgang
	nMEASURE_T400_ANALOGOUTPUT	= Measure__addMeasurement(
									__startExtADC,
									&__nT400AnalogCH,
									ExtADC__isDone,
									TRUE,
									__convertT400U,
									150
								);

	/*!
	 *	Noch nicht implementiert.
	 */
	/*
	nMEASURE_T400_TEMPERATURE = Measure__addMeasurement(
									__temperatureStart,
									NULL,
									__temperatureIsDone,
									TRUE
								);
	*/
}
// Statische Definitionen --------------------------------

const char *measurmentsStrings[5] = {NULL, NULL, NULL, NULL, NULL};

/*!
 *	@function	initMeasurements
 */
void initMeasurements(void) {
	INTERRUPTS_REQUIRED();

	// Module aktivieren
	FreqCounter__enable();
	IntADC__enable();
	ExtADC__enable();

	// Messaufgaben registrieren
	__addMeasurements();

	// Bezeichnungen setzen
	measurmentsStrings[MEASURE_T400_CURRENT]		= "Stra";
	measurmentsStrings[MEASURE_T400_VSENSOR]		= "Sens";
	measurmentsStrings[MEASURE_T400_OCFREQUENCY]	= "Freq";
	measurmentsStrings[MEASURE_T400_ANALOGOUTPUT]	= "Alog";
}

/*!
 *	@function	getMeasurement
 */
bool getMeasurement(Measure__MeasurementID_t nID, ldbl *dResult) {
	return Measure__getMeasuredValue(nID, dResult);
}
