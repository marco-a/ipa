/*!
 *	@file		measurements.h
 *	@brief
 *
 *	@author		Marco Agnoli
 *	@copyright
 */
#if !defined(JAQ_MEASUREMENTS_H)
	#define JAQ_MEASUREMENTS_H 1

	#include <common/common.h>
	#include <Measure/Measure.h>

	#if !defined(MEASUREMENTS_H_EXTERN)
		#define MEASUREMENTS_H_EXTERN extern
	#endif // !defined(MEASUREMENTS_H_EXTERN)

	MEASUREMENTS_H_EXTERN Measure__MeasurementID_t nMEASURE_T400_CURRENT;
	MEASUREMENTS_H_EXTERN Measure__MeasurementID_t nMEASURE_T400_VSENSOR;
	MEASUREMENTS_H_EXTERN Measure__MeasurementID_t nMEASURE_T400_OCFREQUENCY;
	MEASUREMENTS_H_EXTERN Measure__MeasurementID_t nMEASURE_T400_ANALOGOUTPUT;
	MEASUREMENTS_H_EXTERN Measure__MeasurementID_t nMEASURE_TEMPERATURE;

	#define MEASURE_T400_CURRENT		nMEASURE_T400_CURRENT
	#define MEASURE_T400_VSENSOR		nMEASURE_T400_VSENSOR
	#define MEASURE_T400_OCFREQUENCY	nMEASURE_T400_OCFREQUENCY
	#define MEASURE_T400_ANALOGOUTPUT	nMEASURE_T400_ANALOGOUTPUT
	#define MEASURE_TEMPERATURE			nMEASURE_TEMPERATURE

	extern const char *measurmentsStrings[5];

	void initMeasurements(void);
	bool getMeasurement(Measure__MeasurementID_t nID, ldbl *dResult);

#endif // !defined(JAQ_MEASUREMENTS_H)
