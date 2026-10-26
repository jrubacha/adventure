//	ItVerb.cpp


#include	"Adventure.h"

/*
		Routines to process intransitive verbs
*/
void CGameState::itverb()
{
	bool bVerbProcessed = true;	// assume verb is processed unless default.

	//	These verbs need an valid object specified.
	switch(e_verb) 
	{
		case VRB_MAP:
		case VRB_WAVE:
		case VRB_CALM:
		case VRB_RUB:
		case VRB_THROW:
		case VRB_FIND:
		case VRB_FEED:
		case VRB_BREAK:
		case VRB_WAKE:
			needobj();
			bVerbProcessed = true;
			break;

		default:
			bVerbProcessed = false;
			break;
	}

	if ( bVerbProcessed )
		return;

	bVerbProcessed = true;	// assume verb is processed unless default.

	//	These verbs can take an optional e_word2 ( usually not an object )
	switch(e_verb) 
	{
		case VRB_BLAST:	// just being nice by allowing blast whatever.
			vblast();
			break;

		case VRB_FEE:
		case VRB_FIE:
		case VRB_FOE:
		case VRB_FOO:
		case VRB_FUM:
			ivfoo(e_word1, e_verb);
			break;
		case VRB_HOURS:
			actspk(e_verb);
			break;
		case VRB_SAY:
			vsay();
			break;
		case VRB_WALK:
			actspk(e_verb);
			break;
		case VRB_NOTHING:
			SpeakInfoMsg(MSG4_054_OK);
			break;
		case VRB_BRIEF:
			Toggle_brief();
			break;
		case VRB_AUTOCP:
			ivAutoCheckPoint();
			break;
		case VRB_SUSPEND:
			SaveRestoreTime();
			SaveAdv( e_word2 );
			exitAdv(0);
			break;
		case VRB_CHECKPOINT:
			SaveRestoreTime();
			SaveAdv( e_word2 );
			break;
		case VRB_RESTORE:
			RestoreAdv( e_word2 );
			describe(describeTypeLong);
			descitem();
			break;
		case VRB_CRYPT:
			SaveRestoreTime();
			crypt();
			break;
		case VRB_RESET:
			ResetGameState( 0 );
			break;
		case VRB_LOG:
			ivLog();
			break;
		case VRB_FRIENDLY:
			ivFriendly();
			break;
		case VRB_HINTS:
			ivHints();
			break;
		case VRB_VOCABLENGTH:
			ivVocabLength();
			break;
		case VRB_INPUT:
			SetInputType( e_word2 );
			break;

		default:
			bVerbProcessed = false;
			break;
	}

	if ( bVerbProcessed )
		return;

	bVerbProcessed = true;	// assume verb is processed unless default.

	//	These verbs should have no e_word2 specified.

	if ( strlen( e_word2 ) > 0 )
	{
		printf("Sorry, I don't know how to %s %s?\n", e_word1, e_word2 );
		return;
	}

	switch(e_verb) 
	{
		case VRB_QUIT:
			ivquit();
			break;
		case VRB_SCORE:
			SaveRestoreTime();
			score();
			break;
		case VRB_DESCRIBE:
			ivDescribe();
			break;
		case VRB_INVENTORY:
			SaveRestoreTime();
			inventory();
			break;

		default:
			bVerbProcessed = false;
			break;
	}

	if ( bVerbProcessed )
			return;

	bVerbProcessed = true;	// assume verb is processed unless default.

	//	These verbs should have no e_word2 specified
	//	Unless it was an object but we wouldn't be here if e_word2 was an object.

	//	These verbs will try to aquire an object.

	switch(e_verb) 
	{
		case VRB_ON:
		case VRB_OFF:
		case VRB_POUR:
			trverb();
			break;

		case VRB_TAKE:
			ivtake( e_object );
			break;
		case VRB_DROP:
			ivdrop( e_object );
			break;

		case VRB_OPEN:
		case VRB_LOCK:
			ivopen( e_object );
			break;


		case VRB_KILL:
			ivkill( e_verb, e_object );
			break;
		case VRB_EAT:
			iveat( e_object );
			break;
		case VRB_DRINK:
			ivdrink( e_object );
			break;
		case VRB_READ:
			ivread( e_object );
			break;
		case VRB_FILL:
			ivfill( e_object );
			break;

		default:
			bVerbProcessed = false;
			break;
	}

	if ( bVerbProcessed )
		return;

	printf("This intransitive not implemented yet\n");
}

/*
		Routines to process intransitive verbs
*/



/*
		CARRY, TAKE etc.
*/
void CGameState::ivtake( short& object )
{
	short anobj,item;

	if ( strlen( e_word2 ) > 0 )
	{
		printf("I don't see any %s here.\n", e_word2 );
		return;
	}
										
	anobj = 0;
	for( item = 1; item <= LastObjectIndexed(); ++item) 
	{
		if(m_objectLoc1[item]==m_location) 
		{
			if(anobj != 0) 
			{
				needobj();
				return;
			}
			anobj = item;
		}
	}
	if( anobj==0 || (dwarfCheck() && m_dwarfFlag>=2) ) 
	{
		needobj();
		return;
	}
	object = anobj;
	vtake();
}

