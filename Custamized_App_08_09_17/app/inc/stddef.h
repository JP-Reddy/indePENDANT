

/**************************************************************************
**                                                                        *
**  FILE        :  stddef.h                                               *
**                                                                        *
**  DESCRIPTION :  Include file with macros for common use                *
**                                                                        *
**  Copyright 1996-2011 Altium BV                                         *
**                                                                        *
**************************************************************************/

#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef	_SIZE_T_DEF
typedef	unsigned int	size_t;
#define	_SIZE_T_DEF
#endif

#ifndef _WCHAR_T
typedef	int		wchar_t;
#define _WCHAR_T
#endif

#if _MODEL == 'h'
typedef	long		ptrdiff_t;
#else
typedef	int		ptrdiff_t;
#endif

#ifdef	__cplusplus
#define NULL	(0)
#else	/* defined(__cplusplus) */
#define	NULL	( 0)
#endif	/* defined(__cplusplus) */

#ifdef	__cplusplus
#define offsetof( _type, _member )	((size_t)(__INTADDR__(&(((_type *)0)->_member))))
#else	/* defined(__cplusplus) */
#define offsetof( _type, _member )	((size_t)(&((_type *)0)->_member))
#endif  /* defined(__cplusplus) */

#endif  /* _STDDEF_H */
