// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "LibServer.h"
#include "DataSocket.h"
#include "CMessg.h"
#include "LibServerDlg.h"
#include "process.h"
#include "CAdoDB.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define BLOCKSIZE 20480   //缓冲区大小
int m_dwFileSize;
CString strFName;
/////////////////////////////////////////////////////////////////////////////
// CDataSocket



CDataSocket::CDataSocket(CLibServerDlg* pdlgMain)
{
	m_pdlgMain = pdlgMain;
	m_aSessionIn = NULL;
	m_aSessionOut = NULL;
	m_sfSocketFile = NULL;
}
CDataSocket::CDataSocket()
{
	m_pdlgMain = NULL;
	m_aSessionIn = NULL;
	m_aSessionOut = NULL;
	m_sfSocketFile = NULL;

}
CDataSocket::~CDataSocket()
{
	m_pdlgMain = NULL;
	if (m_aSessionIn)
		delete m_aSessionIn;
	if (m_aSessionOut)
		delete m_aSessionOut;
	if (m_sfSocketFile)
		delete m_sfSocketFile;
	CloseSocket();
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
	GetPeerName(strIPAddress,nPort);
	CString ss;
	while(sockflag)
	{
		RecMeg();
	}
}
void CDataSocket::RecMeg()
{
	static CMessg msg;
	CString buffer;
	TRY 
	{
		msg.Serialize(*m_aSessionIn);
		switch(msg.m_type)
		{
		case 'a': AFUCN(msg);break;
		case 'b': BFUCN(msg);break;
		case 'c': CFUCN(msg); break;
		case 'e': EFUCN(msg); break;
		case 'f': FFUCN(msg);break;
		case 'p': PFUCN(msg);break;
			
		}
		
	}
	CATCH (CException, e)
	{
		Close();
		sockflag=false;
	}
	END_CATCH
	
}

BOOL CDataSocket::SendMessage(CMessg &msg)
{
	if (m_aSessionOut!=NULL)
	{
		msg.Serialize(*m_aSessionOut);
		m_aSessionOut->Flush();
		return TRUE;
	}
	else
		return FALSE;
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
	Close();
}

void CDataSocket::OnClose(int nErrorCode) 
{
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
void CDataSocket::FFUCN(CMessg &msg)
{

}

//查询
void CDataSocket::AFUCN(CMessg &msg)
{
	CMessg smsg;		//
	smsg.m_strText="";
	CString result,strsql,strColName;
	//数据库
	CAdoDB db(TEXT("train"));

	strsql = msg.m_strText;
	//AfxMessageBox(strsql);

	//执行查询
	db.ExecuteSQLSelect(strsql);
	if(db.ResultExist())
	{
		int totalnum=db.GetRecCount();
		smsg.m_OperState=totalnum;
		
		long nItemCount  =db.m_pRecordset->GetFields()->GetCount(); 
		smsg.m_Column=nItemCount;
		while(!db.m_pRecordset->adoEOF)
		{
			for(long li=0;li<nItemCount;li++)
			{
				smsg.m_strText += db.GetFieldValue(li)+"|";
			}
			db.m_pRecordset->MoveNext();
		}	
	}
	SendMessage(smsg);
}
//更新操作
void CDataSocket::BFUCN(CMessg &msg)//所有的操作
{
	CAdoDB db(TEXT("train"));
	CString result,strsql;
	strsql.Format("%s",msg.m_strText);
	//执行插入，返回状态
	if(db.ExecuteSQL(strsql))
	{
		msg.m_OperState=1;
	}
	else
	{
		msg.m_OperState=0;
	}
	SendMessage(msg); 

}
//获取记录条数，可以应用于登录成功判断
void CDataSocket::CFUCN(CMessg &msg)
{
	CString strs;
	CMessg smsg;		//
	smsg.m_strText="";
	CString result,strsql,strColName;
	CAdoDB db(TEXT("train"));
	
	strsql = msg.m_strText;
	db.ExecuteSQLSelect(strsql);
	if(db.ResultExist())
	{
		int totalnum=db.GetRecCount();
		smsg.m_OperState=totalnum;
		smsg.m_strText.Format("%d",totalnum);
	}				
	SendMessage(smsg);
}

//分页查询，客户端传当前需要的页数
void CDataSocket::EFUCN(CMessg &msg)  //每次查询N条记录
{
	CMessg smsg;
	smsg.m_strText="";
	CString result,strsql,strColName,strs1,strs2;
	CAdoDB db(TEXT("train"));
	strsql = msg.m_strText;
	long nItemCount;

	db.ExecuteSQLSelect(strsql);
	if(db.ResultExist())
	{
		int totalnum=db.GetRecCount();
		smsg.m_OperState=totalnum;
		nItemCount  =db.m_pRecordset->GetFields()->GetCount(); 

		while(!db.m_pRecordset->adoEOF)
		{
			for(long li=0;li<nItemCount;li++)
			{
				smsg.m_strText += db.GetFieldValue(li)+"|";
			}
			db.m_pRecordset->MoveNext();
		}
		
	}
	
	int N=5;               //每次N条数据
	int k=msg.m_num ;
    
	if(smsg.m_OperState%N==0) smsg.m_pageall=smsg.m_OperState/N;
	else  smsg.m_pageall=smsg.m_OperState/N+1;
	
	if(smsg.m_OperState-N*k<=0)
	{
		smsg.m_OperState=0; 
		SendMessage(smsg);
		return;
	}
	else 
	{
		if(smsg.m_OperState-N*k>=N)  smsg.m_OperState=N;//smsg.m_OperState=100;
		else  smsg.m_OperState=smsg.m_OperState-N*k;
		CString result="";
		for(int i=0; i<smsg.m_OperState; i++)
		{
			for(long li=0;li<nItemCount;li++)
			{
				CString strs;
				AfxExtractSubString(strs,smsg.m_strText,(i+k*N)*nItemCount+li,'|');
				result+=strs+"|";
			}
		}
		smsg.m_strText =result;
		SendMessage(smsg);
		return;
	}
}

void CDataSocket::PFUCN(CMessg &msg) 
{
}

void CDataSocket::OnReceive(int nErrorCode) 
{
	CSocket::OnReceive(nErrorCode);
}
