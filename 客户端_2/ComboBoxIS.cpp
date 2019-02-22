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
#include "stdafx.h"
#include "ComboBoxIS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//----------------------------------------------------------------------
// 描    述 : 简化函数代码，绘制下拉框下箭头
//----------------------------------------------------------------------
#define  scn(a,b) memDC.SetPixel(CPoint(orPnt.x + a,orPnt.y + b),m_colorArrow)
#define  scs(a,b) memDC.SetPixel(CPoint(orPnt.x + a,orPnt.y + b),m_colorArrowSelect)
#define fscn(an,mi,ma) for(int i = mi ; i <= ma;i++) scn(i,an);
#define fscs(an,mi,ma) for(int i = mi ; i <= ma;i++) scs(i,an);

//----------------------------------------------------------------------
// 描    述 : 简化函数代码，绘制下拉框下箭头
//----------------------------------------------------------------------
static WNDPROC m_pWndProc = 0;

//----------------------------------------------------------------------
// 描    述 : 这个CListBox空间实际的拥有者是父窗口，所有所有的消息由父窗口处理.
// 返回类型 : LRESULT
//----------------------------------------------------------------------
extern "C" LRESULT FAR PASCAL BitComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	//TRACE("BitComboBoxListBoxProc 0x%.4X\n",nMsg);
	return CallWindowProc(m_pWndProc, hWnd, nMsg, wParam, lParam);
}
IMPLEMENT_DYNAMIC(CComboBoxIS, CComboBox)

CComboBoxIS::CComboBoxIS()
{
	m_editState = Mouse_Normal;
	m_bTracking = false;
	//Color
	m_colorFrame					= RGB(0,0,0);
	m_colorText						= RGB(0,0,0);
	m_colorBackgroud				= RGB(255,255,255);

	m_colorLBBackgroud				= RGB(255,255,255);
	m_colorLBText					= RGB(0,0,0);

	m_colorLBTextSelect				= RGB(255,255,255);
	m_colorLBBackgroudSelect		= RGB(0,0,0);

	m_colorArrow					= RGB(0,0,0);
	m_colorArrowBackground			= RGB(255,255,255);

	m_colorArrowSelect				= RGB(35,35,0);
	m_colorArrowBackgroundSelect	= RGB(190,220,255);
}

CComboBoxIS::~CComboBoxIS()
{
	if(m_memDC.m_hDC)
	{
		m_memDC.DeleteDC();
	}
}


BEGIN_MESSAGE_MAP(CComboBoxIS, CComboBox)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_CTLCOLORLISTBOX,OnCtlColorListBox)

	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnCbnDropDown)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnCbnEditUpdate)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnCbnSelChange)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropDown)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()


// CComboBoxIS 消息处理程序
void CComboBoxIS::OnCbnEditUpdate()
{
	UpdateData(FALSE);
	WORD nStartChar = 0;
	WORD nEndChar = 0;
	{//获取光标位置
		DWORD dwSel = GetEditSel();
		nStartChar = LOWORD(dwSel);//低字记录了编辑控件中选中的字符串中起始位置
		nEndChar= HIWORD(dwSel);		//高字记录了紧接着选中的字符串的第一个字符的位置
	}

	{//清空下列列表
		int countnum = GetCount();
		if (countnum>0)
		{
			for (int i =0;i<countnum;i++)
			{
				DeleteString(0);
			}
		}
	}

	CString searchName;
	{//判断当前字符是否为NULL
		GetWindowText(searchName);
		if (searchName.IsEmpty() == TRUE)
		{
			m_editState = Mouse_Normal;
			TRACE(_T("On Combo-box Edit Update\n"));
			ShowDropDown(FALSE);
			Invalidate(FALSE);
			return;
		}
	}
	
	{//搜索并显示匹配的字符串
		CStringArray addString ;
		addString.SetSize(100);
		addString.RemoveAll();
		AddString(searchName);
		addString.Add(searchName);
		//添加字符串
		for (int i = 0; i <m_arrData.GetCount();i++)
		{
			CString str = m_arrData[i];
			int n = str.Find(searchName);
			CString tmp;
			tmp.Format(_T("%d"),n);
			OutputDebugString(tmp);
			OutputDebugString(_T("\n"));
			if(n>=0)
			{
				BOOL bAdd = TRUE;
				for(int j =0;j<addString.GetCount();j++)
				{
					if (str == addString[j])
					{
						bAdd = FALSE;
						break;
					}
				}
				if(bAdd == TRUE)
				{
					AddString(m_arrData[i]);
					addString.Add(m_arrData[i]);
					OutputDebugString(m_arrData[i]);
					OutputDebugString(_T("\n"));
				}
			}
		}
		if(addString.GetCount()<=1)
		{
			addString.RemoveAll();;
			int countnum = GetCount();
			if (countnum>0)
			{
				for (int i =0;i<countnum;i++)
				{
					DeleteString(0);
				}
			}
			m_editState = Mouse_Normal;
			ShowDropDown(false);
			Invalidate(FALSE);
			
			//SetEditSel(searchName.GetLength(),searchName.GetLength());
			SetEditSel(nStartChar,nEndChar); //光标跳到相应位置
			return;
		}
		addString.RemoveAll();;
	}

	{
		::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW))); //防止鼠标消失
		m_editState = Mouse_Down;
		ShowDropDown(TRUE);//显示下拉框
		//SetEditSel(searchName.GetLength(),searchName.GetLength()); //光标跳到末尾,//置光标于文本最后，防止全部选中
		SetEditSel(nStartChar,nEndChar); //恢复光标位置 光标跳到相应位置
	}
}


