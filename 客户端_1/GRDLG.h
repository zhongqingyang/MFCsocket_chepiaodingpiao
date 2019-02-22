#pragma once



// GRDLG ������ͼ

class GRDLG : public CFormView
{
	DECLARE_DYNCREATE(GRDLG)

protected:
	GRDLG();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~GRDLG();

public:
	enum { IDD = IDD_FORMVIEW3 };
	void ShowData(CString strsql);
	bool ExeSQL(CString strsql);
	CBitmap bitmap;
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
	virtual void OnInitialUpdate();
	CComboBox m_combo1;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton3();
	CComboBox m_combo2;
//	CString m_edit5;
};


