// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "DataSocket.h"
#include "CMessg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSocket

BOOL m_flag;

CDataSocket::CDataSocket()
{
	m_aSessionIn = NULL;
	m_aSessionOut = NULL;
	m_sfSocketFile = NULL;
	OnLine=false;
}

CDataSocket::~CDataSocket()
{
	if (m_aSessionIn)
		delete m_aSessionIn;
	if (m_aSessionOut)
		delete m_aSessionOut;
	if (m_sfSocketFile)
		delete m_sfSocketFile;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDataSocket, CSocket)
	//{{AFX_MSG_MAP(CDataSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDataSocket member functions

void CDataSocket::Init()
{
	m_sfSocketFile = new CSocketFile(this);
	m_aSessionIn = new CArchive(m_sfSocketFile, CArchive::load);
	m_aSessionOut = new CArchive(m_sfSocketFile, CArchive::store);
	m_flag = true;
}
//设置超时
BOOL CDataSocket::SetTimeOut(UINT uTimeOut)
{   
	//get start cnt
	LARGE_INTEGER llCnt;
	::QueryPerformanceCounter(&llCnt);
	m_llDtStart=llCnt.QuadPart; 
	m_uTimeOut=uTimeOut;
	
	return TRUE;
}

//删除超时参数
BOOL CDataSocket::KillTimeOut()
{
	m_llDtStart=0;//表明取消计时
	return TRUE;
}

//检查是否超时间
BOOL CDataSocket::OnMessagePending() 
{
	if( m_llDtStart )
	{
		LARGE_INTEGER lldtEnd;
		::QueryPerformanceCounter(&lldtEnd);  
		LARGE_INTEGER llFrq;
		::QueryPerformanceFrequency(&llFrq);
		double dbDealy=(double)(lldtEnd.QuadPart-m_llDtStart)*1000/llFrq.QuadPart;
		if( dbDealy>m_uTimeOut )
		{
			CancelBlockingCall();
			return FALSE; // No need for idle time processing.
		}
	}
	
	return CSocket::OnMessagePending();
}
BOOL CDataSocket::SendMessage(CMessg &msg)
{
	try 
	{
		msg.Serialize(*m_aSessionOut);
		m_aSessionOut->Flush();
		m_flag = true;
	}
	catch (...)
	{
		Close();
		return 0;
	}
	return 1;

}
BOOL CDataSocket::RecMessage(CMessg *msg)
{
	CString buffer;
	TRY 
	{
		msg->Serialize(*m_aSessionIn);
	}
	CATCH (CException, e)
	{
		Close();
		return 0;
	}
	END_CATCH

	return TRUE;
}

void CDataSocket::CloseSocket()
{
	if (m_aSessionIn)
	{
		delete m_aSessionIn;
		m_aSessionIn = NULL;
	}
	if (m_aSessionOut)
	{
		delete m_aSessionOut;
		m_aSessionOut = NULL;
	}
	if (m_sfSocketFile)
	{
		delete m_sfSocketFile;
		m_sfSocketFile = NULL;
	}

}
void CDataSocket::OnClose(int nErrorCode) 
{
	OnLine=false;
	CloseSocket();
	
	AfxMessageBox("连接断开,请重新连接!");
	CSocket::OnClose(nErrorCode);
}

CString CDataSocket::GetIPAddress()
{
	hostent* lpHostEnt;
	char szHostName[256];
	LPSTR lpAddr;
	struct in_addr inAddr;
	if (gethostname(szHostName, sizeof(szHostName)) != 0)
		return CString("");
	lpHostEnt = gethostbyname(szHostName);
	if (lpHostEnt == NULL)
	{
		return CString("");
	}
	lpAddr = lpHostEnt->h_addr_list[0];
	if (lpAddr == NULL)
	{
		return CString("");
	}
	memmove(&inAddr,lpAddr,4);
	return CString(inet_ntoa(inAddr));
}