void CComboBoxIS::OnPaint()
{
	CPaintDC dc(this);
	CRect    rc;
	GetClientRect(&rc);

	CDC      memDC;
	CBitmap  memBmp;
	CBitmap  *pOldBmp;
	CDC      bmpDC;

	{
		memBmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());

		memDC.CreateCompatibleDC(&dc);
		pOldBmp = memDC.SelectObject(&memBmp);       //添加缓冲位图

		bmpDC.CreateCompatibleDC(&memDC);            //创建位图缓冲DC
	}

	memDC.FillSolidRect(rc, m_colorBackgroud);

	CRect inRect(rc);	CBrush   inBsh;
	inBsh.CreateSolidBrush(m_colorFrame);	memDC.FrameRect(inRect, &inBsh);

	/*CRect outRect(rc.left-1,rc.top -1,rc.right+1,rc.bottom +1);	CBrush   outBsh;
	outBsh.CreateSolidBrush(m_colorFrame);	memDC.FrameRect(outRect, &outBsh);*/
	{
		CRect rc(rc.right -18 ,2,rc.right -2,rc.Height() - 2);
		CPoint orPnt(rc.right -18,2);//18
		int yo = (rc.Height() - 4)/2+1;
		if(	m_editState == Mouse_Normal)   //贴下拉箭头
		{
			int st = 16/2;
			memDC.FillSolidRect(rc,m_colorArrowBackground);
			fscn(yo-6,st,st+0);
			fscn(yo-5,st,st+1);
			fscn(yo-4,st,st+2);
			fscn(yo-3,st,st+3);
			fscn(yo-2,st,st+4);
			fscn(yo-1,st,st+5);
			fscn(yo+0,st,st+6);
			fscn(yo+1,st,st+5);
			fscn(yo+2,st,st+4);
			fscn(yo+3,st,st+3);
			fscn(yo+4,st,st+2);
			fscn(yo+5,st,st+1);
			fscn(yo+6,st,st+0);
			
		}
		else
		{
			memDC.FillSolidRect(rc,m_colorArrowBackgroundSelect);
			fscs(yo,4,16);
			fscs(yo+1,5,15);
			fscs(yo+2,6,14);
			fscs(yo+3,7,13);
			fscs(yo+4,8,12);
			fscs(yo+5,9,11);
			fscs(yo+6,10,10);
		}
	}
	CString sz;
	GetWindowText(sz);
	//rc.left += nListBoxTextLeft;

	//在创建好CListBox后再选择字体
	/*
	if( m_listbox )
	{
		memDC.SelectObject(m_listbox.GetFont());
	}
	*/
	memDC.SelectObject(GetFont());//重新设置字体

	memDC.SetBkMode(TRANSPARENT);
	RECT rcRepaint;
	GetClientRect(&rcRepaint);
	rcRepaint.left = rcRepaint.left;
	rcRepaint.right = rcRepaint.right;
	rcRepaint.top += 3; rcRepaint.bottom -= 3;
	/*
	{
		memDC.SetTextColor(m_colorText);
		TRACE("聚焦正常的时候重新填充(rc)矩形区域\n");
	}
	*/
	sz = _T(" ") + sz;//防止文字向左边缩
	memDC.DrawText(sz, &rc, DT_VCENTER|DT_SINGLELINE/*DT_CENTER*/);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	bmpDC.DeleteDC();
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBmp.DeleteObject();

}

void CComboBoxIS::OnCbnDropDown()
{

}

