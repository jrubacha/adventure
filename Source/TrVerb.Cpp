//	TrVerb.cpp


#include	"Adventure.h"

short	actmsg[] = 
{		/* action messages	*/
	/* VRB_NULL   0 */	MSG4_000_No_Message,
	/* VRB_TAKE   1 */	MSG4_024_already_carrying_it,
	/* VRB_DROP   2 */	MSG4_029_not_carrying_it,
	/* VRB_SAY    3 */	MSG4_061_What,
	/* VRB_OPEN   4 */	MSG4_033_don_t_know_how_lock_or_unlock_such_thing,
	/* VRB_NOTHING 5 */	MSG4_042_Nothing_happens,
	/* VRB_LOCK   6 */	MSG4_033_don_t_know_how_lock_or_unlock_such_thing,
	/* VRB_ON     7 */	MSG4_038_no_source_light,
	/* VRB_OFF    8 */	MSG4_038_no_source_light,
	/* VRB_WAVE   9 */	MSG4_042_Nothing_happens,
	/* VRB_CALM  10 */	MSG4_014_Would_you_care_explain_how,
	/* VRB_WALK  11 */	MSG4_043_Where,
	/* VRB_KILL  12 */	MSG4_110_don_t_be_ridiculous,
	/* VRB_POUR  13 */	MSG4_029_not_carrying_it,
	/* VRB_EAT   14 */	MSG4_110_don_t_be_ridiculous,
	/* VRB_DRINK 15 */	MSG4_073_You_drink_from_stream_water,
	/* VRB_RUB   16 */	MSG4_075_Rubbing_electric_lamp,
	/* VRB_THROW 17 */	MSG4_029_not_carrying_it,
	/* VRB_QUIT  18 */	MSG4_013_don_t_understand_that,
	/* VRB_FIND  19 */	MSG4_059_only_tell_you_what_you_see,
	/* VRB_INV   20 */	MSG4_059_only_tell_you_what_you_see,
	/* VRB_FEED  21 */	MSG4_174_nothing_here_eat,
	/* VRB_FILL  22 */	MSG4_109_can_t_fill_that,
	/* VRB_BLAST 23 */	MSG4_067_Blasting_requires_dynamite,
	/* VRB_SCORE 24 */	MSG4_013_don_t_understand_that,
	/* VRB_FRIENDLY 25 */	MSG4_147_I_don_t_know_how,
	/* VRB_BRIEF 26 */	MSG4_155_On_what,
	/* VRB_READ  27 */	MSG4_195_don_t_understand,
	/* VRB_BREAK 28 */	MSG4_146_beyond_power_do,
	/* VRB_WAKE  29 */	MSG4_110_don_t_be_ridiculous,
	/* VRB_SUSPEND 30 */	MSG4_013_don_t_understand_that,
	/* VRB_HOURS 31 */	MSG4_228_CaveOpen_24_7_ForNow,			// end of orignal table
	/* VRB_LOG   32 */	MSG4_155_On_what,
	/* VRB_LOOK  33 */	MSG4_059_only_tell_you_what_you_see,
	/* RESERVED  34 */	MSG4_000_No_Message,
	/* RESERVED  35  */	MSG4_000_No_Message,
	/* RESERVED  36 */	MSG4_000_No_Message,
	/* VRB_WATER 37 */	MSG4_074_bottle_now_empty,
	/* VRB_OIL   38 */	MSG4_074_bottle_now_empty,
	/* VRB_MAP   39 */	MSG4_000_No_Message,
	/* VRB_FEE   40 */	MSG4_042_Nothing_happens,
	/* VRB_FIE   41 */	MSG4_042_Nothing_happens,
	/* VRB_FOE   42 */	MSG4_042_Nothing_happens,
	/* VRB_FOO   43 */	MSG4_042_Nothing_happens,
	/* VRB_FUM   44 */	MSG4_000_No_Message,
	/* RESERVED  45 */	MSG4_000_No_Message,
	/* RESERVED  46 */	MSG4_000_No_Message,
	/* RESERVED  47 */	MSG4_000_No_Message,
	/* RESERVED  48 */	MSG4_000_No_Message,
	/* RESERVED  49 */	MSG4_000_No_Message,
	/* VRB_SAVE  50 */	MSG4_000_No_Message,
	/* VRB_RESTORE 51 */	MSG4_000_No_Message,
	/* VRB_RESET 52 */	MSG4_000_No_Message,
	/* VRB_CRYPT 53 */	MSG4_000_No_Message,
	/* VRB_INPUT 54 */	MSG4_000_No_Message,
	/* VRB_VOCABLENGTH 55 */	MSG4_000_No_Message,
	/* VRB_HINTS 56 */	MSG4_000_No_Message,
	/* VRB_AUTOCP 57 */	MSG4_000_No_Message,
	/* RESERVED  58 */	MSG4_000_No_Message,
};

