#ifndef ZZ_TRACE_H
#define ZZ_TRACE_H

struct edict_s;

// TEMP diagnostic helper while tuning necos-zpmod bot behavior.
// Compiled out unless PB_TRACE is defined; writes to /tmp/pb_trace.log.
void pbTrace( const char *fmt, ... );
int pbEntIndex( struct edict_s *e );

#endif
