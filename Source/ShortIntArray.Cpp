// ShortIntArray.cpp: implementation of the CShortIntArray class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <memory.h>
#include "ShortIntArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CShortIntArray::CShortIntArray( short nNumToAllocate, short nNumToGrow )
{
	m_pArray = NULL;
	m_nNumAllocated = 0;
	m_nNumToGrow = 0;
	m_nMaxIndexed = 0;

	Grow( nNumToAllocate, nNumToGrow );
}


CShortIntArray::~CShortIntArray()
{
	if ( m_pArray != NULL )
		free( m_pArray );
	m_pArray = NULL;
}

short& CShortIntArray::operator[](short nNdx)
{
	if ( nNdx >= m_nNumAllocated )
	{
		Grow( nNdx );
	}
	if ( nNdx < m_nNumAllocated )
	{
		m_nMaxIndexed = __max( nNdx, m_nMaxIndexed );
		return m_pArray[nNdx];
	}
	else
	{
		throw 0L;
	}
}

#if 0
const short CShortIntArray::operator[](int nNdx) const
{
	if ( nNdx >= m_nNumAllocated )
		return 0;
	if ( nNdx < 0 )
		throw 0;
	return m_pArray[nNdx];
}
#endif

CShortIntArray& CShortIntArray::operator = (const CShortIntArray& c)
{
	if( this != &c )
	{
		ResetMemory();
		m_nNumToGrow = c.m_nNumToGrow;
		for ( short nNdx = 0; nNdx <= c.m_nMaxIndexed; ++nNdx )
		{
			if ( nNdx >= m_nNumAllocated )
				Grow( nNdx );
			m_pArray[nNdx] = c.m_pArray[nNdx];
			m_nMaxIndexed = nNdx;
		}
	}
	return *this;
}

bool CShortIntArray::Save(FILE* fd)
{
	bool bRc = true;

	fwrite( &m_nNumAllocated, sizeof(m_nNumAllocated), 1, fd );
	if ( ferror( fd ) ) { bRc = false; return bRc; }
	fwrite( &m_nNumToGrow, sizeof(m_nNumToGrow), 1, fd );
	if ( ferror( fd ) ) { bRc = false; return bRc; }
	fwrite( &m_nMaxIndexed, sizeof(m_nMaxIndexed), 1, fd );
	if ( ferror( fd ) ) { bRc = false; return bRc; }
	fwrite( m_pArray, sizeof(m_pArray[0]), m_nMaxIndexed+1, fd );
	if ( ferror( fd ) ) { bRc = false; return bRc; }

	return bRc;
}

bool CShortIntArray::Restore(FILE* fd)
{
	bool bRc = true;
	short nNumAllocated = 0;
	unsigned int nRead = 0;

	ResetMemory();

	fread( &nNumAllocated, sizeof(m_nNumAllocated), 1, fd );
	if ( ferror( fd ) ) { bRc = false; return bRc; }

	if ( nNumAllocated > m_nNumAllocated )
	{
		Grow( nNumAllocated );
	}

	nRead = fread( &m_nNumToGrow, sizeof(m_nNumToGrow), 1, fd );
	if ( ferror( fd ) ) { bRc = false; return bRc; }
	if ( nRead != 1 ) { bRc = false; return bRc; }

	nRead = fread( &m_nMaxIndexed, sizeof(m_nMaxIndexed), 1, fd );
	if ( ferror( fd ) ) { bRc = false; return bRc; }
	if ( nRead != 1 ) { bRc = false; return bRc; }

	nRead = fread( m_pArray, sizeof(m_pArray[0]), m_nMaxIndexed+1, fd );
	if ( ferror( fd ) ) { bRc = false; return bRc; }
	if ( nRead != (unsigned int)(m_nMaxIndexed+1) ) { bRc = false; return bRc; }

	return bRc;
}

void CShortIntArray::Grow( short nNdx, short nNumToGrow )
{
	if ( nNdx >= m_nNumAllocated )
	{
		m_nNumToGrow = nNumToGrow;

		short nNewCount = nNdx + m_nNumToGrow;

		if ( m_nNumAllocated == 0 )
			nNewCount = nNdx;

		m_pArray = (short*)realloc( m_pArray, sizeof(m_pArray[0]) * nNewCount );
		if ( m_pArray == NULL )
		{
			m_nNumAllocated = 0;
			throw;
		}
		short numGrown = nNewCount - m_nNumAllocated;
		memset( &m_pArray[m_nNumAllocated], 0, sizeof(m_pArray[0]) * numGrown );
		m_nNumAllocated = nNewCount;
	}
}

