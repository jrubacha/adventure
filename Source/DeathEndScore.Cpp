//	DeathEndScore.cpp

#include	"Adventure.h"


/*
		Routine to handle player's demise via
		waking up the DoDwarves...
*/
void CGameState::dwarfend()
{
	death();
	normend();
}

/*
		normal end of game
*/
void CGameState::normend(void)
{
	static short limits[] = 
	{35, 100, 200, 250, 300, 330, 350, 400, 450, 1000};
#define NumLimits (sizeof(limits) / sizeof(limits[0]))
	
	short	idx;
	short	nextLevel;
	short	total = score();
	
	for (idx = 0; idx < NumLimits; idx++)
	{
		if ( total < limits[idx] )
			break;
	}
	puts ("\n");
	SpeakInfoMsg (MSG4_202_Rank_Amateur+idx);
	
	if ( idx < NumLimits - 1 )
	{
		nextLevel = limits[idx] - total;
		printf ("\nTo achieve the next higher rating, you need %d more point%s.\n",	nextLevel, nextLevel > 1 ? "s" : "");
	}
	
	exitAdv(1);
}

/*
		scoring
*/
short CGameState::score()
{
	short s = 0;
	short t = 0;
	short k = 0;
										
	short msg = MSG4_000_No_Message;
										
	for (int ii = FirstTreasure(); ii <= LastTreasure(); ++ii)
	{
		if (ii == OBJ_CHEST)
			k = 14;
		else if (ii > OBJ_CHEST)
				k = 16;
		else 
			k = 12;

		if (m_objectProperty[ii] >= 0)
			t += 2;
#if _DEBUG && 0
		else
			printf("Obj(%d) prop(%d)! >=0\n",ii ,m_objectProperty[ii]);
#endif
		if (m_objectLoc1[ii] == LOC_003_well_house
			&& m_objectProperty[ii] == 0)
			t += k-2;
#if _DEBUG && 0
		else
			printf("Obj(%d) not in well house || prop(%d)!0\n",ii ,m_objectProperty[ii]);
#endif
	}
	// t = 218 is all treasures
	printf("%-20s%d\n", "Treasures:", s = t);
	t = (MAXDIE - m_numdie)*10;
	if (t)
		printf("%-20s%d\n", "Survival:", t);
	s += t;
	if (!m_gaveup)
		s += 4;
	t = m_dwarfFlag > 0 || m_locVisitedCnt[LOC_019_hall_mt_king] > 0 ? 25 : 0;
	if (t)
		printf("%-20s%d\n", "Getting well in:", t);
	s += t;
	t = m_closing ? 25 : 0;
	if (t)
		printf("%-20s%d\n", "Masters section:", t);
	s += t;
	if (m_closed)
	{
		if (m_bonus == MSG4_000_No_Message)	// No explosion
			t = 10;
		else if (m_bonus == MSG4_135_loud_explosion_you_are_splashed_across_walls)
				t = 25;
		else if (m_bonus == MSG4_134_loud_explosion_burying_everything)
				t = 30;
		else if (m_bonus == MSG4_133_loud_explosion_hole_You_can_exit)
				t = 45;
		printf("%-20s%d\n", "Bonus:", t);
		s += t;
	}
	if (m_objectLoc1[OBJ_MAGAZINE] == LOC_108_Witt_s_end)
	{
		s += 1;
	}
										
	t = -15 * m_hintTaken;
	if (t)
	{
		printf("%-22s%4d\n", "Hints & intructions:", t);
		s += t;
	}
	s += 2;

	if ( s >= 350 )
	{
		short nMapLoc = m_objectLoc1[OBJ_MAP];
		short mp = nMapLoc == LOC_143_main_office ? 25 : 0;
		short vp = 0;
		short ws = 0;
		short nLocsVisited = 0;
		short nLocsNotVisited = 0;		// don't count FORCED_MOVE
		for ( short loc = 1;  ValidCaveLocation( loc ); loc++ )
		{
			if ( m_locVisitedCnt[loc] > 0 )
			{
				nLocsVisited++;
			}
			else if ( m_locCondition[loc] != FORCED_MOVE )
			{
				nLocsNotVisited++;
//				printf ("Loc(%d) not visited\n", loc );
			}
		}
		vp = nLocsNotVisited < 10 ? 50 : 0;
		ws = mp + vp > 0 ? 25 : 0;
		if ( ws > 0 )
		{
			printf("%-20s%d\n", "Wizards section:", ws);
			if ( nMapLoc == LOC_143_main_office )
				printf("The Wizard thanks you for returning his map.\n");
			printf("%-20s%d\n", "Bonus of:", vp + mp);
			s += ws + vp + mp;
		}
	}
	if (s < 0)
		s = 0;
	printf("%-22s%d\n", "Score:", s);
	printf("%-22s%d\n", "Turns:", m_turns);
										
	return s;
}

/*
		Routine to handle the passing on of one
		of the player's incarnations...
*/
void CGameState::death()
{
	short	yea, j;

	if ( Get_bNoDieTest() )				// used to test all moves
		return;
	
	if (!m_closing) 
	{
		yea = yes(MSG4_081_killed_might_help_you_out+m_numdie*2, MSG4_082_don_t_blame_me+m_numdie*2, MSG4_054_OK);
		if (++m_numdie >= MAXDIE || !yea)
			normend();
		m_objectLoc1[OBJ_WATER] = LOC_000_Limbo;
		m_objectLoc1[OBJ_OIL] = LOC_000_Limbo;
		if (toting(OBJ_LAMP))
			m_objectProperty[OBJ_LAMP] = 0;
		for ( j = LastObjectIndexed(); j > 0; --j )
		{
			if (toting (j))
				drop(j, j == OBJ_LAMP ? LOC_001_end_road : m_oldloc2 );
		}
		m_newloc1 = LOC_003_well_house;
		m_oldloc1 = m_location;
		return;
	}
	/*
			m_closing -- no resurrection...
	*/
	SpeakInfoMsg(MSG4_131_you_re_dead_it_s_close_closing_time_call_it_a_day);
	++m_numdie;
	normend();
}
