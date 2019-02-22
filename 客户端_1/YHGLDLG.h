#pragma once
#include "afxwin.h"



// YHGLDLG 窗体视图

class YHGLDLG : public CFormView
{
	DECLARE_DYNCREATE(YHGLDLG)

protected:
	YHGLDLG();           // 动态创建所使用的受保护的构造函数
	virtual ~YHGLDLG();

public:
	enum { IDD = IDD_FORMVIEW6 };
	void Clear();
	bool ExeSQL(CString strsql);
	void InitList(int rowmax);//GridCtrl初始化信息
	void quanxian();
	CString selectt(CString strsql);
	CBitmap bitmap;
	void ChangeSize();
	int GetRecCount(CString strsql);
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
	CString m_oldpwn;
	CString m_newpwn;
	CString m_zh;
	CString m_pass;
	CString m_pass1;
//	CString m_shenfen;
	afx_msg void OnBnClickedButton2();
	CString m_sex;
	CString m_phone;
	CComboBox m_shenfen;
	CComboBox m_sex1;
	CListBox m_list;
	afx_msg void OnSelchangeList1();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListBox m_list1;
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};


