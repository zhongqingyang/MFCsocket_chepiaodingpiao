#include "StdAfx.h"
#include "CMessg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMessg, CObject)

CMessg::CMessg()
{
	m_strText="";
	m_type='z';
	m_OperState=0;
	m_num=0;
	m_pageall=0;
	m_dwFileSize=0;
	m_start=0;
	m_total=0.0;
	m_Column=0;
}

CMessg::~CMessg()
{
}

void CMessg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strText;
		ar << m_type;
		ar << m_OperState;
		ar << m_num;
		ar << m_pageall;
		ar << m_dwFileSize;
		ar << m_start;
		ar << m_total;
		ar << m_Column;

	}
	else
	{
		ar >> m_strText;
		ar >> m_type;
		ar >> m_OperState;
		ar >> m_num;
		ar >> m_pageall;
		ar >> m_dwFileSize;
		ar >> m_start;
		ar >> m_total;
		ar >> m_Column;

	}
}
