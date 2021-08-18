/*!
 *	@file		ExtADC.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <ExtADC/ExtADC.h>
#include <TWI/TWI.h>		// TWI__*

#define __ADDR_R	0b11010001
#define __ADDR_W	0b11010000

// Statische Definitionen --------------------------------
static bool __bStarted	= FALSE;
static u8 __nCFG		= 0;

static ldbl __dMultiplier[4]		= {
	1E-3,			/* 12 bits = 1mV      */
	250E-6,			/* 14 bits = 250uV    */
	62.5E-6,		/* 16 bits = 62.5uV   */
	15.625E-6		/* 18 bits = 15.625uV */
};

static INLINE bool __cmpCFG(u8 nCFG1, u8 nCFG2) {
	return ((nCFG1 & 0b01111111) == (nCFG2 & 0b01111111));
}

static i32 __fixSign(u8 nResolution, u32 nValue) {
	i32 nReturn	= nValue;
	u8 nNumBits = 12 + (nResolution * 2);

	ASSERT(nResolution < 4);

	// MSB gibt an ob negativ oder positiv
	if (BIT_ISSET32(nValue, (nNumBits - 1))) {
		// Maske um die nicht benötigen Bits zu löschen
		u32 nMask = 0x0003FFFFul;

		ASSERT((3 - nResolution) >= 0);

		nMask >>= (3 - nResolution) * 2;

		//ASSERT((nValue & nMask) == nValue);
		nValue &= nMask;
		nValue ^= nMask;
		nValue += 1;

		// Zeichen wechseln
		nReturn = -nValue;
	}

	return nReturn;
}

static ldbl __toVoltage(u8 nResolution, i32 nValue) {
	ASSERT(nResolution < 4);

	ldbl dMeasuredVoltage = nValue * __dMultiplier[nResolution];

	ASSERT(dMeasuredVoltage <= 2.048L);

	return dMeasuredVoltage;
}

static ldbl __unpack(u8 nCFG, u8 nReadBytes[static 3]) {
	u8 nResolution	= (nCFG & 0b00001100) >> 2u;
	u32 nValue		= 0;
	i32 nFixedValue	= 0;

	// Bytes zusammenfügen
	switch (nResolution) {
		case ExtADC12Bit:
		case ExtADC14Bit:
		case ExtADC16Bit: {
			nValue	 = (u32)nReadBytes[1];
			nValue	|= (u32)nReadBytes[0] << 8ul;
		} break;

		case ExtADC18Bit: {
			nValue	 = (u32)nReadBytes[2];
			nValue	|= (u32)nReadBytes[1] <<  8ul;
			nValue	|= (u32)nReadBytes[0] << 16ul;
		} break;

		default: {
			ASSERT(FALSE);
		} break;
	}

	// Minuswerte behandeln
	nFixedValue = __fixSign(nResolution, nValue);

	// Wert in Spannung umrechnen
	return __toVoltage(nResolution, nFixedValue);
}
// Statische Definitionen --------------------------------

/*!
 *	@function	ExtADC__enable
 */
void ExtADC__enable(void) {
	// I2C Busfrequenz auf 100kHz einstellen
	TWBR	 = 18;
	TWSR	|= 0b00000001;
}

/*!
 *	@function	ExtADC__startMeasurement
 */
void ExtADC__startMeasurement(ExtADC_gain_t nGain, ExtADC_channel_t nCH, ExtADC_resolution_t nRes) {
	ASSERT(nGain < 4);
	ASSERT(nCH < 4);
	ASSERT(nRes < 4);
	ASSERT(__bStarted == FALSE);

	{
		u8 nCFG = 0b10000000;

		nCFG |= (nGain << 0u);
		nCFG |= (nRes << 2u);
		nCFG |= (nCH << 5u);

		TWI__start();
		TWI__selectSlave(__ADDR_W);
		TWI__writeByte(nCFG);

		// Konfiguration lokal speichern
		__nCFG = nCFG;

		TWI__repeatedStart();
		TWI__selectSlave(__ADDR_R);

		/*!
		 *	Um das Konfigurationsregister lesen zu können
		 *	muss erst das Messergebnis gelesen werden.
		 */
		(void)TWI__readByteACK();
		(void)TWI__readByteACK();
		(void)TWI__readByteACK();

		// Konfigurationsregister vergleichen
		nCFG = TWI__readByteACK();

		ASSERT(__cmpCFG(__nCFG, nCFG));

		__bStarted = TRUE;
	}
}

/*!
 *	@function	ExtADC__isDone
 */
bool ExtADC__isDone(ldbl *dResult) {
	bool bIsDone;
	u8 nCFG;

	// Konfiguration lesen
	nCFG = TWI__readByteACK();
	// Prüfen ob Konfiguration richtig gespeichert wurde
	ASSERT(__cmpCFG(nCFG, __nCFG));

	// ADC fertig?
	bIsDone = !BIT_ISSET(nCFG, 7);

	if (bIsDone) {
		// Mit NACK Lesemodus beenden
		(void)TWI__readByteNACK();

		if (dResult != NULL) {
			// Messergebnis holen
			u8 nBytes[3];

			TWI__repeatedStart();
			TWI__selectSlave(__ADDR_R);

			nBytes[0] = TWI__readByteACK();
			nBytes[1] = TWI__readByteACK();
			nBytes[2] = TWI__readByteNACK();

			*dResult = __unpack(nCFG, nBytes);
		}

		// I2C Bus befreien
		TWI__stop();

		__bStarted = FALSE;
	}
	
	return bIsDone;
}
