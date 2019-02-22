// TRDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDB.h"
#include "TRDLG.h"
#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"
#include "CMessg.h"

// TRDLG
static int CurSelRow=0,CurSelCol=0;
const int MAXROW	=6;	//最大行数
const int INFO_LEN	=180;	//每行最大长度
static CString strsqll;
static int num=0;
static int ColNum1=9;
static CString ColsName1[9]={"车次","出发站","终点站","出发时间","到达时间","总票","余票","票价","出发日期"};

static int ColWidth[9]={120,120,120,120,120,120,120,120,120};//列宽
IMPLEMENT_DYNCREATE(TRDLG, CFormView)

TRDLG::TRDLG()
	: CFormView(TRDLG::IDD)
{

	m_Begin = _T("");
	m_End = _T("");
	m_checi = _T("");
}

TRDLG::~TRDLG()
{
}

void TRDLG::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl);
	DDX_GridControl(pDX, IDC_CUSTOM1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_Begin);
	DDX_Text(pDX, IDC_EDIT2, m_End);
	DDX_Text(pDX, IDC_EDIT5, m_checi);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON3, m_button3);
}

BEGIN_MESSAGE_MAP(TRDLG, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &TRDLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TRDLG::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &TRDLG::OnBnClickedButton3)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON4, &TRDLG::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON8, &TRDLG::OnClickedButton8)
END_MESSAGE_MAP()


// TRDLG 诊断

#ifdef _DEBUG
void TRDLG::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TRDLG::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TRDLG 消息处理程序


void TRDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//GDI+初始化
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	m_list.ShowWindow(FALSE);
	ChangeSize();
	m_list.ShowWindow(TRUE);
	InitList(MAXROW);//初始gridctr信息显示

	strsqll.Format("select * from [Train]");
	ShowDatadata(strsqll);
	// TODO: 在此添加专用代码和/或调用基类
}
void TRDLG::InitList(int MaxRow)
{
	TRY 
	{
		m_list.SetRowCount(MaxRow);
		m_list.SetColumnCount(ColNum1);
		m_list.SetFixedRowCount(1);//m_nFixRows=1
		m_list.SetFixedColumnCount(0);
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
		for (int row = 0; row <m_list.GetRowCount(); row++)//返回行（包括固定行）的数目
		{

			for (int col = 0; col < ColNum1; col++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = col;

				if(row < m_list.GetFixedRowCount())//返回固定行的数目
				{
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("%s"),ColsName1[col]);
				}
				m_list.SetItem(&Item);
				if (col==8)
				{
					m_list.SetCellType(row,col,RUNTIME_CLASS(CGridCellDateTime));
				}
				/*//数字格式单元格9
				if(col==2)
				{
					m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellNumeric));
				}
				//组合框格式单元格初始化
				if( col==3)
				{
					m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));

					CGridCellCombo *pCell=(CGridCellCombo *)m_list.GetCell(row,col);
					CStringArray ar;
					ar.SetSize(2);
					ar.SetAt(0,"男");
					ar.SetAt(1,"女");
					pCell->SetOptions(ar);
				}
				if( col==5)
				{
					m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));

					CGridCellCombo *pCell=(CGridCellCombo *)m_list.GetCell(row,col);
					CStringArray ar;
					ar.SetSize(2);
					ar.SetAt(0,"管理员");
					ar.SetAt(1,"录入员");
					pCell->SetOptions(ar);
				}*/
			}

			m_list.ExpandLastColumn();
			m_list.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

			//设置列宽
			for (int i = 0; i < ColNum1; i++)
			{
				m_list.SetColumnWidth(i,ColWidth[i]);
			}
			for (int i = 1; i < MAXROW; i++)
			{
				m_list.SetRowHeight(i,24);//设置行高
			}

			m_list.Invalidate();		
		}
}
void TRDLG::ShowData(CString strsql)
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

void TRDLG::OnBnClickedButton1()
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
	CString strsql;
	strsql.Format("select [trainID],[Start],[Finish],[GoTime],[OverTime],[ZT],[YT],[Price],[banci]  from Train where trainID like '%%%s%%' and Start='%s' and Finish='%s' and banci='%s' ",m_checi,m_Begin,m_End,str1);
	if (GetRecCount(strsql)==0)
	{
		AfxMessageBox("没有该列车");
	}
	Clear();
	ShowData(strsql);
	strsqll=strsql;
	// TODO: 在此添加控件通知处理程序代码
}
bool TRDLG::ExeSQL(CString strsql)
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

int TRDLG::GetRecCount(CString strsql)
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
void TRDLG::Clear(){
	for(int i=0;i<51;i++)
	{
		for(int j=0;j<9;j++)
		{
			m_list.SetItemText(i+1,j,"");//输出

		}
	}
}

