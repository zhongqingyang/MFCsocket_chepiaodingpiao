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
#include "stdafx.h"
#include "ComboBoxIS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//----------------------------------------------------------------------
// ��    �� : �򻯺������룬�����������¼�ͷ
//----------------------------------------------------------------------
#define  scn(a,b) memDC.SetPixel(CPoint(orPnt.x + a,orPnt.y + b),m_colorArrow)
#define  scs(a,b) memDC.SetPixel(CPoint(orPnt.x + a,orPnt.y + b),m_colorArrowSelect)
#define fscn(an,mi,ma) for(int i = mi ; i <= ma;i++) scn(i,an);
#define fscs(an,mi,ma) for(int i = mi ; i <= ma;i++) scs(i,an);

//----------------------------------------------------------------------
// ��    �� : �򻯺������룬�����������¼�ͷ
//----------------------------------------------------------------------
static WNDPROC m_pWndProc = 0;

//----------------------------------------------------------------------
// ��    �� : ���CListBox�ռ�ʵ�ʵ�ӵ�����Ǹ����ڣ��������е���Ϣ�ɸ����ڴ���.
// �������� : LRESULT
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


// CComboBoxIS ��Ϣ�������
void CComboBoxIS::OnCbnEditUpdate()
{
	UpdateData(FALSE);
	WORD nStartChar = 0;
	WORD nEndChar = 0;
	{//��ȡ���λ��
		DWORD dwSel = GetEditSel();
		nStartChar = LOWORD(dwSel);//���ּ�¼�˱༭�ؼ���ѡ�е��ַ�������ʼλ��
		nEndChar= HIWORD(dwSel);		//���ּ�¼�˽�����ѡ�е��ַ����ĵ�һ���ַ���λ��
	}

	{//��������б�
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
	{//�жϵ�ǰ�ַ��Ƿ�ΪNULL
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
	
	{//��������ʾƥ����ַ���
		CStringArray addString ;
		addString.SetSize(100);
		addString.RemoveAll();
		AddString(searchName);
		addString.Add(searchName);
		//����ַ���
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
			SetEditSel(nStartChar,nEndChar); //���������Ӧλ��
			return;
		}
		addString.RemoveAll();;
	}

	{
		::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW))); //��ֹ�����ʧ
		m_editState = Mouse_Down;
		ShowDropDown(TRUE);//��ʾ������
		//SetEditSel(searchName.GetLength(),searchName.GetLength()); //�������ĩβ,//�ù�����ı���󣬷�ֹȫ��ѡ��
		SetEditSel(nStartChar,nEndChar); //�ָ����λ�� ���������Ӧλ��
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
		pOldBmp = memDC.SelectObject(&memBmp);       //��ӻ���λͼ

		bmpDC.CreateCompatibleDC(&memDC);            //����λͼ����DC
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
		if(	m_editState == Mouse_Normal)   //��������ͷ
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

	//�ڴ�����CListBox����ѡ������
	/*
	if( m_listbox )
	{
		memDC.SelectObject(m_listbox.GetFont());
	}
	*/
	memDC.SelectObject(GetFont());//������������

	memDC.SetBkMode(TRANSPARENT);
	RECT rcRepaint;
	GetClientRect(&rcRepaint);
	rcRepaint.left = rcRepaint.left;
	rcRepaint.right = rcRepaint.right;
	rcRepaint.top += 3; rcRepaint.bottom -= 3;
	/*
	{
		memDC.SetTextColor(m_colorText);
		TRACE("�۽�������ʱ���������(rc)��������\n");
	}
	*/
	sz = _T(" ") + sz;//��ֹ�����������
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
	// ѡ��Listbox����,����ɵ�����
	CFont* pOldFont = pDC->SelectObject(pFont);
	// ��ȡ����ָ���е��ַ�ƽ�����
	pDC->GetTextMetrics(&tm);

	const int nCount = GetCount();
	for(int i = 0;i < nCount;++i)
	{
		GetLBText(i,sLBText);
		sz = pDC->GetTextExtent(sLBText);
		// Ϊ�˷�ֹ���ǣ�����ƽ�����
		sz.cx += tm.tmAveCharWidth;
		if(sz.cx > dx)dx = sz.cx;
	}

	// ��DC������Ļؾ�����
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	// ������ֱ�����������ұ߿�Ŀ��
	dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2 * ::GetSystemMetrics(SM_CXEDGE);
	if(GetDroppedWidth() < dx)SetDroppedWidth(dx);

	return Default();
}


