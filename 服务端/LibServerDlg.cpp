// LibServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LibServer.h"
#include "LibServerDlg.h"
#include "CMessg.h"
#include "CAdoDB.h"
#include <assert.h>
#include <vector>
#include "process.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define BLOCKSIZE 20480   //缓冲区大小
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
HANDLE g_ChatWordsEvent;
ULONG WINAPI ListenThread(LPVOID p);

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibServerDlg dialog

CLibServerDlg::CLibServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLibServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLibServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLibServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLibServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLibServerDlg, CDialog)
	//{{AFX_MSG_MAP(CLibServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CRT, OnListen)
	ON_EN_UPDATE(IDC_INPUTTEXT, SendMessageToPeer)
	ON_MESSAGE(WM_CONNECTION_COMEIN, WelcomeNewClient)
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_set, Onset)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_ADDRESS, OnSelchangeComboAddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibServerDlg message handlers

BOOL CLibServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_bActive = FALSE;
	g_ChatWordsEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

TCHAR lpszHostName[ 128 ] = { 0 } ;
	
	if ( gethostname( lpszHostName, sizeof( lpszHostName ) / sizeof( TCHAR ) ) == 0 )
	{
		HOSTENT* lpHost = gethostbyname( lpszHostName ) ;
		
		if ( lpHost ) // 获得主机信息成功
		{
			int nLastIP = 0 ;
			
			// 清空IP列表
			( ( CComboBox* )GetDlgItem( IDC_COMBO_ADDRESS ) )->ResetContent() ;

			// 添加所有可用IP到列表中
			for ( nLastIP = 0 ; lpHost->h_addr_list[ nLastIP ] != NULL ; nLastIP++ ) 
			{
				CString strIPs = inet_ntoa( *(in_addr*)lpHost->h_addr_list[ nLastIP ] ) ;
				( ( CComboBox* )GetDlgItem( IDC_COMBO_ADDRESS ) )->InsertString( 0, strIPs ) ;
			}
			// 选中第一个IP
			( ( CComboBox* )GetDlgItem( IDC_COMBO_ADDRESS ) )->SetFocus() ;
			( ( CComboBox* )GetDlgItem( IDC_COMBO_ADDRESS ) )->SetCurSel(nLastIP-1) ;
		}
	}


	((CEdit*)GetDlgItem(IDC_PORTID))->SetWindowText("34567");
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLibServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLibServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLibServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLibServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CLibServerDlg::OnListen() 
{
	if(m_bActive)
	{
		AfxMessageBox("服务器已经启动！");
		return;
	}
	// TODO: Add your control notification handler code here
	CString buffer;
	((CEdit*)GetDlgItem(IDC_PORTID))->GetWindowText(buffer);

	m_ListenPort = UINT(atoi(LPCSTR(buffer)));
	if (m_ListenPort==0)
	{
		m_ChatWords += "please specify a port num.\r\n";
		RefreshScreen();
		return;
	}
	m_ListenThreadHandle = CreateThread(NULL, 0, ListenThread, this, NULL, NULL);
}

