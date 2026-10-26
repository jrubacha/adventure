//	MoveRequests.Cpp

#include	"Adventure.h"

/*
		Routine to handle motion requests
*/
void CGameState::domove()
{
	/*
			if object was entered, is object here?
	*/
	if ( e_object )
	{
		if (!(m_objectLoc2[e_object] == m_location || here(e_object)))
		{
			printf("I see no %s here.\n", probj(e_object));
			return;
		}
		e_object = OBJ_NONE;
	}

	CTravelArray	travel;
	gettrav( m_location, travel );

	switch(e_motion) 
	{
		case MOV_NOWHERE:
			e_motion = MOV_NONE;
			break;
		case MOV_BACK:
			goback(travel);
			break;
		case MOV_CAVE:
			if (m_location<LOC_008_outside_grate)
				SpeakInfoMsg(MSG4_057_don_t_know_where_cave_is_follow_stream);
			else
				SpeakInfoMsg(MSG4_058_need_more_detailed_instructions);
			e_motion = MOV_NONE;
			break;
		default:
			m_oldloc2 = m_oldloc1;
			m_oldloc1 = m_location;
			dotrav(travel);
	}
}


/*
		Routine to handle request to return
		from whence we came!
*/
void CGameState::goback( CTravelArray&	travel )
{
	short	kk, want;
	short	sav_newloc1 = m_newloc1;
										
	if (forced(m_oldloc1))
		want = m_oldloc2;
	else
		want = m_oldloc1;
	m_oldloc2 = m_oldloc1;
	m_oldloc1 = m_location;

	if (want == m_location) 
	{
		SpeakInfoMsg(MSG4_091_dont_remember_how_you_got_here);
		return;
	}

	for ( kk = 0; kk <= travel.MaxIndexed(); ++kk ) 
	{
		if ( ! travel.Tcond(kk ) )
		{
			if ( travel.Dest(kk) == want ) 
			{
				e_motion = travel.Verb(kk);
				dotrav( travel );
				return;
			}

			if ( ValidCaveLocation( travel.Dest(kk) ) )
			{
				if ( forced( travel.Dest(kk) ) ) 
				{
					CTravelArray ftrav;
					gettrav( travel.Dest(kk), ftrav );
					if ( ftrav.Dest(0) == want)
					{
						e_motion = ftrav.Verb(0);
						dotrav( ftrav );
						if ( m_newloc1 == want )
							return;
						m_newloc1 = sav_newloc1;
					}
				}
			}
		}
	}
	SpeakInfoMsg(MSG4_140_can_t_get_there_from_here);
}

/*
		Routine to figure out a new m_location
		given current m_location and a motion.
*/
void CGameState::dotrav( CTravelArray&	travel )
{
	short	mvflag, hitflag, kk;
	short	rdest, rverb, rcond, rObjOrPct;
	short	pctt;
										
	m_newloc1 = m_location;
	mvflag = hitflag = 0;
	pctt = Rrand(0, 99);
										
	for (kk = 0; kk <= travel.MaxIndexed() && !mvflag; ++kk) 
	{
		bool bRc = travel( kk, rverb, rdest, rcond, rObjOrPct );

		if ( !bRc )
			break;

#ifdef _DEBUG
		if (g_debugFlg)
			printf("rdest = %d, rverb = %d, r_Condition = %d, \
			robject = %d in dotrav\n", rdest, rverb, \
			rcond, rObjOrPct);
#endif
		if ((rverb != 1) && (rverb != e_motion) && !hitflag)
			continue;
		++hitflag;
		switch(rcond) 
		{
			case 0:
				if ((rObjOrPct == 0) || (pctt <= rObjOrPct))
					++mvflag;
				else
#ifdef _DEBUG
					puts("");
				if (rObjOrPct && g_debugFlg)
					printf("%% move %d %d\n",
					pctt, mvflag);
#endif
				break;
			case 1:
				if (rObjOrPct == 0)
					++mvflag;
				else if (toting(rObjOrPct))
						++mvflag;
				break;
			case 2:
				if (toting(rObjOrPct) || at(rObjOrPct))
					++mvflag;
				break;
			case 3:
			case 4:
			case 5:
			case 7:
				if (m_objectProperty[rObjOrPct] != (rcond)-3)
					++mvflag;
				break;
			default:
				printf("dotrav() bad rcond(%d)\n", rcond);
				bug(BUG_PlayGameTurn_Cpp+37);
				return;
		}
	}
	if (!mvflag)
		badmove();
	else if (rdest>500)
			SpeakInfoMsg(rdest-500);
	else if (rdest>300)
			specialMove(rdest);
	else 
	{
		m_newloc1 = rdest;
#ifdef _DEBUG
		if (g_debugFlg)
			printf("m_newloc1 in dotrav = %d\n", m_newloc1);
#endif
	}
}

