// GameState.h: interface for the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#ifndef GameState_H
#define GameState_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShortIntArray.h"
#include "TravelArray.h"
#include "Vocabulary.h"

class CGameState
{

private:

short	m_Version;			/* Program Version */

short	m_newloc1;			/* player location variables */
short	m_location;			/* player location variables */
short	m_oldloc1;			/* player location variables */
short	m_oldloc2;			/* player location variables */

CShortIntArray m_locVisitedCnt;/* # times player has been here	*/
CShortIntArray m_locCondition; /* location status	dark, light etc 2=forced move */

short	m_object1;				/* to help shortrans.	*/
CShortIntArray m_objectLoc1;	/* location of side1 of objecs, index by object */
CShortIntArray m_objectLoc2;	/* location of side2 of  objecs, index by object */
CShortIntArray m_objectProperty;/* status of object, used as an index into advent3.txt	*/

CShortIntArray	m_dwarfLoc1;	/* dwarf locations	*/
CShortIntArray	m_dwarfSeen;	/* dwarf seen flag	*/
CShortIntArray	m_dwarfOldLoc;	/* dwarf old locations	*/

short	m_bPirateEnabled;	/* Pirate Enabled 0	not allowed	*/
short	m_bDwarfsEnabled;	/* Dwarfs Enabled 0	not allowed	*/

short	m_dwarfFlag;		/* dwarf flag       	*/
short	m_dwarfAltLoc;		/* alternate appearance	*/
short	m_dwarfsKilled;		/* dwarves killed	*/

short	m_knifeLoc;			/* knife location	*/
short	m_chestLoc1;		/* chest location	*/
short	m_chestLoc2;		/* not chest message location	*/

short	m_holding;			/* count of held items	*/
short	m_holdMax;			/* max # items a player can carry */
short	m_turns;			/* number of turns made		 */
short	m_wasDark;			/* game state flags	*/
short	m_closing;			/* game state flags	*/
short	m_closed;			/* game state flags	*/
short	m_tally1;			/* item counts		*/
short	m_tally2;			/* item counts		*/
short	m_limit;			/* time m_limit		*/
short	m_lampWarning;		/* lamp warning flag	*/
short	m_clock1;			/* timing variables	*/
short	m_clock2;			/* timing variables	*/
short	m_panic;			/* timing variables	*/

short	m_bonus;			/* to pass to end	*/
short	m_numdie;			/* number of deaths	*/
short	m_foobar;			/* fie fie foe foo...	*/
short	m_gaveup;			/* 1 if he quit early	*/

short	m_detail;			/* LOOK count		*/
short	m_brief;			/* just give brief description */
short	m_nFriendly;		/* Show Location Numbers */
short	m_nVocabCmpLen;		/* max length for vocabulary search compare	*/
short	m_nLogLevel;		/* 0 -> no logging, 1 -> log input commands, 2 -> log all input and output */
char*	m_szLogFileName;	/* log file name */

short	m_nOfferHints;			/* 0, no hints offered else offer hints */
short	m_hintTaken;            /* minus 1 for each hint */
short	m_hintAvail;            /* masks of hints still available */
CShortIntArray	m_hintAreaCnt;	/* Count of moves expended hanging around hint area */

short	m_bAutoSave;		// 0 is no auto save.
short	m_Reserved;			// we'll write a few of these to reserve space for new vars.

short	m_bMapOpen;			// no need to save and restore.
short	m_bNoDieTest;		// no need to save and restore.

private:

void InitPlayLocationCondtion(short loc, short cond);
void InitPlayLocationTables();
void InitPlayObjectLoc(short obj, short loc1, short loc2);
void InitPlayObjectTables();


public:

