// ShortIntArray.h: interface for the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ShortIntArray_h_aw_2007_01_23
#define ShortIntArray_h_aw_2007_01_23

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <STDIO.H>

class CShortIntArray
{
public:

	CShortIntArray( short nNumToAllocate = 100, short nNumToGrow = 10 );
	virtual ~CShortIntArray();
	bool Valid() { return (m_pArray != NULL && m_nNumAllocated > 0 && m_nNumToGrow > 0); }

	void ResetMemory() { if (m_pArray) memset(m_pArray, 0, sizeof(short) * m_nNumAllocated); }

	short& operator[](short nNdx);
//	const short operator[](int nNdx) const ;

	CShortIntArray& operator = (const CShortIntArray& c);

	short	NumAllocated() { return m_nNumAllocated; }
	short	NumToGrow() { return m_nNumToGrow; }
	short	MaxIndexed() { return m_nMaxIndexed; }

	bool Save(FILE* fd);
	bool Restore(FILE* fd);

private:

	void	Grow( short nNdx, short nNumToGrow = 10 );

private:

	short*	m_pArray;
	short	m_nNumAllocated;
	short	m_nNumToGrow;
	short	m_nMaxIndexed;

};

#endif // ShortIntArray_h_aw_2007_01_23
