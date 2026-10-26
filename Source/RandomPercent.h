// RandomPercent.h: interface for the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#ifndef RandomPercent_h_aw_2007_01_23
#define RandomPercent_h_aw_2007_01_23

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <STDIO.H>
#include "ShortIntArray.h"

#define NUM_ITRS	20
#define NUM_VALS	100

class CRandomPercent
{
public:

	CRandomPercent( unsigned int seed = 511 );
	virtual ~CRandomPercent();

	void	Srand( unsigned int seed ) { srand(seed); };

	short	nPct( short& nSeqNum);
	short	nPct( int nSeqCntr = 0 );
	bool	bIsPct( short nLimit, short& nSeqNum );
	bool	bIsPct( short nLimit,  int nSeqCntr = 0 );

	void	ResetSequenceCounters() { m_nNext.ResetMemory(); }

	void	Dump();

private:

	bool	bIsNumInTable( short ii, short jj, short rNum );
	void	Init();

private:

short	m_RandNumArray[NUM_ITRS][NUM_VALS];
CShortIntArray	m_nNext;

};


/*
	Routine true x% of the time.
*/
bool pct(short x, int nCounterNumber );

/*
	Routine to reset and seed the RandomPercent class.
*/
void RandomPercentReset( unsigned int seed = 511 );


/*
		Routine to supply a random number in a selected range
*/
int Rrand(int low, int high);

#endif // RandomPercent_h_aw_2007_01_23
