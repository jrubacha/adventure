//	Adventure.cpp


#include	"Adventure.h"	/* #define preprocessor equates	*/

//	Globals Start

CGameState	gameState;
short		g_debugFlg = 0;				/* if game is in debug	*/

//	Globals End

static bool		bAutoCp = false;			/* Auto CheckPoint */
static short	bFriendly = 0;				/* Used for testing	*/
static short	bDisableDwarves = 0;		/* Used for testing	*/
static short	bDisablePirate = 0;			/* Used for testing	*/


void	ResetGameState( int rFlag, char* rFile )
{
	static bool bFirstTime = true;
										
	if ( bFirstTime )
	{
		IntAdvText();
	}
	else
	{
		closetxt();
	}
										
	bFirstTime = false;
										
	gameState.initplay();

	if ( !load_advent_file_data(false) )
	{
		crypt( "encrypt" );
		if ( !load_advent_file_data(true) )
			exitAdv(-1);
	}
	else
	{
		opentxt();
	}
	
	if (rFlag)
		gameState.RestoreAdv( rFile );
	else if (yes(MSG4_065_Welcome_ADVENTURE_Original_dev_by_Willie_Crowther, MSG4_001_Colossal_Cave_Intro, 0))
		gameState.Set_limit(1000);
	else
		gameState.Set_limit(330);
										
//	srand(511);				/* seed random	*/

#if _DEBUG	&& 0
	gameState.Get_LogLevel(true);		/* display current Log Level. */
	gameState.Get_brief(true);			/* display current brief status. */
	gameState.Get_Friendly(true);		/* display current Friendly status. */
#endif

	if ( bAutoCp )
		gameState.Set_bAutoSave( 1 );

	if ( bFriendly )
		gameState.Set_Friendly( 1 );

	if ( bDisableDwarves )
		gameState.Set_DwarfsEnabled( 0 );

	if ( bDisablePirate )
		gameState.Set_PirateEnabled( 0 );
								
}


void main(int argc, char ** argv)
{
	int	rflag = 0;		/* user restore request option	*/
	char* rFile = NULL;	/* user restore request file	*/
	char** pargv = argv;
	char* p = NULL;
	char  c = '\0';
	char  cPrev = '\0';
										
	g_debugFlg = 0;

#ifndef _DEBUG
	setTextFilePath( argv[0] );		// release version reads the ADVENT*.* files
									// from the .EXE directory unless
									// the -t switchis set to a different path.
#endif
										
	for ( int ii = 1; ii < argc; ++ii ) 
	{
		++pargv;
		p = *pargv;
		c = *p;
																				
		if (c != '-')
		{
			if ( cPrev == 'r' )
			{
				rFile = p;
			}
			else if ( cPrev == 'i' )
			{
				SetInputType( p );
			}
			else if ( cPrev == 'l' )
			{
				if ( strlen(p+1) > 3 && strchr(p+1,'.') > p+1 )
					setLogFileName( p );
			}
			else if ( cPrev == 't' )
			{
				setTextFilePath( p );
			}

			continue;
		}
		++p;
		c = *p;
		cPrev = c;
																				
		switch(tolower(c)) 
		{
			case 'r':
				++rflag;
				if ( strlen( p ) > 1 )
					rFile = p+1;
				continue;
			case 'a':
				if ( stricmp(p, "autocp") == 0)
					bAutoCp = true;
				continue;
			case 'd':
				if ( stricmp(p, "deletelog") == 0)
					deleteLogFile();
				else if ( stricmp(p, "decrypt") == 0)
					crypt( "decrypt" );
				else if ( stricmp(p, "DisableDwarves") == 0)
					bDisableDwarves = true;
				else if ( stricmp(p, "DisablePirate") == 0)
					bDisablePirate = true;
#if _DEBUG
				else
					++g_debugFlg;
#endif
				continue;
			case 'e':
				if ( stricmp(p, "encrypt") == 0)
					crypt( "encrypt" );
				continue;
			case 'f':
				if ( stricmp(p, "friendly") == 0)
					bFriendly = 1;
				continue;
			case 'i':
				if ( strlen( p ) > 1 )
					SetInputType( p+1 );
				continue;
			case 'l':
				if ( strlen(p+1) > 3 && strchr(p+1,'.') > p+1 )
					setLogFileName( p+1 );
				continue;
			case 't':
				if ( strlen( p ) > 1 )
					setTextFilePath( p+1 );
				continue;

			default:
				printf("unknown flag: %c\n", c);
				continue;
		}				/* switch	*/
	}					/* while	*/
	if (g_debugFlg < 2)
		g_debugFlg = 0;	/* must request three times	*/
										
	ResetGameState( rflag, rFile );
										
	while(1) gameState.PlayGameTurn();
										
	exitAdv(1);				/* exitAdv(1) = ok	*/
}						/* main		*/

/* ************************************************************	*/


/*
		Fatal error routine
*/
void bug(short n)
{
	char szBug[100];
	sprintf(szBug, "Fatal error number %d, Version %d\n", n, gameState.Get_Version());
	puts(szBug);

	if ( gameState.Get_bNoDieTest() )		// used to test all moves
	{
		gameState.Set_closed(0);
		gameState.Set_closing(0);
		gameState.Set_tally1(15);
		gameState.Set_tally2(0);

		gameState.Set_limit(10000);
		gameState.Set_lampWarning(0);
		gameState.Set_clock1(300);
		gameState.Set_clock2(500);
		gameState.Set_panic(0);
		
		gameState.Set_bonus(0);
		gameState.Set_numdie(0);
		gameState.Set_foobar(0);
		gameState.Set_gaveup(0);
		return;
	}
	exitAdv(1);
}


/*
		Common Exit routine
*/
void exitAdv(int code)
{
	char buff[100];
	puts("Press Enter to Exit :)");
	GetInputLine( buff, sizeof(buff) );
	closetxt();
	exit( code );
}

