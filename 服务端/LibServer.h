// LibServer.h : main header file for the LibServer application
//

#if !defined(AFX_LibServer_H__AB96501A_F063_4707_9BE5_ABBEBE5DDBA1__INCLUDED_)
#define AFX_LibServer_H__AB96501A_F063_4707_9BE5_ABBEBE5DDBA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
extern CString strmyip,strport;
/////////////////////////////////////////////////////////////////////////////
// CLibServerApp:
// See LibServer.cpp for the implementation of this class
//

class CLibServerApp : public CWinApp
{
public:
	CLibServerApp();
	CString sPath;
	bool brecflag,bsendflag;
	int contno;//ºÏÍ¬±àºÅ

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLibServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern CLibServerApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LibServer_H__AB96501A_F063_4707_9BE5_ABBEBE5DDBA1__INCLUDED_)