//监听线程
ULONG WINAPI ListenThread(LPVOID p)
{
	CSocket listenSocket;//监听
	BOOL bRet;
	CLibServerDlg* pDlg = (CLibServerDlg*)p;
	CSocket dataSocket;
	SOCKET acceptSock;
	bRet = listenSocket.Create(pDlg->m_ListenPort);
	if (bRet==FALSE)
	{
		WaitForSingleObject(g_ChatWordsEvent, INFINITE);
		pDlg->m_ChatWords += "服务器启动失败.\r\n";
		pDlg->GetErrorReason(WSAGetLastError());
		pDlg->RefreshScreen();
		SetEvent(g_ChatWordsEvent);
		listenSocket.Close();
		return -1;
	}
	bRet = listenSocket.Listen(5);
	if (bRet==FALSE)
	{
		WaitForSingleObject(g_ChatWordsEvent, INFINITE);
		pDlg->m_ChatWords += "listen error.\r\n";
		pDlg->GetErrorReason(WSAGetLastError());
		pDlg->RefreshScreen();
		SetEvent(g_ChatWordsEvent);
		listenSocket.Close();
		return -1;
	}
	WaitForSingleObject(g_ChatWordsEvent, INFINITE);

	listenSocket.m_hSocket;
	pDlg->m_ChatWords += "服务器正在运行...\r\n";
	pDlg->RefreshScreen();
	SetEvent(g_ChatWordsEvent);
	pDlg->m_bActive = TRUE;
	while(pDlg->m_bActive)
	{
		if (listenSocket.Accept(dataSocket, NULL, NULL)==FALSE)
			return -1;
		WaitForSingleObject(g_ChatWordsEvent, INFINITE);

		acceptSock = dataSocket.m_hSocket;

		dataSocket.Detach();
		
		_beginthreadex(NULL, NULL, ThreadAccept, &acceptSock, 0, NULL);
		dataSocket.Close();
		SetEvent(g_ChatWordsEvent);
	}
	listenSocket.Close();
	return 0;
}
//应答线程
unsigned __stdcall ThreadAccept(void *pParam)
{	
	if (NULL == pParam)
	{
		AfxMessageBox("NULL");
		return -1;
	}
 

	SOCKET *pSocketRecv = (SOCKET *)pParam;

	int namelen, i;
	CString buffer;
	sockaddr_in name;

	CDataSocket* pSocket;
	SOCKET acceptSock;
	acceptSock = SOCKET(*pSocketRecv);

	pSocket = new CDataSocket();
	if(!pSocket->Attach(acceptSock))
		AfxMessageBox("Attach error");



	namelen = sizeof(name);
	struct sockaddr_in sa;
	int len = sizeof(sa);
	if(!getpeername(acceptSock, (struct sockaddr *)&sa, &len))
	{
		strmyip.Format("%s", inet_ntoa(sa.sin_addr));
		strport.Format("%d", ntohs(sa.sin_port));
	}
	pSocket->sockflag=true;
	pSocket->Init();

}
void CLibServerDlg::GetErrorReason(int nErrorCode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, nErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	m_ChatWords += (char*)lpMsgBuf;
	LocalFree(lpMsgBuf);
}

LRESULT CLibServerDlg::WelcomeNewClient(WPARAM w, LPARAM l)
{
	int namelen, i;
	CString buffer;
	sockaddr_in name;
	CDataSocket* pSocket;
	CMessg msg;
	SOCKET acceptSock;
	namelen = sizeof(name);
	acceptSock = SOCKET(w);
	if (!m_bActive)
	{
		closesocket(acceptSock);
		return 0;
	}
	pSocket = new CDataSocket;
	pSocket->Attach(acceptSock);
	pSocket->Init();

}

void CLibServerDlg::RefreshScreen()
{
	GetDlgItem(IDC_SHOWTEXT)->SetWindowText(m_ChatWords);
}

void CLibServerDlg::SendMessageToPeer()
{
	CString buffer;
	CMessg msg;
	int i;
	((CEdit*)GetDlgItem(IDC_INPUTTEXT))->GetWindowText(buffer);
	if (buffer.GetAt(buffer.GetLength()-1)!='\n')
	{
		return;
	}
	((CEdit*)GetDlgItem(IDC_INPUTTEXT))->SetWindowText("");
	if (!m_bActive)
	{
		WaitForSingleObject(g_ChatWordsEvent, INFINITE);
		m_ChatWords += buffer;
		m_ChatWords += "hint: you are isolated now . \r\n";
		RefreshScreen();
		SetEvent(g_ChatWordsEvent);
		return;
	}
	WaitForSingleObject(g_ChatWordsEvent, INFINITE);
	m_ChatWords += "server:"+buffer;
	RefreshScreen();
	SetEvent(g_ChatWordsEvent);
	msg.m_strText = "server:"+buffer;
	for (i=0;i<m_DataSockets.size();i++)
	{
		m_DataSockets[i]->SendMessage(msg);
	}
}

