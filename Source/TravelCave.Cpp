//	TravelCave.cpp


#include	"Adventure.h"
#include	"CaveArray.h"


/*
		CCaveArray		is an array of CTravelArray classes.	
		CTravelArray	contains a CLongIntArray which requires 32 bit LONG INTEGERS.
		these values are used in CTravelArray
		Dest*1000000 + Verb*1000 + Tcond = value stored
		Tcond = Cond * 100 + ObjOrPct.

		This implmentation allows:
			any number of locations.
			any number of movments for each location.
*/


static CCaveArray cave;


/* **************************************************************************************** */
//		Travel stuff
/* **************************************************************************************** */

/*
		Routine to fill travel array for a given location
*/
void gettrav( short loc, CTravelArray& refTravel )
{
	refTravel.ResetMemory();
										
	if ( !ValidCaveLocation( loc ) )
	{
		printf ("gettrav(Loc %d) is not valid\n");
		bug(BUG_TravelCave_Cpp+34);
		return;
	}

	refTravel = cave[loc];
}


/*
		Routine to list a travel array
*/
void ListTravelArray( CTravelArray& refTravel, short loc /* just for display */ )
{
	printf("\nDumping TravelArray\n");
	for (short kk = 0; kk <= refTravel.MaxIndexed(); ++kk) 
	{
		short nVerb, nDest, nCond, nObjOrPct = 0;
		bool bRc = refTravel( kk, nVerb, nDest, nCond, nObjOrPct );

		if ( !bRc )
			break;

		printf("cave[%d] = %d %d %d %d &ld\n",
						loc,
						nDest,
						nVerb,
						nCond,
						nObjOrPct,
						refTravel[kk]
				);
	}
	printf("\nEnd TravelArray\n");
}

/*
		Routine to copy a travel array
*/
void copytrv( const CTravelArray& trav1, CTravelArray& trav2 )
{
	trav2 = trav1;
}


/* **************************************************************************************** */
//		Cave stuff
/* **************************************************************************************** */

/*
		Routine to reset cave array 
*/
void	InitCaveTable()
{
	cave.ResetMemory();
}


/*
		Routine to fill one cave array movement entry from ADVENT0
*/
bool	LoadCaveTableEntry( char* lineIn )
{
	bool bRc = true;

	if ( lineIn[0] != '~' )
		return bRc;

	int nLoc = atoi(&lineIn[1]);

	if ( nLoc == 0 )
		return bRc;

	if ( nLoc < 1 )
	{
		printf ( "Location (%d) in (%s) < 1\n", nLoc, lineIn );
		bug(BUG_AdvMessageData_Cpp+3);
		return false;
	}

#if _DEBUG
//	if ( nLoc > 7 )
//		printf ( "Location (%d) in (%s)\n", nLoc, lineIn );
#endif		

	char*	pDelim = lineIn;
	char	delim = '|';
	int		numDelims = 0;
	long	mulpTbl[] = {0,1000000,1000,100,1};
	long	lRawLong = 0;
	short	nSeq = 0;

#define NUM_DELIMS (sizeof(mulpTbl) / sizeof(mulpTbl[0]))

#if 0

~loc|seq|dest|move|obj cond|obj or pct|Move names|...| (just for readability)

#		tdest   = dest
#		tverb   = moveId
#		tcond   = rCondition*100 + rObject		
#		lrawLong = tdest*1000000 + tverb*1000 + tcond
#endif

	int		maxDelims = NUM_DELIMS;

	while ( pDelim != NULL && *(++pDelim) && numDelims < maxDelims )
	{
		if ( *pDelim == delim )
		{
			char* pParam = pDelim+1;
			int nVal = atoi(pParam);
			long mulp = mulpTbl[numDelims];

			if ( numDelims == 0 )
			{
				nSeq = nVal - 1;
				if ( nSeq < 0 )
				{
					printf ( "Location (%d) Seq(%d) in (%s) < 1\n", nLoc, nSeq, lineIn );
					bug(BUG_AdvMessageData_Cpp+5);
					return false;
				}
			}
			else
			{
				long	lVal = nVal * mulp;
				lRawLong += lVal;
			}
			++numDelims;
		}
	}

#if 0 // switched init order in Adventure.Cpp
	if ( nLoc == 1 && nSeq <= 0 )
	{
		cave.ResetMemory();
	}
#endif
	cave(nLoc, nSeq) = lRawLong;

	return bRc;
}

