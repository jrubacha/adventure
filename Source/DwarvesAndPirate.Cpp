//	DwarvesAndPirate.Cpp

#include	"Adventure.h"


/*
		DWARF stuff.
*/
void CGameState::DoDwarves()
{
	short	i, j, kMsg, tri, attack, stick, dwarfsTotal;

	/*
			see if DoDwarves allowed here
	*/
	if (m_newloc1 == LOC_000_Limbo
		|| forced(m_newloc1)
		|| m_locCondition[m_newloc1]&NOPIRAT
		)
		return;
	/*
			see if DoDwarves are active.
	*/
	if (!m_dwarfFlag)
	{
		if (m_newloc1 > LOC_015_hall_mists)
			++m_dwarfFlag;
		return;
	}
	/*
			if first close encounter (of 3rd kind)
			kill 0, 1 or 2
	*/
	if (m_dwarfFlag == 1) 
	{
		if (m_newloc1 < LOC_015_hall_mists || pct (86, 1)) // aw was 95
			return;

		++m_dwarfFlag;

		for (i = 1; i < 3; ++i)
			if (pct (50, 2))
				m_dwarfLoc1[Rrand(1,5)] = LOC_000_Limbo;

		for (i = 1; i < DWARFLAST; ++i) 
		{
			if (m_dwarfLoc1[i] == m_newloc1)
				m_dwarfLoc1[i] = m_dwarfAltLoc;
			m_dwarfOldLoc[i] = m_dwarfLoc1[i];
		}
		SpeakInfoMsg(MSG4_003_dwarf_throws_axe_at_you);
		drop(OBJ_AXE, m_newloc1);
		return;
	}
	dwarfsTotal = attack = stick = 0;
	for (i = 1; i < DWARFMAX; ++i) 
	{
		if (m_dwarfLoc1[i] == LOC_000_Limbo)
			continue;
		/*
				move a DWARF at random.  we don't
				have a matrix around to do it
				as in the original version...
		*/
		for (tri = 1; tri < 20; ++tri) 
		{
			j = Rrand(LOC_015_hall_mists, LOC_121_secret_canyon); /* allowed area */

			if (   j != m_dwarfOldLoc[i]
				&& j != m_dwarfLoc1[i]
				&& ! (i == (DWARFLAST) && m_locCondition[j]&NOPIRAT == 1)
				)
				break;
		}
		if (j == 0)
			j = m_dwarfOldLoc[i];

		m_dwarfOldLoc[i] = m_dwarfLoc1[i];
		m_dwarfLoc1[i] = j;

		if ( (m_dwarfSeen[i] && m_newloc1 >= LOC_015_hall_mists)
			|| m_dwarfLoc1[i] == m_newloc1
			|| m_dwarfOldLoc[i] == m_newloc1
			)
			m_dwarfSeen[i] = 1;
		else
			m_dwarfSeen[i] = 0;

#if _DEBUG && 0
		for ( short nDwNum = 0; nDwNum <= DWARFLAST; ++nDwNum )
		{
			printf("Loc(%d) dw(%d) Loc(%d) seen(%d)\n", m_newloc1, nDwNum, m_dwarfLoc1[nDwNum], m_dwarfSeen[nDwNum]);
		}
		puts("\n");
#endif		

		if (!m_dwarfSeen[i])
			continue;

		m_dwarfLoc1[i] = m_newloc1;

		if (i == DWARFLAST)
		{
			DoPirate();							// Do Pirate stuff
		}
		else if ( Get_DwarfsEnabled() )
		{
			++dwarfsTotal;
			if (m_dwarfOldLoc[i] == m_dwarfLoc1[i]) 
			{
				short nPct = Get_Friendly() ? 88 : 95;	// added by aw --

				++attack;
				if (m_knifeLoc >= 0)
					m_knifeLoc = m_newloc1;
				if ( Rrand(0,999) < nPct * (m_dwarfFlag-2) )
					++stick;
			}
		}
		else
		{
			if ( m_dwarfFlag > 2 )
				m_dwarfFlag = 2;
			if ( i != DWARFLAST )
			{
				m_dwarfLoc1[i] = LOC_000_Limbo;
			}
		}
	}
	if (dwarfsTotal == 0)
		return;
	if (dwarfsTotal > 1)
		printf("There are %d threatening little dwarves in the room with you!\n", dwarfsTotal);
	else
		SpeakInfoMsg(MSG4_004_threatening_dwarf_in_room_you);
	if (attack == 0)
		return;
	if (m_dwarfFlag == 2)
		++m_dwarfFlag;
	if (attack > 1) 
	{
		printf("%d of them throw knives at you!!\n", attack);
		kMsg = MSG4_006_None_hit_you;
	}
	else 
	{
		SpeakInfoMsg(MSG4_005_One_sharp_nasty_knife_thrown_you);
		kMsg = MSG4_052_It_misses;
	}
	if (stick <= 1) 
	{
		SpeakInfoMsg( kMsg+stick );
		if (stick == 0)
			return;
	}
	else
	{
		printf("%d of them get you !!!\n", stick);
	}
	m_oldloc2 = m_newloc1;
	death();
}


