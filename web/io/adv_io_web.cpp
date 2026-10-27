/*
	adv_io_web.cpp -- I/O for the Emscripten build.

	The page (web/frontend/app.js, or the Node test driver) passes a "host"
	object as Module.advHost:

		write(text)						game text for the screen
		logged(text)					text appended to the game's log (CRLF)
		readLine(kind)		-> Promise<string|null>	one input line ending in "\n"
		getch()				-> Promise<number>		one key for the map display
		pickFile(path, what) -> Promise<bool>		put a chosen file at path in
														the virtual file system
		fileWritten(path)				a binary file (a .adv save) was written
		autosave(path)					a silent checkpoint was written at path
		allowInput()		-> bool		may "input <file>" read scripts?
		autosaveEnabled()	-> bool

	Strings cross as Latin-1 so every byte the game writes reaches the page.
	Waiting for input uses Asyncify: the game's C++ call stack is suspended
	while the page waits for the player.
*/
#include <emscripten.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/stat.h>

#include "adv_io.h"
#include "Adventure.h"

extern CGameState	gameState;
extern char*		advTempRestoreFile;

EM_JS( void, js_write, (const char* s), {
	Module.advHost.write(Module.advString(s));
});

EM_JS( void, js_logged, (const char* s), {
	Module.advHost.logged(Module.advString(s));
});

EM_ASYNC_JS( int, js_read_line, (char* buf, int n, int kind), {
	const line = await Module.advHost.readLine(kind);
	if (line === null || line === undefined)
		return 0;
	let i = 0;
	for (; i < line.length && i < n - 1; i++)
		HEAPU8[buf + i] = line.charCodeAt(i) & 0xff;
	HEAPU8[buf + i] = 0;
	return 1;
});

EM_ASYNC_JS( int, js_getch, (void), {
	return await Module.advHost.getch();
});

EM_ASYNC_JS( int, js_pick_file, (const char* path, const char* what), {
	const ok = await Module.advHost.pickFile(Module.advString(path), Module.advString(what));
	return ok ? 1 : 0;
});

EM_JS( void, js_file_written, (const char* path), {
	Module.advHost.fileWritten(Module.advString(path));
});

EM_JS( void, js_autosave, (const char* path), {
	Module.advHost.autosave(Module.advString(path));
});

EM_JS( int, js_allow_input, (void), {
	return Module.advHost.allowInput() ? 1 : 0;
});

EM_JS( int, js_autosave_enabled, (void), {
	return Module.advHost.autosaveEnabled() ? 1 : 0;
});

static bool s_bSaving = false;	// inside our own silent SaveAdv

void adv_io_init( void )
{
}

/*
	Save the game silently with the game's own SaveAdv, under the name the
	game uses for its temporary "undo" copy during restore. That name does
	not change the "last checkpoint" restore remembers, and nothing is
	printed or logged while adv_mute is set.
*/
static void Autosave( void )
{
	if ( !js_autosave_enabled() )
		return;
	s_bSaving = true;
	adv_mute = true;
	bool bRc = gameState.SaveAdv( advTempRestoreFile );
	adv_mute = false;
	s_bSaving = false;
	if ( bRc )
		js_autosave( advTempRestoreFile );
	unlink( advTempRestoreFile );
}

char* adv_io_read_line( char* buf, int n )
{
	int kind = adv_prompt_kind();
	if ( kind == ADV_PROMPT_MAIN )
		Autosave();
	if ( !js_read_line( buf, n, kind ) )
		exit( 0 );			// end of input (test driver); like the native build
	return buf;
}

int adv_io_getch( void )
{
	return js_getch();
}

void adv_io_write( const char* s )
{
	js_write( s );
}

void adv_io_logged( const char* s )
{
	js_logged( s );
}

static bool IsDataFile( const char* path )
{
	const char* base = strrchr( path, '/' );
	base = base ? base + 1 : path;
	return strlen( base ) == 11 && strncasecmp( base, "ADVENT", 6 ) == 0
		&& isdigit( (unsigned char)base[6] ) && base[7] == '.';
}

static bool EndsWith( const char* s, const char* suffix )
{
	size_t n = strlen( s ), m = strlen( suffix );
	return n >= m && strcasecmp( s + n - m, suffix ) == 0;
}

/*
	Reads of files the virtual file system doesn't have: a restore offers
	the player a file picker, and so does "input <file>" when the teacher
	allows it. With input not allowed, no text file other than the game's
	data files can be opened, exactly as if the file were missing.
*/
bool adv_io_may_open( const char* path, const char* mode )
{
	if ( mode[0] != 'r' || IsDataFile( path ) )
		return true;

	bool bText = strchr( mode, 'b' ) == NULL;
	if ( bText && !js_allow_input() )
		return false;

	struct stat st;
	if ( stat( path, &st ) == 0 )
		return true;

	if ( strcasecmp( path, advTempRestoreFile ) == 0 )
		return true;
	if ( bText )
		return js_pick_file( path, "input" ) != 0;
	if ( EndsWith( path, ".adv" ) )
		return js_pick_file( path, "restore" ) != 0;
	return true;
}

void adv_io_file_written( const char* path )
{
	if ( s_bSaving || strcasecmp( path, advTempRestoreFile ) == 0 )
		return;
	js_file_written( path );
}
