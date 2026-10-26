// GameStateInitialize.cpp: implementation of the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#include <crtdbg.h>
#include "Adventure.h"
#include "GameState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*
		Initialization of adventure play variables
*/
void CGameState::initplay()
{
	m_Version = 1003;			/* Program Version Should match RC */

	InitCaveTable();						
	InitPlayLocationTables();
	InitPlayObjectTables();

	m_newloc1 = LOC_001_end_road;
	m_location = LOC_001_end_road+1;
	m_oldloc1 = LOC_001_end_road+1;
	m_oldloc2 = LOC_001_end_road+1;
						
	m_dwarfSeen.ResetMemory();
	m_dwarfOldLoc.ResetMemory();
	
	short nIdx = 0;
	m_dwarfLoc1[nIdx++] = LOC_000_Limbo;
	m_dwarfLoc1[nIdx++] = LOC_019_hall_mt_king;
	m_dwarfLoc1[nIdx++] = LOC_027_west_fissure_hall_mists;
	m_dwarfLoc1[nIdx++] = LOC_033_Y2;
	m_dwarfLoc1[nIdx++] = LOC_044_maze;
	m_dwarfLoc1[nIdx++] = LOC_063_Dead_end;
	m_dwarfLoc1[nIdx++] = LOC_114_Dead_end;
	
	m_bPirateEnabled = 1;	//  0 Pirate not allowed
	m_bDwarfsEnabled = 1;	//  0 Dwarfs not allowed
	
	m_dwarfFlag = 0;
	m_dwarfsKilled = 0;
	m_dwarfAltLoc = LOC_018_nugget_gold_room;
	
	m_knifeLoc = LOC_000_Limbo;
	m_chestLoc1 = LOC_114_Dead_end;
	m_chestLoc2 = LOC_140_Dead_end;
	
	m_holding = 0;
	m_holdMax = 7;
	m_turns = 0;
	m_wasDark = 0;
	m_closed = 0;
	m_closing = 0;
	m_tally1 = 15;			// number of treasures dec for each one found
	m_tally2 = 0;			// inc if bird dies before snake.
							// inc for each toll bridge before finding spice.
							// when tally1 meets tally2, time to close cave is near.
	m_limit = 100;
	m_lampWarning = 0;
	m_clock1 = 30;
	m_clock2 = 50;
	m_panic = 0;
	
	m_bonus = 0;
	m_numdie = 0;
	m_foobar = 0;
	m_gaveup = 0;
						
	m_detail = 0;
	m_brief = 1;	// short descriptions after visited
	m_nFriendly = 0;
	m_nLogLevel = 2;
	m_nVocabCmpLen = 20;	// set to 5 to be like orignal game
//	m_szLogFileName = "AdventureCCaw1_log.txt";
							
	m_nOfferHints = 1;
	m_hintTaken = 0;
	m_hintAvail = HINT_MASK;
	m_hintAreaCnt.ResetMemory();
	
	m_bAutoSave = 0;		// 0 is no auto save.
	m_Reserved = 0x1234;	// we'll write a few of these to reserve space for new vars.

	m_bMapOpen = 0;		// no need to save and restore.
	m_bNoDieTest = 0;	// no need to save and restore.
						
	return;
}



/*
		Initialization of adventure play location condition
*/
void CGameState::InitPlayLocationCondtion(short loc, short cond)
{
	m_locCondition[loc] = cond;
}