#define NUM_ACTMSG (sizeof(actmsg) / sizeof(actmsg[0]))

/*
		Routine to process a transitive verb
*/
void CGameState::trverb()
{
	switch(e_verb)
	{
		case VRB_FEE:
		case VRB_FIE:
		case VRB_FOE:
		case VRB_FOO:
		case VRB_FUM:

		case VRB_AUTOCP:
		case VRB_SUSPEND:
		case VRB_CHECKPOINT:
		case VRB_RESTORE:
		case VRB_INPUT:

		case VRB_CRYPT:
			itverb();
			break;

		case VRB_CALM:
		case VRB_WALK:
		case VRB_QUIT:
		case VRB_SCORE:
		case VRB_BRIEF:
		case VRB_HOURS:
		case VRB_RESET:
		case VRB_LOG:
		case VRB_FRIENDLY:
		case VRB_HINTS:
		case VRB_VOCABLENGTH:
			actspk(e_verb);
			break;

		case VRB_TAKE:
			vtake();
			break;
		case VRB_DROP:
			vdrop();
			break;
		case VRB_OPEN:
		case VRB_LOCK:
			vopen();
			break;
		case VRB_SAY:
			vsay();
			break;
		case VRB_NOTHING:
			SpeakInfoMsg(MSG4_054_OK);
			break;
		case VRB_ON:
			von();
			break;
		case VRB_OFF:
			voff();
			break;
		case VRB_WAVE:
			vwave();
			break;
		case VRB_KILL:
			vkill();
			break;
		case VRB_WATER:
			e_verb	= VRB_POUR;
			e_object = OBJ_WATER;
			vpour();
			break;
		case VRB_OIL:
			e_verb	= VRB_POUR;
			e_object = OBJ_OIL;
		case VRB_POUR:
			vpour();
			break;
		case VRB_EAT:
			veat();
			break;
		case VRB_DRINK:
			vdrink();
			break;
		case VRB_RUB:
			if (e_object != OBJ_LAMP)
				SpeakInfoMsg(MSG4_076_Nothing_happens);
			else
				actspk(VRB_RUB);
			break;
		case VRB_THROW:
			vthrow();
			break;
		case VRB_FEED:
			vfeed();
			break;
		case VRB_FIND:
		case VRB_INVENTORY:
			vfind();
			break;
		case VRB_FILL:
			vfill();
			break;
		case VRB_READ:
			vread();
			break;
		case VRB_BLAST:
			vblast();
			break;
		case VRB_BREAK:
			vbreak();
			break;
		case VRB_WAKE:
			vwake();
			break;
		case VRB_MAP:
			cavemap();
			break;
		default:
			printf("This verb is not implemented yet.\n");
	}
}

/*
		Routine to process a transitive verb
*/
/*
		CARRY TAKE etc.
*/
void CGameState::vtake()
{
	short msg = MSG4_025_You_can_t_be_serious;
										
	if (toting(e_object)) 
	{
		actspk(e_verb);
		return;
	}
	/*
			special case objects and m_objectLoc2 objects
	*/
	if (e_object == OBJ_PLANT && m_objectProperty[OBJ_PLANT] <= 0)
		msg = MSG4_115_plant_deep_roots_cannot_be_pulled_free;
	if (e_object == OBJ_BEAR && m_objectProperty[OBJ_BEAR] == 1)
		msg = MSG4_169_bear_still_chained_wall;
	if (e_object == OBJ_CHAIN && m_objectProperty[OBJ_BEAR] != 0)
		msg = MSG4_170_chain_still_locked;
	if (m_objectLoc2[e_object]) 
	{
		SpeakInfoMsg(msg);
		return;
	}
	/*
			special case for liquids
	*/
	if (e_object == OBJ_WATER || e_object == OBJ_OIL) 
	{
		if (!here(OBJ_BOTTLE) || liq() != e_object) 
		{
			e_object = OBJ_BOTTLE;
			if (toting(OBJ_BOTTLE)&&m_objectProperty[OBJ_BOTTLE] == 1)	// empty?
			{
				vfill();
				return;
			}
			if (m_objectProperty[OBJ_BOTTLE] != 1)			//	Not empty?
				msg = MSG4_105_bottle_already_full;
			if (!toting(OBJ_BOTTLE))
				msg = MSG4_104_have_nothing_carry_it;
			SpeakInfoMsg(msg);
			return;
		}
		e_object = OBJ_BOTTLE;
	}
	if (m_holding > m_holdMax)
	{
		SpeakInfoMsg(MSG4_092_can_t_carry_anything_more_have_drop_something_first);
		return;
	}
	/*
			special case for OBJ_BIRD.
	*/
	if (e_object == OBJ_BIRD && m_objectProperty[OBJ_BIRD] == 0) 
	{
		if (toting(OBJ_ROD)) 
		{
			SpeakInfoMsg(MSG4_026_bird_was_unafraid_when_you_entered);
			return;
		}
		if (!toting(OBJ_CAGE)) 
		{
			SpeakInfoMsg(MSG4_027_can_catch_bird_but_not_carry_it);
			return;
		}
		m_objectProperty[OBJ_BIRD] = 1;
		m_hintAvail &= ~HINTB;
	}
	if ( (e_object == OBJ_BIRD
		|| e_object == OBJ_CAGE) &&
		m_objectProperty[OBJ_BIRD] != 0)
	{
		carry((OBJ_BIRD+OBJ_CAGE)-e_object,m_location);
		carry(e_object,m_location);
		SpeakInfoMsg(MSG4_054_OK);
		return;
	}
	/*
			handle liquid in OBJ_BOTTLE
	*/
	if (e_object == OBJ_BOTTLE && liq() != 0)
	{
		carry((OBJ_BOTTLE+liq())-e_object,m_location);
		carry(e_object,m_location);
//		m_objectLoc1[liq()] = LOC_Neg1_PLAYER;
		SpeakInfoMsg(MSG4_054_OK);
		return;
	}
	carry(e_object,m_location);
	SpeakInfoMsg(MSG4_054_OK);
}

