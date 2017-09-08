

/**************************************************************************
**                                                                        *
**  FILE        :  wchar.h                                                *
**                                                                        *
**  DESCRIPTION :  Extended multibute and wide character utilities.       *
**                                                                        *
**  Copyright 1996-2011 Altium BV                                         *
**                                                                        *
**************************************************************************/

#ifndef	_WCHAR_H
#define _WCHAR_H

#include <limits.h>
#include <stdio.h>
#include "cs_types.h"//28-10-14


#ifdef	__cplusplus
extern "C" {
#endif	/* defined(__cplusplus) */

#ifndef	_USMLIB
#define	_USMLIB
#endif

/*
 * The unsigned integral type of the result of the sizeof operator.
 */
	#if 0//28-10-14
#ifndef _SIZE_T_DEF
typedef	unsigned int	size_t;
#define _SIZE_T_DEF
#endif
	#endif
	
	#if 0
#ifndef size_t //28-10-14
typedef unsigned int                size_t;
#endif
	#endif
/*
 * An integral type whose range of values can represent distinct codes for 
 * all members of the largest extended character set specified among the
 * supported locales. The null character shall have the code value zero.
 */
#ifndef _WCHAR_T
typedef	int		wchar_t;
#define _WCHAR_T
#endif

/*
 * An object type that holds the components of a calandar time, called the
 * broken-down time.
 */
#ifndef _STRUCT_TM
#define _STRUCT_TM
//#pragma suspend_align32 ////28-10-14
struct tm
{
	int tm_sec;	/* seconds after the minute - [0, 59] */
	int tm_min;	/* minutes after the hour - [0, 59]   */
	int tm_hour;	/* hours since midnight - [0, 23]     */
	int tm_mday;	/* day of the month - [1, 31]         */
	int tm_mon;	/* months since January - [0, 11]     */
	int tm_year;	/* year since 1900                    */
	int tm_wday;	/* days since Sunday - [0, 6]         */
	int tm_yday;	/* days since January 1 - [0, 365]    */
	int tm_isdst;	/* Daylight Saving Time flag          */
};
//#pragma resume_align32 //28-10-14
#endif /*_STRUCT_TM */

/*
 * Null pointer constant.
 */
#ifndef NULL
#ifdef	__cplusplus
#define NULL	(0)
#else	/* !defined(__cplusplus) */
#define	NULL	( 0)
#endif	/* defined(__cplusplus) */
#endif	/* !defined(NULL) */

/*
 * The minimum value representable by an object of type wchar_t.
 */
#ifndef	WCHAR_MIN
#define WCHAR_MIN ((wchar_t) 0)
#endif	/* !defined(WCHAR_MIN) */

/*
 * The maximum value representable by an object of type wchar_t.
 */
#ifndef	WCHAR_MAX
#define WCHAR_MAX ((wchar_t) INT_MAX)
#endif	/* !defined(WCHAR_MAX) */

/*
 * A wint_t value that does not correspond to any value of the extended 
 * character set. Used by several functions to indicate end-of-file.
 */
#ifndef WEOF
#define WEOF ((wint_t) UINT_MAX)
#endif

/*
 * An integral type that can hold any value of the extended character set 
 * and at least one other value (like WEOF).
 */
#ifndef _WINT_T
typedef unsigned int wint_t;
#define _WINT_T
#endif

/* mbstate_t
 *
 * An object type other than an array type that can hold the conversion
 * state information necessary to convert between sequences of multibyte
 * characters and wide characters.
 *
 * When multibyte characters larger than 1 byte are used this struct
 * will be used to store the conversion information when not all the
 * bytes of a particular multibyte character has been read from the
 * source.  In the current implementation multibyte characters are 1
 * byte long (MB_CUR_MAX and MB_LEN_MAX are defined as 1) and this
 * will never occur.  If this implementation changes, please read the
 * note in <stdio.h> about fpos_t
 */
//#pragma suspend_align32 //28-10-14
typedef struct
{
	wchar_t		wc_value;	/* wide character value solved so far */
	unsigned short	n_bytes;	/* number of bytes of solved multibyte */
	unsigned short	encoding;	/* encoding rule for wide character <=> multibyte conversion */
} mbstate_t;
//#pragma resume_align32 //28-10-14

#define INIT_MBSTATE_OBJECT(mbs) (mbs).wc_value =  0;   (mbs).n_bytes =  0;   (mbs).encoding =  0;
#define IS_INITIAL_MBSTATE(mbs) ((mbs).wc_value == 0 && (mbs).n_bytes == 0 && (mbs).encoding == 0)

/*
 * Formatted wide character input/output functions.
 */
extern	_USMLIB	int			fwprintf  ( FILE *, const wchar_t *, ... );
extern	_USMLIB	int			fwscanf   ( FILE *, const wchar_t *, ... );
extern	_USMLIB	int			wprintf   ( const wchar_t *, ... );
extern	_USMLIB	int			wscanf    ( const wchar_t *, ... );
extern	_USMLIB	int			swprintf  ( wchar_t *, size_t, const wchar_t *, ... );
extern	_USMLIB	int			swscanf   ( const wchar_t *, const wchar_t *, ... );
extern	_USMLIB	int			vfwprintf ( FILE *, const wchar_t *, va_list );
extern	_USMLIB	int			vfwscanf  ( FILE *, const wchar_t *, va_list );
extern	_USMLIB	int			vwprintf  ( const wchar_t *, va_list );
extern	_USMLIB	int			vwscanf   ( const wchar_t *, va_list );
extern	_USMLIB	int			vswprintf ( wchar_t *, size_t, const wchar_t *, va_list );
extern	_USMLIB	int			vswscanf  ( const wchar_t *, const wchar_t *, va_list );

/*
 * Wide character input/output functions.
 */
extern	_USMLIB	wint_t			fgetwc    ( FILE * );
extern	_USMLIB	wchar_t *		fgetws    ( wchar_t *, int, FILE * );
extern	_USMLIB	wint_t			fputwc    ( wchar_t, FILE * );
extern	_USMLIB	int			fputws    ( const wchar_t *, FILE * );
extern	_USMLIB	wint_t			getwc     ( FILE * );
extern	_USMLIB	wint_t			getwchar  ( void );
extern	_USMLIB	wint_t			putwc     ( wchar_t, FILE * );
extern	_USMLIB	wint_t			putwchar  ( wchar_t );
extern	_USMLIB	wint_t			ungetwc   ( wint_t, FILE * );
extern	_USMLIB	int			fwide     ( FILE *, int );

/*
 * Wide string numeric conversion functions.
 */
extern	_USMLIB	double			wcstod   ( const wchar_t *, wchar_t ** );
extern	_USMLIB	float			wcstof   ( const wchar_t *, wchar_t ** );
extern	_USMLIB	long double		wcstold  ( const wchar_t *, wchar_t ** );
extern	_USMLIB	long			wcstol   ( const wchar_t *, wchar_t **, int );
extern	_USMLIB	unsigned long		wcstoul  ( const wchar_t *, wchar_t **, int );


/*
 * Wide string copying functions.
 */
extern	_USMLIB	wchar_t *		wcscpy  ( wchar_t *, const wchar_t * );
extern	_USMLIB	wchar_t *		wcsncpy ( wchar_t *, const wchar_t *, size_t );

/*
 * Wide string concatenation functions.
 */
extern	_USMLIB	wchar_t *		wcscat  ( wchar_t *, const wchar_t * );
extern	_USMLIB	wchar_t *		wcsncat ( wchar_t *, const wchar_t *, size_t );

/*
 * Wide string comparison functions.
 */
extern	_USMLIB	int			wcscmp  ( const wchar_t *, const wchar_t * );
extern	_USMLIB	int			wcscoll ( const wchar_t *, const wchar_t * );
extern	_USMLIB	int			wcsncmp ( const wchar_t *, const wchar_t *, size_t );
extern	_USMLIB	size_t			wcsxfrm ( wchar_t *, const wchar_t *, size_t );

/*
 * Wide string search functions.
 */
extern	_USMLIB	wchar_t *		wcschr  ( const wchar_t *, wchar_t );
extern	_USMLIB	size_t			wcscspn ( const wchar_t *, const wchar_t * );
extern	_USMLIB	wchar_t *		wcspbrk ( const wchar_t *, const wchar_t * );
extern	_USMLIB	wchar_t *		wcsrchr ( const wchar_t *, wchar_t );
extern	_USMLIB	size_t			wcsspn  ( const wchar_t *, const wchar_t * );
extern	_USMLIB	wchar_t *		wcsstr  ( const wchar_t *, const wchar_t * );
extern	_USMLIB	wchar_t *		wcstok  ( wchar_t *, const wchar_t *, wchar_t ** );
extern	_USMLIB	size_t			wcslen  ( const wchar_t * );

/*
 * Wide character array functions.
 */
extern	_USMLIB	wchar_t *		wmemchr  ( const wchar_t *, wchar_t, size_t );
extern	_USMLIB	int			wmemcmp  ( const wchar_t *, const wchar_t *, size_t );
extern	_USMLIB	wchar_t *		wmemcpy  ( wchar_t *, const wchar_t *, size_t );
extern	_USMLIB	wchar_t *		wmemmove ( wchar_t *, const wchar_t *, size_t );
extern	_USMLIB	wchar_t *		wmemset  ( wchar_t *, wchar_t, size_t );

/*
 * Wide string time conversion function.
 */
extern	_USMLIB	size_t			wcsftime ( wchar_t *, size_t, const wchar_t *, const struct tm * );

/*
 * Single-byte wide character conversion functions.
 */
extern	_USMLIB	wint_t			btowc    ( int );
extern	_USMLIB	int			wctob    ( wint_t );
extern	_USMLIB	int			mbsinit  ( const mbstate_t * );

/*
 * Restartable multibyte/wide character conversion functions.
 */
extern	_USMLIB	size_t			mbrlen   ( const char *, size_t, mbstate_t * );
extern	_USMLIB	size_t			mbrtowc  ( wchar_t *, const char *, size_t, mbstate_t * );
extern	_USMLIB	size_t			wcrtomb  ( char *, wchar_t, mbstate_t * );

/*
 * Restartable multibyte/wide string conversion functions.
 */
extern	_USMLIB	size_t			mbsrtowcs( wchar_t *, const char **, size_t, mbstate_t * );
extern	_USMLIB	size_t			wcsrtombs( char *, const wchar_t **, size_t, mbstate_t * );

#ifdef	__cplusplus
}
#endif	/* defined(__cplusplus) */

#endif /* _WCHAR_H */