/*
		pirate stuff
*/

bool CGameState::PirateCanStealIt( short nObject, short nLoc )
{
	bool bRc = true;
	if (   nObject == OBJ_PYRAMID && nLoc == m_objectLoc1[OBJ_PYRAMID]
		|| nObject == OBJ_EMERALD && nLoc == m_objectLoc1[OBJ_EMERALD]
	   )
	   return false;

	if ( Get_Friendly() )	// added by aw --
	{
		if ( nObject == OBJ_TRIDENT && m_objectLoc1[OBJ_PEARL] == LOC_000_Limbo )
		   return false;

		if ( nObject == OBJ_EGGS && m_objectLoc1[OBJ_TROLL] != LOC_000_Limbo )
		   return false;
	}

	return bRc;
}

void CGameState::DoPirate()
{
	short	j, k;


	if ( ! Get_PirateEnabled() )
			return;

	/*
			see if pirate allowed here
	*/

	if (m_newloc1 == LOC_000_Limbo
		|| forced(m_newloc1)
		|| m_locCondition[m_newloc1]&NOPIRAT
		)
		return;

	if (   m_newloc1 == m_chestLoc1
		|| m_objectProperty[OBJ_CHEST] >= 0
		)
		return;

	// added by aw --
	if ( Get_Friendly() && Get_objectLoc1(OBJ_CHEST) != LOC_000_Limbo )
	{
		if ( pct(95, 3) )
		{
			if ( pct(20, 4) )
				SpeakInfoMsg(MSG4_127_rustling_noises_behind_you);
			return;
		}
	}

	k = 0;
	for (j = FirstTreasure(); j <= LastTreasure(); ++j)
	{
		if ( PirateCanStealIt( j, m_newloc1 ) )
		{
			if (toting(j))
				goto stealit;
			if (here(j))
				++k;
		}
	}
	if (m_tally1 == m_tally2+1 && k == 0 && m_objectLoc1[OBJ_CHEST] == LOC_000_Limbo &&
		here(OBJ_LAMP) && m_objectProperty[OBJ_LAMP] == 1) 
	{
		SpeakInfoMsg(MSG4_186_rustling_noises_from_darkness);
		move(OBJ_CHEST, m_chestLoc1, LOC_000_MOVEABLE);
		move(OBJ_MESSAGE, m_chestLoc2, LOC_Neg1_FIXED);
		m_dwarfLoc1[DWARFLAST] = m_chestLoc1;
		m_dwarfOldLoc[DWARFLAST] = m_chestLoc1;
		m_dwarfSeen[DWARFLAST] = 0;
		return;
	}
	if (m_dwarfOldLoc[DWARFLAST] != m_dwarfLoc1[DWARFLAST] && pct(20, 1)) // aw, was 20
	{
		SpeakInfoMsg(MSG4_127_rustling_noises_behind_you);
		return;
	}
	return;
										
	stealit:
										
	SpeakInfoMsg(MSG4_128_from_shadows_pirate_takes_all_booty);
	if (m_objectLoc1[OBJ_MESSAGE] == LOC_000_Limbo)
		move(OBJ_CHEST, m_chestLoc1, LOC_000_MOVEABLE);

	move(OBJ_MESSAGE, m_chestLoc2, LOC_Neg1_FIXED);

	for (j = FirstTreasure(); j <= LastTreasure(); ++j) 
	{
		if ( ! PirateCanStealIt( j, m_newloc1 ) )
			continue;

		if (at(j) && m_objectLoc2[j] == LOC_000_MOVEABLE) // not fixed
			carry(j, m_newloc1);
		if (toting(j))
			drop(j, m_chestLoc1);
	}
	m_dwarfLoc1[DWARFLAST] = m_chestLoc1;
	m_dwarfOldLoc[DWARFLAST] = m_chestLoc1;
	m_dwarfSeen[DWARFLAST] = 0;
}



