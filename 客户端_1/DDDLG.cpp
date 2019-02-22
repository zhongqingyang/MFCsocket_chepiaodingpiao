// DDDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDB.h"
#include "DDDLG.h"
#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"
#include "CMessg.h"
#include "GQDLG.h"
// DDDLG
static int CurSelRow=0,CurSelCol=0;
const int MAXROW	=20;	//最大行数
const int MAXROW1	=4;	//最大行数
const int INFO_LEN	=300;	//每行最大长度

static int ColNum1=7;
static int ColNum2=4;
static CString ColsName1[7]={"订单号","用户","车次","出发时间","到站时间","下单时间","票价"};

static int ColWidth[7]={150,150,150,150,150,150,100};//列宽
IMPLEMENT_DYNCREATE(DDDLG, CFormView)

DDDLG::DDDLG()
	: CFormView(DDDLG::IDD)
{

}

DDDLG::~DDDLG()
{
}

void DDDLG::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_CUSTOM1, m_list);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_GridControl(pDX, IDC_CUSTOM2, m_list2);
}

BEGIN_MESSAGE_MAP(DDDLG, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &DDDLG::OnBnClickedButton1)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &DDDLG::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK,IDC_CUSTOM1, OnClickList1)
END_MESSAGE_MAP()


// DDDLG 诊断

#ifdef _DEBUG
void DDDLG::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void DDDLG::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// DDDLG 消息处理程序


void DDDLG::OnBnClickedButton1()
{
	CString str,strsql,str1,str2,str3,str4,stryp,strmoney;
	int nTemp=m_list.GetFocusCell().row;
	str=m_list.GetItemText(nTemp,0);
	str1=m_list.GetItemText(nTemp,6);
	str2=m_list.GetItemText(nTemp,2);

	str3=m_list.GetItemText(nTemp,3);
	if (str=="")
	{
		AfxMessageBox("请选择行程");
		return;
	}
	if (MessageBox("确认要取消本次行程吗？","提示信息",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
	{

	AfxExtractSubString(str4,(LPCTSTR)str3,0,' ');//分割字符串获取出发日期
	strsql.Format("delete from Oder where OrderID='%s' ",str);
	ExeSQL(strsql);
	strsql.Format("select YT  from Train where trainID='%s' and banci='%s' ",str2,str4);
	stryp=selectt(strsql);
	strsql.Format("update Train set YT='%d' where trainID='%s' and banci='%s' ",atoi(stryp)+1,str2,str4);//余票加一
	ExeSQL(strsql);
	strsql.Format("select money from [User] where UseZH='%s' " ,m_user);
	strmoney=selectt(strsql);
	strmoney.Format("%d",atoi(strmoney)+atoi(str1));

	strsql.Format("update [User] set money ='%s' where UseZH='%s' ",strmoney,m_user);//余额加
	ExeSQL(strsql);
	Clear();
	strsql.Format("select * from Oder where UserZH='%s' ",m_user);
	ShowData(strsql);
	}
}


CString DDDLG::selectt(CString strsql)//查询
{
	CString str;
	CMessg msg;
	msg.m_type = 'a';
	msg.m_strText = strsql;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return  false;
	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		int rowcount=recmsg->m_OperState;
		int column=recmsg->m_Column;
		CString ss;
		ss.Format("%d",column);
		if (recmsg->m_OperState==0)
			return false;
		else
		{
			int i =0,j=0;
			CString output,stemp;
			for(i=0;i<rowcount;i++)
			{
				for(j=0;j<column;j++)
				{
					AfxExtractSubString(output,recmsg->m_strText,i*column+j,'|');
					str=output;
				}
			}
		}
	}
	return str;
}
void DDDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();
	//m_list.ShowWindow(FALSE);
	InitList(MAXROW);//初始gridctr信息显示
	InitList2(MAXROW1);//初始gridctr信息显示
	CString strsql;
	strsql.Format("select * from Oder where UserZH='%s' ",m_user);
	ShowData(strsql);

	// TODO: 在此添加专用代码和/或调用基类
}
void DDDLG::InitList(int MaxRow)//动态显示列名
{
	TRY 
	{
		m_list.SetRowCount(MaxRow);
		m_list.SetColumnCount(ColNum1);
		m_list.SetFixedRowCount(1);//m_nFixRows=1
		m_list.SetFixedColumnCount(0);
		m_list.SetSingleRowSelection(0);
		m_list.SetEditable(FALSE);

		m_list.SetFixedBkColor(RGB(228,243,249));//标题背景颜色
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return ;
	}
	END_CATCH

		// fill rows/cols with text
		for (int row = 0; row <m_list.GetRowCount(); row++)
		{

			for (int col = 0; col < ColNum1; col++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = col;

				if(row < m_list.GetFixedRowCount())
				{
					if (col==3)
					{
						m_list.SetCellType(row,col,RUNTIME_CLASS(CGridCellDateTime));
					}
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("%s"),ColsName1[col]);
				}
				m_list.SetItem(&Item);
			}
		}

		//设置列宽
		for (int i = 0; i < ColNum1; i++)
		{
			m_list.SetColumnWidth(i,ColWidth[i]);
		}
		for (int i = 1; i < MAXROW; i++)
		{
			m_list.SetRowHeight(i,24);//设置行高
		}
		m_list.ExpandLastColumn();
		m_list.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

		m_list.Invalidate();		

}
void DDDLG::ShowData(CString strsql)
{
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
					m_list.SetItemText(i+1,j,output);//输出

				}
			}
		}
	}
	m_list.Invalidate();
}
bool DDDLG::ExeSQL(CString strsql)
{
	CMessg msg;
	msg.m_type = 'b';
	msg.m_strText = strsql;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return false;
	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		if (recmsg->m_OperState==0)
			return false;
		else
		{
			return true;
		}//else
	}
}

