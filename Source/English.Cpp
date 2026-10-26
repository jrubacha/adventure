//	English.cpp

#include	"Adventure.h"
#include "RandomPercent.h"

/*
		English variables
*/

#define	WORDSIZE	80	/* max # of chars in commands	*/

short	e_verb, e_object, e_motion;
char	e_word1[WORDSIZE], e_word2[WORDSIZE];

short	inputType = 0;		// 0 is keyboard, 1 is file
char	inputFileName[_MAX_PATH+1];
FILE*	inputFp = stdin;	

typedef struct
{
	short	type1;
	short	val1_lo;
	short	val1_hi;
	short*	e_vom1;
	short	type2;
	short	val2_lo;
	short	val2_hi;
	short*	e_vom2;
}
 E_GRAMMER;

E_GRAMMER e_grammer [] = 
{
	
//	special commands
	
	{
		VOCAB_TYP_VRB, VRB_SAY,		  VRB_SAY, &e_verb,
		VOCAB_TYP_ANY, VOCAB_VAL_ANY, VOCAB_VAL_ANY, NULL
	},
//	{	VOCAB_TYP_OTH, OTH_HELP,	  OTH_HELP, &e_verb,
//		VOCAB_TYP_VRB, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_object,
//	},

//	two word commands
	
	{
		VOCAB_TYP_VRB, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_verb,
		VOCAB_TYP_OBJ, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_object
	},
	{
		VOCAB_TYP_VRB, VRB_WALK,      VRB_WALK,      NULL,
		VOCAB_TYP_MOV, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_motion
	},
	{
		VOCAB_TYP_VRB, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_verb,
		VOCAB_TYP_MOV, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_object
	},
	{
		VOCAB_TYP_MOV, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_motion,
		VOCAB_TYP_MOV, VOCAB_VAL_MIN, VOCAB_VAL_MAX, NULL
	},
	{
		VOCAB_TYP_MOV, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_motion,
		VOCAB_TYP_OBJ, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_object
	},
	
	
//	one word commands
	
	{
		VOCAB_TYP_MOV, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_motion,
		VOCAB_TYP_NUL, VOCAB_VAL_MIN, VOCAB_VAL_MAX, NULL
	},
	{
		VOCAB_TYP_VRB, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_verb,
		VOCAB_TYP_NUL, VOCAB_VAL_MIN, VOCAB_VAL_MAX, NULL
	},
	{
		VOCAB_TYP_OBJ, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_object,
		VOCAB_TYP_NUL, VOCAB_VAL_MIN, VOCAB_VAL_MAX, NULL
	},
//	{	VOCAB_TYP_OTH, VOCAB_VAL_MIN, VOCAB_VAL_MAX, &e_verb,
//		VOCAB_TYP_NUL, VOCAB_VAL_MIN, VOCAB_VAL_MAX, NULL},
};

#define NUM_GRAMMER_ENTRIES (sizeof(e_grammer) / sizeof(e_grammer[0]))

/*
		Analyze a two word sentence
		return
		0 is bad grammer
		1 is good grammer
*/
short english203CheckGrammer(short type1, short type2, short val1, short val2, short&	p_verb, short& p_object, short& p_motion)
{
	short	nRc = 0;
										
	/* check grammar */
	
	for ( int ii = 0; ii < NUM_GRAMMER_ENTRIES; ++ii )
	{
		p_verb = 0;
		p_object = 0;
		p_motion = 0;
		if (	e_grammer[ii].type1 == type1
			&&	e_grammer[ii].val1_lo <= val1
			&&	e_grammer[ii].val1_hi >= val1
			)
		{
			if ( e_grammer[ii].e_vom1 != NULL )
				if ( val1 > 0 )
				*e_grammer[ii].e_vom1 =  val1;
			
			if (	e_grammer[ii].type2 == type2
				&&(	e_grammer[ii].type2 ==VOCAB_TYP_NUL
				||
				(	e_grammer[ii].val2_lo <= val2
				&&	e_grammer[ii].val2_hi >= val2
				)
				)
				)
			{
				if ( e_grammer[ii].e_vom2 != NULL )
					if ( val2 > 0 )
					*e_grammer[ii].e_vom2 =  val2;
				
				if ( e_grammer[ii].type2 == type2 )
					return 1;	// OK
			}
			
			if ( e_grammer[ii].type2 ==VOCAB_TYP_ANY )
			{
				if ( e_grammer[ii].e_vom2 != NULL )
					if ( val2 > 0 )
					*e_grammer[ii].e_vom2 =  val2;
				
				return 1;	// OK
			}
		}
	}
										
	return(0);
}


