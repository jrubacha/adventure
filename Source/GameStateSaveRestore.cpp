// GameStateSaveRestore.cpp: implementation of the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#include <crtdbg.h>
#include "Adventure.h"
#include "GameState.h"

static char szLastCheckPointName[_MAX_PATH+1];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


typedef struct
{
	short*	var;
	short	rep;
}
INT_VARS;

/*
		save adventure game varables
*/
bool CGameState::IntVars( int nPivNdx, short*& var,  short& rep )
{
	static	INT_VARS playIntVars[] = 
	{
		&	m_Version,1,
		&	m_newloc1,1,
		&	m_location,1,
		&	m_oldloc1,1,
		&	m_oldloc2,1,
																				
		&	m_object1,1,

		&	m_bPirateEnabled,1,
		&	m_bDwarfsEnabled,1,

		&	m_dwarfFlag,1,
		&	m_dwarfAltLoc,1,
		&	m_dwarfsKilled,1,
																				
		&	m_knifeLoc,1,
		&	m_chestLoc1,1,
		&	m_chestLoc2,1,
																				
		&	m_holding,1,
		&	m_holdMax,1,
		&	m_turns,1,
		&	m_wasDark,1,
		&	m_closing,1,
		&	m_closed,1,
		&	m_tally1,1,
		&	m_tally2,1,
		&	m_limit,1,
		&	m_lampWarning,1,
		&	m_clock1,1,
		&	m_clock2,1,
		&	m_panic,1,
																				
		&	m_bonus,1,
		&	m_numdie,1,
		&	m_foobar,1,
		&	m_gaveup,1,
		
		&	m_detail,1,
		&	m_brief,1,
		&	m_nFriendly,1,
		&	m_nVocabCmpLen,1,
		&	m_nLogLevel,1,
						
		&	m_nOfferHints,1,										
		&	m_hintTaken,1,
		&	m_hintAvail,1,

		&	m_bAutoSave,1,

		&	m_Reserved,1,	// = 0x1234; // we'll write a few of these to reserve space for new vars.
		&	m_Reserved,1,	// = 0x1234; // we'll write a few of these to reserve space for new vars.
		&	m_Reserved,1,	// = 0x1234; // we'll write a few of these to reserve space for new vars.
		&	m_Reserved,1,	// = 0x1234; // we'll write a few of these to reserve space for new vars.
		&	m_Reserved,1,	// = 0x1234; // we'll write a few of these to reserve space for new vars.
		&	m_Reserved,1,	// = 0x1234; // we'll write a few of these to reserve space for new vars.
		&	m_Reserved,1,	// = 0x1234; // we'll write a few of these to reserve space for new vars.
		
		NULL,0
	};
#define NUM_INT_VARS (sizeof(playIntVars) / sizeof(playIntVars[0]))

	_ASSERT(NUM_INT_VARS == 48 ); // if this fails adjust the number of reserved.


	if ( nPivNdx < 0 || nPivNdx >= NUM_INT_VARS )
	{
		static short nNumIntVars = NUM_INT_VARS;
		var = &nNumIntVars;
		rep = 1;

		return false;
	}
										
	var = playIntVars[nPivNdx].var;
	rep = playIntVars[nPivNdx].rep;
										
	return true;
}

char* advTempRestoreFile = "advTempRestoreAw1234.adv";

