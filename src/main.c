/*!
 *	@file		main.c
 *	@brief
 *	Individuelle Praktische Arbeit (IPA)
 *	von Marco Agnoli Jahr 2016.
 *	Automatisiertes Prüfsystem für T400-Systeme.
 *	Das Programm utilisiert den externen sowie internen
 *	ADC um diverese Messgrössen zu ermitteln (Spannung, Strom, Temperatur).
 *	Der externe Interrupt (INT2) in Kombination mit dem
 *	Timer0 (8 Bit) wird verwendet um die Frequenz eines Signales zu bestimmen.
 *	Der Timer2 (8 Bit) wird verwendet um die diversen Messgrössen innerhalb eines Zeitfensters
 *	zu messen. Mit dem Timer1 (16 Bit) wird ein Sensorsignal emuliert welches über das
 *	SigGen verändert werden kann.
 *	
 *	Mikrokontroller		:	ATMega16a
 *	Fusebits			:	- Externer Quarz aktiviert
 *						:	- JTAG Interface deaktiviert
 *	Taktfrequenz		:	16MHz
 *
 *	IO Map				:
 *
 *	SW0					:	PD0
 *	SW1					:	PD1
 *	SW2					:	PD2
 *	SW4					:	PD4
 *	U/I_SEL				:	PD7
 *
 *	RELAY_K1_SET		:	PA1
 *	RELAY_K1_RESET		:	PA0
 *	RELAY_K2_SET		:	PA2
 *	RELAY_K2_RESET		:	PA3
 *
 *	T400_NC				:	PB3 (?)
 *	T400_NO				:	PB1 (?)
 *	T400_COM			:	PD6
 *	T400_CRNT			:	ADC7
 *	T400_V+				:	ADC6
 *	T400_SIG			:	PD5
 *
 *	-- SD KARTE
 *	SD_WRITE_PROTECT	:	PA4
 *	SD_CONNECTED		:	PA5
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#include <common/common.h>				// common
#include <LCD/LCD.h>					// LCD_*
#include <SigGen/SigGen.h>				// SigGen_*
#include <Watchdog/Watchdog.h>			// Watchdog_*
#include <measurements.h>				// measurements

// Statische Definitionen --------------------------------
static bool bWatchdogReset = FALSE;

#define SW1 0
#define SW2 1
#define SW3 2
#define SW4 4

// Flankentriggerung der Taster
static u8 nSW, nOldSW;

// Auswahl Messwerte für obere und untere Zeile
static u8 nTopIndex, nBotIndex;

// Messwerte
static ldbl dReadings[5]	= {0, 0, 0, 0, 0};

// Flag ob Display aktualisiert werden sollte
static bool bDisplayShouldUpdate = FALSE;

static INLINE u8 readSwitches(void) {
	return PIND & 0b00010111;
}

static INLINE void pulseRelay(u8 nRelayID) {
	PORTA |= _BV(nRelayID);
	// Warten bis Relais fertig
	_delay_ms(25);
	PORTA &= ~_BV(nRelayID);
	// Warten bis Relais fertig (Sicherheitsmarge)
	_delay_ms(25);
}

static INLINE bool readSwitch(u8 nSwitchID) {
	return (!BIT_ISSET(nSW, nSwitchID) && BIT_ISSET(nOldSW, nSwitchID));
}

static bool bFlip = FALSE;
// Statische Definitionen --------------------------------

/*!
 *	@function	resetRelays
 *	@brief
 *	Setzt beide Relais zurück (K1 & K2).
 */
void resetRelays(void) {
	pulseRelay(PA0);
	pulseRelay(PA3);
}

/*!
 *	@function	setRelayK1
 *	@brief
 *	Aktiviert Relais K1.
 */
void setRelayK1(void) {
	pulseRelay(PA3);
	pulseRelay(PA1);
}

/*!
 *	@function	setRelayK2
 *	@brief
 *	Aktiviert Relais K2.
 */
void setRelayK2(void) {
	pulseRelay(PA0);
	pulseRelay(PA2);
}

/*!
 *	@function	acquireNewValues
 *	@brief
 *	Holt allfällige neue Messwerte ab.
 *
 *	@return		bool
 *	'TRUE' falls neuer Messwert abgeholt wurde.
 *	Ansonsten 'FALSE'.
 */
bool acquireNewValues(void) {
	u8 update = 0;

	// Stromaufnahme T400
	update |= getMeasurement(
								MEASURE_T400_CURRENT,
								&dReadings[MEASURE_T400_CURRENT]
							 );

	// T400 Sensorversorgungsspannung
	update |= getMeasurement(
								MEASURE_T400_VSENSOR,
								&dReadings[MEASURE_T400_VSENSOR]
							 );

	// T400 Openkollektorfrequenz
	update |= getMeasurement(
								MEASURE_T400_OCFREQUENCY,
								&dReadings[MEASURE_T400_OCFREQUENCY]
							 );

	// T400 Analoger Ausgang
	update |= getMeasurement(
								nMEASURE_T400_ANALOGOUTPUT,
								&dReadings[nMEASURE_T400_ANALOGOUTPUT]
							 );

	return update;
}

