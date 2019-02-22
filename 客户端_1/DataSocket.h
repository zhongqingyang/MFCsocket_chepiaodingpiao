#if !defined(AFX_DATASOCKET_H__FD67BC6D_8137_4F59_9667_1B59D302CFD7__INCLUDED_)
#define AFX_DATASOCKET_H__FD67BC6D_8137_4F59_9667_1B59D302CFD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSocket.h : header file
//

class CMessg;

/////////////////////////////////////////////////////////////////////////////
// CDataSocket command target

class CDataSocket : public CSocket
{
// Attributes
public:
	CArchive* m_aSessionIn;
	CArchive* m_aSessionOut;
	CSocketFile* m_sfSocketFile;
//	CChatClientDlg* m_pDlg;

// Operations
public:
	CDataSocket();
	virtual ~CDataSocket();

// Overrides
public:
	BOOL RecMessage(CMessg *msg);
	static CString GetIPAddress();
	void CloseSocket();
	BOOL SendMessage(CMessg& msg);
	void Init();


	BOOL SetTimeOut(UINT uTimeOut=1000);
	BOOL KillTimeOut();
	bool OnLine;// «∑Ò‘⁄œﬂ


private:
	LONGLONG m_llDtStart;
	UINT   m_uTimeOut;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSocket)
	public:
//	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
		virtual BOOL OnMessagePending();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDataSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASOCKET_H__FD67BC6D_8137_4F59_9667_1B59D302CFD7__INCLUDED_)
