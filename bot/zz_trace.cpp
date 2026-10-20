#include "zz_trace.h"
#include "pb_global.h"
#include "bot.h"

#include <stdio.h>
#include <stdarg.h>

void pbTrace( const char *fmt, ... )
{
	static FILE *fp = 0;
	if (!fp) fp = fopen( "/tmp/pb_trace.log", "a" );
	if (!fp) return;
	va_list ap;
	va_start( ap, fmt );
	vfprintf( fp, fmt, ap );
	va_end( ap );
	fflush( fp );
}

int pbEntIndex( edict_t *e )
{
	if (!e) return -1;
	return ENTINDEX( e );
}