/*
		Routine returns the first location in the cave
*/
short	FirstCaveLocation()
{
	return LOC_001_end_road;
}
/*
		Routine returns the last location in the cave
*/
short	LastCaveLocation()
{
	return cave.MaxLocIndexed();
}

/*
		Routine validate a location is in the cave
*/
bool	ValidCaveLocation( short loc )
{
	bool bRc = ( loc >= FirstCaveLocation() && loc <= LastCaveLocation() );
	return bRc;
}


/*
		Routine to handle very special movement.
*/
void PrintSpecialMove(short rdest)
{
	switch(rdest-300) 
	{
		case 1:  /* plover movement via alcove */
			puts("plover movement via alcove\n");
			break;
		case 2:  /* trying to remove plover, bad route */
			puts("trying to remove plover, bad route\n");
			break;
		case 3:  /* TROLL bridge */
			puts("TROLL bridge, need to pay troll\n");
			break;
		default:
			printf("PrintSpecialMove() bad rdest(%d)\n", rdest);
			bug(BUG_PlayGameTurn_Cpp+38);
			return;
	}
}


/*
		Routine to figure out a try_location
		given current m_location and a motion.
*/
static void PrintTravelRestrictions( short kk, CTravelArray& travelMap )
{
	short	mvflag = 0;
	char*	sz_object = NULL;
	char*	msg = NULL;
	char	msgBuff[100];
										
	short	nVerb, nDest, nCond, nObjOrPct = 0;
	bool	bRc = travelMap( kk, nVerb, nDest, nCond, nObjOrPct );

	if ( !bRc )
	{
		puts("Move not valid!");
		return;
	}

	sz_object = vocab(VOCAB_ENTRY_OBJ+nObjOrPct);
										
	if ( nVerb == 1 )
	{
		puts("Move not allowed");
		return;
	}
										
#ifdef _DEBUG
	if (g_debugFlg)
		printf("rdest = %d, rverb = %d, rcond = %d, robject = %d in dotrav\n",
				nDest,      nVerb,      nCond,      nObjOrPct );
#endif
	
	switch(nCond)
	{
		case 0:
//			if ((tcond == 0) || (pctt < nObjOrPct))
			++mvflag;
			if (nObjOrPct)
			{
				sprintf(msgBuff, "Entry Randomly allowed (%d%%).\n", nObjOrPct);
				msg = msgBuff;
			}
			break;
		case 1:
			if (nObjOrPct == 0)
			{
				++mvflag;
			}
//			else if (toting(nObjOrPct))
//			{
//				++mvflag;
//			}
			else
			{
				++mvflag;
				sprintf(msgBuff, "Somthing with %s\n", sz_object); 
//				msg = msgBuff;
			}
			break;
		case 2:
//			if (toting(robject) || at(robject))
//				++mvflag;
				++mvflag;
			sprintf(msgBuff, "Need %s to enter\n", sz_object); 
//			msg = msgBuff;
																														
			break;
		case 3:
		case 4:
		case 5:
		case 7:
			if (gameState.Get_objectProperty(nObjOrPct) != (nCond)-3)
			{
				++mvflag;
			}
			else
			{
				++mvflag;
				sprintf(msgBuff, "Need to do somthing with %s to enter\n", sz_object); 
//				msg = msgBuff;
			}
			break;
		default:
			printf("PrintTravelRestrictions() bad nCond(%d)\n", nCond);
			bug(BUG_TravelCave_Cpp+37);
			return;
	}
	if (!mvflag)
		;	//		puts("Move currently not allowed");
	else if (nDest>500)
		SpeakInfoMsg(nDest-500);
	else if (nDest>300)
		PrintSpecialMove(nDest);
	else 
	{
//		puts("Move allowed.");
		SpeakLocDescShort(nDest, false);	// Brief
	}

	if ( msg )
		puts(msg);
}


/*
		Routine to tell if player is on
		either side of a two sided object.
*/
static short at(short item, short loc)
{
	return(gameState.Get_objectLoc1(item) == loc || gameState.Get_objectLoc2(item) == loc );
}

