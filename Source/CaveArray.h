// CaveArray.h: interface for the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CaveArray_h_aw_2007_01_28
#define CaveArray_h_aw_2007_01_28

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TravelArray.h"

class CCaveArray
{
public:

	CCaveArray( short nNumLocToAllocate = 150, short nNumLocToGrow = 10 );
	virtual ~CCaveArray();

	bool	Valid() { return (m_ppArray != NULL && m_nNumAllocated > 0 && m_nNumToGrow > 0); }

	long&	operator()(short nLocNdx, short nMoveNdx);

	CTravelArray&	operator[]( short nLocNdx );

	short	NumLocAllocated()	{ return m_nNumAllocated; }
	short	NumLocToGrow()		{ return m_nNumToGrow; }
	short	MaxLocIndexed()		{ return m_nMaxIndexed; }

	void	ResetMemory();

private:

	void	Grow( short nNdx, short nNumToGrow = 10 );

private:
	CTravelArray**	m_ppArray;
	short	m_nNumAllocated;
	short	m_nNumToGrow;
	short	m_nMaxIndexed;
};

#endif // CaveArray_h_aw_2007_01_28
