#pragma once


// DinDLG 对话框

class DinDLG : public CDialogEx
{
	DECLARE_DYNAMIC(DinDLG)

public:
	DinDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DinDLG();
	CString selectt(CString strsql);
	
// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