/*
		Routine to describe visible items
*/
static void descitems(short loc)
{
	short	item, state;
										
	puts("\n");
	for ( item = 1; item <= gameState.LastObjectIndexed(); ++item )
	{
		short objectProperty = gameState.Get_objectProperty(item);
		if (at(item, loc))
		{
			if (objectProperty)
			{
				objectProperty = 0;
				if (item == OBJ_RUG || item == OBJ_CHAIN)
					++objectProperty;
			}
			if (item == OBJ_STEPS && loc == gameState.Get_objectLoc2(OBJ_STEPS))
				state = 1;
			else
				state = objectProperty;
			SpeakObjDesc(item, state);
		}
	}
}

/*
		Routine to output a map of one location
*/
void DisplayThisLocationMap( short loc, bool movements, bool objects, bool roomDesc, bool brief )
{

#if ! ( _DEBUG && 1 )
	if ( ! gameState.toting(OBJ_MAP) )
	{
		puts("Sorry but you don't have a map!");
		return;
	}
#endif
					
	short saveFriendly = gameState.Get_Friendly();
	gameState.Set_Friendly(0);
					
	printf("\nLocation %d\n", loc);
					
	if ( roomDesc )
	{
		if ( brief )
			SpeakLocDescShort(loc);	// Brief
		else
			SpeakLocDescLong(loc);	// long
	}
										
	if ( objects )
	{
		descitems(loc);
	}
										
	if ( movements )
	{
		CTravelArray	travelMap;
		char*	sz_object = NULL;
		gettrav( loc, travelMap );
		for (short kk = 0; kk <= travelMap.MaxIndexed(); ++kk) 
		{
			short nVerb, nDest, nCond, nObjOrPct = 0;
			bool bRc = travelMap( kk, nVerb, nDest, nCond, nObjOrPct );

			if ( !bRc )
				break;

			sz_object = vocab(VOCAB_ENTRY_OBJ+nObjOrPct);
																														
			if ( nVerb == 1 )
			{
				puts("Move not allowed\n");
				if ( nDest > 0 )
					SpeakLocDescShort(nDest);	// Brief
				else
					puts("Your dead!!!!!!\n");
				continue;
			}
																														
			if ( nVerb > 1 )
			{
				char* dir = vocab( nVerb );
				if ( dir != NULL && nDest > 0)
				{
					printf("\"%s\" goes to %d: ", dir, nDest);
					PrintTravelRestrictions( kk, travelMap );
				}
			}
		}
	}
	gameState.Set_Friendly(saveFriendly);
}



/*
		Routine to output a map of the cave
*/
void cavemap()
{
	static short loc = LOC_001_end_road;

#if ! ( _DEBUG && 1 )
	if ( ! gameState.toting(OBJ_MAP) )
	{
		puts("Sorry but you don't have a map!");
		return;
	}
#endif
										
										
	short saveBrief = gameState.Get_brief(false);
	short saveDebugFlag = g_debugFlg;
	short moveVector = 1;
	g_debugFlg = 0;
										
	if ( !ValidCaveLocation( loc ) )
		loc = LOC_001_end_road;
										
	for (   ;  ValidCaveLocation( loc ) ; loc+=moveVector)
	{
		bool movements = true;
		bool objects = true;
		bool roomDesc = true;
		bool brief = saveBrief != 0;
#if _DEBUG
		brief = false;
#endif
																				
		DisplayThisLocationMap( loc, movements, objects, roomDesc, brief );
		puts("\n");
		puts("Press + for Forward, - for Backward, Other key to stop mapping: ");
		char ch = getch();
																				
		if ( ch == '-' )
		{
			moveVector = -1;
			continue;
		}
		else if ( ch == '+' )
		{
			moveVector = 1;
			continue;
		}
		else	if ( ch == 'M' )
		{
			gameState.Set_location(loc);
			gameState.Set_newloc1(loc);
			if ( loc > LOC_010_cobble_crawl && gameState.dark())
				gameState.von();
		}
		break;
	}
	g_debugFlg = saveDebugFlag;
	puts("Cave map closed.");
}
