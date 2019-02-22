// ExtSplitter.cpp : implementation file
//
#include "stdafx.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndExt

CSplitterWndExt::CSplitterWndExt(): m_pSavedColInfo(NULL)
{
	m_cxSplitter = 4;    //must >=4 ,拖动时拖动条的宽度
	m_cySplitter = 4;    
	m_cxBorderShare = 0; //按下鼠标时拖动条的偏移量
	m_cyBorderShare = 0;  
	m_cxSplitterGap= 1;  //splitter拖动条的宽度   
	m_cySplitterGap= 1;
}

CSplitterWndExt::~CSplitterWndExt()
{
	if(m_pSavedColInfo)
		delete[] m_pSavedColInfo;
	
	if(m_pSavedRowInfo)
		delete[] m_pSavedRowInfo;
}

BOOL CSplitterWndExt::CreateStatic(CWnd* pParentWnd,
		int nRows, int nCols,
		DWORD dwStyle,
		UINT nID )
{
	BOOL ret;

	ret= CSplitterWnd::CreateStatic(pParentWnd, nRows, nCols, dwStyle , nID );
	Init();
	return ret;
}
BOOL CSplitterWndExt::CreateView( int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext )
{
	BOOL ret = CSplitterWnd::CreateView(row, col, pViewClass, sizeInit, pContext );

	CWnd* pWnd = GetPane(row,col);
	ASSERT(pWnd);

	m_pane_ptr_array(row,col)= pWnd;

	return ret;
}

void CSplitterWndExt::Init()
{
	int i,j;

	m_pane_ptr_array.Init(m_nMaxRows,m_nMaxCols);

	for(i=0; i<m_nMaxRows; i++)
	{
		m_shown_rows.push_back(i);
	}

	for(j=0; j<m_nMaxCols; j++)
	{
		m_shown_cols.push_back(j);
	}

	m_pSavedColInfo = new CRowColInfo[m_nMaxCols];
	m_pSavedRowInfo = new CRowColInfo[m_nMaxRows];
}


void CSplitterWndExt::ShowColumn(int col)
{
     ASSERT_VALID(this);
     ASSERT(m_nCols < m_nMaxCols);

	std::list<int>::iterator col_pos;  

	col_pos = std::find(m_shown_cols.begin(),m_shown_cols.end(),col);
	
	if(col_pos!=m_shown_cols.end())
	{
		ASSERT(0);
		return;
	}

	int place = AbsToRelPosition(m_shown_cols, col);

	m_nCols++;  // add a column
	
	m_shown_cols.push_back(col);
	m_shown_cols.sort();
	m_hid_cols.remove(col);

	RenumeratePanes();

	RestoreColInfo(place,col);
	RecalcLayout();
}

int CSplitterWndExt::AbsToRelPosition(LIST_INT &lists, int value)
{
	LIST_INT::iterator pos;
	int index = -1;

	pos = std::find_if(lists.begin(),lists.end(), std::bind2nd(std::greater<int>(),value));

	if(pos!=lists.end())
	{
		index = std::distance(lists.begin(),pos);
	}
	else
	{
		index = lists.size();
	}

	return index;
}

void CSplitterWndExt::RemoveColInfo(int place,int col)
{
	ASSERT(m_nCols<=m_nMaxCols);

	m_pSavedColInfo[col] = m_pColInfo[place];

	for(int i=place; i<m_nCols; i++)
	{
		m_pColInfo[i] = m_pColInfo[i+1];
	}
}

void CSplitterWndExt::RestoreColInfo(int place,int col)
{
	ASSERT(m_nCols<=m_nMaxCols);

	for(int i = m_nCols-1; i>=place + 1; i--)
	{
		m_pColInfo[i] = m_pColInfo[i-1];
	}

	m_pColInfo[place] = m_pSavedColInfo[col];
}

void CSplitterWndExt::HideColumn(int colHide)
{
	ASSERT_VALID(this);
	ASSERT(m_nCols > 1);
	ASSERT(colHide < m_nMaxCols);

	std::list<int>::iterator col_pos;  

	col_pos = std::find(m_shown_cols.begin(),m_shown_cols.end(),colHide);
	
	if(col_pos==m_shown_cols.end())
	{
		ASSERT(0);
		return;
	}

	int place = std::distance(m_shown_cols.begin(),col_pos);
	
	// if the column has an active window -- change it
    int rowActive, colActive;
    if (GetActivePane(&rowActive, &colActive) != NULL &&
         colActive == place)
    {
          if (++colActive >= m_nCols)
              colActive = 0;
          SetActivePane(rowActive, colActive);
    }

	m_shown_cols.remove(colHide);
	m_hid_cols.push_back(colHide);

    m_nCols--;

	RenumeratePanes();

	RemoveColInfo(place,colHide);

    RecalcLayout();
}


CPoint CSplitterWndExt::RelToAbsPosition(int row, int col)
{
	CPoint pos;

	pos.x = RelToAbsPosition(m_shown_rows, m_hid_rows, row);
	pos.y = RelToAbsPosition(m_shown_cols, m_hid_cols, col);

	return pos;
}

