
// CPDBView.cpp : CCPDBView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CPDB.h"
#endif

#include "CPDBDoc.h"
#include "CPDBView.h"
#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"
#include "CMessg.h"
#include "GRDLG.h"
#include "MainFrm.h"
#include "DinDLG.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int CurSelRow=0,CurSelCol=0;
const int MAXROW	=20;	//最大行数
const int MAXROW1	=4;	//最大行数
const int INFO_LEN	=300;	//每行最大长度

static int ColNum1=8;
static int ColNum2=8;
static CString ColsName1[8]={"车次","出发站","终点站","出发时间","到达时间","余票","票价","出发日期"};

static int ColWidth[8]={120,120,120,120,120,120,120,120};//列宽
// CCPDBView

IMPLEMENT_DYNCREATE(CCPDBView, CFormView)

BEGIN_MESSAGE_MAP(CCPDBView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, &CCPDBView::OnBnClickedButton1)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON2, &CCPDBView::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK,IDC_CUSTOM1, OnClickList1)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CCPDBView 构造/析构

CCPDBView::CCPDBView()
	: CFormView(CCPDBView::IDD)
{
	//  m_Begin = _T("");
	//  m_End = _T("");
	// TODO: 在此处添加构造代码

}

CCPDBView::~CCPDBView()
{
}

void CCPDBView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl);
	DDX_GridControl(pDX, IDC_CUSTOM1, m_list);
	DDX_GridControl(pDX, IDC_CUSTOM2, m_list2);
	//  DDX_Text(pDX, IDC_EDIT1, m_Begin);
	//  DDX_Text(pDX, IDC_EDIT2, m_End);
	DDX_Control(pDX, IDC_x1, m_x1);
	DDX_Control(pDX, IDC_x2, m_x2);
	DDX_Control(pDX, IDC_x3, m_x3);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_COMBO1, m_yfComBox);
	DDX_Control(pDX, IDC_COMBO2, m_yfComBox1);
}

BOOL CCPDBView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CCPDBView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	///GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();
	//GDI+初始化
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	m_list.ShowWindow(FALSE);
	ChangeSize();
	m_list.ShowWindow(TRUE);
	InitList(MAXROW);//初始gridctr信息显示
    InitList2(MAXROW1);//初始gridctr信息显示
	// TODO:  在此添加额外的初始化

	m_yfComBox.Example1();
	DWORD	dwStyle =m_yfComBox.GetStyle();
	dwStyle ;
	CString str;
	str.Format(_T("0x %08x"),dwStyle);
	OutputDebugString(str);
	m_yfComBox1.Example2();
	dwStyle =m_yfComBox1.GetStyle();
	dwStyle ;
	str.Format(_T("0x %08x"),dwStyle);
	OutputDebugString(str);
}

void CCPDBView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCPDBView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCPDBView 诊断

#ifdef _DEBUG
void CCPDBView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCPDBView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCPDBDoc* CCPDBView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCPDBDoc)));
	return (CCPDBDoc*)m_pDocument;
}
#endif //_DEBUG


// CCPDBView 消息处理程序


void CCPDBView::OnBnClickedButton1()
{
	UpdateData();
	CString str1;
	//获取时间
	CTime m_date;
	m_dateCtrl.GetTime(m_date);
	int year=m_date.GetYear();
	int month = m_date.GetMonth();
	int day = m_date.GetDay();
	CTime time;
	time=CTime::GetCurrentTime();
	CString curdata=time.Format("%Y-%m-%d%H:%M:%S");
	str1.Format("%d-""%d-""%d ",year,month,day);
	CString year1=time.Format("%Y");
	CString month1=time.Format("%m");
	CString day1=time.Format("%d");
	if(atoi(year1)>year)
	{
		AfxMessageBox("请选择当前日期之后的车次");
		return;
	}
	if(month<atoi(month1))
	{
		AfxMessageBox("请选择当前日期之后的车次");
		return;
	}
	if(month==atoi(month1)&&day<atoi(day1))
	{
		AfxMessageBox("请选择当前日期之后的车次");
		return;
	}
	CString strbegin,strend;
	GetDlgItem(IDC_COMBO1)->GetWindowText(strbegin);
	GetDlgItem(IDC_COMBO2)->GetWindowText(strend);
	CString strsql;
	strsql.Format("select [trainID],[Start],[Finish],[GoTime],[OverTime],[YT],[Price],[banci]  from Train where Start='%s' and Finish='%s' and banci='%s' ",strbegin,strend,str1);
	if (GetRecCount(strsql)==0)
	{
		AfxMessageBox("没有该列车");
	}
	Clear();
	ShowData(strsql);
	// TODO: 在此添加控件通知处理程序代码
}
void CCPDBView::InitList(int MaxRow)//动态显示列名
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
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("%s"),ColsName1[col]);
				}
				m_list.SetItem(&Item);

				//日期格式单元格
				/*if(row>0)//第一行信息
				{
					if(col==0)
					{
						//日期
						//m_Grid.SetCellType(row,col, RUNTIME_CLASS(CGridCellDateTime));
					}
					if(col==2)
					{
						//组合框格式单元格初始化
						m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));
						CGridCellCombo *pCell=(CGridCellCombo *)m_list.GetCell(row,col);
						CStringArray ar;
						ar.SetSize(2);
						ar.SetAt(0,"M");
						ar.SetAt(1,"F");
						pCell->SetOptions(ar);

					}
					if(col==3)
					{
						//数字
						m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellNumeric));
					}
					if(col==4)
					{
						//组合框格式单元格初始化
						m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));
						CGridCellCombo *pCell=(CGridCellCombo *)m_list.GetCell(row,col);
						CStringArray ar;
						ar.SetSize(3);
						ar.SetAt(0,"数理学院");
						ar.SetAt(1,"信息学院");
						ar.SetAt(2,"体育学院");
						pCell->SetOptions(ar);

					}

				}*/

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
void CCPDBView::ShowData(CString strsql)
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
bool CCPDBView::ExeSQL(CString strsql)
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