/*
		Initialization of adventure play location variables
*/
void CGameState::InitPlayLocationTables()
{
	/* initialize visited locations */
	m_locVisitedCnt.ResetMemory();				// 0 is not visited
	
	/* initialize location status array */
	m_locCondition.ResetMemory();


	return;	// this is now read from ADVENT0.

#if 0
	// Note: All locations are dark unless condition indicates LIGHT.				
	InitPlayLocationCondtion( LOC_000_Limbo,				0 );
	InitPlayLocationCondtion( LOC_001_end_road,				HINTF+LIQUID+LIGHT );
	InitPlayLocationCondtion( LOC_002_hill_road,			HINTF+LIGHT );
	InitPlayLocationCondtion( LOC_003_well_house,			HINTF+LIQUID+LIGHT );
	InitPlayLocationCondtion( LOC_004_valley,				HINTF+LIQUID+LIGHT );
	InitPlayLocationCondtion( LOC_005_forest,				HINTF+LIGHT );
	InitPlayLocationCondtion( LOC_006_forest,				HINTF+LIGHT );
	InitPlayLocationCondtion( LOC_007_slit_streambed,		HINTF+LIQUID+LIGHT );
	InitPlayLocationCondtion( LOC_008_outside_grate,		HINTC+LIGHT );
	InitPlayLocationCondtion( LOC_009_below_grate,			LIGHT );
	InitPlayLocationCondtion( LOC_010_cobble_crawl,			LIGHT );
	InitPlayLocationCondtion( LOC_011_debris_room,		0 );
	InitPlayLocationCondtion( LOC_012_EW_canyon,		0 );
	InitPlayLocationCondtion( LOC_013_bird_chamber,			HINTB );
	InitPlayLocationCondtion( LOC_014_top_pit,			0 );
	InitPlayLocationCondtion( LOC_015_hall_mists,		0 );
	InitPlayLocationCondtion( LOC_016_crack,				FORCED_MOVE );
	InitPlayLocationCondtion( LOC_017_east_bank_fissure,	0 );
	InitPlayLocationCondtion( LOC_018_nugget_gold_room,	0 );
	InitPlayLocationCondtion( LOC_019_hall_mt_king,			HINTS );
	InitPlayLocationCondtion( LOC_020_bottom_pit,			FORCED_MOVE );
	InitPlayLocationCondtion( LOC_021_didn_t_make_it,		FORCED_MOVE );
	InitPlayLocationCondtion( LOC_022_dome_unclimbable,		FORCED_MOVE );
	InitPlayLocationCondtion( LOC_023_west_end_twopit_room,	0 );
	InitPlayLocationCondtion( LOC_024_east_pit,				LIQUID+OIL );
	InitPlayLocationCondtion( LOC_025_west_pit,			0 );
	InitPlayLocationCondtion( LOC_026_clamber_plant,		FORCED_MOVE );
	InitPlayLocationCondtion( LOC_027_west_fissure_hall_mists,	0 );
	InitPlayLocationCondtion( LOC_028_low_NS_passage,	0 );
	InitPlayLocationCondtion( LOC_029_S_side_chamber,	0 );
	InitPlayLocationCondtion( LOC_030_E_chamber_hall_mountain_king,	0 );
	InitPlayLocationCondtion( LOC_031_GT_DOLLAR_LT,			FORCED_MOVE );
	InitPlayLocationCondtion( LOC_032_can_t_get_by_snake,	FORCED_MOVE );
	InitPlayLocationCondtion( LOC_033_Y2,				0 );
	InitPlayLocationCondtion( LOC_034_jumble_rock,		0 );
	InitPlayLocationCondtion( LOC_035_window_pit,		0 );
	InitPlayLocationCondtion( LOC_036_dirty_passage,	0 );
	InitPlayLocationCondtion( LOC_037_climbable_pit,	0 );
	InitPlayLocationCondtion( LOC_038_bottom_pit_stream,	LIQUID );
	InitPlayLocationCondtion( LOC_039_rock_room,		0 );
	InitPlayLocationCondtion( LOC_040_low_passage_p_n_hall_mists,	FORCED_MOVE );
	InitPlayLocationCondtion( LOC_041_west_end_hall_mists,	0 );
	InitPlayLocationCondtion( LOC_042_maze,					HINTM );
	InitPlayLocationCondtion( LOC_043_maze,					HINTM );
	InitPlayLocationCondtion( LOC_044_maze,					HINTM );
	InitPlayLocationCondtion( LOC_045_maze,					HINTM );
	InitPlayLocationCondtion( LOC_046_Dead_end,				HINTM+NOPIRAT );
	InitPlayLocationCondtion( LOC_047_Dead_end,				HINTM+NOPIRAT );
	InitPlayLocationCondtion( LOC_048_Dead_end,				HINTM+NOPIRAT );
	InitPlayLocationCondtion( LOC_049_maze,					HINTM );
	InitPlayLocationCondtion( LOC_050_maze,					HINTM );
	InitPlayLocationCondtion( LOC_051_maze,					HINTM );
	InitPlayLocationCondtion( LOC_052_maze,					HINTM );
	InitPlayLocationCondtion( LOC_053_maze,					HINTM+NOPIRAT );
	InitPlayLocationCondtion( LOC_054_Dead_end,				HINTM );
	InitPlayLocationCondtion( LOC_055_maze,					HINTM+NOPIRAT );
	InitPlayLocationCondtion( LOC_056_Dead_end,			0 );
	InitPlayLocationCondtion( LOC_057_brink_of_pit,			NOPIRAT );
	InitPlayLocationCondtion( LOC_058_Dead_end,			0 );
	InitPlayLocationCondtion( LOC_059_low_passage_p_n_hall_mists,	FORCED_MOVE );
	InitPlayLocationCondtion( LOC_060_E_end_long_hall,	0 );
	InitPlayLocationCondtion( LOC_061_W_end_long_hall,	0 );
	InitPlayLocationCondtion( LOC_062_crossover_high_NS_passage_low_EW_one,	0 );
	InitPlayLocationCondtion( LOC_063_Dead_end,			0 );
	InitPlayLocationCondtion( LOC_064_complex_junction,	0 );
	InitPlayLocationCondtion( LOC_065_bedquilt_passage,	0 );
	InitPlayLocationCondtion( LOC_066_swiss_cheese_room,	0 );
	InitPlayLocationCondtion( LOC_067_east_end_twopit_room,	0 );
	InitPlayLocationCondtion( LOC_068_slab_room,		0 );
	InitPlayLocationCondtion( LOC_069_secret_NS_canyon_above_large_room,	0 );
	InitPlayLocationCondtion( LOC_070_secret_NS_canyon_above_sizable_passage,	0 );
	InitPlayLocationCondtion( LOC_071_junction_three_secret_canyons,	0 );
	InitPlayLocationCondtion( LOC_072_large_low_room,	0 );
	InitPlayLocationCondtion( LOC_073_Dead_end_crawl,	0 );
	InitPlayLocationCondtion( LOC_074_secret_EW_canyon,	0 );
	InitPlayLocationCondtion( LOC_075_wide_place_NS_canyon,	0 );
	InitPlayLocationCondtion( LOC_076_too_tight_canyon,	0 );
	InitPlayLocationCondtion( LOC_077_tall_EW_canyon,	0 );
	InitPlayLocationCondtion( LOC_078_canyon_dead_end,	0 );
	InitPlayLocationCondtion( LOC_079_stream_flows_out_pair_1_foot_diameter_pipes,	OIL+LIGHT );
	InitPlayLocationCondtion( LOC_080_maze,					HINTM );
	InitPlayLocationCondtion( LOC_081_Dead_end,				HINTM );
	InitPlayLocationCondtion( LOC_082_Dead_end,				HINTM+NOPIRAT );
	InitPlayLocationCondtion( LOC_083_maze,				0 );
	InitPlayLocationCondtion( LOC_084_maze,				0 );
	InitPlayLocationCondtion( LOC_085_Dead_end,				NOPIRAT );
	InitPlayLocationCondtion( LOC_086_Dead_end,				HINTM+NOPIRAT );
	InitPlayLocationCondtion( LOC_087_maze,					HINTM );
	InitPlayLocationCondtion( LOC_088_narrow_corridor,	0 );
	InitPlayLocationCondtion( LOC_089_Use_up_or_out_leave_pit,	FORCED_MOVE );
	InitPlayLocationCondtion( LOC_090_climbed_plant_out_of_pit,	FORCED_MOVE );
	InitPlayLocationCondtion( LOC_091_steep_incline_above_large_room,	0 );
	InitPlayLocationCondtion( LOC_092_giant_room,		0 );
	InitPlayLocationCondtion( LOC_093_passage_here_blocked_by_recent_cave_in,	0 );
	InitPlayLocationCondtion( LOC_094_one_end_northsouth_passage,	0 );
	InitPlayLocationCondtion( LOC_095_cavern_waterfall,		LIQUID );
	InitPlayLocationCondtion( LOC_096_soft_room,		0 );
	InitPlayLocationCondtion( LOC_097_oriental_room,	0 );
	InitPlayLocationCondtion( LOC_098_misty_cavern,		0 );
	InitPlayLocationCondtion( LOC_099_alcove,				HINTP );
	InitPlayLocationCondtion( LOC_100_plover_room,			HINTP+LIGHT );
	InitPlayLocationCondtion( LOC_101_dark_room,		0 );
	InitPlayLocationCondtion( LOC_102_arched_hall,		0 );
	InitPlayLocationCondtion( LOC_103_shell_room,		0 );
	InitPlayLocationCondtion( LOC_104_corridor_ragged_sharp_walls,	0 );
	InitPlayLocationCondtion( LOC_105_cul_de_sac,		0 );
	InitPlayLocationCondtion( LOC_106_anteroom,			0 );
	InitPlayLocationCondtion( LOC_107_maze,				0 );
	InitPlayLocationCondtion( LOC_108_Witt_s_end,			HINTW );
	InitPlayLocationCondtion( LOC_109_mirror_canyon,	0 );
	InitPlayLocationCondtion( LOC_110_window_pit,		0 );
	InitPlayLocationCondtion( LOC_111_top_stalactite,	0 );
	InitPlayLocationCondtion( LOC_112_maze,				0 );
	InitPlayLocationCondtion( LOC_113_reservoir,			LIQUID );
	InitPlayLocationCondtion( LOC_114_Dead_end,			0 );
	InitPlayLocationCondtion( LOC_115_ne_end_repository,	LIGHT );
	InitPlayLocationCondtion( LOC_116_sw_end_repository,	LIGHT );
	InitPlayLocationCondtion( LOC_117_sw_chasm,			0 );
	InitPlayLocationCondtion( LOC_118_sloping_corridor,	0 );
	InitPlayLocationCondtion( LOC_119_secret_canyon,	0 );
	InitPlayLocationCondtion( LOC_120_secret_canyon,	0 );
	InitPlayLocationCondtion( LOC_121_secret_canyon,	0 );
	InitPlayLocationCondtion( LOC_122_ne_chasm,				NOPIRAT );
	InitPlayLocationCondtion( LOC_123_corridor,				NOPIRAT );
	InitPlayLocationCondtion( LOC_124_fork_path,			NOPIRAT );
	InitPlayLocationCondtion( LOC_125_junction_warm_walls,	NOPIRAT );
	InitPlayLocationCondtion( LOC_126_breath_taking_view,	NOPIRAT );
	InitPlayLocationCondtion( LOC_127_chamber_boulders,		NOPIRAT );
	InitPlayLocationCondtion( LOC_128_limestone_passage,	NOPIRAT );
	InitPlayLocationCondtion( LOC_129_front_barren_room,	NOPIRAT );
	InitPlayLocationCondtion( LOC_130_barren_room,			NOPIRAT );
	InitPlayLocationCondtion( LOC_131_maze,				0 );
	InitPlayLocationCondtion( LOC_132_maze,				0 );
	InitPlayLocationCondtion( LOC_133_maze,				0 );
	InitPlayLocationCondtion( LOC_134_maze,				0 );
	InitPlayLocationCondtion( LOC_135_maze,				0 );
	InitPlayLocationCondtion( LOC_136_maze,				0 );
	InitPlayLocationCondtion( LOC_137_maze,				0 );
	InitPlayLocationCondtion( LOC_138_maze,				0 );
	InitPlayLocationCondtion( LOC_139_maze,				0 );
	InitPlayLocationCondtion( LOC_141_forest,				LIGHT+NOPIRAT );
	InitPlayLocationCondtion( LOC_142_tree_house,			LIGHT+NOPIRAT );
	InitPlayLocationCondtion( LOC_143_main_office,			LIGHT+NOPIRAT );
#endif

}

