#include "debug.h"

void TRACE( const char* format, ... ) {
	#ifndef NDEBUG
    char buffer[1024];

    va_list ap;

	va_start( ap, format );
		vsprintf( buffer, format, ap );
		std::cout << buffer;
	va_end(ap);
	#endif
}
