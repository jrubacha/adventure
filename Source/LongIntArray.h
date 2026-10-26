// LongIntArray.h: interface for the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#ifndef LongIntArray_h_aw_2007_01_23
#define LongIntArray_h_aw_2007_01_23

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <STDIO.H>
#include <memory.h>

class CLongIntArray
{
public:

	CLongIntArray( int nNumToAllocate = 100, int nNumToGrow = 10 );
	virtual ~CLongIntArray();
	bool Valid() { return (m_pArray != NULL && m_nNumAllocated > 0 && m_nNumToGrow > 0); }

	void ResetMemory() { if (m_pArray) memset(m_pArray, 0, sizeof(long) * m_nNumAllocated); m_nMaxIndexed = 0; }

	long& operator[](int nNdx);
//	const long operator[](int nNdx) const ;

	CLongIntArray& operator = (const CLongIntArray& c);

	short	NumAllocated() { return m_nNumAllocated; }
	short	NumToGrow() { return m_nNumToGrow; }
	short	MaxIndexed() { return m_nMaxIndexed; }

	bool Save(FILE* fd);
	bool Restore(FILE* fd);

private:

	void	Grow( short nNdx, short nNumToGrow = 10 );

private:

	long*	m_pArray;
	short	m_nNumAllocated;
	short	m_nNumToGrow;
	short	m_nMaxIndexed;

};

#endif // LongIntArray_h_aw_2007_01_23
