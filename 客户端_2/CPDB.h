
// CPDB.h : CPDB Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "DataSocket.h"
// CCPDBApp:
// �йش����ʵ�֣������ CPDB.cpp
//
	extern CString m_pwd;
	extern CString m_user;
	extern CString m_qx;
	extern CString m_dd;
	extern CString m_seat;
	extern	CString m_begin;
	extern CString m_trainID;
class CCPDBApp : public CWinAppEx
{
public:
	CCPDBApp();
	int ports,m_sid,g_port;
	CDataSocket m_HCSocket;
	CString m_ips,g_ip,sPath,oldPath,configpath;

	bool closeflag;
	CString m_name,m_power;
	BOOL ConnectSocket(CDataSocket * pSocket);

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCPDBApp theApp;
