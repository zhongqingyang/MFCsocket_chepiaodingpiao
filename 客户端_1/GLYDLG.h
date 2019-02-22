#pragma once

#include "xSkinButton.h"

// GLYDLG 窗体视图
//#include "MyTreeCtrl.h"
class GLYDLG : public CFormView
{
	DECLARE_DYNCREATE(GLYDLG)

protected:
	GLYDLG();           // 动态创建所使用的受保护的构造函数
	virtual ~GLYDLG();

public:
	enum { IDD = IDD_FORMVIEW1 };
	CxSkinButton	m_btn;
	CBitmap bitmap;
	void quanxian();
	void ChangeSize();
	CString selectt(CString strsql);

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
	afx_msg void OnBnClickedButton4();
	virtual void OnInitialUpdate();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	CTreeCtrl m_webTree;
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};