/*!
 *	@function	readInputs
 *	@brief
 *	Liest die Taster ein.
 */
void readInputs(void) {
	nSW = readSwitches();
}

/*!
 *	@function	processData
 *	@brief
 *	Führt die Messungen durch.
 */
void processData(void) {
	// Messungen durchführen
	Measure__acquire();

	bDisplayShouldUpdate = acquireNewValues();

	// Wechseln der Anzeige
	if (readSwitch(SW1)) {
		if (++nTopIndex == 4) nTopIndex = 0;

		bDisplayShouldUpdate = TRUE;
	} else if (readSwitch(SW2)) {
		if (++nBotIndex == 4) nBotIndex = 0;

		bDisplayShouldUpdate = TRUE;
	} else if (readSwitch(SW3)) {
		// TODO: über API
		bFlip = !bFlip;

		Watchdog__reset();
		if (bFlip) {
			resetRelays();
			Watchdog__reset();
			setRelayK1();
		} else {
			resetRelays();
			Watchdog__reset();
			setRelayK2();
		}
		Watchdog__reset();
	} else if (readSwitch(SW4)) {
		for (;;);
	}
}

/*!
 *	@function	output
 *	@brief
 *	Erzeugt Ausgabe am LC-Display.
 *	Setzt zusätzlich den Watchdog zurück.
 */
void output(void) {
	if (bDisplayShouldUpdate) {
		LCD__clearScreen();

		// Ausgabe der Messwerte mit Beschreibung
		// TODO: Messgrösse (Hz, mA) hinzufügen
		LCD__print("%s : %3.3f\n%s : %3.3f", measurmentsStrings[nTopIndex], (double)dReadings[nTopIndex], measurmentsStrings[nBotIndex], (double)dReadings[nBotIndex]);

		Watchdog__reset();
	}

	// Für Flankentriggerung
	nOldSW = nSW;
}

void checkWatchdog(void) {
	if (bWatchdogReset) {
		u8 nNum				= 0;
		bool bUpdate		= FALSE;
		bool bSW1, bSW2, bSW3, bSW4;

		bSW1 = bSW2 = bSW3 = bSW4 = FALSE;

		LCD__print("Watchdog reset!");

		/*!
		 *	Falls der Watchdog den Mikrokontroller
		 *	zurückgesetzt hat muss der Benutzer
		 *	den Mikrokontroller freigeben.
		 *	Dies geschieht in dem alle Taster
		 *	einmal gedrückt werden (SW1 bis SW4).
		 */
		do {
			nSW = readSwitches();

			if (readSwitch(SW1) && !bSW1) {
				bSW1 = TRUE;
				bUpdate = TRUE;
			} else if (readSwitch(SW2) && !bSW2) {
				bSW2 = TRUE;
				bUpdate = TRUE;
			} else if (readSwitch(SW3) && !bSW3) {
				bSW3 = TRUE;
				bUpdate = TRUE;
			} else if (readSwitch(SW4) && !bSW4) {
				bSW4 = TRUE;
				bUpdate = TRUE;
			}

			if (bUpdate) {
				bUpdate = FALSE;

				LCD__clearLine(1);

				LCD__print("%c%c%c%c",
					(bSW1 ? 'X' : ' '),
					(bSW2 ? 'X' : ' '),
					(bSW3 ? 'X' : ' '),
					(bSW4 ? 'X' : ' ')
				);

				 ++nNum;
			}

			nOldSW = nSW;
		} while (nNum < 4);
	}
}

/*!
 *	@function	init
 *	@brief
 *	Initialisierung des Programmes.
 */
void init(void) {
	bWatchdogReset = Watchdog__wasResetted();

	LCD__init();
	LCD__clearScreen();

	LCD__print("T400 %s\n%s", "Pruefsystem", "IPA Marco A.");

	DDRD 	= 0b000100000;
	PORTD	= 0b000000000;

	DDRA	= 0b000011111;
	PORTA	= 0b000000000;

	resetRelays();

	_delay_ms(1000);

	nSW		= readSwitches();
	nOldSW	= nSW;

	ENABLE_INTERRUPTS();
	initMeasurements();
	SigGen__enable();

	nTopIndex = 0;
	nBotIndex = 0;

	LCD__clearScreen();

	if ((PIND & _BV(SW1)) == 0) {
		_delay_ms(500);
		setRelayK1();
		_delay_ms(500);
		resetRelays();
		setRelayK2();
		_delay_ms(500);
		resetRelays();
	}

	// Frequenz auf 1kHz setzen
	SigGen__setFrequency(1000);

	checkWatchdog();
	Watchdog__init();
}

/*!
 *	@function	main
 *	@brief
 *	Hautprogramm nach dem EVA-Prinzip:
 *		- Eingaben lesen
 *		- Verarbeitung der Daten
 *		- Ausgabe
 */
int main(void) {
	init();

	for (;;) {
		readInputs();
		processData();
		output();
	}
	
	return 0;
}