BOOL CComboBoxIS::OnDropDown()
{
	int dx = 0;
	CSize sz(0,0);
	TEXTMETRIC tm;
	CString sLBText;
	CDC* pDC = GetDC();
	CFont* pFont = GetFont();
	// 选择Listbox字体,保存旧的字体
	CFont* pOldFont = pDC->SelectObject(pFont);
	// 获取文字指标中的字符平均宽度
	pDC->GetTextMetrics(&tm);

	const int nCount = GetCount();
	for(int i = 0;i < nCount;++i)
	{
		GetLBText(i,sLBText);
		sz = pDC->GetTextExtent(sLBText);
		// 为了防止覆盖，增加平均宽度
		sz.cx += tm.tmAveCharWidth;
		if(sz.cx > dx)dx = sz.cx;
	}

	// 将DC的字体改回旧字体
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	// 调整垂直滚动条和左右边框的宽度
	dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2 * ::GetSystemMetrics(SM_CXEDGE);
	if(GetDroppedWidth() < dx)SetDroppedWidth(dx);

	return Default();
}


BOOL CComboBoxIS::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect  rc;
	this->GetClientRect(&rc);
	CRect inRect(rc);

	if (m_bTracking)
	{
		/*CBrush  outBsh;
		outBsh.CreateSolidBrush(m_colorFrame);
		inRect.left -= 1;
		inRect.top -= 1;
		inRect.right += 1;
		inRect.bottom += 1;
		pDC->FrameRect(inRect, &outBsh);*/
	}
	return TRUE;
	//return CComboBox::OnEraseBkgnd(pDC);
}

LRESULT CComboBoxIS::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_editState = Mouse_Normal;
	Invalidate(FALSE);
	m_bTracking = false;
	TRACE(_T("Mouse Leave\n"));
	return S_OK;
}

LRESULT CComboBoxIS::OnMouseHover( WPARAM wParam, LPARAM lParam )
{
	m_editState = Mouse_Hover;
	Invalidate(FALSE);
	TRACE(_T("Mouse Hover\n"));
	return S_OK;
}


void CComboBoxIS::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bTracking) //鼠标不在按钮区域.
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
		//m_editState = Mouse_Hover;
		Invalidate(FALSE);
	}
	CComboBox::OnMouseMove(nFlags, point);
}

void CComboBoxIS::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_editState = Mouse_Hover;
	Invalidate(FALSE);
	CComboBox::OnLButtonUp(nFlags, point);
}

void CComboBoxIS::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_editState = Mouse_Down;
	Invalidate(FALSE);
	CComboBox::OnLButtonDown(nFlags, point);
}

void CComboBoxIS::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CComboBox::OnLButtonDblClk(nFlags, point);
}

afx_msg LRESULT CComboBoxIS::OnCtlColorListBox(WPARAM wParam, LPARAM lParam)
{
	//这里我们需要获取combobox中listbox的引用（也就是下拉显示的部分）. 我们可以使用 
	//TRACE("OnCtlColorListBox()\n");
	if (this->m_listbox.m_hWnd == 0) {
		HWND hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd) 
		{
			//保存句柄
			m_listbox.m_hWnd = hWnd;

			//子类化ListBox
			m_pWndProc = (WNDPROC)GetWindowLong(m_listbox.m_hWnd, GWL_WNDPROC);
			//SetWindowLong(m_listbox.m_hWnd, GWL_WNDPROC, (LONG)BitComboBoxListBoxProc);
			SetWindowLong(m_listbox, GWL_WNDPROC, reinterpret_cast<long>(BitComboBoxListBoxProc));
		}
	}
	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

void CComboBoxIS::OnCbnSelChange()
{
	// TODO: 在此添加控件通知处理程序代码
	m_editState = Mouse_Normal;
	int nIndex = GetCurSel();
	if(nIndex >= 0 && nIndex < GetCount())
		Invalidate(FALSE);
}

void CComboBoxIS::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
//	ModifyStyle(CBS_SORT, CBS_OWNERDRAWFIXED|CBS_DROPDOWN|CBS_HASSTRINGS);
	SetItemHeight(-1,19);
	CComboBox::PreSubclassWindow();
}

BOOL CComboBoxIS::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: 在此添加专用代码和/或调用基类
	//dwStyle &= ~0xF;
	//dwStyle |= CBS_DROPDOW;
	//dwStyle |= CBS_OWNERDRAWFIXED;
	//dwStyle |= CBS_HASSTRINGS;
	//dwStyle &= ~CBS_SORT;       //移除排序
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CComboBoxIS::OnSelChange()
{
	// TODO: 在此处添加消息处理程序代码
	int nIndex = GetCurSel();
	if(nIndex >= 0 && nIndex < GetCount())
		Invalidate(FALSE);
	return Default();
}

void CComboBoxIS::OnDestroy()
{
	// TODO: 在此处添加消息处理程序代码
	if (m_listbox.GetSafeHwnd() != NULL)
		m_listbox.UnsubclassWindow();
	CComboBox::OnDestroy();
}