/*
		Analyze a two word sentence
		return
		-1 is unknown word(s)
		0 is bad grammer
		1 is good grammer
*/
short english202ParseWords(char* p_word1, char* p_word2)
{
	short	type1, type2, val1, val2, nRc;
										
	e_verb = e_object = e_motion = 0;
	type2 = val2 = -1;
	type1 = val1 = -1;
										
					
	// check for verb object or verb movement
	{
		nRc = analyze(p_word1, &type1, &val1, false, VOCAB_ENTRY_VRB, VOCAB_ENTRY_VRB + VOCAB_VAL_MAX);
		if ( nRc )	// verb?
		{
			if ( val1 == VRB_WALK )
			{
				// verb/move
				nRc = analyze(p_word2, &type2, &val2, false, VOCAB_ENTRY_MOV, VOCAB_ENTRY_MOV + VOCAB_VAL_MAX);
			}
			else
			{
				// verb/object
				nRc = analyze(p_word2, &type2, &val2, false, VOCAB_ENTRY_OBJ, VOCAB_ENTRY_OBJ + VOCAB_VAL_MAX);
			}
		}
		if ( nRc )
		{
			nRc = english203CheckGrammer( type1, type2, val1, val2, e_verb, e_object, e_motion);
			if ( nRc )
				return nRc;
		}
		
		type2 = val2 = -1;
		type1 = val1 = -1;
	}
	if ( type1 == -1 )
	{
		nRc = analyze(p_word1, &type1, &val1, false);	/* check p_word1	*/
		if ( ! nRc )	// Not a word?
			return(-1);		/* didn't know it	*/
	}
					
	if ( type2 == -1 && *p_word2 )
	{
		nRc = analyze(p_word2, &type2, &val2, false);	// second word can be unknown
		if ( ! nRc )	// Not a word?
			return(-1);		/* didn't know it	*/
	}
					
	nRc = english203CheckGrammer( type1, type2, val1, val2, e_verb, e_object, e_motion);
	if ( nRc )
		return nRc;
	
	return(0);
}