void CLibServerDlg::ReceiveMessage(CDataSocket* pSocket, CMessg& msg)
{
	int i;
	for (i=0;i<m_DataSockets.size();i++)
	{
		if (pSocket!=m_DataSockets[i])
		{
			m_DataSockets[i]->SendMessage(msg);
		}
	}
	WaitForSingleObject(g_ChatWordsEvent, INFINITE);
	m_ChatWords += msg.m_strText;
	RefreshScreen();
	SetEvent(g_ChatWordsEvent);
}

void CLibServerDlg::OnServerClose(CDataSocket* pSocket)
{
	int i, namelen;
	CString buffer;
	sockaddr_in name;
	CMessg msg;

	namelen = sizeof(name);
	pSocket->GetPeerName((sockaddr*)&name, &namelen);
	pSocket->Close();
	for (i=0, it_DataSockets=m_DataSockets.begin(); i<m_DataSockets.size(); i++, it_DataSockets++)
	{
		if (m_DataSockets[i]==pSocket)
		{
			m_DataSockets.erase(it_DataSockets);
			break;
		}
	}
	buffer.Format("client %s:%d left.\r\t", inet_ntoa(name.sin_addr), ntohs(name.sin_port));
	msg.m_strText = buffer;
	for (i=0; i<m_DataSockets.size(); i++)
	{
		m_DataSockets[i]->SendMessage(msg);
	}
	m_ChatWords += buffer;
	RefreshScreen();
	delete pSocket;
}

void CLibServerDlg::OnRButtonDblClk(UINT nFlags, CPoint point) 
{

	
	CDialog::OnRButtonDblClk(nFlags, point);
}



void CLibServerDlg::Onset() 
{
	if(MessageBox(_T("确定要清除所有数据吗?"),_T("注意"),MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION)==IDNO)
	{

	}
	else
	{
	}
	
}

void CLibServerDlg::OnClose() 
{
	if(MessageBox(_T("确定要关闭服务器吗?"),_T("注意"),MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION)==IDNO)
	{
		return;
	}
	CDialog::OnClose();
}

//接收文件线程
UINT _ListenThread(LPVOID lparam)
{
	CLibServerDlg* pDlg = (CLibServerDlg*) lparam;
	//创建套接字
	CSocket sockSrvr;
	if(!sockSrvr.Create(pDlg->m_ListenPort+1000))
	{
		return -1;
	}
	//开始监听
	if(!sockSrvr.Listen())
	{
		return -1;
	}

	CSocket recSo;
	if(!sockSrvr.Accept(recSo))
	{
		return -1;
	}
	sockSrvr.Close();

	pDlg->ReceiveFile(recSo);

	return 0;
}

void CLibServerDlg::ReceiveFile(CString strFilename,int m_dwFileSize)
{
	strFileName=strFilename;
	m_FileSize=m_dwFileSize;
	theApp.brecflag=false;

	::AfxBeginThread(_ListenThread,this);
}

//接收文件
void CLibServerDlg::ReceiveFile(CSocket &recSo)
{
	int nSize = 0;
	DWORD dwCount = 0;
	char buf[BLOCKSIZE] = {0};
	//创建一个文件
	CFile file(strFileName,CFile::modeCreate|CFile::modeWrite);
	//开始接收文件
	double secs,mbsec;
	COleDateTimeSpan dlElapsed;
	CString v,mbReceived;

	for (;;)
	{
		//每次接收BLOCKSIZE大小的文件的内容
		nSize = recSo.Receive(buf,BLOCKSIZE);
		if (nSize == 0)//接收失败
		{
			break ;
		}

		//将接收到得文件写到新文件中去
		file.Write(buf,nSize);
		dwCount +=nSize;
	}

	//关闭文件
	file.Close();
	//关闭套接字
	recSo.Close();
}



void CLibServerDlg::OnSelchangeComboAddress() 
{
OnListen();	
}
