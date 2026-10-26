#ifndef Adventure_h
#define Adventure_h

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<conio.h>
#include	<ctype.h>
#include	<memory.h>

#include "GameState.h"
#include "AdvMessageData.h"
#include "TravelCave.h"
#include "Vocabulary.h"
#include "English.h"
#include "RandomPercent.h"


extern	short	g_debugFlg;		/* if game is in debug	*/

// Prototypes from Adventure.CPP

//void main(int argc, char ** argv);

void ResetGameState( int rFlag = 0, char* rFile = NULL );

void exitAdv(int code);

void bug(short n);

#define BUG_Adventure_cpp				1000
#define BUG_AdvMessageData_Cpp			2000
#define BUG_English_Cpp					3000
#define BUG_GameState_cpp				4000
#define BUG_GameStateInitialize_cpp		5000
#define BUG_GameStateSaveRestore_cpp	6000
#define BUG_ItVerb_Cpp					7000
#define BUG_PlayGameTurn_Cpp			8000
#define BUG_TravelCave_Cpp				9000
#define BUG_TrVerb_Cpp					10000
#define BUG_Vocabulary_Cpp				11000


#endif // #ifndef Adventure_h
