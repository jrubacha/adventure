/*
	adv_platform.cpp -- implementations behind platform.h.

	- MSVC-compatible rand()/srand().
	- File wrappers that behave like the MSVC runtime in text mode: files
	  opened without "b" write "\n" as "\r\n" and read "\r\n" as "\n", so
	  logs match the Windows exe byte for byte and CRLF command scripts work.
	  Paths with "\" are converted to "/".
	- Console I/O and file events are passed to the io layer (web/io).
	- Prompt-kind wrappers for GetInputLine (see build-native.sh / build.sh,
	  which map GetInputLine to these per source file).
	- int main() that calls the game's "void main", renamed adv_main.
*/
#include "platform.h"
#include "../io/adv_io.h"

#include <stdarg.h>
#include <stdint.h>

#undef fopen
#undef fclose
#undef fgets
#undef fputs
#undef fprintf
#undef remove
#undef unlink
#undef mkdir
#undef getch
#undef rand
#undef srand

bool adv_mute = false;

/* ------------------------------------------------------------------ */
/*	rand() / srand() from the MSVC C runtime.							*/

static uint32_t s_holdrand = 1;

void adv_srand( unsigned int seed )
{
	// The teacher's randomSeed (or ADV_SEED for tests) replaces the seed
	// the game asks for, which is always 511.
	const char* szOverride = getenv( "ADV_SEED" );
	if ( szOverride && *szOverride )
		seed = (unsigned int)strtoul( szOverride, NULL, 10 );
	s_holdrand = seed;
}

int adv_rand( void )
{
	s_holdrand = s_holdrand * 214013u + 2531011u;
	return (int)( ( s_holdrand >> 16 ) & 0x7fff );
}

/* ------------------------------------------------------------------ */
/*	File wrappers.														*/

struct AdvOpenFile
{
	FILE*	fp;
	bool	bText;
	bool	bWrite;
	bool	bAppend;
	char	szPath[_MAX_PATH+1];
};

static AdvOpenFile s_files[32];

static void FixPath( const char* name, char* out )
{
	strncpy( out, name, _MAX_PATH );
	out[_MAX_PATH] = '\0';
	for ( char* p = out; *p; ++p )
		if ( *p == '\\' )
			*p = '/';
}

static AdvOpenFile* FindFile( FILE* fp )
{
	if ( fp == NULL )
		return NULL;
	for ( int i = 0; i < (int)(sizeof(s_files)/sizeof(s_files[0])); ++i )
		if ( s_files[i].fp == fp )
			return &s_files[i];
	return NULL;
}

FILE* adv_fopen( const char* name, const char* mode )
{
	char szPath[_MAX_PATH+1];
	char szMode[8];
	int  m = 0;

	if ( name == NULL || mode == NULL )
		return NULL;

	FixPath( name, szPath );

	// While muted, the log (the only file opened for append) is not written.
	if ( adv_mute && mode[0] == 'a' )
		return NULL;

	if ( !adv_io_may_open( szPath, mode ) )
		return NULL;

	for ( const char* p = mode; *p && m < (int)sizeof(szMode)-1; ++p )
		if ( *p != 't' )
			szMode[m++] = *p;
	szMode[m] = '\0';

	FILE* fp = fopen( szPath, szMode );
	if ( fp == NULL )
		return NULL;

	AdvOpenFile* pFile = NULL;
	for ( int i = 0; pFile == NULL && i < (int)(sizeof(s_files)/sizeof(s_files[0])); ++i )
		if ( s_files[i].fp == NULL )
			pFile = &s_files[i];
	if ( pFile )
	{
		pFile->fp = fp;
		pFile->bText = strchr( mode, 'b' ) == NULL;
		pFile->bWrite = mode[0] == 'w' || mode[0] == 'a' || strchr( mode, '+' ) != NULL;
		pFile->bAppend = mode[0] == 'a';
		strcpy( pFile->szPath, szPath );
	}
	return fp;
}

int adv_fclose( FILE* fp )
{
	AdvOpenFile* pFile = FindFile( fp );
	int nRc = fclose( fp );
	if ( pFile )
	{
		bool bNotify = pFile->bWrite && !pFile->bText;
		pFile->fp = NULL;
		if ( bNotify && nRc == 0 )
			adv_io_file_written( pFile->szPath );
	}
	return nRc;
}

