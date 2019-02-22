#pragma once


#include "GridCtrl_src/GridCtrl.h"
// YHDLG 窗体视图

class YHDLG : public CFormView
{
	DECLARE_DYNCREATE(YHDLG)

protected:
	YHDLG();           // 动态创建所使用的受保护的构造函数
	virtual ~YHDLG();

public:
	enum { IDD = IDD_FORMVIEW7 };
	CGridCtrl m_list,m_list2;
	void InitList(int rowmax);//GridCtrl初始化信息
	void InitList2(int rowmax);//GridCtrl初始化信息
	void ShowData(CString strsql);
	void ShowData1(CString strsql);
	bool ExeSQL(CString strsql);
	void Clear();
	void ChangeSize();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual void OnInitialUpdate();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_button1;
	CButton m_button2;
};


