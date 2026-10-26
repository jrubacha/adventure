//	PlayGameTurn.cpp

#include	<direct.h>			// needed for mkdir();
#include	"Adventure.h"



/*
		Routine to take 1 Turn
*/
void CGameState::PlayGameTurn()
{
	short	i;
				
	++m_turns;
	
	if ( m_location != LOC_000_Limbo && ! ValidCaveLocation( m_location ) )
	{
		printf("PlayGameTurn() bad m_location(%d)\n", m_location);
		bug(BUG_PlayGameTurn_Cpp+21);
	}

	ShowMapFirst();
										
	/*
			if m_closing, then he can't leave except via
			the main office.
	*/
	if ( m_location != LOC_143_main_office )
	{
		if (m_newloc1 < LOC_009_below_grate && m_newloc1 != LOC_000_Limbo && m_closing) 
		{
			SpeakInfoMsg(MSG4_130_exit_closed_leave_via_main_office);
			m_newloc1 = m_location;
			if (!m_panic)
				m_clock2 = 15;
			m_panic = 1;
		}
	}
	/*
			see if a DWARF has seen him and has come
			from where he wants to go.
	*/
	if (m_newloc1 != m_location && !forced(m_location) && m_locCondition[m_location]&NOPIRAT  ==  0)
	{
		for (i = 1; i< (DWARFLAST); ++i)
		{
			if (m_dwarfOldLoc[i] == m_newloc1 && m_dwarfSeen[i])
			{
				m_newloc1  =  m_location;
				SpeakInfoMsg(MSG4_002_dwarf_knife_blocks_way);
				break;
			}
		}
	}
										
	DoDwarves();	/* & special DWARF(pirate who steals)	*/
										
	/* added by BDS C conversion */
	if (m_location != m_newloc1)
	{
		m_location = m_newloc1;
		
		/* check for death */
		if (m_location == LOC_000_Limbo) 
		{
			death();
			return;
		}
																				
		/* check for forced move */
		if (forced (m_location)) 
		{
			describe();
			domove();
			return;
		}
																				
		/* check for wandering in dark */
		if ( m_wasDark && dark() && pct(35, 10) ) 
		{
			SpeakInfoMsg(MSG4_023_fell_into_pit_broke_every_bone_body);
			m_oldloc2 = m_location;
			death();
			return;
		}
																				
		/* describe his situation */
		describe();
		if (!dark ()) 
		{
			++m_locVisitedCnt[m_location];
			descitem();
		}
																				
	}  /* if (m_location != m_newloc1)	*/
	else if ( e_motion )
	{
		/* describe his situation */
		describe();
		if (!dark ()) 
		{
			++m_locVisitedCnt[m_location];
			descitem();
		}
	}
													
	if (m_closed) 
	{
		if (m_objectProperty[OBJ_OYSTER] < 0 && toting (OBJ_OYSTER))
			SpeakObjDesc(OBJ_OYSTER, 1);

		for ( i = 1; i <= LastObjectIndexed(); ++i )
			if (toting (i) && m_objectProperty[i] < 0)
				m_objectProperty[i] = -1-m_objectProperty[i];
	}
										
	m_wasDark = dark();
	if (m_knifeLoc > LOC_000_Limbo && m_knifeLoc != m_location)
		m_knifeLoc  =  LOC_000_Limbo;
										
	if (stimer())	/* as the grains of sand slip by	*/
		return;
										
	DoHint(m_location);
										
	if ( Get_bAutoSave() &&  e_motion )
	{
		char* autoCpDir = "AutoCP";
		char autoCpFileName[_MAX_PATH+1];
		mkdir( autoCpDir );
		sprintf( autoCpFileName, "%s\\Turn_%04d_Loc_%03d", autoCpDir, m_turns, Get_location() );
		SaveAdv(autoCpFileName);
	}

	while (!english());	/* retrieve player instructions	*/
		
#if _DEBUG	
	if (g_debugFlg)
		printf("m_location = %d, e_verb = %d, e_object = %d, \
		motion = %d\n", m_location, e_verb, e_object, e_motion);
#endif
										
	if (e_motion)		/* execute player instructions	*/
		domove();
	else if (e_object)
			doobj();
	else
		itverb();
}


