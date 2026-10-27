/*
	adv_io.h -- the seam between the platform layer and the outside world.

	adv_platform.cpp calls these functions for console input/output and at
	interesting file events. There are two implementations:
		adv_io_native.cpp	stdin/stdout, for the native reference build
		adv_io_web.cpp		the browser front end (async input, downloads,
							file picker, autosave)
*/
#ifndef ADV_IO_H
#define ADV_IO_H

#include <stdio.h>

/* What the game is waiting for when it asks for a line of input. */
enum AdvPromptKind
{
	ADV_PROMPT_MAIN = 0,		/* the ">" command prompt (getwords)			*/
	ADV_PROMPT_QUESTION = 1,	/* yes/no questions (yes() and friends)		*/
	ADV_PROMPT_FILENAME = 2,	/* save/restore "name of the saved game?"	*/
	ADV_PROMPT_EXIT = 3			/* "Press Enter to Exit :)"					*/
};

int		adv_prompt_kind( void );

/* platform -> io */
void	adv_io_init( void );
char*	adv_io_read_line( char* buf, int n );		/* NULL at end of input */
int		adv_io_getch( void );
void	adv_io_write( const char* s );				/* game text for the screen */
void	adv_io_logged( const char* s );				/* text appended to the log, CRLF */
bool	adv_io_may_open( const char* path, const char* mode );	/* may prompt for a file */
void	adv_io_file_written( const char* path );	/* a file opened for writing was closed */

/* platform state the io layer may use */
extern bool	adv_mute;			/* true while the io layer saves silently */

#endif	// #ifndef ADV_IO_H