/*
		The player tried a poor move option.
*/
void CGameState::badmove()
{
	short msg = MSG4_012_don_t_know_how_apply_that_here;
										
	if (e_motion >= MOV_EAST && e_motion <= MOV_NW)
		msg = MSG4_009_There_no_way_that_direction;
	if (e_motion == MOV_UP || e_motion == MOV_DOWN)
		msg = MSG4_009_There_no_way_that_direction;
	if (e_motion == MOV_FORWARD || e_motion == MOV_LEFT	|| e_motion == MOV_RIGHT)
		msg = MSG4_010_Use_compass_points_or_nearby_objects;
	if (e_motion == MOV_OUTSIDE || e_motion == MOV_INWARD)
		msg = MSG4_011_Use_compass_points_or_name_something;
	if (e_verb == VRB_FIND || e_verb == VRB_INVENTORY)
		msg = MSG4_059_only_tell_you_what_you_see;
	if (e_motion == MOV_XYZZY || e_motion == MOV_PLUGH)
		msg = MSG4_042_Nothing_happens;
	if (e_motion == MOV_CRAWL) msg = MSG4_080_Which_way;
	e_motion = MOV_NONE;
	SpeakInfoMsg(msg);
}

/*
		Routine to handle very special movement.
*/
void CGameState::specialMove(short rdest)
{
	switch(rdest-300) 
	{
		case 1:  /* plover movement via alcove */
			if (!m_holding || (m_holding == 1 && toting(OBJ_EMERALD)))
				// alcove to plover or plover to alcove
				m_newloc1 = (LOC_099_alcove+LOC_100_plover_room)-m_location;
			else
				SpeakInfoMsg(MSG4_117_Something_you_re_carrying_won_t_fit_through_tunnel);
			break;
		case 2:  /* trying to remove plover, bad route */
			drop(OBJ_EMERALD, m_location);
			SpeakInfoMsg (MSG4_054_OK);
			break;
		case 3:  /* OBJ_TROLL bridge */
			if (m_objectProperty[OBJ_TROLL] == 1) 
			{
				SpeakObjDesc(OBJ_TROLL, 1);
				m_objectProperty[OBJ_TROLL] = 0;
				move(OBJ_TROLL2, LOC_000_Limbo, LOC_000_MOVEABLE);
				move(OBJ_TROLL, LOC_117_sw_chasm, LOC_122_ne_chasm);
				juggle(OBJ_CHASM);
				m_newloc1 = m_location;
			}
			else 
			{
				m_newloc1 = (m_location == LOC_117_sw_chasm ? LOC_122_ne_chasm : LOC_117_sw_chasm);
				if (m_objectProperty[OBJ_TROLL] == 0)
					++m_objectProperty[OBJ_TROLL];
				if (!toting (OBJ_BEAR))
					return;
				SpeakInfoMsg(MSG4_162_as_you_reach_other_side_bridge_buckles);
				m_objectProperty[OBJ_CHASM] = 1;
				m_objectProperty[OBJ_TROLL] = 2;
				drop(OBJ_BEAR, m_newloc1);
				m_objectLoc2[OBJ_BEAR] = LOC_Neg1_FIXED;
				m_objectProperty[OBJ_BEAR] = 3;
				if (m_objectProperty[OBJ_SPICES]<0)
					++m_tally2;
				m_oldloc2 = m_newloc1;
				death();
			}
			break;
		default:
			printf("dotrav() bad rdest(%d)\n", rdest);
			bug(BUG_PlayGameTurn_Cpp+38);
			return;
	}
}