char* adv_fgets( char* buf, int n, FILE* fp )
{
	char* pRet = NULL;

	if ( fp == stdin )
		pRet = adv_io_read_line( buf, n );
	else
		pRet = fgets( buf, n, fp );

	if ( pRet == NULL )
		return NULL;

	AdvOpenFile* pFile = FindFile( fp );
	if ( pFile && pFile->bText )
	{
		size_t nLen = strlen( buf );
		if ( nLen >= 2 && buf[nLen-2] == '\r' && buf[nLen-1] == '\n' )
		{
			buf[nLen-2] = '\n';
			buf[nLen-1] = '\0';
		}
	}
	return pRet;
}

int adv_fputs( const char* s, FILE* fp )
{
	if ( fp == stdout )
	{
		if ( !adv_mute )
			adv_io_write( s );
		return 0;
	}

	AdvOpenFile* pFile = FindFile( fp );
	if ( pFile == NULL || !pFile->bText )
		return fputs( s, fp );

	// Text mode: "\n" is written as "\r\n".
	size_t nLen = strlen( s );
	size_t nLines = 0;
	for ( const char* p = s; *p; ++p )
		if ( *p == '\n' )
			++nLines;

	char* szOut = (char*)malloc( nLen + nLines + 1 );
	if ( szOut == NULL )
		return EOF;
	char* q = szOut;
	for ( const char* p = s; *p; ++p )
	{
		if ( *p == '\n' )
			*q++ = '\r';
		*q++ = *p;
	}
	*q = '\0';

	int nRc = fputs( szOut, fp );
	// The only file the game opens for append is its log; the browser
	// keeps a copy as the transcript.
	if ( pFile->bAppend && nRc != EOF )
		adv_io_logged( szOut );
	free( szOut );
	return nRc;
}

int adv_fprintf( FILE* fp, const char* fmt, ... )
{
	char buff[4096];
	va_list marker;
	va_start( marker, fmt );
	vsnprintf( buff, sizeof(buff), fmt, marker );
	va_end( marker );
	return adv_fputs( buff, fp ) == EOF ? -1 : (int)strlen( buff );
}

int adv_remove( const char* name )
{
	char szPath[_MAX_PATH+1];
	FixPath( name, szPath );
	return remove( szPath );
}

int adv_unlink( const char* name )
{
	char szPath[_MAX_PATH+1];
	FixPath( name, szPath );
	return unlink( szPath );
}

int adv_mkdir( const char* name )
{
	char szPath[_MAX_PATH+1];
	FixPath( name, szPath );
	return mkdir( szPath, 0777 );
}

int adv_getch( void )
{
	return adv_io_getch();
}

/* ------------------------------------------------------------------ */
/*	GetInputLine wrappers. Each game source file except English.Cpp is	*/
/*	compiled with GetInputLine mapped to one of these, so the io layer	*/
/*	knows what kind of prompt the game is showing. English.Cpp's only	*/
/*	call is the main command prompt.									*/

void GetInputLine( char* buff, short nMaxLen, bool bTolower );

static int s_promptKind = ADV_PROMPT_MAIN;

int adv_prompt_kind( void )
{
	return s_promptKind;
}

static void GetInputLineAs( int kind, char* buff, short nMaxLen, bool bTolower )
{
	int savedKind = s_promptKind;
	s_promptKind = kind;
	GetInputLine( buff, nMaxLen, bTolower );
	s_promptKind = savedKind;
}

void adv_GetInputLine_question( char* buff, short nMaxLen, bool bTolower )
{
	GetInputLineAs( ADV_PROMPT_QUESTION, buff, nMaxLen, bTolower );
}

void adv_GetInputLine_filename( char* buff, short nMaxLen, bool bTolower )
{
	GetInputLineAs( ADV_PROMPT_FILENAME, buff, nMaxLen, bTolower );
}

void adv_GetInputLine_exit( char* buff, short nMaxLen, bool bTolower )
{
	GetInputLineAs( ADV_PROMPT_EXIT, buff, nMaxLen, bTolower );
}

/* ------------------------------------------------------------------ */
/*	The game's entry point is "void main", which g++ and clang reject.	*/
/*	The game sources are compiled with -Dmain=adv_main.					*/

void adv_main( int argc, char** argv );

int main( int argc, char** argv )
{
	adv_io_init();
	adv_main( argc, argv );
	return 0;
}
