
// CPDBView.h : CCPDBView 类的接口
//

#pragma once

#include "resource.h"
#include "CPDBDoc.h"
#include "GridCtrl_src/GridCtrl.h"
#include "ComboBoxIS.h"

class CCPDBView : public CFormView
{
protected: // 仅从序列化创建
	CCPDBView();
	DECLARE_DYNCREATE(CCPDBView)

public:
	enum{ IDD = IDD_CPDB_FORM };
	CGridCtrl m_list,m_list2;
	void Clear();
	bool ExeSQL(CString strsql);
	void ShowData(CString strsql);
	int GetRecCount(CString strsql);
	CString Seat(CString strsql);
	void InitList(int rowmax);//GridCtrl
	void InitList2(int rowmax);//GridCtrl初始化信息
	void ChangeSize();
	CString selectt(CString strsql);
	void ShowAllInfo();
	CDateTimeCtrl m_dateCtrl;
	CComboBoxIS m_yfComBox;
	CComboBoxIS m_yfComBox1;
// 特性
public:
	CCPDBDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
// 实现
public:
	virtual ~CCPDBView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

//	CString m_Begin;
//	CString m_End;
	CBitmap bitmap;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_x1;
	CStatic m_x2;
	CStatic m_x3;
	CButton m_button1;
	CButton m_button2;
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

#ifndef _DEBUG  // CPDBView.cpp 中的调试版本
inline CCPDBDoc* CCPDBView::GetDocument() const
   { return reinterpret_cast<CCPDBDoc*>(m_pDocument); }
#endif

