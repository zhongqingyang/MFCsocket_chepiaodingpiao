#pragma once
#include "GridCtrl_src/GridCtrl.h"

// GQDLG �Ի���

class GQDLG : public CDialogEx
{
	DECLARE_DYNAMIC(GQDLG)

public:
	GQDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GQDLG();
	CGridCtrl m_list;
	CGridCtrl m_list1;
	void Clear();
	bool ExeSQL(CString strsql);
	void ShowData(CString strsql);
	void ShowData1(CString strsql);
	int GetRecCount(CString strsql);
	void InitList(int rowmax);//GridCtrl��ʼ����Ϣ
	void InitList1(int rowmax);//GridCtrl��ʼ����Ϣ
	CString selectt(CString strsql);
	void dd();
// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton1();
//	CString m_Begdin;
	CString m_End;
	CString m_Begin;
	CDateTimeCtrl m_dateCtrl;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
