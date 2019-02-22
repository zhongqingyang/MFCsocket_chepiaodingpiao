/************************************************************************/
/*  �̳�CSplitterWnd�࣬��дMove��LButtonDown��LButtonUp������			*/
/*	�Ӷ�ʵ�ַָ���ͼ�Ĺ̶�												*/
/************************************************************************/

#if !defined(AFX_EXTSPLITTER_H__BB454230_2E7D_4794_BE28_A5020F3B054C__INCLUDED_)
#define AFX_EXTSPLITTER_H__BB454230_2E7D_4794_BE28_A5020F3B054C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtSplitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndExt frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <list>
#include <algorithm>
#include "2DArray.h"

class CSplitterWndExt : public CSplitterWnd
{

public:
	typedef std::list<int> LIST_INT;

	CSplitterWndExt();           

	virtual ~CSplitterWndExt();

	BOOL	CreateStatic(CWnd* pParentWnd,
			int nRows, int nCols,
			DWORD dwStyle = WS_CHILD | WS_VISIBLE,
			UINT nID = AFX_IDW_PANE_FIRST);
	
	virtual BOOL CreateView( int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext );

	void	HideColumn(int colHide);
	void	ShowColumn(int colShow);
	void	HideRow(int colRow);
	void	ShowRow(int row);

protected:

	void	Init();
	int		AbsToRelPosition(LIST_INT &list, int col);
	void	RestoreColInfo(int place,int col);
	void	RestoreRowInfo(int place,int col);
	void	RemoveColInfo(int place,int col);
	void	RemoveRowInfo(int place,int col);
	void	RenumeratePanes();
	CPoint	RelToAbsPosition(int i, int j);
	int		RelToAbsPosition(LIST_INT &vis_list, LIST_INT &hid_list, int cur_index);
	BOOL	IsPaneVisible(int row, int col);



public:	
		
	LIST_INT m_shown_cols;		//shown  column list
	LIST_INT m_hid_cols;		//hidden column list
	
	LIST_INT m_shown_rows;		//shown  rows list
	LIST_INT m_hid_rows;		//hidden rows list

protected:
	C2DArray m_pane_ptr_array;	// array of pointers to splitter panes

	CRowColInfo* m_pSavedColInfo;
	CRowColInfo* m_pSavedRowInfo;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterWndExt)
	protected:
	//}}AFX_VIRTUAL


	// Generated message map functions
	//{{AFX_MSG(CSplitterWndExt)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTSPLITTER_H__BB454230_2E7D_4794_BE28_A5020F3B054C__INCLUDED_)
