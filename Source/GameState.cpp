// GameState.cpp: implementation of the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#include "Adventure.h"
#include "GameState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameState::CGameState() : m_locVisitedCnt(150,50), m_locCondition(150,50), m_hintAreaCnt(16,16),
						 m_objectLoc1(100), m_objectLoc2(100), m_objectProperty(100),
						 m_dwarfLoc1(DWARFMAX), m_dwarfSeen(DWARFMAX), m_dwarfOldLoc(DWARFMAX)
{
}


CGameState::~CGameState()
{
										
}

/*
		Utility Routines
*/

/*
		display current brief status.
*/
bool CGameState::Get_brief( bool bDisplay )
{
	bool bRc = m_brief != 0;

	if ( m_brief < 0 )
		m_brief = 0;
	if ( m_brief > 1 )
		m_brief = 1;
							
	if ( bDisplay )
		SpeakInfoMsg(MSG4_220_BriefMessageOff+m_brief);
							
	return ( bRc ); 
}

bool CGameState::Set_brief( short brief, bool bDisplay )
{
	m_brief = brief;
	return Get_brief( bDisplay );
}

bool CGameState::Toggle_brief( bool bDisplay )
{
	m_brief ^= 1;	// toggle brief
	return Get_brief( bDisplay ); 
}

short CGameState::Get_Friendly( bool bDisplay )
{
	if ( m_nFriendly < 0 )
		m_nFriendly = 0;
	if ( m_nFriendly > 1 )
		m_nFriendly = 1;

	if ( bDisplay )
		SpeakInfoMsg(MSG4_222_FriendlyOff+m_nFriendly);
	return m_nFriendly;
}

short CGameState::Toggle_Friendly( bool bDisplay )
{
	m_nFriendly ^= 1;
	return Get_Friendly(bDisplay);
}

short CGameState::Get_VocabCmpLen( bool bDisplay )
{
	if ( bDisplay )
	{
		if ( m_nVocabCmpLen == 5)
			SpeakInfoMsg(MSG4_231_VocabLen5);
		else if ( m_nVocabCmpLen == 20)
			SpeakInfoMsg(MSG4_232_VocabLen20);
		else
		{
			printf("vocabulary search length is now %d\n", m_nVocabCmpLen);
			puts("Say \"vocab\" to switch to exact matching.\n");
		}
	}
	return m_nVocabCmpLen;
}

short CGameState::Toggle_VocabCmpLen( bool bDisplay )
{
	m_nVocabCmpLen = m_nVocabCmpLen == 20 ? 5 : 20;
	return Get_VocabCmpLen(bDisplay);
}

short CGameState::Get_LogLevel( bool bDisplay )
{
	if ( bDisplay )
		SpeakInfoMsg(MSG4_224_LogToFileOff+m_nLogLevel);
	return m_nLogLevel;
}

short CGameState::Toggle_LogLevel( bool bDisplay )
{
	if ( ++m_nLogLevel > 2 )
		m_nLogLevel = 0;
	return Get_LogLevel(bDisplay);
}

short CGameState::Get_OfferHints( bool bDisplay )
{
	if ( m_nOfferHints < 0 )
		m_nOfferHints = 0;
	if ( m_nOfferHints > 1 )
		m_nOfferHints = 1;

	if ( bDisplay )
		SpeakInfoMsg(MSG4_233_OfferHintsOff+m_nOfferHints);
	return m_nOfferHints;
}

short CGameState::Toggle_OfferHints( bool bDisplay )
{
	m_nOfferHints ^= 1;
	return Get_OfferHints(bDisplay);
}

short CGameState::Get_bAutoSave( bool bDisplay )
{
	if ( m_bAutoSave < 0 )
		m_bAutoSave = 0;
	if ( m_bAutoSave > 1 )
		m_bAutoSave = 1;

	if ( bDisplay )
		SpeakInfoMsg(MSG4_235_AutoSaveOff+m_bAutoSave);
	return m_bAutoSave;
}

short CGameState::Toggle_bAutoSave( bool bDisplay )
{
	m_bAutoSave ^= 1;
	return Get_bAutoSave(bDisplay);
}

void  CGameState::Set_PirateEnabled( short bPirateEnabled)
{
	m_bPirateEnabled = bPirateEnabled;
	if ( m_bPirateEnabled == 0 )
		if ( Get_objectLoc1( OBJ_CHEST ) == LOC_000_Limbo )
			gameState.move( OBJ_CHEST, Get_chestLoc1(), LOC_000_MOVEABLE );
}


