#ifndef _IS_CComboBoxIS_H_20161207114623_SMQH__
#define _IS_CComboBoxIS_H_20161207114623_SMQH__
//////////////////////////////////////////////////////////////////////////
//
// ComboBoxIS.h : 版本 2.0 - 见文章 www.csdn.net
//
// 作者:	少莫千华  
//          
//
// 描述:
//		CComboBoxIS是由CComboBox派生的空间，支持输入时自动匹配提示，自动补全。
//		CComboBox控件基础属性
//		(Combo-box Control) IComboEditor
//		行为
//			Has Strings = True
//			Owner Draw = Fixed
//		外观
//			Type = Dropdown
//
// 历史
//     版本 1.0 - 2014-02-20
//     - 初次公开发布
//	   版本 2.0 - 2016-12-07
//	   - 增加自绘部分
//
///////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CMessg.h"
#include "CPDB.h"
//CComboBoxIS
enum MOUSE_STATE
{
	Mouse_Normal,
	Mouse_Hover,
	Mouse_Down,
	Mouse_Disable
};

class CComboBoxIS : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxIS)

public:
	CComboBoxIS();
	virtual ~CComboBoxIS();
	bool ExeSQL(CString strsql);
protected:
	DECLARE_MESSAGE_MAP()

protected://CWnd
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//afx_msg void OnMouseLeave();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

public://CWnd Virtual Msg
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	
public://CCombox
	afx_msg BOOL OnDropDown();
	afx_msg BOOL OnSelChange();
	afx_msg void OnCbnDropDown();
	afx_msg void OnCbnSelChange();
	afx_msg void OnCbnEditUpdate();

public://Call Back CCombox
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);

public:
	virtual void PreSubclassWindow();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) ;

public://Owner Draw = Fixed | Variable
	virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);

public:
	void SetNormalPositionColor(COLORREF colBackGround, COLORREF colText);
	void SetHighlightColor(COLORREF colBackGround, COLORREF colText);
	void DrawListLeft(int nHeight=0);
private:
	COLORREF m_colorFrame;
	COLORREF m_colorBackgroud;
	COLORREF m_colorText;

	COLORREF m_colorLBText;
	COLORREF m_colorLBBackgroud;
	COLORREF m_colorLBTextSelect;
	COLORREF m_colorLBBackgroudSelect;
	COLORREF m_colorArrow;
	COLORREF m_colorArrowBackground;
	COLORREF m_colorArrowBackgroundSelect;
	COLORREF m_colorArrowSelect;
private:
	CStringArray m_arrData;
	MOUSE_STATE      m_editState;
	BOOL             m_bTracking;
	CDC              m_memDC;   //选中列表的时候位图颜色，在DrawItem中，CreateCompatibleDC第二次会失败
	CListBox         m_listbox;

public://从资源界面的工具箱创建
	void Example1();

public://动态创建
	void Example2(CWnd *pParentWnd = NULL,CRect rc = CRect(100,10,300,40),UINT nID = 1001);

public://匹数数据接口
	void AddMDBString(CString str);
	void AddMDBString(CStringArray str);
	void CleanMDB();
};


#endif //_IS_CComboBoxIS_H_20161207114623_SMQH__