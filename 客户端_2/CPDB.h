
// CPDB.h : CPDB 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include "DataSocket.h"
// CCPDBApp:
// 有关此类的实现，请参阅 CPDB.cpp
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

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCPDBApp theApp;
