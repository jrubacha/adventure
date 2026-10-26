//	DoTrPrObject.Cpp

#include	"Adventure.h"



/*
		Routine to process an object.
*/
void CGameState::doobj()
{
	/*
			is he trying to take or drop everything (take all or drop all)?
	*/
	if (e_object == OBJ_ALL) 
	{
		if (e_verb == VRB_DROP)
		{
			if ( m_holding < 1 )
			{
				printf("You have nothing to %s\n", e_word1);
				return;
			}
			for ( e_object = 1; e_object <= LastObjectIndexed(); ++e_object )
			{
				if ( toting (e_object) )
				{
					if ( ! ( e_object == OBJ_WATER || e_object == OBJ_OIL || e_object == OBJ_BIRD ) )
					{
						printf("\t%s\t", vocab(VOCAB_ENTRY_OBJ+e_object));
						trobj();
					}
				}
			}
		}
		else if (e_verb == VRB_TAKE)
		{
			int nObjsFound = 0;
			for ( e_object = 1; e_object <= LastObjectIndexed(); ++e_object )
			{
				if ( m_objectLoc1[e_object] == m_location )
				{
					printf("\t%s\t", vocab(VOCAB_ENTRY_OBJ+e_object));
					trobj();
					++nObjsFound;
				}
			}
			if ( nObjsFound == 0 )
				printf("I see nothing here you can carry.\n");
		}
		else
		{
			printf("Sorry but I don't know how.\n");
		}
		return;
	}	

	/*
			is object here?  if so, transitive
	*/
	if (m_objectLoc2[e_object] == m_location || here(e_object))
		trobj();
	/*
			did he give GRATE as destination?
	*/
	else if (e_object == OBJ_GRATE) 
	{
		if (m_location == LOC_001_end_road || m_location == LOC_004_valley || m_location == LOC_007_slit_streambed) 
		{
			e_motion = MOV_DEPRESSION;
			domove();
		}
		else if (m_location>LOC_009_below_grate && m_location<LOC_015_hall_mists) 
		{
			e_motion = MOV_ENTRANCE;
			domove();
		}
	}
	/*
			is he trying to get at dynamite?
	*/
	else if (e_object == OBJ_ROD && here(OBJ_ROD2)) 
	{
		e_object = OBJ_ROD2;
		trobj();
	}
	/*
			is he trying to get/use a liquid?
	*/
	else if ((liq() == e_object && here(OBJ_BOTTLE)) ||
			liqloc(m_location) == e_object)
			trobj();
	else if (e_object == OBJ_PLANT && at(OBJ_PLANT2) &&
			m_objectProperty[OBJ_PLANT2] == 0) 
	{
		e_object = OBJ_PLANT2;
		trobj();
	}
	/*
			is he trying to grab a knife?
	*/
	else if (e_object == OBJ_KNIFE && m_knifeLoc == m_location) 
	{
		SpeakInfoMsg(MSG4_116_Dwarves_knives_vanish_as_they_strike_walls);
		m_knifeLoc = -1;
	}
	/*
			is he trying to get at shadow troll?
	*/
	else if (e_object == OBJ_TROLL && here(OBJ_TROLL2)) 
	{
		e_object = OBJ_TROLL2;
		trobj();
	}
	/*
			is he trying to get at shadow plant?
	*/
	else if (e_object == OBJ_PLANT && here(OBJ_PLANT2)) 
	{
		e_object = OBJ_PLANT2;
		trobj();
	}
	/*
			is it a DWARF he is after?
	*/
	else if (dwarfCheck() && m_dwarfFlag >= 2) 
	{
		e_object = OBJ_DWARF;
		trobj();
	}
	else
	{
		printf("I see no %s here.\n", probj(e_object));
	}
}

/*
		Routine to process an object being
		referred to.
*/
void CGameState::trobj()
{
	if (e_verb)
		trverb();
	else
		printf("What do you want to do with the %s?\n",	probj(e_object));
}

/*
		Routine to print word corresponding to object
*/
char* CGameState::probj(short object)
{
	short	wtype, wval;
	analyze(e_word1, &wtype, &wval, false);
	return (wtype == VOCAB_TYP_OBJ ? e_word1 : e_word2);
}