/*
		Initialization of adventure play Object
*/
void CGameState::	InitPlayObjectLoc(short obj, short loc1, short loc2)
{
	m_objectLoc1[obj] = loc1;
	m_objectLoc2[obj] = loc2;

	if ( obj >= OBJ_NUGGET )
		m_objectProperty[obj] = -1;	// just in case someone adds treasure > OBJ_LAST_TREASURE
}

/*
		Initialization of adventure play Object variables
*/
void CGameState::InitPlayObjectTables()
{
	_ASSERT(OBJ_LAST_TREASURE < 100 /*MAXOBJ*/);

	//	0 is normal but can have different meaning for each object(item).
	m_objectProperty.ResetMemory();

	// -1 is treasure not found
	for (short obj = FirstTreasure(); obj <= LastTreasure(); ++obj)
		m_objectProperty[obj] = -1;

	m_objectLoc1.ResetMemory();		// normal or fixed object location
	m_objectLoc2.ResetMemory();		// 0 is normal (get-able), not 0 is fixed or not get-able  object
	
	return;	// this is now read from ADVENT0.

#if 0
	InitPlayObjectLoc(OBJ_KEYS,			LOC_003_well_house, 0);
	InitPlayObjectLoc(OBJ_LAMP,			LOC_003_well_house, 0);
	InitPlayObjectLoc(OBJ_GRATE,		LOC_008_outside_grate,				LOC_009_below_grate);
	InitPlayObjectLoc(OBJ_CAGE,			LOC_010_cobble_crawl, 0);
	InitPlayObjectLoc(OBJ_ROD,			LOC_011_debris_room, 0);
	InitPlayObjectLoc(OBJ_ROD2,			LOC_000_Limbo, 0);	// dynamite in repository
	InitPlayObjectLoc(OBJ_STEPS,		LOC_014_top_pit,					LOC_015_hall_mists);
	InitPlayObjectLoc(OBJ_BIRD,			LOC_013_bird_chamber, 0);
	InitPlayObjectLoc(OBJ_DOOR,			LOC_094_one_end_northsouth_passage, -1);
	InitPlayObjectLoc(OBJ_PILLOW,		LOC_096_soft_room, 0);
	InitPlayObjectLoc(OBJ_SNAKE,		LOC_019_hall_mt_king,			-1);
	InitPlayObjectLoc(OBJ_FISSURE,		LOC_017_east_bank_fissure,			LOC_027_west_fissure_hall_mists);
	InitPlayObjectLoc(OBJ_TABLET,		LOC_101_dark_room,				-1);
	InitPlayObjectLoc(OBJ_CLAM,			LOC_103_shell_room, -1);
	InitPlayObjectLoc(OBJ_OYSTER,		LOC_000_Limbo, 0);	// replaces clam after clam opened
	InitPlayObjectLoc(OBJ_MAGAZINE,		LOC_106_anteroom, 0);
	InitPlayObjectLoc(OBJ_DWARF,		LOC_000_Limbo,					-1);
	InitPlayObjectLoc(OBJ_KNIFE,		LOC_000_Limbo,	0);
	InitPlayObjectLoc(OBJ_FOOD,			LOC_003_well_house,	0);
	InitPlayObjectLoc(OBJ_BOTTLE,		LOC_003_well_house,	0);
	InitPlayObjectLoc(OBJ_MIRROR,		LOC_109_mirror_canyon,			-1);
	InitPlayObjectLoc(OBJ_PLANT,		LOC_025_west_pit,				-1);
	InitPlayObjectLoc(OBJ_PLANT2,		LOC_023_west_end_twopit_room,		LOC_067_east_end_twopit_room);
	InitPlayObjectLoc(OBJ_STALACTITE,	LOC_111_top_stalactite,			-1);
	InitPlayObjectLoc(OBJ_FIGURE,		LOC_035_window_pit,					LOC_110_window_pit);
	InitPlayObjectLoc(OBJ_AXE,			LOC_000_Limbo, 0);	
	InitPlayObjectLoc(OBJ_DRAWING,		LOC_097_oriental_room,			-1);
	InitPlayObjectLoc(OBJ_DRAGON,		LOC_119_secret_canyon,				LOC_121_secret_canyon);
	InitPlayObjectLoc(OBJ_CHASM,		LOC_117_sw_chasm,					LOC_122_ne_chasm);
	InitPlayObjectLoc(OBJ_TROLL,		LOC_117_sw_chasm,					LOC_122_ne_chasm);
	InitPlayObjectLoc(OBJ_TROLL2,		LOC_000_Limbo, 0);	// replaces first troll
	InitPlayObjectLoc(OBJ_BEAR,			LOC_130_barren_room,			-1);
	InitPlayObjectLoc(OBJ_MESSAGE,		LOC_000_Limbo, -1);
	InitPlayObjectLoc(OBJ_VOLCANO,		LOC_126_breath_taking_view,		-1);
	InitPlayObjectLoc(OBJ_VENDING,		LOC_140_Dead_end,				-1);
	InitPlayObjectLoc(OBJ_BATTERIES,	LOC_000_Limbo, 0);	// inside vending machine
	InitPlayObjectLoc(OBJ_CARPET,		LOC_096_soft_room,				-1);
	InitPlayObjectLoc(OBJ_MAP,			LOC_142_tree_house,	0);
	InitPlayObjectLoc(OBJ_TREE,			LOC_141_forest,					-1);

//	Treasures have a value >= OBJ_NUGGET  and <= OBJ_LAST_TREASURE

	InitPlayObjectLoc(OBJ_NUGGET,		LOC_018_nugget_gold_room, 0);
	InitPlayObjectLoc(OBJ_DIAMONDS,		LOC_027_west_fissure_hall_mists, 0);
	InitPlayObjectLoc(OBJ_SILVER,		LOC_028_low_NS_passage, 0);
	InitPlayObjectLoc(OBJ_JEWELS,		LOC_029_S_side_chamber, 0);
	InitPlayObjectLoc(OBJ_COINS,		LOC_030_E_chamber_hall_mountain_king, 0);
	InitPlayObjectLoc(OBJ_CHEST,		LOC_000_Limbo, 0);
	InitPlayObjectLoc(OBJ_EGGS,			LOC_092_giant_room, 0);
	InitPlayObjectLoc(OBJ_TRIDENT,		LOC_095_cavern_waterfall, 0);
	InitPlayObjectLoc(OBJ_VASE,			LOC_097_oriental_room, 0);	// need pillow to drop on
	InitPlayObjectLoc(OBJ_EMERALD,		LOC_100_plover_room, 0);
	InitPlayObjectLoc(OBJ_PYRAMID,		LOC_101_dark_room, 0);
	InitPlayObjectLoc(OBJ_PEARL,		LOC_000_Limbo, 0);	// inside clam, need trident to get.
	InitPlayObjectLoc(OBJ_RUG,			LOC_119_secret_canyon,				LOC_121_secret_canyon);
	InitPlayObjectLoc(OBJ_SPICES,		LOC_127_chamber_boulders,	0);
	InitPlayObjectLoc(OBJ_CHAIN,		LOC_130_barren_room, 0);
#endif

}