int CSplitterWndExt::RelToAbsPosition(LIST_INT &vis_list, LIST_INT &hid_list, int cur_index)
{
	int org_index;

	int i;
	LIST_INT::iterator it;
	if( cur_index < vis_list.size())
	{
		it=vis_list.begin();
		for(i=0;i<cur_index; i++)
		{
			it++;
		}

		org_index = *it;
	}
	else
	{
		it=hid_list.begin();
		for(i=0;i<cur_index-vis_list.size(); i++)
		{
			it++;
		}

		org_index = *it;
	}

	return org_index;
}


BOOL CSplitterWndExt::IsPaneVisible(int row, int col)
{
	BOOL  bRow, bCol;

	ASSERT(m_nRows<=m_nMaxCols);

	if( m_shown_rows.end() != std::find(m_shown_rows.begin(),m_shown_rows.end(),row))
	{
		bRow = TRUE;	
	}
	else
	{
		bRow=FALSE;
	}

	ASSERT(m_nCols<=m_nMaxCols);

	if( m_shown_cols.end() != std::find(m_shown_cols.begin(),m_shown_cols.end(),col))
	{
		bCol = TRUE;	
	}
	else
	{
		bCol=FALSE;
	}

	return bRow && bCol;
}

void CSplitterWndExt::RenumeratePanes()
{
		int i,j,id;

	for(i=0; i<m_nMaxRows; i++)
	{
		for(j=0;j<m_nMaxCols;j++)
		{
			CPoint pos	= RelToAbsPosition(i,j);
			CWnd* pPane = (CWnd*) m_pane_ptr_array(pos.x, pos.y);
			ASSERT(pPane != NULL);
				
			id=AFX_IDW_PANE_FIRST + i * 16 + j;
					
			int r=pPane->SetDlgCtrlID(id);
			ASSERT(r);

			if(IsPaneVisible(pos.x,pos.y))
				pPane->ShowWindow(SW_SHOW);
			else
				pPane->ShowWindow(SW_HIDE);

		}

	}

}

void CSplitterWndExt::HideRow(int rowHide)
{
	ASSERT_VALID(this);
	ASSERT(m_nCols > 1);
	ASSERT(rowHide < m_nMaxCols);

	std::list<int>::iterator row_pos;  

	row_pos = std::find(m_shown_rows.begin(),m_shown_rows.end(),rowHide);
	
	if(row_pos==m_shown_rows.end())
	{
		ASSERT(0);
		return;
	}

	int place = std::distance(m_shown_rows.begin(),row_pos);
	
	// if the column has an active window -- change it
    int rowActive, colActive;
    if (GetActivePane(&rowActive, &colActive) != NULL &&
         rowActive == place)
    {
          if (++rowActive >= m_nRows)
              rowActive = 0;
          SetActivePane(rowActive, colActive);
    }


	m_shown_rows.remove(rowHide);
	m_hid_rows.push_back(rowHide);


    m_nRows--;

	RenumeratePanes();

	RemoveRowInfo(place,rowHide);

    RecalcLayout();
}

void CSplitterWndExt::RemoveRowInfo(int place,int col)
{
	ASSERT(m_nRows<=m_nMaxRows);

	m_pSavedRowInfo[col] = m_pRowInfo[place];

	for(int i=place; i<m_nRows; i++)
	{
		m_pRowInfo[i] = m_pRowInfo[i+1];
	}
}

void CSplitterWndExt::ShowRow(int row)
{
     ASSERT_VALID(this);
     ASSERT(m_nRows < m_nMaxRows);

	std::list<int>::iterator row_pos;  

	row_pos = std::find(m_shown_rows.begin(),m_shown_rows.end(),row);
	
	if(row_pos!=m_shown_rows.end())
	{
		ASSERT(0);
		return;
	}

	int place = AbsToRelPosition(m_shown_rows, row);

	m_nRows++;  // add a column
	
	m_shown_rows.push_back(row);
	m_shown_rows.sort();
	m_hid_rows.remove(row);

	RenumeratePanes();

	RestoreRowInfo(place, row);
	RecalcLayout();
}

void CSplitterWndExt::RestoreRowInfo(int place,int col)
{
	ASSERT(m_nRows<=m_nMaxRows);

	for(int i = m_nRows-1; i>=place + 1; i--)
	{
		m_pRowInfo[i] = m_pRowInfo[i-1];
	}

	m_pRowInfo[place] = m_pSavedRowInfo[col];
}


BEGIN_MESSAGE_MAP(CSplitterWndExt, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitterWndExt)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndExt message handlers


void CSplitterWndExt::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CSplitterWnd::OnLButtonDown(nFlags, point);
}


void CSplitterWndExt::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CSplitterWnd::OnLButtonUp(nFlags, point);
}


void CSplitterWndExt::OnMove(int x, int y)
{
	//CSplitterWnd::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
}


void CSplitterWndExt::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CSplitterWnd::OnMouseMove(nFlags, point);
}
