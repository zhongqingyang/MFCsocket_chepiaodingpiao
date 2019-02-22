#if !defined(AFX_LOGIN_H__ACFD2CA6_DD54_4E95_A022_E41902451190__INCLUDED_)
#define AFX_LOGIN_H__ACFD2CA6_DD54_4E95_A022_E41902451190__INCLUDED_
#include "IniFile.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLDLG 对话框
#include "xSkinButton.h"
class DLDLG : public CDialogEx
{
	DECLARE_DYNAMIC(DLDLG)

public:
	DLDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DLDLG();
	BOOL m_bIsExpand; 
	int m_nDlgWidth ; // 登陆窗口宽度
	int m_nDlgHight ; // 登陆窗口高度
	CString strIP;
	int   nPort;
	CBrush brush;
	CBitmap bitmap;
	HICON m_hIcon;
	CxSkinButton	m_btn,m_btn1,m_btn2;
	bool    bnoflag;
	BOOL Connect();//连接是否成功
	void ConnectFailed(); // 连接服务器失败
	void quanxian();
	CString selectt(CString strsql);
	CString GetAddr() ; // 获得用户输入的服务器地址
	int GetPort() ; // 获得用户输入的服务器端口

	CIniFile Inifile;
	void InitNet() ; // 初始化网络设置
	BOOL ReadCfgFile(); //读取配置文件
	// Dialog Data
	//{{AFX_DATA(CLogin)
	enum { IDD = IDD_DIALOG1};
//	CString	m_user;
//	CString	m_pwd;
//	int		m_nPort;
// 对话框数据

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnInternet();
//	virtual BOOL OnInitDialog();
	afx_msg void OnSelectdefault();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
//	int m_user;

	int m_nPort;
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton4();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
};
#endif