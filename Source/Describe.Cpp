//	Describe.Cpp

#include	"Adventure.h"

/*
		Routine to describe current location
*/
void CGameState::describe( short nType ) //  nType =0, normal; =1, brief; =2, long
{
	if ( ! ValidCaveLocation( m_location ) )
	{
		printf("describe() bad m_location(%d)\n", m_location);
		bug(BUG_PlayGameTurn_Cpp+25);
		return;
	}
	if (toting(OBJ_BEAR))
		SpeakInfoMsg(MSG4_141_followed_by_tame_bear);
	if (dark())
		SpeakInfoMsg(MSG4_016_pitch_dark_If_proceed_likely_fall_into_pit);
	else if ( nType == 2 )
		SpeakLocDescLong(m_location);	// long
	else if ( nType == 1 )
			SpeakLocDescShort(m_location);	// Brief
	else if (m_locVisitedCnt[m_location] && m_brief )
			SpeakLocDescShort(m_location);	// Brief
	else
		SpeakLocDescLong(m_location);	// long
										
	if (m_location == LOC_033_Y2 && pct(25, 12) && !m_closing)
		SpeakInfoMsg(MSG4_008_hollow_voice_says_Plugh);
}

/*
		Routine to describe visible items
*/
void CGameState::descitem()
{
	short	i, state;
	bool	bFirst = true;
										
	if ( ! ValidCaveLocation( m_location ) )
	{
		printf("descitem() bad m_location(%d)\n", m_location);
		bug(BUG_PlayGameTurn_Cpp+27);
		return;
	}

	for ( i = 1; i <= LastObjectIndexed(); ++i ) 
	{
		if (at(i)) 
		{
			if (i == OBJ_STEPS && toting(OBJ_NUGGET))
				continue;
			if (m_objectProperty[i]<0) 
			{
				if (m_closed)
					continue;
				else 
				{
					m_objectProperty[i] = 0;
					if (i == OBJ_RUG || i == OBJ_CHAIN)
						++m_objectProperty[i];
					--m_tally1;
				}
			}
			if (i == OBJ_STEPS && m_location == m_objectLoc2[OBJ_STEPS])
				state = 1;
			else
				state = m_objectProperty[i];
			if ( bFirst )
			{
				puts("\n");
				bFirst = false;
			}
			SpeakObjDesc(i, state);
		}
	}
	if (m_tally1 == m_tally2 && m_tally1 != 0 && m_limit > 35)
		m_limit = 35;
}