int DDDLG::GetRecCount(CString strsql)
{
	int num =0 ;
	CMessg msg;
	msg.m_type = 'c';
	msg.m_strText = strsql;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return 0;
	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		num = recmsg->m_OperState;
	}
	return num;
}
void DDDLG::Clear(){
	for(int i=0;i<51;i++)
	{
		for(int j=0;j<7;j++)
		{
			m_list.SetItemText(i+1,j,"");//输出

		}
	}
}

BOOL DDDLG::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\4.jpg");
	Image img(bkimg.AllocSysString(), TRUE);
	//输出图片
	bmpGraphics.DrawImage(&img, 0, 0, rect.Width(), rect.Height());
	//拷贝输出到屏幕
	Graphics graphics(pDC->m_hDC);
	CachedBitmap cachedBmp(&bmp, &graphics);
	graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
	return true;


	return CFormView::OnEraseBkgnd(pDC);
}


HBRUSH DDDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if(   CTLCOLOR_STATIC   ==   nCtlColor   )  
	{ 
		//pDC->SetBkColor(RGB(255,0,0));  //需要指定背景色的时候用 
		pDC->SetTextColor(RGB(205,   149,   12));    //文字前景色
		pDC->SetBkMode(TRANSPARENT);    //设置透明
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //获取画笔颜色混合后的画笔，完成透明 
	}  
	return hbr;
}

void DDDLG::ChangeSize()
{
	if(m_list.m_hWnd) 
	{
		CRect rect,rectts;
		GetClientRect(&rect);

		rectts.left=rect.left+210;
		rectts.right=rect.right-210;
		rectts.top=rect.bottom-400;
		rectts.bottom=rect.bottom-250;
		m_list.MoveWindow(rectts,true);

		rectts.left=rect.left+270;
		rectts.right=rect.right-270;
		rectts.top=rect.bottom-200;
		rectts.bottom=rect.bottom-40;
		m_list2.MoveWindow(rectts,true);

		rectts.left=rect.left+400;
		rectts.right=rect.left+500;
		rectts.top=rect.bottom-550;
		rectts.bottom=rect.bottom-500;
		m_button1.MoveWindow(rectts,true);

		rectts.left=rect.left+700;
		rectts.right=rect.left+800;
		rectts.top=rect.bottom-550;
		rectts.bottom=rect.bottom-500;
		m_button2.MoveWindow(rectts,true);
		m_list.ShowWindow(TRUE);
	}
}
void DDDLG::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ChangeSize();
	CFormView::ShowScrollBar(SB_BOTH, false);
}


