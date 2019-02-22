#pragma once


#include "GridCtrl_src/GridCtrl.h"
// DDGLDLG 窗体视图

class DDGLDLG : public CFormView
{
	DECLARE_DYNCREATE(DDGLDLG)

protected:
	DDGLDLG();           // 动态创建所使用的受保护的构造函数
	virtual ~DDGLDLG();

public:
	enum { IDD = IDD_FORMVIEW5 };
	CGridCtrl m_list;
	void Clear();
	bool ExeSQL(CString strsql);
	void ShowData(CString strsql);
	void ShowDatadata(CString strsql);
	int GetRecCount(CString strsql);
	void InitList(int rowmax);//GridCtrl初始化信息
	void ChangeSize();
	void quanxian();
	CString selectt(CString strsql);
	CDateTimeCtrl m_dateCtrl;
	CBitmap bitmap;
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual void OnInitialUpdate();
	CString m_dindan;
	CString m_yonghu;
	CString m_checi;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_button1;
	CButton m_button2;
	CButton m_button4;
	afx_msg void OnClickedButton7();
};