/*
		special time limit stuff...
*/
short CGameState::stimer()
{
	short	msg = MSG4_000_No_Message;
	short	i = 0;
										
	SaveRestoreTime( true );	// save stimer vars for possible restore
										
	m_foobar = m_foobar > 0 ?  -m_foobar : 0;
	if (m_tally1 == 0
		&& m_location >= LOC_015_hall_mists
		&& m_location != LOC_033_Y2
		)
		--m_clock1;
						
	if (m_clock1 == 0)
	{
		/*
				start m_closing the cave
		*/
		m_objectProperty[OBJ_GRATE] = 0;
		m_objectProperty[OBJ_FISSURE] = 0;
		for (i = 1; i<DWARFMAX; ++i)
			m_dwarfSeen[i] = 0;
		move(OBJ_TROLL, LOC_000_Limbo, LOC_000_MOVEABLE);
		move(OBJ_TROLL2, LOC_117_sw_chasm, LOC_122_ne_chasm);
		juggle(OBJ_CHASM);
		if (m_objectProperty[OBJ_BEAR] != 3)
			dstroy(OBJ_BEAR);
		m_objectProperty[OBJ_CHAIN] = 0;
		m_objectLoc2[OBJ_CHAIN] = LOC_000_Limbo;
		m_objectProperty[OBJ_AXE] = 0;
		m_objectLoc2[OBJ_AXE] = LOC_000_Limbo;

		Set_DwarfsEnabled(0);

		SpeakInfoMsg(MSG4_129_Cave_closing_soon_please_exit_now);
		m_clock1 = -1;
		m_closing = 1;
		return(0);
	}
	if (m_clock1 < 0)
		--m_clock2;
	if (m_clock2 == 0) 
	{
		/*
				set up storage room...
				and close the cave...
		*/
		m_objectProperty[OBJ_BOTTLE] = put(OBJ_BOTTLE, LOC_115_ne_end_repository, 1);
		m_objectProperty[OBJ_PLANT] = put(OBJ_PLANT, LOC_115_ne_end_repository, 0);
		m_objectProperty[OBJ_OYSTER] = put(OBJ_OYSTER, LOC_115_ne_end_repository, 0);
		m_objectProperty[OBJ_LAMP] = put(OBJ_LAMP, LOC_115_ne_end_repository, 0);
		m_objectProperty[OBJ_ROD] = put(OBJ_ROD, LOC_115_ne_end_repository, 0);
		m_objectProperty[OBJ_DWARF] = put(OBJ_DWARF, LOC_115_ne_end_repository, 0);

		m_oldloc1 = LOC_116_sw_end_repository;
		m_location = LOC_116_sw_end_repository;
		m_newloc1 = LOC_115_ne_end_repository;

		put(OBJ_GRATE, LOC_116_sw_end_repository, 0);

		m_objectProperty[OBJ_SNAKE] = put(OBJ_SNAKE, LOC_116_sw_end_repository, 1);
		m_objectProperty[OBJ_BIRD] = put(OBJ_BIRD, LOC_116_sw_end_repository, 1);
		m_objectProperty[OBJ_CAGE] = put(OBJ_CAGE, LOC_116_sw_end_repository, 0);
		m_objectProperty[OBJ_ROD2] = put(OBJ_ROD2, LOC_116_sw_end_repository, 0);
		m_objectProperty[OBJ_PILLOW] = put(OBJ_PILLOW, LOC_116_sw_end_repository, 0);
		m_objectProperty[OBJ_MIRROR] = put(OBJ_MIRROR, LOC_115_ne_end_repository, 0);
		m_objectLoc2[OBJ_MIRROR] = LOC_116_sw_end_repository;

		for ( i = 1; i <= LastObjectIndexed(); ++i )
			if (toting(i))
				dstroy(i);

		move(OBJ_KEYS, LOC_143_main_office, LOC_000_MOVEABLE);

		SpeakInfoMsg(MSG4_132_cave_now_closed);

		m_closed = 1;
		return(1);
	}
	if (m_objectProperty[OBJ_LAMP] == 1)
		--m_limit;
	if (m_limit <= 30 &&
		here(OBJ_BATTERIES) && m_objectProperty[OBJ_BATTERIES] == 0 &&
		here(OBJ_LAMP)) 
	{
		SpeakInfoMsg(MSG4_188_lamp_getting_dim_replacing_batteries);
		m_objectProperty[OBJ_BATTERIES] = 1;
		if (toting(OBJ_BATTERIES))
			drop(OBJ_BATTERIES, m_location);
		m_limit += 2500;
		m_lampWarning = 0;
		return(0);
	}
	if (m_limit == 0) 
	{
		--m_limit;
		m_objectProperty[OBJ_LAMP] = 0;
		if (here(OBJ_LAMP))
			SpeakInfoMsg(MSG4_184_lamp_has_run_out_power);
		return(0);
	}
	if (m_limit < 0 && m_location <= LOC_008_outside_grate) 
	{
		SpeakInfoMsg(MSG4_185_no_point_wandering_out_here_So_let_s_call);
		m_gaveup = 1;
		normend();
	}
	if (m_limit <= 30) 
	{
		if (m_lampWarning || !here(OBJ_LAMP))
			return(0);
		m_lampWarning = 1;
		msg = MSG4_187_lamp_getting_dim_go_back_for_batteries;
		if (m_objectLoc1[OBJ_BATTERIES] == LOC_000_Limbo)
			msg = MSG4_183_lamp_getting_dim_best_wrap_up_or_find_batteries;
		if (m_objectProperty[OBJ_BATTERIES] == 1)
			msg = MSG4_189_lamp_getting_dim_no_spare_batteries;
		SpeakInfoMsg(msg);
		return(0);
	}
	return(0);
}
