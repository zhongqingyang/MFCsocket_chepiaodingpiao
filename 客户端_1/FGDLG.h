#pragma once

#include "xSkinButton.h"

// FGDLG 窗体视图

class FGDLG : public CFormView
{
	DECLARE_DYNCREATE(FGDLG)

protected:
	FGDLG();           // 动态创建所使用的受保护的构造函数
	virtual ~FGDLG();

public:
	enum { IDD = IDD_FORMVIEW };
	CxSkinButton	m_btn;
	CBitmap bitmap;
	CImageList m_imageList;
	//CFont m_editFont; 
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
	//void ChangeSize();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton4();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton2();
//	afx_msg void OnBnClickedButton3();
	CTreeCtrl m_webTree;
//	afx_msg void OnBnClickedButton1();
};