BOOL CComboBoxIS::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_bTracking) //��겻�ڰ�ť����.
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_editState = Mouse_Hover;
	Invalidate(FALSE);
	CComboBox::OnLButtonUp(nFlags, point);
}

void CComboBoxIS::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_editState = Mouse_Down;
	Invalidate(FALSE);
	CComboBox::OnLButtonDown(nFlags, point);
}

void CComboBoxIS::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CComboBox::OnLButtonDblClk(nFlags, point);
}

afx_msg LRESULT CComboBoxIS::OnCtlColorListBox(WPARAM wParam, LPARAM lParam)
{
	//����������Ҫ��ȡcombobox��listbox�����ã�Ҳ����������ʾ�Ĳ��֣�. ���ǿ���ʹ�� 
	//TRACE("OnCtlColorListBox()\n");
	if (this->m_listbox.m_hWnd == 0) {
		HWND hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd) 
		{
			//������
			m_listbox.m_hWnd = hWnd;

			//���໯ListBox
			m_pWndProc = (WNDPROC)GetWindowLong(m_listbox.m_hWnd, GWL_WNDPROC);
			//SetWindowLong(m_listbox.m_hWnd, GWL_WNDPROC, (LONG)BitComboBoxListBoxProc);
			SetWindowLong(m_listbox, GWL_WNDPROC, reinterpret_cast<long>(BitComboBoxListBoxProc));
		}
	}
	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

void CComboBoxIS::OnCbnSelChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_editState = Mouse_Normal;
	int nIndex = GetCurSel();
	if(nIndex >= 0 && nIndex < GetCount())
		Invalidate(FALSE);
}

void CComboBoxIS::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
//	ModifyStyle(CBS_SORT, CBS_OWNERDRAWFIXED|CBS_DROPDOWN|CBS_HASSTRINGS);
	SetItemHeight(-1,19);
	CComboBox::PreSubclassWindow();
}

BOOL CComboBoxIS::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: �ڴ����ר�ô����/����û���
	//dwStyle &= ~0xF;
	//dwStyle |= CBS_DROPDOW;
	//dwStyle |= CBS_OWNERDRAWFIXED;
	//dwStyle |= CBS_HASSTRINGS;
	//dwStyle &= ~CBS_SORT;       //�Ƴ�����
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CComboBoxIS::OnSelChange()
{
	// TODO: �ڴ˴������Ϣ����������
	int nIndex = GetCurSel();
	if(nIndex >= 0 && nIndex < GetCount())
		Invalidate(FALSE);
	return Default();
}

void CComboBoxIS::OnDestroy()
{
	// TODO: �ڴ˴������Ϣ����������
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
	// TODO:  ������Ĵ����Ի���ָ����
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

	//����CListBox���ͼ��
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

	// ����Ŀ���ı����Ƶ��ַ���
	this->GetLBText(lpDrawItemStruct->itemID, sItem);
	dc.SetBkMode(TRANSPARENT);

	// ��ͼ�����λ�ú�����ı�
	lpDrawItemStruct->rcItem.left = 2;
	dc.SelectObject(m_listbox.GetFont());
	dc.DrawText(sItem, &lpDrawItemStruct->rcItem, /*DT_CENTER|*/DT_VCENTER | DT_SINGLELINE);
	dc.SetTextColor(dcTextColorOld);
}

int CComboBoxIS::CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/)
{
	// TODO:  ������Ĵ�����ȷ��ָ���������˳��
	// ���� -1 ��ʾ�� 1 ������ 2 ֮ǰ
	// ���� 0 ��ʾ�� 1 ���� 2 ˳����ͬ
	// ���� 1 ��ʾ�� 1 ������ 2 ֮��
	return 0;
}

void CComboBoxIS::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// TODO: �ڴ����ר�ô����/����û���
	CComboBox::DeleteItem(lpDeleteItemStruct);
}

void CComboBoxIS::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{
	// TODO:  ������Ĵ�����ȷ��ָ����Ĵ�С
}

void CComboBoxIS::Example1()
{
	CString strsql;
	strsql.Format("select [Start] from Train");
	CMessg msg;
	//��ʾ��������
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
	//��ʾ��������
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

