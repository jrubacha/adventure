/*
	platform.h -- MSVC compatibility layer for building AdventureCCaw1 with
	g++/clang (native reference build) and Emscripten (browser build).

	This header is force-included into every game source file with
	"-include web/platform/platform.h", so nothing in Source/ has to change.
	It maps the MSVC-only names the game uses onto portable equivalents and
	routes file and console I/O through the adv_* wrappers in
	adv_platform.cpp, which emulate Windows text-mode files and give the
	browser build its hooks (see web/io/adv_io.h).
*/
#ifndef ADV_PLATFORM_H
#define ADV_PLATFORM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* MSVC string and math helpers */
#define stricmp		strcasecmp
#define _stricmp	strcasecmp
#define __max(a,b)	(((a) > (b)) ? (a) : (b))
#define __min(a,b)	(((a) < (b)) ? (a) : (b))

#ifndef _MAX_PATH
#define _MAX_PATH	260
#endif

/* MSVC C runtime rand(): same sequence as the Windows exe for a given seed. */
#undef RAND_MAX
#define RAND_MAX	0x7fff
int		adv_rand( void );
void	adv_srand( unsigned int seed );
#define rand		adv_rand
#define srand		adv_srand

/* File and console I/O */
FILE*	adv_fopen( const char* name, const char* mode );
int		adv_fclose( FILE* fp );
char*	adv_fgets( char* buf, int n, FILE* fp );
int		adv_fputs( const char* s, FILE* fp );
int		adv_fprintf( FILE* fp, const char* fmt, ... );
int		adv_remove( const char* name );
int		adv_unlink( const char* name );
int		adv_mkdir( const char* name );		/* <direct.h> one-argument mkdir */
int		adv_getch( void );					/* <conio.h> */

#define fopen		adv_fopen
#define fclose		adv_fclose
#define fgets		adv_fgets
#define fputs		adv_fputs
#define fprintf		adv_fprintf
#define remove		adv_remove
#define unlink		adv_unlink
#define mkdir(d)	adv_mkdir(d)
#define getch		adv_getch

#endif	// #ifndef ADV_PLATFORM_H