/*
		DROP etc.
*/
void CGameState::vdrop()
{
	short msg = MSG4_054_OK;
										
	/*
			check for dynamite
	*/
	if (toting(OBJ_ROD2) && e_object == OBJ_ROD && !toting(OBJ_ROD))
		e_object = OBJ_ROD2;
	if (!toting(e_object))
	{
		actspk(e_verb);
		return;
	}
	/*
			SNAKE and BIRD
	*/
	if (e_object == OBJ_BIRD && here(OBJ_SNAKE)) 
	{
		SpeakInfoMsg(MSG4_030_bird_attacks_green_snake_drives_away);
		msg = MSG4_000_No_Message;
		if (m_closed)
			dwarfend();
		dstroy(OBJ_SNAKE);
		m_objectProperty[OBJ_SNAKE] = -1;
		m_hintAvail &= ~HINTS;
	}
	/*
			COINS and vending machine
	*/
	else if (e_object == OBJ_COINS && here(OBJ_VENDING)) 
	{
		dstroy(OBJ_COINS);
		drop(OBJ_BATTERIES,m_location);
		SpeakObjDesc(OBJ_BATTERIES,0);
		return;
	}
	/*
			OBJ_BIRD and OBJ_DRAGON (ouch!!)
	*/
	else if (e_object == OBJ_BIRD && at(OBJ_DRAGON) && m_objectProperty[OBJ_DRAGON] == 0)
	{
		SpeakInfoMsg(MSG4_154_bird_attacks_dragon_gets_burnt);
		msg = MSG4_000_No_Message;
		dstroy(OBJ_BIRD);
		m_objectProperty[OBJ_BIRD] = 0;
		if (m_objectLoc1[OBJ_SNAKE] != LOC_000_Limbo)
			++m_tally2;
		return;
	}
	/*
			OBJ_BEAR and OBJ_TROLL
	*/
	if (e_object == OBJ_BEAR && at(OBJ_TROLL)) 
	{
		SpeakInfoMsg(MSG4_163_bear_lumbers_toward_troll_who_shriek);
		move(OBJ_TROLL,LOC_000_Limbo, LOC_000_MOVEABLE);
		move(OBJ_TROLL2,LOC_117_sw_chasm, LOC_122_ne_chasm);
		juggle(OBJ_CHASM);
		m_objectProperty[OBJ_TROLL] = 2;
	}
	/*
			OBJ_VASE
	*/
	else if (e_object == OBJ_VASE) 
	{
		if (m_location == LOC_096_soft_room)
			SpeakInfoMsg(MSG4_054_OK);
		else 
		{
			m_objectProperty[OBJ_VASE] = at(OBJ_PILLOW) ? 0 : 2;
			SpeakObjDesc(OBJ_VASE,m_objectProperty[OBJ_VASE]+1);
			if (m_objectProperty[OBJ_VASE] != 0)
				m_objectLoc2[OBJ_VASE] = LOC_Neg1_FIXED;
		}
		msg = MSG4_000_No_Message;
	}
	/*
			handle liquid and OBJ_BOTTLE
	*/
	if (e_object == liq())
		e_object = OBJ_BOTTLE;
	if (e_object == OBJ_BOTTLE && liq() != OBJ_NOTHING)
		m_objectLoc1[liq()] = LOC_000_Limbo;
	/*
			handle OBJ_BIRD and OBJ_CAGE
	*/
	if (e_object == OBJ_CAGE && m_objectProperty[OBJ_BIRD] != 0)
		drop(OBJ_BIRD,m_location);
	if (e_object == OBJ_BIRD)
		m_objectProperty[OBJ_BIRD] = 0;

	drop(e_object,m_location);

	if ( msg )
		SpeakInfoMsg (msg);
}