void DDDLG::OnBnClickedButton2()
{
	CString str,strsql,str1,str2,str3,str4,stryp,strmoney;
	int nTemp=m_list.GetFocusCell().row;
	str=m_list.GetItemText(nTemp,0);
	str1=m_list.GetItemText(nTemp,6);
	str2=m_list.GetItemText(nTemp,2);

	str3=m_list.GetItemText(nTemp,3);
	if (str=="")
	{
		AfxMessageBox("请选择行程");
		return;
	}
	m_dd=str;
	GQDLG mydlg;
	if (mydlg.DoModal()==IDOK)
	{
		Clear();
		CString strsql;
		strsql.Format("select * from Oder where UserZH='%s' ",m_user);
		ShowData(strsql);
	}
	
}
void DDDLG::InitList2(int MaxRow)
{
	TRY 
	{
		m_list2.SetRowCount(MaxRow);
		m_list2.SetColumnCount(ColNum2);
		//		m_Grid.SetFixedRowCount(1);//m_nFixRows=1
		//		m_Grid.SetFixedColumnCount(0);
		m_list2.SetFixedBkColor(RGB(228,243,249));//标题背景颜色
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return ;
	}
	END_CATCH

		for (int col = 0; col <m_list2.GetColumnCount(); col++)
		{

			for (int row = 0; row < ColNum2; row++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = col;
				if(col ==0 )
				{
					m_list2.SetItemBkColour(row,col,RGB(228,243,249));
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("%s"),ColsName1[row]);
					m_list2.SetItemState(row,col, GVIS_READONLY);
				}
				else if(col==2)
				{
					m_list2.SetItemBkColour(row,col,RGB(228,243,249));
					Item.nFormat = DT_CENTER|DT_WORDBREAK;

					Item.strText.Format(_T("%s"),ColsName1[row+4]);

					m_list2.SetItemState(row,col, GVIS_READONLY);
				}
				m_list2.SetItem(&Item);

				//设置列宽
				for (int i = 0; i < ColNum1; i++)
				{
					m_list2.SetColumnWidth(i,200);
				}
				for (int i = 0; i < MAXROW; i++)
				{
					m_list2.SetRowHeight(i,40);//设置行高
				}

				m_list2.Invalidate();


			}
		}
}

void DDDLG::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW*) pNMHDR;
	//查询数据，并显示数据
	ShowAllInfo();

	*pResult = 0;
}
void DDDLG::ShowAllInfo()
{
	CString str0,str1,str2,str3,str4,str5,str6;
	int nTemp=m_list.GetFocusCell().row;
	str0=m_list.GetItemText(nTemp,0);
	str1=m_list.GetItemText(nTemp,1);
	str2=m_list.GetItemText(nTemp,2);
	str3=m_list.GetItemText(nTemp,3);
	str4=m_list.GetItemText(nTemp,4);
	str5=m_list.GetItemText(nTemp,5);
	str6=m_list.GetItemText(nTemp,6);
	m_list2.SetItemText(0,1,str0);//输出
	m_list2.SetItemText(1,1,str1);//输出
	m_list2.SetItemText(2,1,str2);//输出
	m_list2.SetItemText(3,1,str3);//输出
	m_list2.SetItemText(0,3,str4);//输出
	m_list2.SetItemText(1,3,str5);//输出
	m_list2.SetItemText(2,3,str6);//输出
	m_list2.Invalidate();
}