void TRDLG::OnBnClickedButton2()
{
	m_list.EndEdit();//确认编辑的结果
	CString strtp1,strsql,strtp2;
	vector<int> SaveRow;
	for(int i=1;i<20;i++)//
	{
		for(int j=0;j<2;j++)
		{
			strtp1 = m_list.GetItemText(i,0);
			strtp1.TrimRight();//消除右边空格字符
			strtp2 = m_list.GetItemText(i,1);
			strtp2.TrimRight();
		}
		if( strtp1 ==""&&strtp2!="")//防止查到后面的空白行
		{
			AfxMessageBox("车次不能为空");
			return;
		}
		else if( strtp1 !=""&&strtp2!="")
			SaveRow.push_back(i);//有数据的行
	}
	//!=0才清空、覆盖
	for(int i=0;i<SaveRow.size();i++)
	{
		strsql.Format("delete from  [Train] where trainID='%s'  and banci='%s' ",m_list.GetItemText(SaveRow[i],0),m_list.GetItemText(SaveRow[i],8));//删除所有数据
		if(!ExeSQL(strsql))
		{
			AfxMessageBox("操作失败");
			return;
		}
	}

	for(int i=0;i<SaveRow.size();i++)
	{
		CString strtemp;
		strsql.Format("insert into Train (trainID,Start,Finish,GoTime,OverTIme,ZT,YT,Price,banci) values(");//重新添加所有更新的数据
		for(int j=0;j<ColNum1;j++)
		{
			if(j==0)
				strtemp.Format("'%s'",m_list.GetItemText(SaveRow[i],j));
			else
				strtemp.Format(",'%s'",m_list.GetItemText(SaveRow[i],j));
			strsql+=strtemp;

		}
		strsql+=")";
		if(!ExeSQL(strsql))
		{
			AfxMessageBox("操作失败");
			return;
		}
	}
	//ClearItem();
	//ShowAllTeam();
	AfxMessageBox("保存数据成功！");
	CString strsql1;
	strsql1.Format("select * from [Train]");
	ShowData(strsql1);
}


void TRDLG::OnBnClickedButton3()
{
	if (MessageBox("确认要删除本次列车吗？","提示信息",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
	{
		CString strsql,str1,str2;
		int nTemp=m_list.GetFocusCell().row;
		str1=m_list.GetItemText(nTemp,0);
		str2=m_list.GetItemText(nTemp,8);
		strsql.Format("delete from Train where trainID='%s' and banci='%s' ",str1,str2);
		ExeSQL(strsql);
		AfxMessageBox("删除成功！");
		Clear();
		strsql.Format("select * from Train ");
		ShowData(strsql);
	}
	// TODO: 在此添加控件通知处理程序代码
}


void TRDLG::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ChangeSize();
	CFormView::ShowScrollBar(SB_BOTH, false);
}


HBRUSH TRDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL TRDLG::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\2.jpg");
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
void TRDLG::ChangeSize()
{
	if(m_list.m_hWnd) 
	{
		CRect rect,rectts;
		GetClientRect(&rect);

		rectts.left=rect.left+100;
		rectts.right=rect.right-259;
		rectts.top=rect.bottom-480;
		rectts.bottom=rect.bottom-300;
		m_list.MoveWindow(rectts,true);

		rectts.left=rect.left+300;
		rectts.right=rect.left+420;
		rectts.top=rect.bottom-100;
		rectts.bottom=rect.bottom-50;
		m_button2.MoveWindow(rectts,true);

		rectts.left=rect.left+500;
		rectts.right=rect.left+620;
		rectts.top=rect.bottom-100;
		rectts.bottom=rect.bottom-50;
		m_button3.MoveWindow(rectts,true);

		m_list.ShowWindow(TRUE);
	}
}


void TRDLG::OnBnClickedButton4()
{
	int k;
	if (num==1)
	{
		GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	}


	Clear();
	k=GetRecCount(strsqll)/5;
	if (num==k)
	{
		GetDlgItem(IDC_BUTTON8)->EnableWindow(true);
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
				for(j=0;j<9;j++)
				{

					AfxExtractSubString(output,recmsg->m_strText,i*9+j,'|');
					m_list.SetItemText(i+1,j,output);//输出

				}
			}
		}
	}
	m_list.Invalidate();

}
void TRDLG::ShowDatadata(CString strsql){
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
				for(j=0;j<9;j++)
				{

					AfxExtractSubString(output,recmsg->m_strText,i*9+j,'|');
					m_list.SetItemText(i+1,j,output);//输出

				}
			}
		}
	}
}



void TRDLG::OnClickedButton8()
{
	int k;
	if (num==0)
	{
		GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
	}

	num++;
	Clear();
	CString strsql;
	strsql.Format("select * from [Train]");
	CMessg msg;
	k=GetRecCount(strsqll)/5;
	if (num==k)
	{
		GetDlgItem(IDC_BUTTON8)->EnableWindow(false);
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
				for(j=0;j<9;j++)
				{

					AfxExtractSubString(output,recmsg->m_strText,i*9+j,'|');
					m_list.SetItemText(i+1,j,output);//输出

				}
			}
		}
	}
	m_list.Invalidate();
}