/*
		LOCK, UNLOCK, OPEN, CLOSE etc.
*/

void CGameState::vopen()
{
	short msg = MSG4_000_No_Message;
	short oyclam;
										
	switch(e_object) 
	{
		case OBJ_MAP:
			if ( ! m_bMapOpen )
			{
				SpeakInfoMsg(MSG4_217_Map_out_of_date);
				m_bMapOpen = yes(MSG4_216_Cost_15_points_open_map, MSG4_200_Is_this_acceptable, MSG4_218_Proably_good_choice);
			}
			if ( m_bMapOpen )
			{
				++m_hintTaken;
				DisplayThisLocationMap( m_location );
			}
			break;
		case OBJ_CLAM:
		case OBJ_OYSTER:
			oyclam = (e_object == OBJ_OYSTER ? 1 : 0);
			if (e_verb == VRB_LOCK)
				msg = MSG4_061_What;
			else if (!toting(OBJ_TRIDENT))
					msg = MSG4_122_don_t_have_anything_to_open_clam+oyclam;
			else if (toting(e_object))
					msg = MSG4_120_advise_put_down_clam_before_opening_it_STRAIN+oyclam;
			else 
			{
				msg = MSG4_124_pearl_falls_out_of_clam+oyclam;
				dstroy(OBJ_CLAM);
				drop(OBJ_OYSTER,m_location);
				drop(OBJ_PEARL,LOC_105_cul_de_sac);
			}
			break;
		case OBJ_DOOR:
			msg = (m_objectProperty[OBJ_DOOR] == 1 ? MSG4_054_OK : MSG4_111_rusty_door_wont_open);
			break;
		case OBJ_CAGE:
			msg = MSG4_032_no_lock;
			break;
		case OBJ_KEYS:
			msg = MSG4_055_can_t_unlock_keys;
			break;
		case OBJ_CHAIN:
			if (!here(OBJ_KEYS))
				msg = MSG4_031_no_keys;
			else if (e_verb == VRB_LOCK) 
			{
				if (m_objectProperty[OBJ_CHAIN] != 0)
					msg = MSG4_034_already_locked;
				else if (m_location != LOC_130_barren_room)
						msg = MSG4_173_nothing_here_chain_can_be_locked;
				else 
				{
					m_objectProperty[OBJ_CHAIN] = 2;
					if (toting(OBJ_CHAIN))
						drop(OBJ_CHAIN,m_location);
					m_objectLoc2[OBJ_CHAIN] = LOC_Neg1_FIXED;
					msg = MSG4_172_chain_now_locked;
				}
			}
			else 
			{
				if (m_objectProperty[OBJ_BEAR] == 0) // aw 400 ????
					msg = MSG4_041_no_way_past_bear_unlock_chain;
				else if (m_objectProperty[OBJ_CHAIN] == 0)
						msg = MSG4_037_already_unlocked;
				else 
				{
					m_objectProperty[OBJ_CHAIN] = 0;
					m_objectLoc2[OBJ_CHAIN] = LOC_000_Limbo;
					if (m_objectProperty[OBJ_BEAR] != 3)
						m_objectProperty[OBJ_BEAR] = 2;
					m_objectLoc2[OBJ_BEAR] = 2-m_objectProperty[OBJ_BEAR]; 	// ?? when bear prob is 2,free the bear
					msg = MSG4_171_chain_now_unlocked;
				}
			}
			break;
		case OBJ_GRATE:
			if (!here(OBJ_KEYS))
				msg = MSG4_031_no_keys;
			else if (m_closing) 
			{
				if (!m_panic) 
				{
					m_clock2 = 15;
					++m_panic;
				}
				msg = MSG4_130_exit_closed_leave_via_main_office;
			}
			else 
			{
				msg = MSG4_034_already_locked+m_objectProperty[OBJ_GRATE];
				m_objectProperty[OBJ_GRATE] = (e_verb == VRB_LOCK ? 0 : 1);
				msg += 2*m_objectProperty[OBJ_GRATE];
			}
			break;
		default:
			msg = MSG4_033_don_t_know_how_lock_or_unlock_such_thing;
	}
	if ( msg )
		SpeakInfoMsg(msg);
}