int CCPDBView::GetRecCount(CString strsql)
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
void CCPDBView::Clear(){
	for(int i=0;i<51;i++)
	{
		for(int j=0;j<8;j++)
		{
			m_list.SetItemText(i+1,j,"");//输出

		}
	}
}

BOOL CCPDBView::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\7.jpg");
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


void CCPDBView::OnBnClickedButton2()
{
	CString strmoney;CString str,strsql,str1,str2,str3,str4,str5,str6,stryp,str7,str8,str9,str10;
	int nTemp=m_list.GetFocusCell().row;

	str=m_list.GetItemText(nTemp,0);
	str2=m_list.GetItemText(nTemp,3);
	str4=m_list.GetItemText(nTemp,4);
	stryp=m_list.GetItemText(nTemp,5);
	str8=m_list.GetItemText(nTemp,6);
	if (str=="")
	{
		AfxMessageBox("请选择列车");
		return;
	}
	if (MessageBox("请确认乘坐本次列车","提示信息",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
	{
		strsql.Format("select money from [User] where UseZH='%s' " ,m_user);
		strmoney=selectt(strsql);
		strmoney.Format("%d",atoi(strmoney)-atoi(str8));
		if (atoi(strmoney)-atoi(str8)<0)
		{
			AfxMessageBox("余额不足，请前往充值");
			return;
		}
		CTime m_date;
		CTime time;
		time = CTime::GetCurrentTime();
		CString curdata = time.Format("%Y-%m-%d %H:%M:%S ");


		m_dateCtrl.GetTime(m_date);
		int year=m_date.GetYear();
		int month = m_date.GetMonth();
		int day = m_date.GetDay();
		str1.Format("%d-""%d-""%d ",year,month,day);
		str7=str1;
		str3=str1;
		str1+=str2;
		str3+=str4;

		strsql.Format("select [Price] from train where trainID='%s' ",str);


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
						str6=output;
					}
				}
			}
		}
		strsql.Format("select * from Oder where [UserZH]='%s' and [trainID]='%s' and [trainBegin]='%s' and [trainEnd]='%s'",m_user,str,str1,str3);
		if (GetRecCount(strsql)!=0)
		{
			AfxMessageBox("您已购买该票。请勿重复购票");
			return;
		}
		strsql.Format("select top 1 [Seat] from Oder where  [trainID]='%s' and [trainBegin]='%s' and [trainEnd]='%s' order by OrderID desc",str,str1,str3);
		AfxExtractSubString(str9,Seat(strsql),0,',');
		AfxExtractSubString(str10,Seat(strsql),1,',');
		if (str10=="100")
		{
			str10="1";
			str9.Format("%d",atoi(str9)+1); 
		}
		else{
			str10.Format("%d",atoi(str10)+1);
		}
		str10=str9+","+str10;
		m_seat=str10;
		m_begin=str1;
		m_trainID=str;
		strsql.Format("insert into Oder ([UserZH],[trainID],[trainBegin],[trainEnd],[OrderTime],[Price],[Seat])values('%s','%s','%s','%s','%s','%s','%s') ",m_user,str,str1,str3,curdata,str6,str10);//订单
		ExeSQL(strsql);
		strsql.Format("update Train set YT='%d' where trainID='%s' and banci='%s' ",atoi(stryp)-1,str,str7);//余票减一
		ExeSQL(strsql);
	


		strsql.Format("update [User] set money ='%s' where UseZH='%s' ",strmoney,m_user);//余额减
		ExeSQL(strsql);
		UpdateData();
		Clear();
		CString strbegin,strend;
		GetDlgItem(IDC_COMBO1)->GetWindowText(strbegin);
		GetDlgItem(IDC_COMBO2)->GetWindowText(strend);
		strsql.Format("select [trainID],[Start],[Finish],[GoTime],[OverTime],[YT],[Price],[banci]  from Train where Start='%s' and Finish='%s' and banci='%s' ",strbegin,strend,str7);
		ShowData(strsql);
			}	AfxMessageBox("订单已生成");
	DinDLG mydlg;
	mydlg.DoModal();

	}