/*
		Analyze a one word sentence
		or a special one or two word command
		return	-3 is XXX
		-2 is unknown words
		-1 is unknown word
		0 is bad grammer
		1 is good grammer
		2 is completed
*/
short english101SpecialCmds(char* p_word1, char* p_word2)
{
	short	type1, type2, val1, val2, nRc;
										
	e_verb = e_object = e_motion = 0;
	type2 = val2 = -1;
	type1 = val1 = -1;
										
	short n1Rc = analyze(p_word1, &type1, &val1, false);
	short n2Rc = analyze(p_word2, &type2, &val2, false);
	
	if ( n1Rc == 0 && n2Rc == 0 )	// Not a word?
		return(-2);		/* didn't know either word	*/
	
	if (type1 == VOCAB_TYP_XXX || type2 == VOCAB_TYP_XXX )
	{
		SpeakInfoMsg(MSG4_079_Watch_it);
		return -3;
	}
					
	if ( n1Rc == 0 )	// Not a word?
		return(-1);		/* didn't know it	*/
	
	if ((type1 == VOCAB_TYP_OTH) && (type2 == VOCAB_TYP_OTH) &&
		(val1 == OTH_HELP) && (val2 == OTH_HELP))
	{
		// help help or ? ?
		outwords();
		return(2);
	}
	
	if ((type1 == VOCAB_TYP_OTH) && (val1 == OTH_HELP))
	{
		// help verb or ? verb
		nRc = analyze(p_word2, &type2, &val2, false, VOCAB_ENTRY_MOV, VOCAB_ENTRY_MOV + VOCAB_VAL_MAX);

		if (type2 == VOCAB_TYP_VRB)
		{
			SpeakHelp( val2 );
			return(2);
		}
		n2Rc = analyze(p_word2, &type2, &val2, false);
	}

	if ( type1 == VOCAB_TYP_OTH )
	{
		if      ( val1 == OTH_TPORT )	// magic Tela-port
		{
			short	loc = atoi( e_word2 );

			if ( !ValidCaveLocation( loc ) )
			{
				ReportUnknowWord();
				return(2);
			}
			gameState.Set_location(loc);
			gameState.Set_newloc1(loc);
			gameState.move ( OBJ_LAMP, LOC_Neg1_PLAYER, LOC_000_MOVEABLE );
			if ( gameState.dark() )
			{
				gameState.von();
			}
			else
			{
				gameState.describe();
				gameState.descitem();
			}
			gameState.inventory();
			gameState.Inc_locVisitedCnt( loc );
			return(2);
		}
		else if ( val1 == OTH_TGRAB )	// magic Tela-Grab
		{
			// object
			analyze(p_word2, &type2, &val2, false, VOCAB_ENTRY_OBJ, VOCAB_ENTRY_OBJ + VOCAB_VAL_MAX);
			if ( type2 == VOCAB_TYP_OBJ )
			{
				e_object = val2;
				gameState.move ( e_object, LOC_Neg1_PLAYER, LOC_000_MOVEABLE );
				gameState.inventory();
			}
			else
				ReportUnknowWord();
			return(2);
		}
		else if ( val1 == OTH_DWSET )	// dwset 0 disables dwarf movements, 1 enables
		{
			short	nVal = atoi( e_word2 );
			if ( nVal == -1 )
			{
				strcpy ( e_word2, "0" ); // for backward compatibility
				nVal = 0;
			}
			if ( strlen(e_word2) == 1 && isdigit(e_word2[0] ) )
				gameState.Set_DwarfsEnabled( nVal );
			else
				ReportUnknowWord();
			return(2);
		}
		else if ( val1 == OTH_PISET )	// dwset 0 disables pirate movements, 1 enables
		{
			short	nVal = atoi( e_word2 );
			if ( strlen(e_word2) == 1 && isdigit(e_word2[0] ) )
				gameState.Set_PirateEnabled( nVal );
			else
				ReportUnknowWord();
			return(2);
		}
		else if ( val1 == OTH_NODIE )	// nodie or nodie 1 disables death, 0 enables death
		{
			short	nVal = atoi( e_word2 );
			if ( nVal == 1 || nVal == 0 )
			{
				if ( strlen( e_word2 ) == 0 )
					nVal = 1;
				gameState.Set_bNoDieTest( nVal );
				gameState.move(OBJ_CHEST, gameState.Get_chestLoc1(), LOC_000_MOVEABLE);
			}
			return(2);
		}
		
	}

	if (type1 == VOCAB_TYP_OTH)
	{
		SpeakInfoMsg(val1);
		return(2);
	}

	if (type2 == VOCAB_TYP_OTH)
	{
		SpeakInfoMsg(val2);
		return(2);
	}
	
	if (type2 == -1 || !(*p_word2))	// Just one reconized word so process default order
	{
		nRc = english203CheckGrammer( type1, type2, val1, val2, e_verb, e_object, e_motion);
		return nRc;
	}
	
	return(0);
}


void	swap_E_Words()
{
	char	word[WORDSIZE];
	strcpy(word, e_word1);
	strcpy(e_word1, e_word2);
	strcpy(e_word2, word);
}