/*
		save adventure game
*/
bool CGameState::SaveAdv( char* szFileName )
{
	FILE*	savefd;
	char	fileName[_MAX_PATH+1];
	char	*sptr;
	bool	bRc = true;
	short	rep = 0;
	short*	var = NULL;
										
	if ( szFileName && strlen(szFileName) > 0 )
	{
		strcpy(fileName, szFileName);
	}
	else
	{
		printf("What do you want to name the saved game? ");
//		scanf("%s", fileName);
		GetInputLine( fileName, sizeof(fileName)-1, false);
	}
										
	if (sptr = strchr(fileName, '.'))
		*sptr = '\0';		/* kill extension	*/
	if (strlen(fileName) > _MAX_PATH - 4)
		fileName[_MAX_PATH - 4] = '\0';
	strcat(fileName, ".adv");
										
	savefd = fopen(fileName, "wb");
	if (savefd == NULL) 
	{
		printf("Sorry, I can't create the file...%s\n", fileName);
		return false;
	}

	//	First lets write the count of IntVars to write.
	IntVars(  -1 ,  var,  rep );
	fwrite( var, sizeof(*var), rep, savefd);
	if ( ferror( savefd ) )
	{
		printf("Write error on save file...%s\n", fileName);
		bRc = false;
	}

	if ( bRc )
	{
		for ( int	nPiv = 0;  ; ++nPiv )
		{
			bRc = IntVars(  nPiv ,  var,  rep );
			if ( !bRc || var == NULL )
				break;
			fwrite( var, sizeof(*var), rep, savefd);
			if ( ferror( savefd ) )
			{
				printf("Write error on save file...%s\n", fileName);
				bRc = false;
				break;
			}
		}
	}

	if ( bRc )
		bRc = m_locVisitedCnt.Save(savefd);
//	if ( bRc )
//		bRc = m_locCondition.Save(savefd);		// never changed during the game
																				
	if ( bRc )
		bRc = m_objectLoc1.Save(savefd);
	if ( bRc )
		bRc = m_objectLoc2.Save(savefd);
	if ( bRc )
		bRc = m_objectProperty.Save(savefd);

	if ( bRc )
		bRc = m_dwarfLoc1.Save(savefd);
	if ( bRc )
		bRc = m_dwarfSeen.Save(savefd);
	if ( bRc )
		bRc = m_dwarfOldLoc.Save(savefd);

	if ( bRc )
		bRc = m_hintAreaCnt.Save(savefd);


	if (fclose(savefd)  ==  -1) 
	{
		printf("Sorry, I can't close the file...%s\n", \
		fileName);
		bRc = false;
	}
	if ( bRc )
		SpeakInfoMsg(MSG4_054_OK);
										
	if ( bRc )
	{
		if ( stricmp( szFileName, advTempRestoreFile ) != 0 )
			strcpy( szLastCheckPointName, fileName );
	}
	else
	{
		printf("Save Failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		unlink( fileName );
	}
	return bRc;
}

/*
		restore saved game handler
*/
bool CGameState::RestoreAdv( char* szFileName )
{
	FILE*	restfd;
	char	fileName[_MAX_PATH+1];
	char	*sptr;
	short	rep = 0;
	short*	var = NULL;
	bool	bRc = true;

	
	if ( szFileName && strlen(szFileName) > 0 )
	{
		strcpy(fileName, szFileName);
	}
	else
	{
		if ( strlen( szLastCheckPointName) > 0 )
		{
			strcpy( fileName, szLastCheckPointName );
		}
		else
		{
			printf("What is the name of the saved game? ");
//			scanf("%s", fileName);
			GetInputLine( fileName, sizeof(fileName)-1, false);
		}
	}
										
	if (sptr = strchr(fileName, '.'))
		*sptr = '\0';		/* kill extension	*/
	if (strlen(fileName) > _MAX_PATH - 4)
		fileName[_MAX_PATH - 4] = '\0';
	strcat(fileName, ".adv");

	restfd = fopen(fileName, "rb");
	if (restfd == NULL)
	{
		printf("Sorry, I can't open the file...%s\n", fileName);
		return false;
	}

	// lets save a copy to restore if we fail.
	if ( stricmp( szFileName, advTempRestoreFile ) != 0 )
	{
		if ( ! SaveAdv( advTempRestoreFile ) )
		{
			printf("Failed to save current state.\n");
			printf("If the restore fails, it will be fatal.\n");
			if ( ! yes(MSG4_230_Continue) )
				return false;
		}
	}

	//  First lets get the count of InVars we need to read.
	IntVars(  -1 ,  var,  rep );
	short nNumIntVarsToRead = *var;

	//	Next lets read the count of IntVars to written.
	fread( var, sizeof(*var), rep, restfd );
	if ( ferror( restfd ) )
	{
		printf("Read error on save file...%s\n", fileName);
		bRc = false;
	}
	if ( bRc )
	{
		if ( nNumIntVarsToRead != *var )
		{
			printf("Save file...%s has (%d) IntVars vs (%d) IntVars Expected\n", fileName, nNumIntVarsToRead ,*var );
			printf("Can't restore! file...%s\n", fileName);
			bRc = false;
		}
	}
		
	if ( bRc )
	{
		for ( int	nPiv = 0;  ; ++nPiv )
		{
			bRc = IntVars(  nPiv ,  var,  rep );
			if ( !bRc || var == NULL )
				break;
			fread(var, sizeof(*var), rep, restfd);
			if ( ferror( restfd ) )
			{
				printf("Read error on save file...%s\n", fileName);
				bRc = false;
				break;
			}
		}
	}

	if ( bRc )
		bRc = m_locVisitedCnt.Restore(restfd);
//	if ( bRc )
//		bRc = m_locCondition.Restore(restfd);		// never changed during the game
																				
	if ( bRc )
		bRc = m_objectLoc1.Restore(restfd);
	if ( bRc )
		bRc = m_objectLoc2.Restore(restfd);
	if ( bRc )
		bRc = m_objectProperty.Restore(restfd);

	if ( bRc )
		bRc = m_dwarfLoc1.Restore(restfd);
	if ( bRc )
		bRc = m_dwarfSeen.Restore(restfd);
	if ( bRc )
		bRc = m_dwarfOldLoc.Restore(restfd);

	if ( bRc )
		bRc = m_hintAreaCnt.Restore(restfd);

	if (fclose(restfd)  ==  -1)
	{
		printf("Warning -- can't close save file...%s\n", \
		fileName);
		bRc = false;
	}

	if ( stricmp( szFileName, advTempRestoreFile ) == 0 )
	{
		unlink ( advTempRestoreFile );
		if ( bRc )
		{
			SpeakInfoMsg(MSG4_054_OK);
		}
		else
		{
			printf("Undo Failed! *** Fatal ***\n");
			bug(BUG_GameStateSaveRestore_cpp+501);  // bad restore
		}
	}
	else
	{
		if ( bRc )
		{
			unlink ( advTempRestoreFile );
			SpeakInfoMsg(MSG4_054_OK);
		}
		else
		{
			printf("Restore Failed! --- Trying Undo ---\n");
			RestoreAdv( advTempRestoreFile );
		}
	}
	return bRc;
}
