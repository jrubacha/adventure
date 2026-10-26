// CaveArray.cpp: implementation of the CCaveArray class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif 

#include <stdlib.h>
#include "CaveArray.h"

//In every CPP file, include these statements immediately after the last #include statement (before the first malloc/new statement):

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCaveArray::CCaveArray( short nNumToAllocate, short nNumToGrow )
{
	m_ppArray = NULL;
	m_nNumAllocated = 0;
	m_nNumToGrow = 0;
	m_nMaxIndexed = 0;

	Grow( nNumToAllocate, nNumToGrow );
}


CCaveArray::~CCaveArray()
{
	if ( m_ppArray != NULL )
	{
		for ( short nNdx = 0; nNdx < m_nNumAllocated; ++ nNdx )
		{
			CTravelArray*	pArray = m_ppArray[nNdx];
			if ( pArray != NULL )
			{
				delete pArray;
				m_ppArray[nNdx] = NULL;
			}
		}
		free(m_ppArray);
		m_ppArray = NULL;
	}
}

long& CCaveArray::operator()(short nLocNdx, short nMoveNdx)
{
	if ( nLocNdx >= m_nNumAllocated )
	{
		Grow( nLocNdx );
	}
	if ( nLocNdx < m_nNumAllocated )
	{
		CTravelArray*	pArray = m_ppArray[nLocNdx];
		m_nMaxIndexed = __max( nLocNdx, m_nMaxIndexed );
		return (*pArray)[nMoveNdx];
	}
	else
	{
		throw 0L;
	}
}

CTravelArray&	CCaveArray::operator[]( short nLocNdx )
{
	if ( nLocNdx >= m_nNumAllocated )
	{
		Grow( nLocNdx );
	}
	if ( nLocNdx < m_nNumAllocated )
	{
		m_nMaxIndexed = __max( nLocNdx, m_nMaxIndexed );
		return (*m_ppArray[nLocNdx]);
	}
	else
	{
		throw 0L;
	}
}

void CCaveArray::Grow( short nNdx, short nNumToGrow )
{
	if ( nNdx >= m_nNumAllocated )
	{
		m_nNumToGrow = nNumToGrow;

		short nNewCount = nNdx + m_nNumToGrow;

		if ( m_nNumAllocated == 0 )
			nNewCount = nNdx;

		m_ppArray = (CTravelArray**)realloc( m_ppArray, sizeof(m_ppArray[0]) * nNewCount );
		for ( short ii = m_nNumAllocated; ii < nNewCount; ++ii )
		{
			CTravelArray* ppArray = new CTravelArray(4);
			m_ppArray[ii] = ppArray;
		}

		if ( m_ppArray == NULL )
		{
			m_nNumAllocated = 0;
			throw;
		}
		short numGrown = nNewCount - m_nNumAllocated;
		m_nNumAllocated = nNewCount;
	}
}


void CCaveArray::ResetMemory()
{
	for ( short nNdx = 0; nNdx <= m_nMaxIndexed; ++nNdx )
	{
		CTravelArray*	pArray = 	m_ppArray[nNdx];
		pArray->ResetMemory();
	}
}