short ConvertLocCondStringToShort( char* locCondStr )
{
/*
	BIT mapping of "m_locCondition" array which indicates location status
*/
	char word[20];
	short ii = 0;
	
	while ( ii < sizeof(word) )
	{
		char c = locCondStr[ii];
		if ( c > ' ' && c != '|' )
			word[ii++] = c;
		else
			break;
	}
	word[ii] = '\0';

	if ( stricmp( "DARK", word ) == 0 )
		return DARK;
	if ( stricmp( "LIGHT", word ) == 0 )
		return LIGHT;
	if ( stricmp( "OIL", word ) == 0 )
		return OIL;
	if ( stricmp( "LIQUID", word ) == 0 )
		return LIQUID;
	if ( stricmp( "NOPIRAT", word ) == 0 )
		return NOPIRAT;
	if ( stricmp( "FORCED_MOVE", word ) == 0 )
		return FORCED_MOVE;
	if ( stricmp( "HINTC", word ) == 0 )
		return HINTC;
	if ( stricmp( "HINTB", word ) == 0 )
		return HINTB;
	if ( stricmp( "HINTS", word ) == 0 )
		return HINTS;
	if ( stricmp( "HINTM", word ) == 0 )
		return HINTM;
	if ( stricmp( "HINTP", word ) == 0 )
		return HINTP;
	if ( stricmp( "HINTW", word ) == 0 )
		return HINTW;
	if ( stricmp( "HINTF", word ) == 0 )
		return HINTF;
	if ( stricmp( "HINTO", word ) == 0 )
		return HINTO;

	return 0;
}

