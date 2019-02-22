// DDGLDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDB.h"
#include "DDGLDLG.h"
#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"
#include "CMessg.h"
#include "GQDLG.h"
static int CurSelRow=0,CurSelCol=0;
const int MAXROW	=6;	//最大行数
const int INFO_LEN	=300;	//每行最大长度
static CString strsqll;
static int ColNum1=7;
static CString ColsName1[7]={"订单号","用户","车次","出发时间","到站时间","下单时间","票价"};
static int num=0;
static int ColWidth[7]={150,150,150,150,150,150,120};//列宽
// DDGLDLG

IMPLEMENT_DYNCREATE(DDGLDLG, CFormView)

	DDGLDLG::DDGLDLG()
	: CFormView(DDGLDLG::IDD)
{

	m_dindan = _T("");
	m_yonghu = _T("");
	m_checi = _T("");
}

DDGLDLG::~DDGLDLG()
{
}

void DDGLDLG::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl);
	DDX_GridControl(pDX, IDC_CUSTOM1, m_list);
	DDX_Text(pDX, IDC_EDIT6, m_dindan);
	DDX_Text(pDX, IDC_EDIT7, m_yonghu);
	DDX_Text(pDX, IDC_EDIT8, m_checi);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON4, m_button4);
}

BEGIN_MESSAGE_MAP(DDGLDLG, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &DDGLDLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &DDGLDLG::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &DDGLDLG::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &DDGLDLG::OnBnClickedButton4)

	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON7, &DDGLDLG::OnClickedButton7)
END_MESSAGE_MAP()


// DDGLDLG 诊断

#ifdef _DEBUG
void DDGLDLG::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void DDGLDLG::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// DDGLDLG 消息处理程序


void DDGLDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//GDI+初始化
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	//GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();
	//m_list.ShowWindow(FALSE);
	InitList(MAXROW);//初始gridctr信息显示
	strsqll.Format("select * from Oder");
	ShowDatadata(strsqll);
	quanxian();
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	// TODO: 在此添加专用代码和/或调用基类
}
void DDGLDLG::InitList(int MaxRow)//动态显示列名
{
	TRY 
	{
		m_list.SetRowCount(MaxRow);
		m_list.SetColumnCount(ColNum1);
		m_list.SetFixedRowCount(1);//m_nFixRows=1
		m_list.SetFixedColumnCount(0);
		m_list.SetSingleRowSelection(0);
		m_list.SetEditable(TRUE);

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
				if (col==3)
				{
					m_list.SetCellType(row,col,RUNTIME_CLASS(CGridCellDateTime));
				}
				if(row < m_list.GetFixedRowCount())
				{
					
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
void DDGLDLG::ShowData(CString strsql)
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
bool DDGLDLG::ExeSQL(CString strsql)
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

int DDGLDLG::GetRecCount(CString strsql)
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
void DDGLDLG::Clear(){
	for(int i=0;i<51;i++)
	{
		for(int j=0;j<7;j++)
		{
			m_list.SetItemText(i+1,j," ");//输出

		}
	}
}
CString DDGLDLG::selectt(CString strsql)//查询
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

void DDGLDLG::OnBnClickedButton1()//上一页
{
	int k;
	if (num==1)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	}


	Clear();
	k=GetRecCount(strsqll)/5;
	if (num==k)
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	}num--;
	CMessg msg;
	//显示多条数据
	msg.m_type = 'e';
	msg.m_strText = strsqll;
	msg.m_num=num;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return ;

	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		int rowcount=recmsg->m_OperState;
		int column=recmsg->m_Column;
		CString ss;
		ss.Format("%d",column);
		CString str=recmsg->m_strText;
		if (recmsg->m_OperState==0)
			return;
		else
		{
			int i =0,j=0;
			CString output,stemp;
			for(i=0;i<rowcount;i++)
			{
				for(j=0;j<7;j++)
				{

					AfxExtractSubString(output,recmsg->m_strText,i*7+j,'|');
					m_list.SetItemText(i+1,j,output);//输出

				}
			}
		}
	}
	m_list.Invalidate();

}


void DDGLDLG::OnBnClickedButton2()//下一页
{
	int k;
	if (num==0)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	}

	num++;
	Clear();
	CString strsql;
	strsql.Format("select * from Oder");
	CMessg msg;
	k=GetRecCount(strsqll)/5;
	if (num==k)
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	}
	//显示多条数据
	msg.m_type = 'e';
	msg.m_strText = strsqll;
	msg.m_num=num;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return ;

	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		int rowcount=recmsg->m_OperState;
		int column=recmsg->m_Column;
		CString ss;
		ss.Format("%d",column);
		CString str=recmsg->m_strText;
		if (recmsg->m_OperState==0)
			return;
		else
		{
			int i =0,j=0;
			CString output,stemp;
			for(i=0;i<rowcount;i++)
			{
				for(j=0;j<7;j++)
				{

					AfxExtractSubString(output,recmsg->m_strText,i*7+j,'|');
					m_list.SetItemText(i+1,j,output);//输出

				}
			}
		}
	}
	m_list.Invalidate();
}