	CGameState();
	virtual ~CGameState();

/*
		Routine to fill one State table entry
*/
bool	LoadStateTableEntry( char* lineIn );

void initplay();

bool IntVars( int nPivNdx, short*& var,  short& rep );
bool SaveAdv( char* szFileName = NULL );
bool RestoreAdv( char* szFileName = NULL );

void SaveRestoreTime( bool bSave = false );

public:

// Helper functions not valid untill after table initialization.

// short	LastLoccationIndexed() { return m_locCondition.MaxIndexed(); }
// See TravelCave.h for other location functions

short	FirstTreasure() { return OBJ_NUGGET; }
short	LastTreasure() { return __max( m_objectProperty.MaxIndexed(), OBJ_LAST_TREASURE); }

short	LastObjectIndexed() { return m_objectLoc1.MaxIndexed(); }
short	LastDwarfIndexed() { return m_dwarfLoc1.MaxIndexed(); }
short	LastHintIndexed() { return m_hintAreaCnt.MaxIndexed(); }


public:

short Get_Version() { return m_Version; }
void  Set_Version( short Version ) { m_Version = Version; }

short Get_newloc1() { return m_newloc1; }
void Set_newloc1( short newloc1) {  m_newloc1 = newloc1; }
//	short Inc_newloc1() { return ++m_newloc1; }
//	short Dec_newloc1() { return --m_newloc1; }

short Get_location() { return m_location; }
void Set_location( short location) {  m_location = location; }
//	short Inc_location() { return ++m_location; }
//	short Dec_location() { return --m_location; }

short Get_oldloc1() { return m_oldloc1; }
void Set_oldloc1( short oldloc1) {  m_oldloc1 = oldloc1; }
//	short Inc_oldloc1() { return ++m_oldloc1; }
//	short Dec_oldloc1() { return --m_oldloc1; }

short Get_oldloc2() { return m_oldloc2; }
void Set_oldloc2( short oldloc2) {  m_oldloc2 = oldloc2; }
//	short Inc_oldloc2() { return ++m_oldloc2; }
//	short Dec_oldloc2() { return --m_oldloc2; }


short Get_locVisitedCnt( short nIdx ) { return m_locVisitedCnt[nIdx]; }
void Set_locVisitedCnt( short visited, short nIdx ) {  m_locVisitedCnt[nIdx] = visited; }
void ResetMemory_locVisitedCnt() {  m_locVisitedCnt.ResetMemory(); }
	short Inc_locVisitedCnt( short nIdx ) { return ++m_locVisitedCnt[nIdx]; }
//	short Dec_locVisitedCnt( short nIdx ) { return --m_locVisitedCnt[nIdx]; }

short Get_locCondition( short nIdx ) { return m_locCondition[nIdx]; }
void Set_locCondition( short cond, short nIdx ) {  m_locCondition[nIdx] = cond; }
void ResetMemory_locCondition() {  m_locCondition.ResetMemory(); }
//	short Inc_locCondition( short nIdx ) { return ++m_locCondition[nIdx]; }
//	short Dec_locCondition( short nIdx ) { return --m_locCondition[nIdx]; }


short Get_object1() { return m_object1; }
void Set_object1( short object1) {  m_object1 = object1; }
//	short Inc_object1() { return ++m_object1; }
//	short Dec_object1() { return --m_object1; }

short Get_objectLoc1( short nIdx ) { return m_objectLoc1[nIdx]; }
void Set_objectLoc1( short place, short nIdx ) {  m_objectLoc1[nIdx] = place; }
void ResetMemory_objectLoc1() {  m_objectLoc1.ResetMemory(); }
//	short Inc_objectLoc1( short nIdx ) { return ++m_objectLoc1[nIdx]; }
//	short Dec_objectLoc1( short nIdx ) { return --m_objectLoc1[nIdx]; }

short Get_objectLoc2( short nIdx ) { return m_objectLoc2[nIdx]; }
void Set_objectLoc2( short fixed, short nIdx ) {  m_objectLoc2[nIdx] = fixed; }
void ResetMemory_objectLoc2() {  m_objectLoc2.ResetMemory(); }
//	short Inc_objectLoc2( short nIdx ) { return ++m_objectLoc2[nIdx]; }
//	short Dec_objectLoc2( short nIdx ) { return --m_objectLoc2[nIdx]; }

short Get_objectProperty( short nIdx ) { return m_objectProperty[nIdx]; }
void Set_objectProperty( short prop, short nIdx ) {  m_objectProperty[nIdx] = prop; }
void ResetMemory_objectProperty() {  m_objectProperty.ResetMemory(); }
//	short Inc_objectProperty( short nIdx ) { return ++m_objectProperty[nIdx]; }
//	short Dec_objectProperty( short nIdx ) { return --m_objectProperty[nIdx]; }


short Get_PirateEnabled() { return m_bPirateEnabled; }
void  Set_PirateEnabled( short bPirateEnabled);

short Get_DwarfsEnabled() { return m_bDwarfsEnabled; }
void  Set_DwarfsEnabled( short bDwarfsEnabled) {  m_bDwarfsEnabled = bDwarfsEnabled; }


short Get_dwarfLoc1( short nIdx ) { return m_dwarfLoc1[nIdx]; }
void Set_dwarfLoc1( short dwarfLoc1, short nIdx ) {  m_dwarfLoc1[nIdx] = dwarfLoc1; }
void ResetMemory_dwarfLoc1() {  m_dwarfLoc1.ResetMemory(); }

short Get_dwarfSeen( short nIdx ) { return m_dwarfSeen[nIdx]; }
void Set_dwarfSeen( short dwarfSeen, short nIdx ) {  m_dwarfSeen[nIdx] = dwarfSeen; }
void ResetMemory_dwarfSeen() {  m_dwarfSeen.ResetMemory(); }

short Get_dwarfOldLoc( short nIdx ) { return m_dwarfOldLoc[nIdx]; }
void Set_dwarfOldLoc( short oldDwarfLoc, short nIdx ) {  m_dwarfOldLoc[nIdx] = oldDwarfLoc; }
void ResetMemory_oldDwarfLoc() {  m_dwarfOldLoc.ResetMemory(); }


short Get_dwarfFlag() { return m_dwarfFlag; }
void Set_dwarfFlag( short dwarfFlag) {  m_dwarfFlag = dwarfFlag; }

short Get_dwarfAltLoc() { return m_dwarfAltLoc; }
void Set_dwarfAltLoc( short dwarfAltLoc) {  m_dwarfAltLoc = dwarfAltLoc; }

short Get_dwarfsKilled() { return m_dwarfsKilled; }
void Set_dwarfsKilled( short dwarfsKilled) {  m_dwarfsKilled = dwarfsKilled; }
//	short Inc_dwarfsKilled() { return ++m_dwarfsKilled; }
//	short Dec_dwarfsKilled() { return --m_dwarfsKilled; }


short Get_knfloc() { return m_knifeLoc; }
void Set_knfloc( short knfloc) {  m_knifeLoc = knfloc; }

short Get_chestLoc1() { return m_chestLoc1; }
void Set_chestLoc1( short chestLoc1) {  m_chestLoc1 = chestLoc1; }

short Get_chestLoc2() { return m_chestLoc2; }
void Set_chestLoc2( short chestLoc2) {  m_chestLoc2 = chestLoc2; }


short Get_holding() { return m_holding; }
void Set_holding( short holding) {  m_holding = holding; }
short Inc_holding() { return ++m_holding; }
short Dec_holding() { return --m_holding; }

short Get_holdMax() { return m_holdMax; }
void Set_holdMax( short holdMax) {  m_holdMax = holdMax; }
short Inc_holdMax() { return ++m_holdMax; }
short Dec_holdMax() { return --m_holdMax; }

short Get_turns() { return m_turns; }
void Set_turns( short turns) {  m_turns = turns; }
//	short Inc_turns() { return ++m_turns; }
//	short Dec_turns() { return --m_turns; }

short Get_wasDark() { return m_wasDark; }
void Set_wasDark( short wzdark) {  m_wasDark = wzdark; }
//	short Inc_wasDark() { return ++m_wasDark; }
//	short Dec_wasDark() { return --m_wasDark; }

short Get_closing() { return m_closing; }
void Set_closing( short closing) {  m_closing = closing; }
//	short Inc_closing() { return ++m_closing; }
//	short Dec_closing() { return --m_closing; }

short Get_closed() { return m_closed; }
void Set_closed( short closed) {  m_closed = closed; }
//	short Inc_closed() { return ++m_closed; }
//	short Dec_closed() { return --m_closed; }

short Get_tally1() { return m_tally1; }
void Set_tally1( short tally1) {  m_tally1 = tally1; }
//	short Inc_tally1() { return ++m_tally1; }
//	short Dec_tally1() { return --m_tally1; }

short Get_tally2() { return m_tally2; }
void Set_tally2( short tally2) {  m_tally2 = tally2; }
//	short Inc_tally2() { return ++m_tally2; }
//	short Dec_tally2() { return --m_tally2; }

short Get_limit() { return m_limit; }
void Set_limit( short limit) {  m_limit = limit; }
//	short Inc_limit() { return ++m_limit; }
//	short Dec_limit() { return --m_limit; }

short Get_lampWarning() { return m_lampWarning; }
void Set_lampWarning( short lampWarning) {  m_lampWarning = lampWarning; }
//	short Inc_lmwarn() { return ++m_lampWarning; }
//	short Dec_lmwarn() { return --m_lampWarning; }

short Get_clock1() { return m_clock1; }
void Set_clock1( short clock1) {  m_clock1 = clock1; }
//	short Inc_clock1() { return ++m_clock1; }
//	short Dec_clock1() { return --m_clock1; }

short Get_clock2() { return m_clock2; }
void Set_clock2( short clock2) {  m_clock2 = clock2; }
//	short Inc_clock2() { return ++m_clock2; }
//	short Dec_clock2() { return --m_clock2; }

short Get_panic() { return m_panic; }
void Set_panic( short panic) {  m_panic = panic; }
//	short Inc_panic() { return ++m_panic; }
//	short Dec_panic() { return --m_panic; }


short Get_bonus() { return m_bonus; }
void Set_bonus( short bonus) {  m_bonus = bonus; }
//	short Inc_bonus() { return ++m_bonus; }
//	short Dec_bonus() { return --m_bonus; }

short Get_numdie() { return m_numdie; }
void Set_numdie( short numdie) {  m_numdie = numdie; }
//	short Inc_numdie() { return ++m_numdie; }
//	short Dec_numdie() { return --m_numdie; }

short Get_foobar() { return m_foobar; }
void Set_foobar( short foobar) {  m_foobar = foobar; }
//	short Inc_foobar() { return ++m_foobar; }
//	short Dec_foobar() { return --m_foobar; }

short Get_gaveup() { return m_gaveup; }
void Set_gaveup( short gaveup) {  m_gaveup = gaveup; }
//	short Inc_gaveup() { return ++m_gaveup; }
//	short Dec_gaveup() { return --m_gaveup; }


short Get_detail() { return m_detail; }
void Set_detail( short detail) {  m_detail = detail; }
//	short Inc_detail() { return ++m_detail; }
//	short Dec_detail() { return --m_detail; }

bool Get_brief( bool bDisplay = false );
bool Set_brief( short brief, bool bDisplay = false);
bool Toggle_brief( bool bDisplay = true );

short Get_Friendly( bool bDisplay = false );
short Set_Friendly( short nFriendly,  bool bDisplay = false) { m_nFriendly = nFriendly; return Get_Friendly( bDisplay ); }
short Toggle_Friendly( bool bDisplay = true );

short Get_VocabCmpLen( bool bDisplay = false  );
short Set_VocabCmpLen( short nVocabCmpLen,  bool bDisplay = false) { m_nVocabCmpLen = nVocabCmpLen; return Get_VocabCmpLen( bDisplay ); }
short Toggle_VocabCmpLen( bool bDisplay = true );

short Get_LogLevel( bool bDisplay = false  );
short Set_LogLevel( short nLogLevel,  bool bDisplay = false ) { m_nLogLevel = nLogLevel; return Get_LogLevel( bDisplay ); }
short Toggle_LogLevel( bool bDisplay = true );

short Get_OfferHints( bool bDisplay = false  );
short Set_OfferHints( short nOfferHints,  bool bDisplay = false) { m_nOfferHints = nOfferHints; return Get_OfferHints( bDisplay ); }
short Toggle_OfferHints( bool bDisplay = true );

short Get_hintTaken() { return m_hintTaken; }
void  Set_hintTaken( short hintTaken) {  m_hintTaken = hintTaken; }
short Inc_hintTaken() { return ++m_hintTaken; }
short Dec_hintTaken() { return --m_hintTaken; }

short Get_hintAvail() { return m_hintAvail; }
void  Set_hintAvail( short hintAvail) {  m_hintAvail = hintAvail; }
void  And_hintAvail( short mask) {  m_hintAvail &= mask; }

short Get_hintAreaCnt( short nIdx ) { return m_hintAreaCnt[nIdx]; }
void  Set_hintAreaCnt( short hintAreaCnt, short nIdx ) {  m_hintAreaCnt[nIdx] = hintAreaCnt; }
void  ResetMemory_hintAreaCnt() {  m_hintAreaCnt.ResetMemory(); }
//	short Inc_hintAreaCnt( short nIdx ) { return ++m_hintAreaCnt[nIdx]; }
//	short Dec_hintAreaCnt( short nIdx ) { return --m_hintAreaCnt[nIdx]; }

short Get_bNoDieTest() { return m_bNoDieTest; }
void  Set_bNoDieTest( short bNoDieTest ) {  m_bNoDieTest = bNoDieTest; }

short Get_bAutoSave( bool bDisplay = false );
void  Set_bAutoSave( short bAutoSave,  bool bDisplay = false ) {  m_bAutoSave = bAutoSave; Get_bAutoSave(bDisplay); }
short Toggle_bAutoSave( bool bDisplay = true );

short Get_bMapOpen() { return m_bMapOpen; }
void  Set_bMapOpen( short bMapOpen ) {  m_bMapOpen = bMapOpen; }

// Prototypes from PlayGameTurn.Cpp

void PlayGameTurn();
short stimer();


// Prototypes from Hints.Cpp

void tryhint (int imsg, short mask, int idx);
void DoHint(short loc);


// Prototypes from Describe.Cpp

#define describeTypeNormal	0
#define describeTypeShort	1
#define describeTypeLong	2

void describe(  short nType = describeTypeNormal ); //  nType =0, normal; =1, brief; =2, long
void descitem();

// Prototypes from MoveRequests.Cpp

void domove();
void goback( CTravelArray&	travel );
void dotrav( CTravelArray&	travel );
void badmove();
void specialMove(short rdest);

// Prototypes from DeathEndScore.Cpp

void dwarfend();
void normend();
short score();
void death();

// Prototypes from DoTrPrObject.Cpp

void doobj();
void trobj();
char* probj(short object);

// Prototypes from DwarvesAndPirate.Cpp

void DoDwarves();
bool PirateCanStealIt( short nObject, short nLoc  );
void DoPirate();



/*
	Routine to process a transitive verb
*/
// Prototypes from TRVERB.Cpp
void trverb();
void vtake();
void vdrop();
void vopen();
void vsay();
void von();
void voff();
void vwave();
void vkill();
void vpour();
void veat();
void vdrink();
void vthrow();
void vfind();
void vfill();
void vfeed();
void vread();
void vblast();
void vbreak();
void vwake();
void actspk(short verb);
void needobj();


/*
	Routines to process intransitive verbs
*/
// Prototypes from ITVERB.Cpp
void itverb();
void ivtake( short& e_object );
void ivdrop( short& e_object );
void ivopen( short& e_object );
void ivkill( short e_verb, short& e_object );
void iveat( short& e_object );
void ivdrink( short& e_object );
void ivquit();
void ivfill( short& e_object );
void ivfoo( char* e_word1, short e_verb );
void ivread( short& e_object );
void inventory();
void ivDescribe();
void ivLog();
void ivAutoCheckPoint();
void ivFriendly();
void ivHints();
void ivVocabLength();

void addobj( short obj,  short& e_object );

/*
	Utility Routines
*/
short dark();
short here(short item);
short toting(short item);
short forced(short atloc);
bool ShowMapFirst( bool bForced = false );
short at(short item);
void dstroy(short obj);
void move(short obj, short where1, short where2 );
void juggle(short loc);
void carry(short obj, short where);
void drop(short obj, short where);
short put(short obj, short where, short pval);
short dwarfCheck();
short liq();
short liqloc(short loc);
short liq2(short pbottle);


};