#if 0 // comments from some fortran code.

      SUBROUTINE DWARFS(IRET)
C
      INCLUDE 'ADVCOM.COM'
      INCLUDE 'NEWCOM.COM'
      INCLUDE 'FUNCT.H'
C
C
C
C
C
      IF(NEWLOC.GE.9.OR.NEWLOC.EQ.0.OR..NOT.CLOSNG) GOTO 71
      CALL RSPEAK(130)
      NEWLOC=LOC
      IF(.NOT.PANIC) CLOCK2=15
      PANIC=.TRUE.
      
C
C  SEE IF A DWARF HAS SEEN HIM AND HAS COME FROM WHERE HE WANTS TO GO.  IF SO,
C  THE DWARF'S BLOCKING HIS WAY.  IF COMING FROM PLACE FOBIDDEN TO PIRATE
C  (DWARVES ROOTED IN PLACE) LET HIM GET OUT (AND ATTACKED).

   71 IF(NEWLOC.EQ.LOC.OR.FORCED(LOC).OR.BITSET(LOC,3)) GOTO 74
      DO 73 I=1,5
      IF(ODLOC(I).NE.NEWLOC.OR..NOT.DSEEN(I)) GOTO 73
      NEWLOC=LOC
      CALL RSPEAK(2)
      GOTO 74
   73 CONTINUE
   74 LOC=NEWLOC
   
C  DWARF STUFF.  SEE EARLIER COMMENTS FOR DESCRIPTIONS OF VARIABLES.  REMEMBER
C  SIXTH DWARF IS PIRATE AND IS THUS VERY DIFFERENT EXCEPT FOR MOTION RULES.

C  FIRST OFF, DON'T LET THE DWARVES FOLLOW HIM INTO A PIT OR A WALL.  ACTIVATE
C  THE WHOLE MESS THE FIRST TIME HE GETS AS FAR AS THE HALL OF MISTS (LOC 15).
C  IF NEWLOC IS FORBIDDEN TO PIRATE (IN PARTICULAR, IF IT'S BEYOND THE TROLL
C  BRIDGE), BYPASS DWARF STUFF.  THAT WAY PIRATE CAN'T STEAL RETURN TOLL, AND
C  DWARVES CAN'T MEET THE BEAR.  ALSO MEANS DWARVES WON'T FOLLOW HIM INTO DEAD
C  END IN MAZE, BUT C'EST LA VIE. THEY'LL WAIT FOR HIM OUTSIDE THE DEAD END.

      IF(LOC.EQ.0.OR.FORCED(LOC).OR.BITSET(NEWLOC,3)) GOTO 2000
      IF(DFLAG.NE.0) GOTO 6000
      IF(LOC.GE.15) DFLAG=1
      GOTO 2000
      
