//Download by http://www.NewXing.com
#if !defined(AFX_LISTENSOCKET_H__F106E35C_3544_4A8B_8E0E_0ECF8E614812__INCLUDED_)
#define AFX_LISTENSOCKET_H__F106E35C_3544_4A8B_8E0E_0ECF8E614812__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//

#include "LibServerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CListenSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CListenSocket();
	virtual ~CListenSocket();

// Overrides
public:
	CListenSocket(CLibServerDlg* pdlgMain);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CLibServerDlg* m_pdlgMain;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__F106E35C_3544_4A8B_8E0E_0ECF8E614812__INCLUDED_)