/*
		SAY etc.
*/
void CGameState::vsay()
{
	short wtype,wval;
										
	analyze(e_word1,&wtype,&wval, false);
	printf("Okay.\n%s\n",wval == VRB_SAY ? e_word2 : e_word1);
}

/*
		ON etc.
*/
void CGameState::von()
{
	if ( ! ValidCaveLocation( m_location ) )
	{
		printf("von bad m_location(%d)\n", m_location);
		bug(BUG_TrVerb_Cpp+21);
	}
	if (!here(OBJ_LAMP))
		actspk(e_verb);
	else if (m_limit<0)
			SpeakInfoMsg(MSG4_184_lamp_has_run_out_power);
	else 
	{
		m_objectProperty[OBJ_LAMP] = 1;
		SpeakInfoMsg(MSG4_039_lamp_now_on);
		if (m_wasDark) 
		{
			m_wasDark = 0;
			describe(describeTypeLong);
			++m_locVisitedCnt[m_location];
			descitem();
		}
	}
}

/*
		OFF etc.
*/
void CGameState::voff()
{
	if (!here(OBJ_LAMP))
		actspk(e_verb);
	else 
	{
		m_objectProperty[OBJ_LAMP] = 0;
		SpeakInfoMsg(MSG4_040_lamp_now_off);
	}
}

/*
		WAVE etc.
*/
void CGameState::vwave()
{
	if (!toting(e_object) &&
		(e_object != OBJ_ROD || !toting(OBJ_ROD2)))
		SpeakInfoMsg(MSG4_029_not_carrying_it);
	else if (e_object != OBJ_ROD || !at(OBJ_FISSURE) ||
			!toting(e_object) || m_closing)
			actspk(e_verb);
	else 
	{
		m_objectProperty[OBJ_FISSURE] = 1-m_objectProperty[OBJ_FISSURE];
		SpeakObjDesc(OBJ_FISSURE,2-m_objectProperty[OBJ_FISSURE]);
	}
}

/*
		ATTACK, KILL etc.
*/
void CGameState::vkill()
{
	short msg = MSG4_000_No_Message;
	short i;
										
	switch(e_object) 
	{
		case OBJ_BIRD:
			if (m_closed)
				msg = MSG4_137_leave_poor_bird_alone;
			else 
			{
				dstroy(OBJ_BIRD);
				m_objectProperty[OBJ_BIRD] = 0;
				if (m_objectLoc1[OBJ_SNAKE] == LOC_019_hall_mt_king)
					++m_tally2;
				msg = MSG4_045_bird_now_dead_Its_body_disappears;
			}
			break;
		case 0:
			msg = MSG4_044_There_nothing_here_attack;
			break;
		case OBJ_CLAM:
		case OBJ_OYSTER:
			msg = MSG4_150_shell_very_strong_impervious;
			break;
		case OBJ_SNAKE:
			msg = MSG4_046_Attacking_snake_doesn_t_work_very_dangerous;
			break;
		case OBJ_DWARF:
			if (m_closed)
				dwarfend();
			msg = MSG4_049_what_with_bare_hands;
			break;
		case OBJ_TROLL:
			msg = MSG4_157_Trolls_skin_as_tough_as_rhinoceros;
			break;
		case OBJ_BEAR:
			msg = MSG4_165_what_your_bare_hands_Agains_HIS_bear_hands+(m_objectProperty[OBJ_BEAR]+1)/2;
			break;
		case OBJ_DRAGON:
			if (m_objectProperty[OBJ_DRAGON] != 0)
			{
				msg = MSG4_167_For_crying_out_loud_poor_thing_already_dead;
				break;
			}
			if (!yes(MSG4_049_what_with_bare_hands,0,0))
				break;
			SpeakObjDesc(OBJ_DRAGON,1);
			m_objectProperty[OBJ_DRAGON] = 2;
			m_objectProperty[OBJ_RUG] = 0;
			move(OBJ_DRAGON,LOC_120_secret_canyon, LOC_Neg1_FIXED);
			move(OBJ_RUG,LOC_120_secret_canyon, LOC_000_MOVEABLE);
			for( i = 1; i <= LastObjectIndexed(); ++i )
				if (   m_objectLoc1[i] == LOC_119_secret_canyon
					|| m_objectLoc1[i] == LOC_121_secret_canyon)
					move(i,LOC_120_secret_canyon, LOC_Neg2_NO_CHANGE);
			m_newloc1 = LOC_120_secret_canyon;
			return;
		default:
			actspk(e_verb);
			return;
	}
	if ( msg )
		SpeakInfoMsg(msg);
}