/*
		Routine to fill one State table entry
*/
bool	CGameState::LoadStateTableEntry( char* lineIn )
{
	bool bRc = true;

	if ( lineIn[0] != '!' )
		return bRc;

	int nLoc = atoi(&lineIn[1]);

	if ( nLoc == 0 )
		return bRc;

	if ( nLoc < 1 )
	{
		printf ( "Location (%d) in (%s) < 1\n", nLoc, lineIn );
		bug(BUG_AdvMessageData_Cpp+3);
	}

	bool	bContinue = true;
	char*	pDelim = lineIn;
	char	delim = '|';
	int		numDelims = 0;
	short	nSeq = 0;
	short	nLocCondition = 0;
	bool	bLocCondProcess = false;

	short	nObj,	nLoc1, nLoc2 = 0;
	bool	bObjLocProcess = false;

#if 0

-- !loc|objLocTbl|objNum|loc1|loc2|Movable or Fixed  |object|
!000|objLocTbl|61|000|000|Movable|PEARL|


-- !loc|locCondTbl|condition|...|
!001|locCondTbl|HINTF|LIQUID|LIGHT|

#endif

	while ( pDelim != NULL && *(++pDelim) && bContinue )
	{
		if ( *pDelim == delim )
		{
			char* pParam = pDelim+1;

			if ( bObjLocProcess )
			{
				int nVal = *pParam == '-' ? -1 : atoi(pParam);
				switch ( numDelims )
				{
					case 1:	nObj	= nVal; break;
					case 2:	nLoc1	= nVal; break;
					case 3:	nLoc2	= nVal; bContinue = false; break;
				}
				++numDelims;
				continue;
			}
			else if ( bLocCondProcess )
			{
				short nVal = ConvertLocCondStringToShort( pParam );
				nLocCondition += nVal;
				++numDelims;
				continue;
			}
			int nVal = atoi(pParam);

			if ( numDelims == 0 )
			{
				if ( nVal == 0 && strncmp(pParam, "objLocTbl", 9) == 0 )
				{
					nSeq = nVal;
					bObjLocProcess = true;
				}
				else if ( nVal == 0 && strncmp(pParam, "locCondTbl", 10) == 0 )
				{
					nSeq = nVal;
					bLocCondProcess = true;
				}
			}
			++numDelims;
		}
	}

	if ( bObjLocProcess )
	{
		InitPlayObjectLoc(nObj,	nLoc1, nLoc2);
	}
	else if ( bLocCondProcess )
	{
		InitPlayLocationCondtion( nLoc, nLocCondition );
	}

	return bRc;
}

