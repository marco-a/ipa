/*!
 *	@file		TWI.h
 *	@brief
 *	I2C Hilfsfunktionen.
 *	Diese Funktionen brechen bei
 *	einem Fehlerfall sofort das Programm ab!
 *	(Kann man noch verbessern mit boolean return values)
 *
 *	Beispiel:
 *	START-Kondition erzeugen:
 *	TWI__start();
 *	Slave im Schreibmodus adressieren:
 *	TWI__selectSlave(0b10101010);
 *	Byte schreiben:
 *	TWI__writeByte(0xFF);
 *	REPEATED-START-Kondition erzeugen:
 *	TWI__repeatedStart();
 *	Slave im Lesemodus adressieren:
 *	TWI__selectSlave(0b10101011);
 *	Byte mit ACK lesen:
 *	TWI__readByteACK();
 *	Byte mit NACK lesen:
 *	TWI__readByteNACK();
 *	STOP-Kondition generieren:
 *	TWI__stop();
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_TWI_H)
	#define JAQ_TWI_H 1

	#include <common/common.h>
		
	#define TWI_STATUS					(TWSR & 0xF8u)

	// Start
	#define TWI_STATUS__START			0x08
	#define TWI_STATUS__RSTART			0x10
	#define TWI_STATUS__ARBLOST			0x38

	// Master <--- Slave READ
	#define TWI_STATUS__SLAR_ACK		0x40
	#define TWI_STATUS__SLAR_NACK		0x48
	#define TWI_STATUS__READ_ACK		0x50
	#define TWI_STATUS__READ_NACK		0x58

	// Master ---> Slave WRITE
	#define TWI_STATUS__SLAW_ACK		0x18
	#define TWI_STATUS__SLAW_NACK		0x20
	#define TWI_STATUS__WRITE_ACK		0x28
	#define TWI_STATUS__WRITE_NACK		0x30

	/*!
	 *	@function	TWI__start
	 *	@brief
	 *	Generiert eine START-Kondition
	 *	auf dem I2C Bus.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void TWI__start(void);

	/*!
	 *	@function	TWI__start
	 *	@brief
	 *	Generiert eine REPEATED-START-Kondition
	 *	auf dem I2C Bus.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void TWI__repeatedStart(void);

	/*!
	 *	@function	TWI__selectSlave
	 *	@brief
	 *	Wählt den Slave mit der Adresse `nAddr` aus.
	 *
	 *	@param		nAddr
	 *	Adresse des Slaves.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void TWI__selectSlave(u8 nAddr);

	/*!
	 *	@function	TWI__writeByte
	 *	@brief
	 *	Schreibt ein Byte an den zuvor adressierten Slave.
	 *
	 *	@param		nByte
	 *	Datenbyte welches gesendet werden soll.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void TWI__writeByte(u8 nByte);

	/*!
	 *	@function	TWI__readByteACK
	 *	@brief
	 *	Liest ein Byte vom Slave und sendet
	 *	ein Acknowledgment zurück.
	 *
	 *	@return		u8
	 *	Gelesenes Datenbyte vom Slave.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	u8 TWI__readByteACK(void);

	/*!
	 *	@function	TWI__readByteNACK
	 *	@brief
	 *	Liest ein Byte vom Slave und sendet
	 *	kein Acknowledgment zurück.
	 *
	 *	@return		u8
	 *	Gelesenes Datenbyte vom Slave.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	u8 TWI__readByteNACK(void);

	/*!
	 *	@function	TWI__stop
	 *	@brief
	 *	Generiert eine STOP-Kondition
	 *	auf dem I2C Bus.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	void TWI__stop(void);

#endif // !defined(JAQ_TWI_H)