/*
		POUR
*/
void CGameState::vpour()
{
	if (	e_object == OBJ_BOTTLE
		||	e_object == OBJ_NONE
		)
		e_object = liq();
	if (e_object == OBJ_NONE) 
	{
		needobj();
		return;
	}
	if (!toting(e_object)) 
	{
		actspk(e_verb);
		return;
	}
	if (e_object != OBJ_OIL && e_object != OBJ_WATER) 
	{
		SpeakInfoMsg(MSG4_078_You_can_t_pour_that);
		return;
	}
	m_objectProperty[OBJ_BOTTLE] = 1;		// bottle_prop_empty
	m_objectLoc1[e_object] = LOC_000_Limbo;
	if (at(OBJ_PLANT)) 
	{
		if (e_object != OBJ_WATER)
			SpeakInfoMsg(MSG4_112_plant_shakes_oil_off_wants_Water);
		else 
		{
			SpeakObjDesc(OBJ_PLANT,m_objectProperty[OBJ_PLANT]+1);
			m_objectProperty[OBJ_PLANT] = (m_objectProperty[OBJ_PLANT]+2)%6;
			m_objectProperty[OBJ_PLANT2] = m_objectProperty[OBJ_PLANT]/2;
			describe();
			// descitem(); //aw ???
		}
	}
	else if (at(OBJ_DOOR)) 
	{
		m_objectProperty[OBJ_DOOR] = (e_object == OBJ_OIL ? 1 : 0);
		SpeakInfoMsg(MSG4_113_hinges_are_rusted_now_won_t_budge+m_objectProperty[OBJ_DOOR]);
	}
	else
		SpeakInfoMsg(MSG4_077_bottle_empty_ground_wet);
}

/*
		EAT
*/
void CGameState::veat()
{
	short msg;
										
	switch(e_object) 
	{
		case OBJ_FOOD:
			dstroy(OBJ_FOOD);
			msg = MSG4_072_Thank_you_delicious;
			break;
		case OBJ_BIRD: case OBJ_SNAKE: case OBJ_CLAM: case OBJ_OYSTER:
		case OBJ_DWARF: case OBJ_DRAGON: case OBJ_TROLL: case OBJ_BEAR:
			msg = MSG4_071_lost_my_appetite;
			break;
		default:
			actspk(e_verb);
			return;
	}
	SpeakInfoMsg(msg);
}

/*
		DRINK
*/
void CGameState::vdrink()
{
	if (e_object != OBJ_WATER)
		SpeakInfoMsg(MSG4_110_don_t_be_ridiculous);
	else if (liq() != OBJ_WATER || !here(OBJ_BOTTLE))
			actspk(e_verb);
	else 
	{
		m_objectProperty[OBJ_BOTTLE] = 1;		// prop_empty
		m_objectLoc1[OBJ_WATER] = LOC_000_Limbo;
		SpeakInfoMsg(MSG4_074_bottle_now_empty);
	}
}

/*
		THROW etc.
*/
void CGameState::vthrow()
{
	short msg = MSG4_000_No_Message;
	short i;
										
	if (toting(OBJ_ROD2) && e_object == OBJ_ROD && !toting(OBJ_ROD))
		e_object = OBJ_ROD2;

	if (!toting(e_object)) 
	{
		actspk(e_verb);
		return;
	}
	/*
			treasure to TROLL
	*/
	if (at(OBJ_TROLL) && e_object >= OBJ_NUGGET && e_object<=LastObjectIndexed()) 
	{
		SpeakInfoMsg(MSG4_159_troll_catches_treasure_scurries_away);
		drop(e_object,LOC_000_Limbo);
		move(OBJ_TROLL,LOC_000_Limbo, LOC_000_MOVEABLE);
		move(OBJ_TROLL2,LOC_117_sw_chasm, LOC_122_ne_chasm);
		juggle(OBJ_CHASM);
		return;
	}
	/*
			feed the bears...
	*/
	if (e_object == OBJ_FOOD && here(OBJ_BEAR)) 
	{
		e_object = OBJ_BEAR;
		vfeed();
		return;
	}
	/*
			if not OBJ_AXE, same as drop...
	*/
	if (e_object != OBJ_AXE) 
	{
		vdrop();
		return;
	}


	/*
			********************   AXE is THROWN ***********************
	*/

	e_word1[0] = '\0';	// clear axe because we are going to replace verb
	e_word2[0] = '\0';	// clear axe because we are going to replace object

	/*
			at a OBJ_DWARF...
	*/
	if (i = dwarfCheck()) 
	{
		short nPct = Get_Friendly() ? 44 : 33;	// added by aw --
		msg = MSG4_048_You_attack_dwarf_he_dodges_out_way;
		if ( pct(nPct, 11) ) 
		{
			m_dwarfSeen[i] = m_dwarfLoc1[i] = 0;
			msg = MSG4_047_You_killed_dwarf;
			++m_dwarfsKilled;
			if (m_dwarfsKilled == 1)
				msg = MSG4_149_killed_Dwarf_vanished_smoke;
		}
	}
	/*
			at a OBJ_DRAGON...
	*/
	else if (at(OBJ_DRAGON) && m_objectProperty[OBJ_DRAGON] == 0) 
			msg = MSG4_152_axe_bounces_off_dragon_scales;
	/*
			at the OBJ_TROLL...
	*/
	else if (at(OBJ_TROLL))
			msg = MSG4_158_troll_catches_axe_tosses_it_back;
	/*
			at the OBJ_BEAR...
	*/
	else if (here(OBJ_BEAR) && m_objectProperty[OBJ_BEAR] == 0) 
	{
		SpeakInfoMsg(MSG4_164_axe_misses_lands_near_bear_where_you_can_t_get_it);
		drop(OBJ_AXE,m_location);
		m_objectLoc2[OBJ_AXE] = LOC_Neg1_FIXED;
		m_objectProperty[OBJ_AXE] = 1;
		juggle(OBJ_BEAR);
		return;
	}
	/*
			otherwise it is an attack
	*/
	else 
	{
		e_verb = VRB_KILL;
		e_object = OBJ_NONE;
		itverb();
//		return;
	}
	/*
			handle the left over OBJ_AXE...
	*/
	SpeakInfoMsg(msg);
	drop(OBJ_AXE,m_location);
	describe();
//	descitem(); // aw ???
}

