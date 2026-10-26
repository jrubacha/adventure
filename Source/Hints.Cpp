//	Hints.Cpp

#include	"Adventure.h"
/*
		Routine to request whether a hint is desired.
*/
void CGameState::tryhint (int imsg, short mask, int idx)
{
	if ( ! Get_OfferHints( false ) )
		return;

	puts ("\n");
	if (yes (imsg, MSG4_000_No_Message, MSG4_054_OK))
		if (yes (MSG4_087_Hint_Offer, imsg+1, MSG4_054_OK)) 
	{
		++m_hintTaken;
//		m_hintAvail &= ~mask;
	}
	m_hintAvail &= ~mask; // aw moved to here, once is all it is offered

//	m_hintAreaCnt[idx] = 0;	// aw removed because we're only offering each hint once per game, I don't like tellamarketers :)
}

/*
		Routine to check and process hints.
*/
void CGameState::DoHint(short loc)
{
	if ( ! Get_OfferHints( false ) )
		return;

	if (m_locCondition[loc]&m_hintAvail)
	{
		switch (m_locCondition[loc]&HINT_MASK)
		{
			case HINTF:
				++m_hintAreaCnt[HINTAREAF];
				if (m_hintAreaCnt[HINTAREAF]>20 && m_locVisitedCnt[LOC_008_outside_grate] == 0 /* never in yet */)
					tryhint (MSG4_056_trying_to_find_cave, HINTF, HINTAREAF);
				break;
			case HINTC:
				++m_hintAreaCnt[HINTAREAC];
				if (m_hintAreaCnt[HINTAREAC]>3 && m_objectProperty[OBJ_GRATE] == 0 && !toting(OBJ_KEYS))
					tryhint (MSG4_062_trying_get_into_cave, HINTC, HINTAREAC);
				break;
			case HINTB:
				++m_hintAreaCnt[HINTAREAB];
				if (m_hintAreaCnt[HINTAREAB]>4 && m_objectLoc1[OBJ_BIRD] == loc && toting(OBJ_ROD))
					tryhint (MSG4_018_trying_catch_bird, HINTB, HINTAREAB);
				break;
			case HINTS:
				++m_hintAreaCnt[HINTAREAS];
				if (m_hintAreaCnt[HINTAREAS]>5 && m_objectLoc1[OBJ_SNAKE] == loc && !toting(OBJ_BIRD))
					tryhint (MSG4_020_Are_you_trying_deal_with_snake, HINTS, HINTAREAS);
				break;
			case HINTM:
				++m_hintAreaCnt[HINTAREAM];
				if (m_hintAreaCnt[HINTAREAM]>15)
					tryhint (MSG4_176_need_help_getting_out_maze, HINTM, HINTAREAM);
				break;
			case HINTP:
				++m_hintAreaCnt[HINTAREAP];
				if (m_hintAreaCnt[HINTAREAP]>5 && m_objectLoc1[OBJ_EMERALD] == LOC_100_plover_room)		// was != ??? aw.
					tryhint (MSG4_178_trying_explore_beyond_plover_room, HINTP, HINTAREAP);
				break;
			case HINTW:
				++m_hintAreaCnt[HINTAREAW];
				if (m_hintAreaCnt[HINTAREAW]>15)
					tryhint (MSG4_180_need_help_getting_out_wits_end, HINTW, HINTAREAW);
				break;
			case HINTO:
#if 0
				++m_hintAreaCnt[HINTAREAO];
				if (m_hintAreaCnt[HINTAREAO]>15)
					tryhint (, HINTO, HINTAREAO);
#endif
				break;
		}
	}
	else
	{
		m_hintAreaCnt.ResetMemory();
	}
}