void CComboBoxIS::SetNormalPositionColor( COLORREF colBackGround, COLORREF colText )
{
	//m_colBackGroundNormal = colBackGround; m_colTextNormal = colText;
}

void CComboBoxIS::SetHighlightColor( COLORREF colBackGround, COLORREF colText )
{
	//m_colBackGroundHighLight = colBackGround; m_colTextHighLight = colText;
}

void CComboBoxIS::DrawListLeft( int nHeight/*=0*/ )
{
	CRect wndRect;
	CRect rc;
	CDC MemDC;
	CDC *pDC = m_listbox.GetDC();
	MemDC.CreateCompatibleDC(pDC);
	m_listbox.GetClientRect(&rc);
	CBrush  bsh;
	bsh.CreateSolidBrush(RGB(0, 147, 209));
	rc.left -= 1;
	rc.top -= 1;
	rc.right += 1;
	rc.bottom += 1;
	pDC->FrameRect(rc, &bsh);
	bsh.DeleteObject();
}

void CComboBoxIS::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	if( lpDrawItemStruct->itemID == -1 )
		return;

	CRect       itemRect;
	CDC			dc;
	dc.Attach(lpDrawItemStruct->hDC);
	CString		sItem;

	if(m_memDC.m_hDC == NULL)
	{
		int a = m_memDC.CreateCompatibleDC(&dc);
	}

	//绘制CListBox左侧图像
	DrawListLeft();

	COLORREF dcTextColorOld ;
	itemRect = lpDrawItemStruct->rcItem;
	if((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.FillSolidRect(itemRect, m_colorLBBackgroudSelect);
		dcTextColorOld = dc.SetTextColor(m_colorLBTextSelect);
	}
	else
	{
		dc.FillSolidRect(itemRect, m_colorLBBackgroud);
		dcTextColorOld =  dc.SetTextColor(m_colorLBText);
	}

	// 将项目的文本复制到字符串
	this->GetLBText(lpDrawItemStruct->itemID, sItem);
	dc.SetBkMode(TRANSPARENT);

	// 在图像左边位置后绘制文本
	lpDrawItemStruct->rcItem.left = 2;
	dc.SelectObject(m_listbox.GetFont());
	dc.DrawText(sItem, &lpDrawItemStruct->rcItem, /*DT_CENTER|*/DT_VCENTER | DT_SINGLELINE);
	dc.SetTextColor(dcTextColorOld);
}

int CComboBoxIS::CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/)
{
	// TODO:  添加您的代码以确定指定项的排序顺序
	// 返回 -1 表示项 1 排在项 2 之前
	// 返回 0 表示项 1 和项 2 顺序相同
	// 返回 1 表示项 1 排在项 2 之后
	return 0;
}

void CComboBoxIS::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// TODO: 在此添加专用代码和/或调用基类
	CComboBox::DeleteItem(lpDeleteItemStruct);
}

void CComboBoxIS::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{
	// TODO:  添加您的代码以确定指定项的大小
}

void CComboBoxIS::Example1()
{
	CString strsql;
	strsql.Format("select [Start] from Train");
	CMessg msg;
	//显示多条数据
	msg.m_type = 'a';
	msg.m_strText = strsql;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return ;
	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		int rowcount=recmsg->m_OperState;
		int column=recmsg->m_Column;
		CString ss;
		ss.Format("%d",column);
		if (recmsg->m_OperState==0)
			return;
		else
		{
			int i =0,j=0;
			CString output,stemp;
			for(i=0;i<rowcount;i++)
			{
				for(j=0;j<column;j++)
				{
					AfxExtractSubString(output,recmsg->m_strText,i*column+j,'|');
					m_arrData.Add(output);
				}
			}
		}
	}
	
}

void CComboBoxIS::Example2(CWnd *pParentWnd,CRect rc,UINT nID)
{
	CString strsql;
	strsql.Format("select [Finish] from Train");
	CMessg msg;
	//显示多条数据
	msg.m_type = 'a';
	msg.m_strText = strsql;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return ;
	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		int rowcount=recmsg->m_OperState;
		int column=recmsg->m_Column;
		CString ss;
		ss.Format("%d",column);
		if (recmsg->m_OperState==0)
			return;
		else
		{
			int i =0,j=0;
			CString output,stemp;
			for(i=0;i<rowcount;i++)
			{
				for(j=0;j<column;j++)
				{
					AfxExtractSubString(output,recmsg->m_strText,i*column+j,'|');
					m_arrData.Add(output);
				}
			}
		}
	}
}

void CComboBoxIS::CleanMDB()
{
	m_arrData.RemoveAll();
}

void CComboBoxIS::AddMDBString( CString str )
{
	m_arrData.Add(str);
}

void CComboBoxIS::AddMDBString( CStringArray str )
{
	m_arrData.Append(str);
}