extern	CGameState	gameState;

//#define	MAXOBJ	(short)100		/* max # of objects in cave,	code could be changed to doubble or tripple this.*/
//#define	MAXLOC	(short)150		/* max # of cave locations, 300 is the absolute max*/

#define	DWARFMAX	(short)7			/* max # of nasty dwarves	*/
#define	DWARFLAST	(short)(DWARFMAX-1)	/* last entry of nasty dwarves	*/
#define	MAXDIE		(short)3			/* max # of deaths before close	*/

/*
	BIT mapping of "m_locCondition" array which indicates location status
*/
#define DARK		 (short)0			// No light
#define LIGHT		 (short)1			// Light
#define OIL			 (short)2			// LIQUID + OIL is Oil
#define LIQUID		 (short)4			// LIQUID is Water
#define NOPIRAT		 (short)8			// No Pirate or Dwarves allowed
#define FORCED_MOVE (short)16			// Forced move cond value
#define HINTC       (short)32			// trying_get_into_cave?
#define HINTB       (short)64			// trying_catch_bird?
#define HINTS      (short)128			// trying_deal_with_snake
#define HINTM      (short)256			// help_getting_out_maze?
#define HINTP      (short)512			// explore_beyond_plover_room?
#define HINTW     (short)1024			// need_help_getting_out_wits_end
#define HINTF     (short)2048			// trying_to_find_cave ?
#define HINTO     (short)4096			// Other hint???
#define HINT_MASK (short)8160			// Hint Mask

#define HINTAREAC    (short)0
#define HINTAREAB    (short)1
#define HINTAREAS    (short)2
#define HINTAREAM    (short)3
#define HINTAREAP    (short)4
#define HINTAREAW    (short)5
#define HINTAREAF    (short)6



#endif // GameState_H
