#ifndef _IS_CComboBoxIS_H_20161207114623_SMQH__
#define _IS_CComboBoxIS_H_20161207114623_SMQH__
//////////////////////////////////////////////////////////////////////////
//
// ComboBoxIS.h : �汾 2.0 - ������ www.csdn.net
//
// ����:	��Īǧ��  
//          
//
// ����:
//		CComboBoxIS����CComboBox�����Ŀռ䣬֧������ʱ�Զ�ƥ����ʾ���Զ���ȫ��
//		CComboBox�ؼ���������
//		(Combo-box Control) IComboEditor
//		��Ϊ
//			Has Strings = True
//			Owner Draw = Fixed
//		���
//			Type = Dropdown
//
// ��ʷ
//     �汾 1.0 - 2014-02-20
//     - ���ι�������
//	   �汾 2.0 - 2016-12-07
//	   - �����Ի沿��
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
	CDC              m_memDC;   //ѡ���б��ʱ��λͼ��ɫ����DrawItem�У�CreateCompatibleDC�ڶ��λ�ʧ��
	CListBox         m_listbox;

public://����Դ����Ĺ����䴴��
	void Example1();

public://��̬����
	void Example2(CWnd *pParentWnd = NULL,CRect rc = CRect(100,10,300,40),UINT nID = 1001);

public://ƥ�����ݽӿ�
	void AddMDBString(CString str);
	void AddMDBString(CStringArray str);
	void CleanMDB();
};


#endif //_IS_CComboBoxIS_H_20161207114623_SMQH__