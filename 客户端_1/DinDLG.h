#pragma once


// DinDLG �Ի���

class DinDLG : public CDialogEx
{
	DECLARE_DYNAMIC(DinDLG)

public:
	DinDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DinDLG();
	CString selectt(CString strsql);
	
// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
