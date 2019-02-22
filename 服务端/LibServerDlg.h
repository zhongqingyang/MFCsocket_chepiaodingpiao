// LibServerDlg.h : header file
//

#if !defined(AFX_LibServerDLG_H__E19306F8_5224_4E50_ADA8_645B2C649C28__INCLUDED_)
#define AFX_LibServerDLG_H__E19306F8_5224_4E50_ADA8_645B2C649C28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLibServerDlg dialog
#include <afxtempl.h>
#include <afxsock.h>
#include <vector>
#include "DataSocket.h"
using namespace std;

#define WM_CONNECTION_COMEIN WM_USER+100

typedef vector<CDataSocket*> vec_DataSocket;

static unsigned __stdcall ThreadAccept(void *pParam);
class CLibServerDlg : public CDialog
{
// Construction
public:
	CLibServerDlg(CWnd* pParent = NULL);	// standard constructor
    CString GetNetIP();
// Dialog Data
	//{{AFX_DATA(CLibServerDlg)
	enum { IDD = IDD_LibServer_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
public:
	void SendFile(CString,CString strIPAddress,UINT nPort);
	void SendFile(CSocket &recSo);

	void ReceiveFile(CString ,int);
	void ReceiveFile(CSocket &recSo);
	void OnServerClose(CDataSocket* pSocket);
	void ReceiveMessage(CDataSocket* pSocket, CMessg& msg);
	void RefreshScreen();
	void GetErrorReason(int nErrorCode);
	UINT m_ListenPort;
	CString m_ChatWords;
	BOOL m_bActive;
	CString strIPAddress,strFileName,strSendFileName;
	int m_FileSize,nPort;

	vec_DataSocket m_DataSockets;
	vec_DataSocket::iterator it_DataSockets;
private:
	HANDLE m_ListenThreadHandle;

	// Generated message map functions
	//{{AFX_MSG(CLibServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnListen();
	afx_msg void SendMessageToPeer();
	afx_msg LRESULT WelcomeNewClient(WPARAM w, LPARAM l);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void Onset();
	afx_msg void OnClose();
	afx_msg void OnSelchangeComboAddress();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LibServerDLG_H__E19306F8_5224_4E50_ADA8_645B2C649C28__INCLUDED_)