C  WHEN WE ENCOUNTER THE FIRST DWARF, WE KILL 0, 1, OR 2 OF THE 5 DWARVES.  IF
C  ANY OF THE SURVIVORS IS AT LOC, REPLACE HIM AT THE ALTERNATIVE LOCATION.

 6000 IF(DFLAG.NE.1) GOTO 6010
      IF(LOC.LT.15.OR.PCT(95)) GOTO 2000
      DFLAG=2
      DO 6001 I=1,2
      J=1+RAN(5)
C  IF SAVED NOT = -1, HE BYPASSED THE "START" CALL.
 6001 IF(PCT(50).AND.SAVED.EQ.-1) DLOC(J)=0
      DO 6002 I=1,5
      IF(DLOC(I).EQ.LOC) DLOC(I)=DALTLC
 6002 ODLOC(I)=DLOC(I)
      CALL RSPEAK(3)
      CALL DROP(AXE,LOC)
      GOTO 2000
      
C  THINGS ARE IN FULL SWING.  MOVE EACH DWARF AT RANDOM, EXCEPT IF HE'S SEEN US
C  HE STICKS WITH US.  DWARVES NEVER GO TO LOCS <15.  IF WANDERING AT RANDOM,
C  THEY DON'T BACK UP UNLESS THERE'S NO ALTERNATIVE.  IF THEY DON'T HAVE TO
C  MOVE, THEY ATTACK.  AND, OF COURSE, DEAD DWARVES DON'T DO MUCH OF ANYTHING.

 6010 DTOTAL=0
      ATTACK=0
      STICK=0
      DO 6030 I=1,6
      IF(DLOC(I).EQ.0) GOTO 6030
      J=1
      KK=DLOC(I)
      KK=KEY(KK)
      IF(KK.EQ.0) GOTO 6016
 6012 NEWLOC=MOD(IABS(TRAVEL(KK))/1000,1000)
      IF(NEWLOC.GT.300.OR.NEWLOC.LT.15.OR.NEWLOC.EQ.ODLOC(I)
     1   .OR.(J.GT.1.AND.NEWLOC.EQ.TK(J-1)).OR.J.GE.20
     2   .OR.NEWLOC.EQ.DLOC(I).OR.FORCED(NEWLOC)
     3   .OR.(I.EQ.6.AND.BITSET(NEWLOC,3))
     4   .OR.IABS(TRAVEL(KK))/1000000.EQ.100) GOTO 6014
      TK(J)=NEWLOC
      J=J+1
 6014 KK=KK+1
      IF(TRAVEL(KK-1).GE.0) GOTO 6012
 6016 TK(J)=ODLOC(I)
      IF(J.GE.2) J=J-1
      J=1+RAN(J)
      ODLOC(I)=DLOC(I)
      DLOC(I)=TK(J)
      DSEEN(I)=(DSEEN(I).AND.LOC.GE.15)
     1         .OR.(DLOC(I).EQ.LOC.OR.ODLOC(I).EQ.LOC)
      IF(.NOT.DSEEN(I)) GOTO 6030
      DLOC(I)=LOC
      IF(I.NE.6) GOTO 6027
      
C  THE PIRATE'S SPOTTED HIM.  HE LEAVES HIM ALONE ONCE WE'VE FOUND CHEST.
C  K COUNTS IF A TREASURE IS HERE.  IF NOT, AND TALLY=TALLY2 PLUS ONE FOR
C  AN UNSEEN CHEST, LET THE PIRATE BE SPOTTED.

      IF(LOC.EQ.CHLOC.OR.PROP(CHEST).GE.0) GOTO 6030
      K=0
      DO 6020 J=50,MAXTRS