HBRUSH CCPDBView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CCPDBView::ChangeSize()
{
	if(m_list.m_hWnd) 
	{
		CRect rect,rectts;
		GetClientRect(&rect);

		rectts.left=rect.left;
		rectts.right=rect.right;
		rectts.top=rect.bottom-400;
		rectts.bottom=rect.bottom-250;
		m_list.MoveWindow(rectts,true);

		rectts.left=rect.left;
		rectts.right=rect.right;
		rectts.top=rect.bottom-200;
		rectts.bottom=rect.bottom;
		m_list2.MoveWindow(rectts,true);
		/*rectts.left=rect.left+50;
		rectts.right=rect.left+150;
		rectts.top=rect.bottom-550;
		rectts.bottom=rect.bottom-500;
		m_x1.MoveWindow(rectts,true);*/

		/*rectts.left=rect.left+50;
		rectts.right=rect.left+150;
		rectts.top=rect.bottom-550;
		rectts.bottom=rect.bottom-500;
		m_Begin.MoveWindow(rectts,true);*/

		m_list.ShowWindow(TRUE);
	}
}

void CCPDBView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ChangeSize();
	CFormView::ShowScrollBar(SB_BOTH, false);
	// TODO: 在此处添加消息处理程序代码
}
CString CCPDBView::selectt(CString strsql)//查询
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

void CCPDBView::InitList2(int MaxRow)
{
	TRY 
	{
		m_list2.SetRowCount(MaxRow);
		m_list2.SetColumnCount(ColNum1);
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

				//日期格式单元格
				/*if(col==1)
				{
					m_list2.SetCellType(3,col, RUNTIME_CLASS(CGridCellCombo));
					CGridCellCombo *pCell=(CGridCellCombo *)m_list2.GetCell(3,col);
					CStringArray ar;
					ar.SetSize(5);
					ar.SetAt(0,"江苏省常州市");
					ar.SetAt(1,"江苏省苏州市");
					ar.SetAt(2,"江苏省南京市");
					ar.SetAt(3,"江苏省南通市");
					ar.SetAt(4,"江苏省无锡市");
					pCell->SetOptions(ar);
				}
				if(col==3)
				{
					m_list2.SetCellType(0,col, RUNTIME_CLASS(CGridCellCombo));
					CGridCellCombo *pCell=(CGridCellCombo *)m_list2.GetCell(0,col);
					CStringArray ar;
					ar.SetSize(5);
					ar.SetAt(0,"江苏省常州市");
					ar.SetAt(1,"江苏省苏州市");
					ar.SetAt(2,"江苏省南京市");
					ar.SetAt(3,"江苏省南通市");
					ar.SetAt(4,"江苏省无锡市");
					pCell->SetOptions(ar);
					m_list2.SetCellType(2,col, RUNTIME_CLASS(CGridCellDateTime));
					//					CGridCellCombo *pCell=(CGridCellCombo *)m_list2.GetCell(2,col);

				}
				m_list2.SetCellType(4,3, RUNTIME_CLASS(CGridCellCombo));
				CGridCellCombo *pCell=(CGridCellCombo *)m_list2.GetCell(4,3);
				CStringArray ar;
				ar.SetSize(2);
				ar.SetAt(0,"已签收");
				ar.SetAt(1,"未签收");
				pCell->SetOptions(ar);



				m_list2.ExpandLastColumn();
				m_list2.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);
				*/
				//设置列宽
				for (int i = 0; i < ColNum1; i++)
				{
					m_list2.SetColumnWidth(i,270);
				}
				for (int i = 0; i < MAXROW; i++)
				{
					m_list2.SetRowHeight(i,40);//设置行高
				}

				m_list2.Invalidate();


			}
		}
}
void CCPDBView::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW*) pNMHDR;
	//查询数据，并显示数据
	ShowAllInfo();

	*pResult = 0;
}
void CCPDBView::ShowAllInfo()
{
	//Clear();
	CString str0,str1,str2,str3,str4,str5,str6,str7;
	int nTemp=m_list.GetFocusCell().row;
	str0=m_list.GetItemText(nTemp,0);
	str1=m_list.GetItemText(nTemp,1);
	str2=m_list.GetItemText(nTemp,2);
	str3=m_list.GetItemText(nTemp,3);
	str4=m_list.GetItemText(nTemp,4);
	str5=m_list.GetItemText(nTemp,5);
	str6=m_list.GetItemText(nTemp,6);
	str7=m_list.GetItemText(nTemp,7);
	m_list2.SetItemText(0,1,str0);//输出
	m_list2.SetItemText(1,1,str1);//输出
	m_list2.SetItemText(2,1,str2);//输出
	m_list2.SetItemText(3,1,str3);//输出
	m_list2.SetItemText(0,3,str4);//输出
	m_list2.SetItemText(1,3,str5);//输出
	m_list2.SetItemText(2,3,str6);//输出
	m_list2.SetItemText(3,3,str7);//输出
	m_list2.Invalidate();

}


HCURSOR CCPDBView::OnQueryDragIcon()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CFormView::OnQueryDragIcon();
}


void CCPDBView::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFormView::OnSysCommand(nID, lParam);
}
CString CCPDBView::Seat(CString strsql){
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