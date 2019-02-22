#if !defined(AFX_DATASOCKET_H__FD67BC6D_8137_4F59_9667_1B59D302CFD7__INCLUDED_)
#define AFX_DATASOCKET_H__FD67BC6D_8137_4F59_9667_1B59D302CFD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSocket.h : header file
//

class CLibServerDlg;
class CMessg;

/////////////////////////////////////////////////////////////////////////////
// CDataSocket command target

class CDataSocket : public CSocket
{
// Attributes
public:
	bool sockflag;
	CArchive* m_aSessionIn;
	CArchive* m_aSessionOut;
	CSocketFile* m_sfSocketFile;
	CString strIPAddress;
	UINT nPort;

// Operations
public:
	CDataSocket();
	CDataSocket(CLibServerDlg * pdlg);
	virtual ~CDataSocket();

// Overrides
public:
	void AFUCN(CMessg &msg);
	void BFUCN(CMessg &msg);
	void CFUCN(CMessg &msg);
    void DFUCN(CMessg &msg);
	void EFUCN(CMessg &msg);
	void FFUCN(CMessg &msg);
	void SFUCN(CMessg &msg);
	void GFUCN(CMessg &msg);
	void PFUCN(CMessg &msg);

	void RecMeg();
	static CString GetIPAddress();
	void CloseSocket();
	BOOL SendMessage(CMessg& msg);
	void ReceiveMsg(CMessg& msg);

	//CClientSocket(CLibServerDlg* pdlgMain);
	void Init();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDataSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CLibServerDlg* m_pdlgMain;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASOCKET_H__FD67BC6D_8137_4F59_9667_1B59D302CFD7__INCLUDED_)
