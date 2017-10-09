#ifndef STDLIB_H
#define	STDLIB_H

#include "cs_types.h"
#include "stddef.h"

/* Standard atoi() function. Work as the libc one. */
int atoi(const char *s);
#define atol(str) ((long) atoi((str)))

long strtol(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);


typedef INT STD_COMPAR_FUNC_T (const VOID*, const VOID*);

PUBLIC VOID  qsort (VOID *baseP, INT32 nElem, INT32 width, STD_COMPAR_FUNC_T *compar);

#endif /* STDLIB_H */

