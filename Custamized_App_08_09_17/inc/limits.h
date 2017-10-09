

/**************************************************************************
**                                                                        *
**  FILE        :  limits.h                                               *
**                                                                        *
**  DESCRIPTION :  Sizes of Integral Types                                *
**                                                                        *
**  Copyright 1996-2011 Altium BV                                         *
**                                                                        *
**************************************************************************/

#ifndef _LIMITS_H
#define _LIMITS_H

#include "cs_types.h"//28-10-14

#define CHAR_BIT	(8)		/* number of bits in a byte */
#define SCHAR_MIN	(-SCHAR_MAX-1)	/* minimum value of signed char */
#define SCHAR_MAX	(0x7f)  	/* maximum value of signed char */
#define UCHAR_MAX	(0xffU) 	/* maximum value of unsigned char */
#define CHAR_MIN	(SCHAR_MIN)	/* minimum value of 'plain' char */
#define CHAR_MAX	(SCHAR_MAX)	/* maximum value of 'plain' char */

#define MB_LEN_MAX	(1)

#define SHRT_MIN	(-SHRT_MAX-1)	/* minimum value of short int */
#define SHRT_MAX	(0x7fff)	/* maximum value of short int */
#define USHRT_MAX	(0xffffU)	/* maximum value of unsigned short */

#define INT_MIN		(-INT_MAX-1)	/* minimum value of int */
//#define INT_MAX		(0x7fff)	/* maximum value of int */
#define UINT_MAX	(0xffffU)	/* maximum value of unsigned int */

//#define LONG_MIN	(-LONG_MAX-1)	/* minimum value of long int */
//#define LONG_MAX	(0x7fffffffL)	/* maximum value of long int */
//#define ULONG_MAX	(0xffffffffUL)	/* maximum value of unsigned long */


 /* derived constants, not ANSI defined */
#define INT_BIT         (16)
#define LONG_BIT        (32)
#define HPTR_BIT        (32)            /* Huge pointer */
#define FPTR_BIT        (32)            /* Far pointer  */
#define NPTR_BIT        (16)            /* Near pointer */
#endif	/* _LIMITS_H */
