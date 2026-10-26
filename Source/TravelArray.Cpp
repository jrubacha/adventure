// TravelArray.cpp: implementation of the CTravelArray class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "TravelArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTravelArray::CTravelArray( short nNumToAllocate ) : m_longArray(nNumToAllocate, nNumToAllocate)
{
	m_lEntry	= 0;
	m_nDest		= 0;
	m_nVerb		= 0;
	m_nCond		= 0;
	m_nObjOrPct	= 0;
	m_nTcond	= 0;
}


CTravelArray::~CTravelArray()
{
}

CTravelArray& CTravelArray::operator = (const CTravelArray& c)
{
	if ( this != &c )
	{
		m_longArray = c.m_longArray;
	}
	return *this;
}

bool	CTravelArray::operator()( short nMoveNdx, short& nVerb, short& nDest, short& nCond, short& nObjOrPct )
{
	nDest		= Dest(nMoveNdx);
	nVerb		= Verb(nMoveNdx);
	nCond		= Cond(nMoveNdx);
	nObjOrPct	= ObjOrPct(nMoveNdx);
//	nTcond		= Tcond(nMoveNdx);

	return nVerb > 0;
}


bool	CTravelArray::Calculate( short nMoveNdx )
{
	if ( m_lEntry != m_longArray[nMoveNdx] )
	{
		m_lEntry = m_longArray[nMoveNdx];

		long t = m_lEntry / 1000;

		m_nVerb		= (t % 1000);
		t /= 1000;
		m_nDest		= (t % 1000);

		m_nTcond	= m_lEntry %1000;
		m_nCond		= m_nTcond / 100;
		m_nObjOrPct = m_nTcond % 100;
	}

	return m_lEntry > 0;
}

short	CTravelArray::Verb(short nMoveNdx)		// returns verb
{
	Calculate(nMoveNdx);
	return m_nVerb;
}

short	CTravelArray::Dest(short nMoveNdx)		// returns destination
{
	Calculate(nMoveNdx);
	return m_nDest;
}

short	CTravelArray::Cond(short nMoveNdx)		// returns Rcond / 100
{
	Calculate(nMoveNdx);
	return m_nCond;
}

short	CTravelArray::ObjOrPct(short nMoveNdx)	// returns Rcond % 100
{
	Calculate(nMoveNdx);
	return m_nObjOrPct;
}

short	CTravelArray::Tcond(short nMoveNdx)		// returns long / 1000
{
	Calculate(nMoveNdx);
	return m_nTcond;
}