/*
		DROP etc.
*/
void CGameState::ivdrop( short& object )
{
	short anobj,item;

	if ( strlen( e_word2 ) > 0 )
	{
		printf("Sorry, you're not holding any %s\n", e_word2 );
		return;
	}

	if ( m_holding < 1 )
	{
		printf("You have nothing to %s\n", e_word1);
		return;
	}
	if ( m_holding > 1 )
	{
		needobj();
		return;
	}
										
	anobj = 0;
	for( item = 1; item <= m_objectLoc1.MaxIndexed();++item ) 
	{
		if( toting(item) ) 
		{
			anobj = item;
		}
	}
	if( anobj == 0 ) 
	{
		needobj();
		return;
	}
	object = anobj;
	vdrop();
}

/*
		OPEN, LOCK, UNLOCK
*/
void CGameState::ivopen( short& object )
{
	if ( strlen( e_word2 ) > 0 )
	{
		printf("I don't know how to do that to %s\n", e_word2 );
		return;
	}

	if(here(OBJ_CLAM))
		object=OBJ_CLAM;
	if(here(OBJ_OYSTER))
		object=OBJ_OYSTER;
	if(at(OBJ_DOOR))
		object=OBJ_DOOR;
	if(at(OBJ_GRATE))
		object=OBJ_GRATE;
	if(here(OBJ_CHAIN)) 
	{
		if(object != 0) 
		{
			needobj();
			return;
		}
		object=OBJ_CHAIN;
	}
	if(toting(OBJ_MAP))
		object=OBJ_MAP;
	if(object==0) 
	{
		SpeakInfoMsg(MSG4_028_nothing_here_to_lock);
		return;
	}
	vopen();
}

/*
		ATTACK, KILL etc
*/
void CGameState::ivkill( short verb, short& object )
{
	if ( strlen( e_word2 ) > 0 )
	{
		printf("How can I %s %s?\n", e_word1, e_word2 );
		return;
	}
	m_object1 = 0;
	if( dwarfCheck() && m_dwarfFlag >=2 )
		object=OBJ_DWARF;
	if( here(OBJ_SNAKE) )
		addobj(OBJ_SNAKE, object);
	if( at(OBJ_DRAGON) && m_objectProperty[OBJ_DRAGON]==0 )
		addobj(OBJ_DRAGON, object);
	if( at(OBJ_TROLL) )
		addobj(OBJ_TROLL, object);
	if( here(OBJ_BEAR) && m_objectProperty[OBJ_BEAR]==0 )
		addobj(OBJ_BEAR, object);
	if( m_object1 != 0 ) 
	{
		needobj();
		return;
	}
	if(object != OBJ_NONE) 
	{
		vkill();
		return;
	}
	if(here(OBJ_BIRD) && verb!= VRB_THROW)
		object=OBJ_BIRD;
	if(here(OBJ_CLAM) || here(OBJ_OYSTER))
		addobj(OBJ_CLAM, object);
	if(m_object1 != OBJ_NONE) 
	{
		needobj();
		return;
	}
	vkill();
}

/*
		EAT
*/
void CGameState::iveat( short& object )
{
	if ( strlen( e_word2 ) > 0 )
	{
		printf("%s tastes like chicken.\n",  e_word2 );
		return;
	}

	if(!here(OBJ_FOOD))
		needobj();
	else 
	{
		object=OBJ_FOOD;
		veat();
	}
}

/*
		DRINK
*/
void CGameState::ivdrink( short& object )
{
	if ( strlen( e_word2 ) > 0 )
	{
		printf("%s made me sick.\n",  e_word2 );
		return;
	}
	if(liqloc(m_location) != OBJ_WATER &&
		(liq()!= OBJ_WATER || !here(OBJ_BOTTLE)))
		needobj();
	else 
	{
		object=OBJ_WATER;
		vdrink();
	}
}

/*
		QUIT
*/
void CGameState::ivquit()
{
	if(m_gaveup=yes(MSG4_022_really_want_quit_now,MSG4_054_OK,MSG4_054_OK))
		normend();
}

/*
		FILL
*/

void CGameState::ivfill( short& object )
{
	if ( strlen( e_word2 ) > 0 )
	{
		printf("Sorry, I don't know how to %s %s?\n", e_word1, e_word2 );
		return;
	}
	if(!here(OBJ_BOTTLE))
		needobj();
	else 
	{
		object=OBJ_BOTTLE;
		vfill();
	}
}

