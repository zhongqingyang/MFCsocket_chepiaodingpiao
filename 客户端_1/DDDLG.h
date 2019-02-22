#pragma once


#include "GridCtrl_src/GridCtrl.h"
// DDDLG ������ͼ

class DDDLG : public CFormView
{
	DECLARE_DYNCREATE(DDDLG)

protected:
	DDDLG();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~DDDLG();

public:
	enum { IDD = IDD_FORMVIEW2 };
	CGridCtrl m_list,m_list2;
	void Clear();
	bool ExeSQL(CString strsql);
	void ShowData(CString strsql);
	int GetRecCount(CString strsql);
	void InitList(int rowmax);//GridCtrl��ʼ����Ϣ
	void InitList2(int rowmax);//GridCtrl��ʼ����Ϣ
	CString selectt(CString strsql);
	CBitmap bitmap;
	void ChangeSize();
	void ShowAllInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_button1;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_button2;
};