/*
		Analyze a two word sentence
		Return 0 is don't process
		Return 1 is OK to process
*/
short english()
{
										
	char	*msg = "bad grammar...\n";
	short	nRc = 0;
										
	getwords();
										
	if (!(*e_word1))
		return(0);		/* ignore whitespace	*/
	
	for ( int iii = 0; iii < 2; ++iii ) // try word order each way
	{
		nRc = english101SpecialCmds( e_word1, e_word2 );
		
		switch ( nRc )
		{
			case -3:	// XXX
			return	0;
			break;
			case -2:	// unknown words (both)
			ReportUnknowWord();
			return	0;
			break;
			case -1:	// unknown word
			case 0:		// bad grammer
				break;
			case 1:		// good grammer
				return	1;
				break;
			case 2:		// completed
				return	0;
				break;
			default:
				printf("english() bug 101\n");
				bug(BUG_English_Cpp+41);
				return -1;
		}
							
		nRc = english202ParseWords( e_word1, e_word2 );
		
		switch ( nRc )
		{
			case -1:	// unknown word
			case 0:		// bad grammer
				break;
			case 1:		// good grammer
				return	1;
				break;
			default:
				printf("english() bug 202\n");
				bug(BUG_English_Cpp+40);
				return -1;
		}
		
		swap_E_Words();
	}				
	
	swap_E_Words();
	
	if ( nRc == -1 )
		ReportUnknowWord();
	else
		puts(msg);
	
	return(0);
}

/*
		Routine to report an unknown word.
*/
void ReportUnknowWord()
{
	short msg = MSG4_013_don_t_understand_that;
														
	switch(Rrand( 0, 2 )) 
	{
		case 0:
			msg = MSG4_060_don_t_know_word;
			break;
		case 1:
			msg = MSG4_061_What;
			break;
		default:
			msg = MSG4_013_don_t_understand_that;
	}
	SpeakInfoMsg(msg);
}


/*
		Routine to analyze a word.
*/
short analyze(char * word, short * type, short * value, bool bReportError, short lowWordVal, short hiWordVal )
{
	short	wordval = vocab(word, lowWordVal, hiWordVal);
										
	/* make sure I understand */
	if ( wordval == -1 )
	{
		if ( bReportError )
		{
			ReportUnknowWord();
		}
		return(0);
	}
	*type = wordval/VOCAB_TYP_MUL;
	*value = wordval%VOCAB_TYP_MUL;
	return(1);
}

/*
		retrieve input line (max 2*WORDSIZE+1), convert to lower case
		& rescan for first two words (max. WORDSIZE-1 chars).
*/
void getwords()
{
	char words[WORDSIZE*2+2];
	char scanfStr[40];
										
	e_word1[0] = e_word2[0] = '\0';

	GetInputLine(words, sizeof(words));
	sprintf( scanfStr, "%%%ds %%%ds", sizeof(e_word1)-1, sizeof(e_word2)-1 );
	sscanf(words, scanfStr, e_word1, e_word2);

#ifdef _DEBUG
	if (g_debugFlg)
		::printf("e_word1 = %s, e_word2 = %s\n", e_word1, e_word2);
#endif

	return;
}

/*
		retrieve input line and optionaly convert to lower case ( default ).
*/
void GetInputLine(char*	buff, short nMaxLen, bool bTolower)
{
	char  tBuff[1000];
	short pLen = __max( sizeof(tBuff), nMaxLen );
	char* pBuff = sizeof(tBuff) > nMaxLen ? tBuff : buff;
	do
	{
		puts(">");

		fgets( pBuff, pLen-1, inputFp );			// stdin or file

		if ( inputType != 0 )
		{
			if ( ferror(inputFp) || feof(inputFp) )
			{
				SetInputType( "keyboard" );
				buff[0] = '\0';
				puts("\n");
				return;
			}
			else
			{
				fputs( pBuff, stdout );
			}
		}
		appendTextFile(pBuff, 1);
		
	} while ( inputType == 1 && *pBuff != '\n' && !isalpha(*pBuff) ); // don't process comments from input file.

	if ( pBuff != buff )
	{
		strncpy( buff, pBuff, nMaxLen );
		buff[nMaxLen-1] = '\0';
	}

	if ( bTolower )
	{
		char* wptr = buff;
		while (*wptr = tolower(*wptr))
			++wptr;
	}

	return;
}

void SetInputType( char* szTypeName )
{
	strcpy ( inputFileName, szTypeName );

	if ( inputFp != stdin || inputType != 0 )
	{
		if ( inputFp != NULL )
			fclose ( inputFp );

		inputFp = stdin;
		inputType = 0;
	}
	if ( strcmp (inputFileName, "keyboard") != 0 )
	{
		inputFp = fopen( inputFileName, "r" );
		if ( inputFp == NULL )
			inputFp = stdin;
		else
			inputType = 1;
	}
}
