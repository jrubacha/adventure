/*
	adv_io_native.cpp -- console I/O for the native reference build.

	Game text goes to stdout; input lines and map keys come from stdin.
	When stdin runs out the game exits, so test runs always end.
*/
#include "adv_io.h"

#include <stdlib.h>
#include <string.h>

void adv_io_init( void )
{
}

char* adv_io_read_line( char* buf, int n )
{
	char* pRet = fgets( buf, n, stdin );
	if ( pRet == NULL )
	{
		fflush( stdout );
		exit( 0 );
	}
	return pRet;
}

int adv_io_getch( void )
{
	int c = getchar();
	if ( c == EOF )
		return '\n';
	return c;
}

void adv_io_write( const char* s )
{
	fputs( s, stdout );
}

void adv_io_logged( const char* )
{
}

bool adv_io_may_open( const char*, const char* )
{
	return true;
}

void adv_io_file_written( const char* )
{
}
