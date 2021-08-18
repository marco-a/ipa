/*!
 *	@file		common.h
 *	@brief
 *	Diese Datei wird überall mit eingebunden und
 *	enthält diverse globale Definitionen / Funktionen etc.
 *
 *	@author		Marco Agnoli
 *	@copyright	2016 <Marco Agnoli>
 *	@date		11.05.2016
 *	@version	1.0.0
 */
#if !defined(JAQ_COMMON_H)
	#define JAQ_COMMON_H 1

	#include	<avr/io.h>			// ios
	#include	<avr/interrupt.h>	// sei, cli
	#include	<util/delay.h>		// _delay_ms
	#include	<util/atomic.h>		// ATOMIC_BLOCK
	#include	<stdint.h>			// uintN_t
	#include	<stdlib.h>			// NULL
	#include	<stdio.h>			// *snprintf
	#include	<inttypes.h>		// PRI*n
	#include	<string.h>			// strrchr
	#include	<math.h>			// NAN

	typedef		uint8_t		u8;
	typedef		uint16_t	u16;
	typedef		uint32_t	u32;
	typedef		uint64_t	u64;

	typedef		int8_t		i8;
	typedef		int16_t		i16;
	typedef		int32_t		i32;
	typedef		int64_t		i64;

	typedef long double		ldbl;

	typedef u8				bool;

	#define TRUE			((u8)1u)
	#define FALSE			((u8)0u)

	#define ENABLE_INTERRUPTS	sei
	#define DISABLE_INTERRUPTS	cli

	#define BIT_ISSET(_byte, _bitpos)	(((_byte) & _BV(_bitpos)) > 0u ? TRUE : FALSE)
	#define BIT_ISSET16(_byte, _bitpos)	(((_byte) & ((u16)1u << (_bitpos))) > 0u ? TRUE : FALSE)
	#define BIT_ISSET32(_byte, _bitpos)	(((_byte) & ((u32)1ul << (_bitpos))) > 0u ? TRUE : FALSE)

	#define UNUSED			__attribute__((__unused__))
	#define NORETURN		__attribute__((__noreturn__))
	#define FORMAT(_1, _2)	__attribute__((__format__(printf, _1, _2)))
	#define INLINE			inline __attribute__((__always_inline__))

	/*!
	 *	@function	throw
	 *	@brief
	 *	Bricht das Programm ab und gibt eine Fehlermeldung aus.
	 *
	 *	@author		Marco Agnoli
	 *	@copyright	2016 <Marco Agnoli>
	 *	@date		11.05.2016
	 *	@version	1.0.0
	 */
	NORETURN void throw(const char *sFile, u32 nLine, const char *sMsg, ...) FORMAT(3, 4);

	#define DEBUG 1

	#if defined(DEBUG)
		#define PANIC(_msg, ...)		throw((strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__), __LINE__, _msg, ##__VA_ARGS__)
		#define ASSERT(_expr)			if (!(_expr)) { PANIC("ASSERT"); }
		#define INTERRUPTS_REQUIRED()	ASSERT(BIT_ISSET(SREG, 7))
	#else
		#define PANIC(_msg, ...)
		#define ASSERT(_expr)
		#define INTERRUPTS_REQUIRED()
	#endif // defined(DEBUG)


	#if F_CPU != 16000000UL
		#error Taktfrequenz muss auf 16MHz eingestellt werden.
	#endif // F_CPU != 16000000UL

#endif // !defined(JAQ_COMMON_H)
