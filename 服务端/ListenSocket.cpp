//Download by http://www.NewXing.com
// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "LibServerDlg.h"
#include "ListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
	m_pdlgMain = NULL;
}
CListenSocket::CListenSocket(CLibServerDlg* pdlgMain)
{
	m_pdlgMain = pdlgMain;
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_pdlgMain->ProcessAccept();
	CSocket::OnAccept(nErrorCode);
}
