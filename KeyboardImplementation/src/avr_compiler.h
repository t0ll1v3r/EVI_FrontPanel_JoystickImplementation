#ifndef COMPILER_AVR_H
#define COMPILER_AVR_H

#ifndef F_CPU
/*! \brief Define default CPU frequency, if this is not already defined. */
	#define F_CPU 32000000UL
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*! \brief This macro will protect the following code from interrupts. */
#define AVR_ENTER_CRITICAL_REGION( ) uint8_t volatile saved_sreg = SREG; \
                                     cli();

/*! \brief This macro must always be used in conjunction with AVR_ENTER_CRITICAL_REGION
 *        so the interrupts are enabled again.
 */
#define AVR_LEAVE_CRITICAL_REGION( ) SREG = saved_sreg;

#if defined( __ICCAVR__ )

#include <inavr.h>
#include <ioavr.h>
#include <intrinsics.h>
#include <pgmspace.h>

#ifndef __HAS_ELPM__
#define _MEMATTR  __flash
#else /* __HAS_ELPM__ */
#define _MEMATTR  __farflash
#endif /* __HAS_ELPM__ */

/*! \brief Perform a delay of \c us microseconds.
 *
 *  The macro F_CPU is supposed to be defined to a constant defining the CPU
 *  clock frequency (in Hertz).
 *
 *  The maximal possible delay is 262.14 ms / F_CPU in MHz.
 *
 *  \note For the IAR compiler, currently F_CPU must be a
 *        multiple of 1000000UL (1 MHz).
 */
#define delay_us( us )   ( __delay_cycles( ( F_CPU / 1000000UL ) * ( us ) ) )

/*! \brief Preprocessor magic.
 *
 *  Some preprocessor magic to allow for a header file abstraction of
 *  interrupt service routine declarations for the IAR compiler.  This
 *  requires the use of the C99 _Pragma() directive (rather than the
 *  old #pragma one that could not be used as a macro replacement), as
 *  well as two different levels of preprocessor concetanations in
 *  order to do both, assign the correct interrupt vector name, as well
 *  as construct a unique function name for the ISR.
 *
 *  \note Do *NOT* try to reorder the macros below, as this will only
 *        work in the given order.
 */
#define PRAGMA(x) _Pragma( #x )
#define ISR(vec) PRAGMA( vector=vec ) __interrupt void handler_##vec(void)
#define sei( ) (__enable_interrupt( ))
#define cli( ) (__disable_interrupt( ))

/*! \brief Define the no operation macro. */
#define nop( ) (__no_operation())

/*! \brief Define the watchdog reset macro. */
#define watchdog_reset( ) (__watchdog_reset( ))


#define INLINE PRAGMA( inline=forced ) static

#define FLASH_DECLARE(x) _MEMATTR x
#define FLASH_STRING(x) ((_MEMATTR const char *)(x))
#define FLASH_STRING_T  char const _MEMATTR *
#define FLASH_BYTE_ARRAY_T uint8_t const _MEMATTR *
#define PGM_READ_BYTE(x) *(x)
#define PGM_READ_WORD(x) *(x)

#define SHORTENUM /**/

#elif defined( __GNUC__ )

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/*! \brief Define the delay_us macro for GCC. */
#define delay_us( us )   (_delay_us( us ))

#define INLINE static inline

/*! \brief Define the no operation macro. */
#define nop()   do { __asm__ __volatile__ ("nop"); } while (0)

#define MAIN_TASK_PROLOGUE int


#define MAIN_TASK_EPILOGUE() return -1;

#define SHORTENUM __attribute__ ((packed))

#else
#error Compiler not supported.
#endif

#endif
