

/**************************************************************************
**                                                                        *
**  FILE        :  assert.h                                               *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes and macros to add         *
**                 diagnostics to programs                                *
**                                                                        *
**  Copyright 1996-2011 Altium BV                                         *
**                                                                        *
**************************************************************************/

#undef assert
	//#if 0
#ifndef	_STDIO_H
#include <stdio.h>	/* prototype for 'printf' */
#endif  /* _STDIO_H */

#ifndef	_STDLIB_H
#include <stdlib.h>	/* prototype for 'abort' */
#endif  /* _STDLIB_H */
	//#endif
#ifndef NDEBUG

#define assert(_expr)								\
	((void) (!(_expr) ?							\
	  printf("Assertion failed: (" #_expr ") file %s, line %d\n",		\
	  	 __FILE__, __LINE__), abort() : (void) 0			\
	))

#else

#define	assert(_expr)		((void) 0)

#endif  /* NDEBUG */
