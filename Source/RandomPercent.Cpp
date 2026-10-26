// RandomRandomPercent.cpp: implementation of the CRandomPercent class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <memory.h>
#include "RandomPercent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRandomPercent::CRandomPercent( unsigned int seed ) : m_nNext(16,4)
{
	Srand( seed );
	Init();
#if _DEBUG && 0
	Dump();
#endif
}


CRandomPercent::~CRandomPercent()
{
}


short	CRandomPercent::nPct( short& nSeqNum )
{
	if ( nSeqNum < 0 )
		nSeqNum = 0;

	short	ii = (nSeqNum / NUM_VALS) % NUM_ITRS;
	short	jj = nSeqNum % NUM_VALS;
	short	rNum = m_RandNumArray[ii][jj];

	++nSeqNum;

	return rNum;
}

short	CRandomPercent::nPct( int nSeqCntr )
{
	short rNum = nPct( m_nNext[nSeqCntr] );

	return rNum;
}


bool	CRandomPercent::bIsPct( short nLimit, short& nSeqNum )
{
	bool bRc ;
	short rNum = nPct(nSeqNum);
	bRc = rNum <= nLimit;
	return bRc;
}

bool	CRandomPercent::bIsPct( short nLimit,  int nSeqCntr  )
{
	bool bRc ;
	short rNum = nPct( m_nNext[nSeqCntr] );
	bRc = rNum <= nLimit;
	return bRc;
}



bool	CRandomPercent::bIsNumInTable( short ii, short jj, short rNum )
{
	bool	bRc = false;
	for ( short xx = 0; xx < jj; ++xx )
		if ( m_RandNumArray[ii][xx] == rNum )
			return true;
	return bRc;
}

		// Both options work well
#if 0	// This option has a less even spread of numbers
void	CRandomPercent::Init()
{
	for ( short ii = 0; ii < NUM_ITRS; ++ii )
	{
		for ( short jj = 0; jj < NUM_VALS; ++jj )
		{
			short rNum = 0;
			do
			{
				rNum = Rrand(1, 100);
			}	while ( bIsNumInTable( ii, jj, rNum ) );
			m_RandNumArray[ii][jj] = rNum;
		}
	}
}
#else	// This option has a more even spread of numbers
void	CRandomPercent::Init()
{
	for ( short nSeqNum = 0; nSeqNum < NUM_ITRS*NUM_VALS; ++nSeqNum )
	{
		short nMdx = nSeqNum%10; // 0-9,0-9,0-9
		short nRdx = (nSeqNum/10)%10; // 0000000000,1111111111,2222222222,
		short rNum = (nMdx*10)+(nRdx)+1; // 01,11,21,31,41,51,61,71,81,91, 02,12,22,32

		short	ii = (nSeqNum / NUM_VALS) % NUM_ITRS;
		short	jj = nSeqNum % NUM_VALS;


		m_RandNumArray[ii][jj] = rNum;
	}
}
#endif

void	CRandomPercent::Dump()
{
	FILE* fd = NULL;
	fd = fopen( "RandomPercentDump.txt", "w" );
	if ( fd == NULL )
		return;

	short	nSeqNum = 0;
	do
	{
		short rNum = nPct( nSeqNum );
		fprintf(fd, "%003d\t%003d\n", nSeqNum, rNum );
	} while ( nSeqNum < NUM_ITRS*NUM_VALS );

	fclose( fd );
}


// ********************************************************************************** //

/*
		Routine true x% of the time.
*/

static CRandomPercent cRndPct(511);

bool pct(short x, int nCounterNumber )
{
	// Both options work well
#if 0	// this option gives each usage it's own string of numbers.
		// plays more uniform.
	return cRndPct.bIsPct( x, nCounterNumber );
#else	// this option shares usage of one string of numbers.
		// playes more random.
	return cRndPct.bIsPct( x, 0 );
#endif
}

/*
	Routine to reset and seed the RandomPercent class.
*/
void RandomPercentReset( unsigned int seed )
{
	cRndPct.Srand( seed );
	cRndPct.ResetSequenceCounters();
}


/*
		Routine to supply a random number in a selected range
*/
int Rrand(int low, int high)
{
	unsigned range = (unsigned) high - (unsigned) low + 1;
	unsigned partition = ((unsigned) RAND_MAX + 1) / range;
	int      discard_limit = partition * range - 1;
	int      x;
	int      y = -1;
										
	do
	{
		if ((x = rand()) <= discard_limit)
			y = low + x / partition;
	} while (y == -1);
	return y;
}