/*
		INVENTORY, FIND etc.
*/
void CGameState::vfind()
{
	short msg;
	if (toting(e_object))
		msg = MSG4_024_already_carrying_it;
	else if (m_closed)
			msg = MSG4_138_whatever_you_want_is_here_somewhere;
	else if (dwarfCheck() && m_dwarfFlag >= 2 && e_object == OBJ_DWARF)
			msg = MSG4_094_what_you_want_right_here;
	else if (at(e_object) ||
			(liq() == e_object && here(OBJ_BOTTLE)) ||
			e_object == liqloc(m_location))
			msg = MSG4_094_what_you_want_right_here;
	else 
	{
		actspk(e_verb);
		return;
	}
	SpeakInfoMsg(msg);
}

/*
		FILL
*/
void CGameState::vfill()
{
	short msg;
	short i;
										
	if ( ! ValidCaveLocation( m_location ) )
	{
		printf("vfill() bad m_location(%d)\n", m_location);
		bug(BUG_TrVerb_Cpp+61);
	}

	switch(e_object) 
	{
		case OBJ_BOTTLE:
			if (liq() != 0)
				msg = MSG4_105_bottle_already_full;
			else if (liqloc(m_location) == 0)
					msg = MSG4_106_nothing_here_fill_bottle;
			else 
			{
				m_objectProperty[OBJ_BOTTLE] = m_locCondition[m_location] & OIL;
				i = liq();
				if (toting(OBJ_BOTTLE))
					m_objectLoc1[i] = LOC_Neg1_PLAYER;
				msg = (i == OBJ_OIL ? MSG4_108_bottle_now_full_oil : MSG4_107_bottle_now_full_water);
			}
			break;
		case OBJ_VASE:
			if (liqloc(m_location) == 0) 
			{
				msg = MSG4_144_nothing_here_to_fill_vase;
				break;
			}
			if (!toting(OBJ_VASE)) 
			{
				msg = MSG4_029_not_carrying_it;
				break;
			}
			SpeakInfoMsg(MSG4_145_temperature_change_shattered_vase);
			vdrop();
			return;
		default:
			msg = MSG4_029_not_carrying_it;
	}
	SpeakInfoMsg(msg);
}

