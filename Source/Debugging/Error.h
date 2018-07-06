//-----------------------------------------------------------
// File: Error.h
// Description: Couple of defined asserts for easier debugging
//				standard assert and one that allows a message to
//				be returned
//-----------------------------------------------------------

#ifndef ERROR_H
#define ERROR_H

#if defined(_DEBUG)
#include <cassert>
#define ASSERT(expr) assert(expr)

#define ASSERTMSG( expr, msg ) assert( expr && msg )
#else
	#define ASSERT(expr) (expr)
	#define ASSERTMSG( expr, msg ) (expr, msg)
#endif

#endif
