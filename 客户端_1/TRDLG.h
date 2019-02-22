#pragma once


#include "GridCtrl_src/GridCtrl.h"
// TRDLG 窗体视图

class TRDLG : public CFormView
{
	DECLARE_DYNCREATE(TRDLG)

protected:
	TRDLG();           // 动态创建所使用的受保护的构造函数
	virtual ~TRDLG();

public:
	enum { IDD = IDD_FORMVIEW4 };
	CGridCtrl m_list;
	void Clear();
	bool ExeSQL(CString strsql);
	void ShowData(CString strsql);
	int GetRecCount(CString strsql);
	void InitList(int rowmax);
	void ShowDatadata(CString strsql);
	//void ShowData(CString strsql);
	CDateTimeCtrl m_dateCtrl;
	CBitmap bitmap;
	void ChangeSize();
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
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButton1();
	CString m_Begin;
	CString m_End;
	CString m_checi;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CButton m_button2;
	CButton m_button3;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnClickedButton8();
};