C  PIRATE WON'T TAKE PYRAMID FROM PLOVER ROOM OR DARK ROOM (TOO EASY!).
      IF(J.EQ.PYRAM.AND.(LOC.EQ.PLAC(PYRAM)
     1   .OR.LOC.EQ.PLAC(EMRALD))) GOTO 6020
      IF(TOTING(J)) GOTO 6022
 6020 IF(HERE(J)) K=1
      IF(TALLY.EQ.TALLY2+1.AND.K.EQ.0.AND.PLACE(CHEST).EQ.0
     1   .AND.HERE(LAMP).AND.PROP(LAMP).EQ.1) GOTO 6025
      IF(ODLOC(6).NE.DLOC(6).AND.PCT(20)) CALL RSPEAK(127)
      GOTO 6030
      
 6022 CALL RSPEAK(128)
 
C  DON'T STEAL CHEST BACK FROM TROLL!
      IF(PLACE(MESSAG).EQ.0) CALL MOVE(CHEST,CHLOC)
      CALL MOVE(MESSAG,CHLOC2)
      DO 6023 J=50,MAXTRS
      IF(J.EQ.PYRAM.AND.(LOC.EQ.PLAC(PYRAM)
     1   .OR.LOC.EQ.PLAC(EMRALD))) GOTO 6023
      IF(AT(J).AND.FIXED(J).EQ.0) CALL CARRY(J,LOC)
      IF(TOTING(J)) CALL DROP(J,CHLOC)
 6023 CONTINUE
 6024 DLOC(6)=CHLOC
      ODLOC(6)=CHLOC
      DSEEN(6)=.FALSE.
      GOTO 6030
      
 6025 CALL RSPEAK(186)
      CALL MOVE(CHEST,CHLOC)
      CALL MOVE(MESSAG,CHLOC2)
      GOTO 6024
      
C  THIS THREATENING LITTLE DWARF IS IN THE ROOM WITH HIM!

 6027 DTOTAL=DTOTAL+1
      IF(ODLOC(I).NE.DLOC(I)) GOTO 6030
      ATTACK=ATTACK+1
      IF(KNFLOC.GE.0) KNFLOC=LOC
      IF(RAN(1000).LT.95*(DFLAG-2)) STICK=STICK+1
 6030 CONTINUE

C  NOW WE KNOW WHAT'S HAPPENING.  LET'S TELL THE POOR SUCKER ABOUT IT.

      IF(DTOTAL.EQ.0) GOTO 2000
      IF(DTOTAL.EQ.1) GOTO 75
      WRITE(*,67) DTOTAL
   67 FORMAT(/' THERE ARE 'I1,' THREATENING LITTLE DWARVES IN THE',
     1        ' ROOM WITH YOU.')
      GOTO 77
   75 CALL RSPEAK(4)
   77 IF(ATTACK.EQ.0) GOTO 2000
      IF(DFLAG.EQ.2) DFLAG=3
C  IF SAVED NOT =-1, HE BYPASSED THE "START" CALL.  DWARVES GET *VERY* MAD!
      IF(SAVED.NE.-1) DFLAG=20
      IF(ATTACK.EQ.1) GOTO 79
      WRITE(*,78) ATTACK
   78 FORMAT(/' ',I1,' OF THEM THROW KNIVES AT YOU!')
      K=6
   82 IF(STICK.GT.1) GOTO 83
      CALL RSPEAK(K+STICK)
      IF(STICK.EQ.0) GOTO 2000
      GOTO 84
   83 WRITE(*,68) STICK
   68 FORMAT(/' ',I1,' OF THEM GET YOU!')
   84 OLDLC2=LOC
      GOTO 99
      
   79 CALL RSPEAK(5)
      K=52
      GOTO 82
      
C
C  SET UP RETURN PARAMETER.
   99 CONTINUE
      IRET=2
      RETURN
C
 2000 CONTINUE
      IRET=1
      RETURN
C
      END

#endif