/*
		Handle fee fie foe foo...
*/
void CGameState::ivfoo( char* e_word1, short verb )
{
	short msg = MSG4_042_Nothing_happens;
	short k = verb - VRB_FEE + 1;
										
	if ( strlen( e_word2 ) > 0 )
	{
		printf("%s %s doesn't work here\n", e_word1, e_word2 );
		return;
	}

	if (m_foobar != 1-k)
	{
		if (m_foobar != 0)
			msg = MSG4_151_can_t_you_read_start_over;
		SpeakInfoMsg(msg);
		return;
	}
	m_foobar = k;
	if (k != 4)
		return;
	m_foobar = 0;
	if (m_objectLoc1[OBJ_EGGS] == LOC_092_giant_room ||
		(toting(OBJ_EGGS)
		&& m_location == LOC_092_giant_room)) 
	{
		SpeakInfoMsg(msg);
		return;
	}
	if (m_objectLoc1[OBJ_EGGS] == LOC_000_Limbo
		&& m_objectLoc1[OBJ_TROLL] == LOC_000_Limbo
		&& m_objectProperty[OBJ_TROLL] == 0)
		m_objectProperty[OBJ_TROLL] = 1;
	if (here(OBJ_EGGS))
		k = 1;
	else if (m_location == LOC_092_giant_room)
			k = 0;
	else
		k = 2;
	move(OBJ_EGGS,LOC_092_giant_room, LOC_000_MOVEABLE);
	SpeakObjDesc(OBJ_EGGS,k);
	return;
}

/*
		read etc...
*/
void CGameState::ivread( short& object )
{
	if ( strlen( e_word2 ) > 0 )
	{
		printf("Sorry, I can't %s %s?\n", e_word1, e_word2 );
		return;
	}
	if (here(OBJ_MAP))
		object = OBJ_MAP;
	if (here(OBJ_MAGAZINE))
		object = OBJ_MAGAZINE;
	if (here(OBJ_TABLET))
		object = object*(LastObjectIndexed()+1) + OBJ_TABLET;
	if (here(OBJ_MESSAGE))
		object = object*(LastObjectIndexed()+1) + OBJ_MESSAGE;
	if (object > LastObjectIndexed() || object == OBJ_NONE || dark()) 
	{
		needobj();
		return;
	}
	vread();
}

/*
		INVENTORY 
*/
void CGameState::inventory()
{
	short msg = MSG4_098_You_re_not_carrying_anything;
											
	for ( short i=1; i <= LastObjectIndexed(); ++i ) 
	{
		if (i==OBJ_BEAR || !toting(i))
			continue;
		if (msg)
			SpeakInfoMsg(MSG4_099_You_are_currently_holding_following);
		msg = MSG4_000_No_Message;
		SpeakObjDesc(i,-1);
	}
	if (toting(OBJ_BEAR))
		msg = MSG4_141_followed_by_tame_bear;
	if (msg)
		SpeakInfoMsg(msg);
}

/*
		VRB_DESCRIBE
*/
void CGameState::ivDescribe()
{
	if (m_detail++<3)
		SpeakInfoMsg(MSG4_015_not_allowed_more_detail_will_repeat_description);
	describe(describeTypeLong);
	descitem();
}

/*
		VRB_LOG
*/
void CGameState::ivLog()
{
	if ( strlen( e_word2 ) == 1 && isdigit(*e_word2) )
	{
		short nLogLevelSpecified = atoi( e_word2 );
		if ( nLogLevelSpecified < 3 )
		{
			Set_LogLevel( nLogLevelSpecified );
			return;
		}
	}
	if ( stricmp( e_word2, "delete") == 0)
	{
		deleteLogFile();
	}
	else if ( strlen( e_word2 ) == 0 )
	{
		Toggle_LogLevel();
	}
	else
	{
		setLogFileName( e_word2 );
	}
	return;
}

/*
		VRB_AUTOCP
*/
void CGameState::ivAutoCheckPoint()
{
	short	nVal = atoi( e_word2 );

	if ( strlen( e_word2 ) == 0 )
		Toggle_bAutoSave();
	else
		Set_bAutoSave( nVal );
}

/*
		VRB_FRIENDLY
*/
void CGameState::ivFriendly()
{
	short	nVal = atoi( e_word2 );

	if ( strlen( e_word2 ) == 0 )
		Toggle_Friendly();
	else
		Set_Friendly( nVal );
}
/*
		VRB_HINTS
*/
void CGameState::ivHints()
{
	short	nVal = atoi( e_word2 );

	if ( strlen( e_word2 ) == 0 )
		Toggle_OfferHints();
	else
		Set_OfferHints( nVal );
}
/*
		VRB_VOCABLENGTH
*/
void CGameState::ivVocabLength()
{
	short nNewLenSpecified = __min( 20, atoi( e_word2 ) );
	if ( nNewLenSpecified > 3 )
		Set_VocabCmpLen( nNewLenSpecified, true );
	else
		Toggle_VocabCmpLen();
}

/*
		ensure uniqueness as objects are searched
		out for an intransitive verb
*/
void CGameState::addobj( short obj,  short& object )
{
	if(m_object1 != OBJ_NONE)
		return;
	if(object != OBJ_NONE) 
	{
		m_object1 = -1;
		return;
	}
	object = obj;
}