/*
		Routine to save and restore stimer vars.
*/
void CGameState::SaveRestoreTime( bool bSave )
{
	static short	tally1;			/* item counts		*/
	static short	tally2;			/* item counts		*/
	static short	limit;			/* time m_limit		*/
	static short	wzdark;			/* game state flags	*/
	static short	closing;		/* game state flags	*/
	static short	closed;			/* game state flags	*/
	static short	clock1;			/* timing variables	*/
	static short	clock2;			/* timing variables	*/
	static short	panic;			/* timing variables	*/
	static short	lampWarning;	/* lamp warning flag*/
	static short	foobar;			/* fie fie foe foo...*/
										
	static bool bFirst = true;
										
	if ( bSave || bFirst )
	{
		tally1		= m_tally1;	
		tally2		= m_tally2;	
		limit		= m_limit;	
		wzdark		= m_wasDark;	
		closing		= m_closing;	
		closed		= m_closed;	
		clock1		= m_clock1;	
		clock2		= m_clock2;	
		panic		= m_panic;	
		lampWarning	= lampWarning;	
		foobar		= m_foobar;
																				
		bFirst	= false;
	}
	else if ( ! m_closed )  // else restore
	{
		m_tally1		= tally1;	
		m_tally2		= tally2;	
		m_limit			= limit;	
		m_wasDark		= wzdark;	
		m_closing		= closing;	
		m_closed		= closed;	
		m_clock1		= clock1;	
		m_clock2		= clock2;	
		m_panic			= panic;	
		m_lampWarning	= lampWarning;
		m_foobar		= foobar;
	}
}


/*
		Routine to test for darkness
*/
short CGameState::dark()
{
	if ( ! ValidCaveLocation( m_location ) )
		return DARK;
	return(!(m_locCondition[m_location] & LIGHT) &&
	(!m_objectProperty[OBJ_LAMP] ||
	!here(OBJ_LAMP)));
}

/*
		Routine to tell if an item is present.
*/
short CGameState::here(short item)
{
	return(m_objectLoc1[item] == m_location || toting(item));
}

/*
		Routine to tell if an item is being carried.
*/
short CGameState::toting(short item)
{
	return(m_objectLoc1[item] == LOC_Neg1_PLAYER);
}

/*
		Routine to tell if a location causes
		a forced move.
*/
short CGameState::forced(short atloc)
{
	if ( ! ValidCaveLocation( atloc ) )
		return 0;

	return(m_locCondition[atloc] & FORCED_MOVE);
}

/*
		Map starts in the tree house.
		Routine to Place the Cave Map in the forrest
		after the player wanders around a while
		and has taken all the items	from the well house
		but hasen't found the cave or the map.
*/
bool CGameState::ShowMapFirst( bool bForced )
{
	if ( m_objectLoc1[OBJ_MAP] != LOC_142_tree_house )
		return false;
	if ( m_objectProperty[OBJ_MAP] > 0 )
		return false;

	if (	m_dwarfFlag > 0
		||	m_locVisitedCnt[LOC_009_below_grate] > 0
		||	m_locVisitedCnt[LOC_011_debris_room] > 0
		||	m_locVisitedCnt[LOC_033_Y2] > 0
		)
		return false;
	
	short	visitCntr = 0;
	short	visitCntrMin = LOC_008_outside_grate * 2;
	short	numObjsIn3 = 0;
	short	mapLoc = bForced ? LOC_003_well_house : LOC_005_forest;
	
	int ii = 0;
	
	for ( ii = LOC_001_end_road; ii < LOC_008_outside_grate; ++ii )
	{
		visitCntr += m_locVisitedCnt[ii];
	}
	visitCntr += m_locVisitedCnt[LOC_141_forest];
	visitCntr += m_locVisitedCnt[LOC_143_main_office];

	for ( ii = 1; ii <= LastObjectIndexed(); ++ii )
	{
		if ( m_objectLoc1[ii] == LOC_003_well_house )
			++numObjsIn3;
	}
	if ( bForced || (visitCntr > visitCntrMin && numObjsIn3 == 0 && mapLoc != m_location ) )
	{
		m_objectLoc1[OBJ_MAP] = mapLoc;
		m_objectLoc2[OBJ_MAP] = LOC_000_MOVEABLE; // getable/carryable .. not fixed
		m_objectProperty[OBJ_MAP] = 0;
		++m_holdMax;
	}
	return m_objectLoc1[OBJ_MAP] > LOC_000_Limbo;
}

/*
		Routine to tell if player is on
		either side of a two sided object.
*/
short CGameState::at(short item)
{
	return(m_objectLoc1[item] == m_location || m_objectLoc2[item] == m_location);
}

/*
		Routine to destroy an object
*/
void CGameState::dstroy(short object)
{
	move(object, LOC_000_Limbo, LOC_000_MOVEABLE);
}

/*
		Juggle an object
		
		C  JUGGLE AN OBJECT BY PICKING IT UP AND PUTTING IT DOWN AGAIN, THE PURPOSE
		C  BEING TO GET THE OBJECT TO THE FRONT OF THE CHAIN OF THINGS AT ITS LOC.
*/
void CGameState::juggle(short object)
{
#if 0								// correct but necessary because we don't use a link.
	int loc1, loc2;
										
	loc1 = m_objectLoc1[object];
	loc2 = m_objectLoc2[object];
	move( object, loc1, loc2 );
#endif
	
}


