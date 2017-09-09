#ifndef STDIO_H
#define	STDIO_H

#include "stddef.h"
#include "stdarg.h"

/* Standard sprintf() function. Work as the libc one. */
int sprintf(char * buf, const char *fmt, ...);
/* Standard snprintf() function from BSD, more secure... */
int snprintf(char * buf, size_t len, const char *fmt, ...);

/* Standard sscanf() function. Work as the libc one. */
int sscanf(const char * buf, const char * fmt, ...);/*do not use*/
/* If you need to code your own printf... */
int vsprintf(char *buf, const char *fmt, va_list ap);

/*int vsscanf (const char *fp, const char *fmt0, va_list ap); do not use*/

int sget_response(const char* resp, const char *fmt, ...);
//11-2-15- by Sangram
int printf(const char *format, ...);
void abort(void);
#endif /* STDIO_H */