/*
		FEED
*/
void CGameState::vfeed()
{
	short msg;
										
	switch(e_object) 
	{
		case OBJ_BIRD:
			msg = MSG4_100_It_s_not_hungry_and_You_have_no_bird_seed;
			break;
		case OBJ_DWARF:
			if (!here(OBJ_FOOD)) 
			{
				actspk(e_verb);
				return;
			}
			++m_dwarfFlag;
			msg = MSG4_103_fool_Dwarves_eat_only_coal;
			break;
		case OBJ_BEAR:
			if (!here(OBJ_FOOD)) 
			{
				if (m_objectProperty[OBJ_BEAR] == 0)
					msg = MSG4_102_nothing_here_it_wants_eat_Except_you;
				else if (m_objectProperty[OBJ_BEAR] == 3)
						msg = MSG4_110_don_t_be_ridiculous;
				else 
				{
					actspk(e_verb);
					return;
				}
				break;
			}
			dstroy(OBJ_FOOD);
			m_objectProperty[OBJ_BEAR] = 1;

			m_objectLoc2[OBJ_AXE] = LOC_000_MOVEABLE;
			m_objectProperty[OBJ_AXE] = 0;

			msg = MSG4_168_bear_wolfs_down_food_becomes_friendly;
			break;
		case OBJ_DRAGON:
			msg = (m_objectProperty[OBJ_DRAGON] != 0 ? MSG4_110_don_t_be_ridiculous : MSG4_102_nothing_here_it_wants_eat_Except_you);
			break;
		case OBJ_TROLL:
			msg = MSG4_182_Gluttony_not_Troll_vice;
			break;
		case OBJ_SNAKE:
			if (m_closed || !here(OBJ_BIRD)) 
			{
				msg = MSG4_102_nothing_here_it_wants_eat_Except_you;
				break;
			}
			msg = MSG4_101_snake_has_now_devoured_bird;
			dstroy(OBJ_BIRD);
			m_objectProperty[OBJ_BIRD] = 0;
			++m_tally2;
			break;
		default:
			msg = MSG4_014_Would_you_care_explain_how;
	}
	SpeakInfoMsg(msg);
}

/*
		READ etc.
*/
void CGameState::vread()
{
	short msg = MSG4_000_No_Message;
										
	if (dark()) 
	{
		printf("I see no %s here.\n",probj(e_object));
		return;
	}
	switch(e_object) 
	{
		case OBJ_MAP:
			msg = MSG4_215_Handle_with_care;
			break;
		case OBJ_MAGAZINE:
			msg = MSG4_190_magazine_in_Dwarvish;
			break;
		case OBJ_TABLET:
			msg = MSG4_196_Congratulations_bringing_light_into_dark_room;
			break;
		case OBJ_MESSAGE:
			msg = MSG4_191_not_maze_where_pirate_treasure_chest;
			break;
		case OBJ_OYSTER:
			if (!toting(OBJ_OYSTER) || !m_closed)
				break;
			yes(MSG4_192_looks_like_clue_cost_you_10_points,MSG4_193_says_something_strange_about_this_place,MSG4_054_OK);
			return;
		default:
			;
	}
	if (msg)
		SpeakInfoMsg(msg);
	else
		actspk(e_verb);
}

/*
		BLAST etc.
*/
void CGameState::vblast()
{
	if (m_objectProperty[OBJ_ROD2] < 0 || !m_closed)
		actspk(e_verb);
	else 
	{
		m_bonus = MSG4_133_loud_explosion_hole_You_can_exit;
		if (m_location == LOC_115_ne_end_repository)
			m_bonus = MSG4_134_loud_explosion_burying_everything;
		if (here(OBJ_ROD2))
			m_bonus = MSG4_135_loud_explosion_you_are_splashed_across_walls;
		SpeakInfoMsg(m_bonus);
		normend();
	}
}

/*
		BREAK etc.
*/
void CGameState::vbreak()
{
	short msg;
	if (e_object == OBJ_MIRROR) 
	{
		msg = MSG4_148_too_far_up_to_reach;
		if (m_closed) 
		{
			SpeakInfoMsg(MSG4_197_You_strike_mirror_shatters);
			dwarfend();
		}
	}
	else if (e_object == OBJ_VASE && m_objectProperty[OBJ_VASE] == 0) 
	{
		msg = MSG4_198_vase_hurled_ground;
		if (toting(OBJ_VASE))
			drop(OBJ_VASE,m_location);
		m_objectProperty[OBJ_VASE] = 2;
		m_objectLoc2[OBJ_VASE] = LOC_Neg1_FIXED;
	}
	else 
	{
		actspk(e_verb);
		return;
	}
	SpeakInfoMsg(msg);
}

/*
		WAKE etc.
*/
void CGameState::vwake()
{
	if (e_object != OBJ_DWARF || !m_closed)
		actspk(e_verb);
	else 
	{
		SpeakInfoMsg(MSG4_199_woke_nearest_Dwarf);
		dwarfend();
	}
}

/*
		Routine to speak default verb message
*/
void CGameState::actspk(short pverb)
{
	short msg;
										
	if (pverb < 1 || pverb >= NUM_ACTMSG)
	{
		printf("actspk() bad m_location(%d)\n", m_location );
		bug(BUG_TrVerb_Cpp+81);
		return;
	}
	msg = actmsg[pverb];
	if (msg)
		SpeakInfoMsg(msg);
}

/*
		Routine to indicate no reasonable
		e_object for verb found.  Used mostly by
		intransitive verbs.
*/
void CGameState::needobj()
{
	short wtype,wval;
										
	analyze(e_word1,&wtype,&wval, true);
	printf("%s what?\n",wtype == VOCAB_TYP_VRB ? e_word1 : e_word2);
}



