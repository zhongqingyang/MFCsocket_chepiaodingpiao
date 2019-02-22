#pragma once
#include "afxwin.h"


// DADLG 对话框

class DADLG : public CDialogEx
{
	DECLARE_DYNAMIC(DADLG)

public:
	DADLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DADLG();
	CString selectt(CString strsql);
	bool ExeSQL(CString strsql);
	int GetRecCount(CString strsql);

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list;
	CButton m_ch1;
	CButton m_ch2;
	CButton m_ch3;
	CButton m_ch4;
	CButton m_ch5;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