/*
		routine to move an object and return a
		value used to set the negated m_objectProperty values
		for the repository.
*/
short CGameState::put(short object, short where, short pval)
{
	if ( object < 1 || object > LastObjectIndexed() )
	{
		printf("put(%d) object < 1 || object > LastObjectIndexed()\n", object);
		bug(BUG_GameState_cpp+99);
		return -1;
	}
										
	move(object, where, LOC_Neg2_NO_CHANGE);
	return((-1)-pval);
}


/*
		Routine to move an object

		object is the normal object number to move.
		where1 refers to m_objectLoc1
		where2 refers to m_objectLoc2, used for fixed objects.
*/
void CGameState::move(short object, short where1, short where2)
{
	short fromLoc = 0;
										
	if (   where1 < LOC_Neg1_FIXED
		|| where2 < LOC_Neg2_NO_CHANGE
		// || where >= MAXLOC
		)
	{
		printf("move(%d, %d, %d) bad where\n", object, where1, where2);
		bug(BUG_GameState_cpp+101);
		return;
	}
										
	if ( object < 1 || object > LastObjectIndexed() )
	{
		printf("move(%d) object < 1 || object > LastObjectIndexed()\n", object);
		bug(BUG_GameState_cpp+121);  // invalid object
		return;
	}

	fromLoc = m_objectLoc1[object];

	if ( fromLoc != where1 )
	{
		if ( where1 == LOC_Neg1_PLAYER )
			carry( object, fromLoc );

		if ( fromLoc == LOC_Neg1_PLAYER )
			drop( object, where1 );
		else
			m_objectLoc1[object] = where1;
	}

	if ( where2 != LOC_Neg2_NO_CHANGE )
		m_objectLoc2[object] = where2;	// fixed or moveable
}


/*
		Routine to carry an object
*/
void CGameState::carry(short object, short where)
{
	if ( object < 1 || object > LastObjectIndexed() )
	{
		printf("carry(%d, %d) object < 1 || object > LastObjectIndexed()\n", object, where);
		bug(BUG_GameState_cpp+121);  // invalid object
		return;
	}
										
	if ( m_objectLoc1[object] == LOC_Neg1_PLAYER)
		return;	// already carrying

	m_objectLoc1[object] = LOC_Neg1_PLAYER;

	if ( ! ( object == OBJ_WATER || object == OBJ_OIL ) )
		++m_holding;
}

/*
		Routine to drop an object
*/
void CGameState::drop(short object, short where)
{
	if ( object < 1 || object > LastObjectIndexed() )
	{
		printf("drop(%d, %d) object < 1 || object > LastObjectIndexed()\n", object, where);
		bug(BUG_GameState_cpp+131);  // invalid object
		return;
	}

	if (m_objectLoc1[object] == LOC_Neg1_PLAYER && where == LOC_Neg1_PLAYER )
		return;		// do nothing, must be a juggler or a tela-graber.
									
	if (m_objectLoc1[object] == LOC_Neg1_PLAYER)
		if ( ! ( object == OBJ_WATER || object == OBJ_OIL ) )
			--m_holding;

	if ( m_holding < 0 )
	{
		printf("drop(%d, %d) (m_holding(%d) < 0)\n", object, where, m_holding);
		bug(BUG_GameState_cpp+132);  // invalid condition
	}

	m_objectLoc1[object] = where;
}


/*
		Routine to check for presence
		of dwarves..
*/
short CGameState::dwarfCheck()
{
	short	i;
										
	for (i =1; i < DWARFLAST; ++i)
		if (m_dwarfLoc1[i] == m_location)
			return(i);
	return(0);
}

/*
		Determine liquid in the bottle
		Returns OBJ_WATER
				OBJ_NOTHING
				OBJ_OIL
*/
short CGameState::liq()
{
	short	i, j;
	i=m_objectProperty[OBJ_BOTTLE];		// 0 = water, 1 = empty, 2 oil
										// or
	j=-1-i;								// or
	return(liq2(i>j ? i : j));
}

/*
		Determine liquid at a location
		Returns OBJ_WATER
				OBJ_NOTHING
				OBJ_OIL
*/
short CGameState::liqloc(short location)
{
	if ( ! ValidCaveLocation( location ) )
		return liq2(1);
	if (m_locCondition[location]&LIQUID)
		return(liq2(m_locCondition[location]&OIL));
	else
		return(liq2(1));	// OBJ_NOTHING
}

/*
		Convert	0 to OBJ_WATER		21
				1 to OBJ_NOTHING	 0
				2 to OBJ_OIL		22
*/
short CGameState::liq2(short pbottle)
{
#if 1
	return((1-pbottle)*OBJ_WATER+(pbottle/2)*(OBJ_WATER+OBJ_OIL)); // very clever
#else
	// same as
	switch ( pbottle )
	{
		case 0:	return OBJ_WATER;
		case 1:	return OBJ_NOTHING;
		case 2:	return OBJ_OIL;
	}
#endif
}

