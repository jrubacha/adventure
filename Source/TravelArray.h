// TravelArray.h: interface for the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#ifndef TravelArray_h_aw_2007_01_28
#define TravelArray_h_aw_2007_01_28

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LongIntArray.h"

class CTravelArray
{
public:

	CTravelArray( short nNumToAllocate = 18 );
	virtual ~CTravelArray();
	bool	Valid() { return m_longArray.Valid(); }

	long&	operator[]( short nMoveNdx ) { return m_longArray[nMoveNdx]; };
	CTravelArray& operator = ( const CTravelArray& c );

	bool	operator()( short nMoveNdx, short& nVerb, short& nDest, short& nCond, short& nObjOrPct );

	short	Verb(short nMoveNdx);		// returns verb
	short	Dest( short nMoveNdx );		// returns destination

	short	Tcond( short nMoveNdx );	// returns long / 1000
	short	Cond( short nMoveNdx );		// returns Tcond / 100
	short	ObjOrPct( short nMoveNdx );	// returns Tcond % 100

	short	NumAllocated() { return m_longArray.NumAllocated(); }
	short	NumToGrow() { return m_longArray.NumToGrow(); }
	short	MaxIndexed() { return m_longArray.MaxIndexed(); }

	void	ResetMemory() {m_longArray.ResetMemory();}

private:
	CLongIntArray m_longArray;

private:
	bool Calculate( short nMoveNdx );
	long	m_lEntry;
	short	m_nDest;
	short	m_nVerb;
	short	m_nCond;
	short	m_nObjOrPct;
	short	m_nTcond;
};

#endif // TravelArray_h_aw_2007_01_28