void DDGLDLG::ShowDatadata(CString strsql){
	CMessg msg;
	//显示多条数据
	msg.m_type = 'e';
	msg.m_strText = strsql;
	msg.m_num=0;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return ;

	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		int rowcount=recmsg->m_OperState;
		//int column=recmsg->m_Column;
		CString ss;
		//ss.Format("%d",column);
		CString str=recmsg->m_strText;
		if (recmsg->m_OperState==0)
		{
		//	AfxMessageBox("1");
			return;
		}
		else
		{
			int i =0,j=0;
			CString output,stemp;
			for(i=0;i<rowcount;i++)
			{
				for(j=0;j<7;j++)
				{

					AfxExtractSubString(output,recmsg->m_strText,i*7+j,'|');
					m_list.SetItemText(i+1,j,output);//输出

				}
			}
		}
	}
}

void DDGLDLG::OnBnClickedButton3()
{
	Clear();
	UpdateData();
	CTime m_date;
	m_dateCtrl.GetTime(m_date);
	int year=m_date.GetYear();
	int month = m_date.GetMonth();
	int day = m_date.GetDay();

	CString strsql,str1,str,str2;
	str1.Format("%d-",year);
	str.Format("%d-",month); 
	str1=str1+"%"+str;
	str2.Format("%d",day);
	str1=str1+"%"+str2;
	strsql.Format("select * from Oder where [OrderID] like '%s%%' and [UserZH] like '%s%%' and [trainID] like '%s%%' and [OrderTime] like '%s%%' ",m_dindan,m_yonghu,m_checi,str1);
	if (GetRecCount(strsql)==0)
	{
		AfxMessageBox("无数据");
		return;
	}
	ShowData(strsql);
	strsqll=strsql;
}
void DDGLDLG::quanxian(){
	CString strsql,str,output;
	CString strsz[5];
	strsql.Format("select [UserQX] from [User] where UseZH='%s' ",m_user);
	str=selectt(strsql);
	for(int i=0;i<5;i++)
	{
		AfxExtractSubString(output,str,i,',');
		strsz[i]=output;

	}
	if (strsz[0]=="0")
	{		
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);            
	}
	else
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);           

}

void DDGLDLG::OnBnClickedButton4()//删除
{
	CString str,strsql,str1,str2,str3,str4,stryp,strmoney,struse;
		int nTemp=m_list.GetFocusCell().row;
		str=m_list.GetItemText(nTemp,0);
		str1=m_list.GetItemText(nTemp,6);
		str2=m_list.GetItemText(nTemp,2);
		str3=m_list.GetItemText(nTemp,3);
		struse=m_list.GetItemText(nTemp,1);
		if (str=="")
		{
			AfxMessageBox("请选择订单");
			return;
		}
	if (MessageBox("确认要删除该订单吗？","提示信息",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
	{
		
		AfxExtractSubString(str4,(LPCTSTR)str3,0,' ');//分割字符串获取出发日期
		strsql.Format("delete from Oder where OrderID='%s' ",str);
		ExeSQL(strsql);
		strsql.Format("select YT  from Train where trainID='%s' and banci='%s' ",str2,str4);
		stryp=selectt(strsql);
		strsql.Format("update Train set YT='%d' where trainID='%s' and banci='%s' ",atoi(stryp)+1,str2,str4);//余票加一
		
		ExeSQL(strsql);
		strsql.Format("select money from [User] where UseZH='%s' " ,struse);
		strmoney=selectt(strsql);
		strmoney.Format("%d",atoi(strmoney)+atoi(str1));

		strsql.Format("update [User] set money ='%s' where UseZH='%s' ",strmoney,struse);//余额加
		

		ExeSQL(strsql);
		Clear();

		ShowDatadata(strsqll);
		m_list.Invalidate();
	}

}


HBRUSH DDGLDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL DDGLDLG::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\3.jpg");
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


void DDGLDLG::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ChangeSize();
	CFormView::ShowScrollBar(SB_BOTH, false);
}
void DDGLDLG::ChangeSize()
{
	if(m_list.m_hWnd) 
	{
		CRect rect,rectts;
		GetClientRect(&rect);

		rectts.left=rect.left;
		rectts.right=rect.right;
		rectts.top=rect.bottom-500;
		rectts.bottom=rect.bottom-335;
		m_list.MoveWindow(rectts,true);

		rectts.left=rect.left+200;
		rectts.right=rect.left+300;
		rectts.top=rect.bottom-275;
		rectts.bottom=rect.bottom-225;
		m_button1.MoveWindow(rectts,true);


		rectts.left=rect.left+400;
		rectts.right=rect.left+500;
		rectts.top=rect.bottom-275;
		rectts.bottom=rect.bottom-225;
		m_button2.MoveWindow(rectts,true);

		rectts.left=rect.left+600;
		rectts.right=rect.left+700;
		rectts.top=rect.bottom-275;
		rectts.bottom=rect.bottom-225;
		m_button4.MoveWindow(rectts,true);


		m_list.ShowWindow(TRUE);

	}
}



void DDGLDLG::OnClickedButton7()
{
	CString str,strsql,str1,str2,str3,str4,stryp,strmoney,struse;
	int nTemp=m_list.GetFocusCell().row;

	str=m_list.GetItemText(nTemp,0);
	m_dd=str;
	if (str=="")
	{
		AfxMessageBox("请选择订单");
		return;
	}
	GQDLG mydlg;
	if (mydlg.DoModal()==IDOK)
	{
		strsqll.Format("select * from Oder");
		ShowDatadata(strsqll);
	}

}
