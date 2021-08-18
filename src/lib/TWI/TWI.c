/*!
 *	@file		TWI.c
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 */
#include <TWI/TWI.h>

#define __FLAGS		_BV(TWEN) | _BV(TWINT)
#define __WAIT()		while (!BIT_ISSET(TWCR, TWINT))

/*!
 *	@function	TWI__start
 */
void TWI__start(void) {
	// I2C Bus beschlangnahmen
	TWCR = __FLAGS | _BV(TWSTA);
	// Warten bis Hardware fertig
	__WAIT();
	ASSERT(TWI_STATUS == TWI_STATUS__START);
}

/*!
 *	@function	TWI__repeatedStart
 */
void TWI__repeatedStart(void) {
	TWCR = __FLAGS | _BV(TWSTA);
	__WAIT();
	ASSERT(TWI_STATUS == TWI_STATUS__RSTART);
}

/*!
 *	@function	TWI__selectSlave
 */
void TWI__selectSlave(u8 nAddr) {
	bool bRead = (nAddr & 0b00000001);

	TWDR = nAddr;
	TWCR = __FLAGS;
	__WAIT();

	ASSERT(TWI_STATUS == (bRead ? TWI_STATUS__SLAR_ACK : TWI_STATUS__SLAW_ACK));
}

/*!
 *	@function	TWI__writeByte
 */
void TWI__writeByte(u8 nByte) {
	TWDR = nByte;
	TWCR = __FLAGS;
	__WAIT();
	ASSERT(TWI_STATUS == TWI_STATUS__WRITE_ACK);
}

/*!
 *	@function	TWI__readByteACK
 */
u8 TWI__readByteACK(void) {
	TWCR = __FLAGS | _BV(TWEA);
	__WAIT();
	ASSERT(TWI_STATUS__READ_ACK);

	return TWDR;
}

/*!
 *	@function	TWI__readByteNACK
 */
u8 TWI__readByteNACK(void) {
	TWCR = __FLAGS;
	__WAIT();
	ASSERT(TWI_STATUS__READ_NACK);

	return TWDR;
}

/*!
 *	@function	TWI__stop
 */
void TWI__stop(void) {
	TWCR = __FLAGS | _BV(TWSTO);

	while (BIT_ISSET(TWCR, TWSTO));
}
