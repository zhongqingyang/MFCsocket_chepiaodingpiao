// GQDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDB.h"
#include "GQDLG.h"
#include "afxdialogex.h"
#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"
#include "CMessg.h"
static int CurSelRow=0,CurSelCol=0;
const int MAXROW	=6;	//最大行数
const int INFO_LEN	=300;	//每行最大长度
static CString strsqll;
static int ColNum1=7;
static CString ColsName1[7]={"订单号","用户","车次","出发时间","到站时间","下单时间","票价"};
static int ColWidth[7]={150,150,150,150,150,150,120};//列宽
bool la=true;
static CString struser;
const int MAXROW1	=20;	//最大行数
static int ColNum2=9;
static CString ColsName2[9]={"车次","出发站","终点站","出发时间","到达时间","总票","余票","票价","出发日期"};

static int ColWidth1[9]={120,120,120,120,120,120,120,120,120};//列宽
// GQDLG 对话框

IMPLEMENT_DYNAMIC(GQDLG, CDialogEx)

GQDLG::GQDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(GQDLG::IDD, pParent)
	, m_End(_T(""))
{

	m_Begin = _T("");
}

GQDLG::~GQDLG()
{
}

void GQDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_CUSTOM1, m_list);
	DDX_GridControl(pDX, IDC_CUSTOM2, m_list1);
	//  DDX_Text(pDX, IDC_EDIT1, m_Begdin);
	DDX_Text(pDX, IDC_EDIT2, m_End);
	DDX_Text(pDX, IDC_EDIT1, m_Begin);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl);
}


BEGIN_MESSAGE_MAP(GQDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &GQDLG::OnBnClickedOk)
//	ON_BN_CLICKED(IDC_BUTTON1, &GQDLG::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON1, &GQDLG::OnBnClickedButton1)
ON_WM_ERASEBKGND()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// GQDLG 消息处理程序


void GQDLG::OnBnClickedOk()
{
	if (MessageBox("确认要改签该订单吗？","提示信息",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
	{
		
		
		CString str,strsql,str1,str2,str3,str4,stryp,strmoney,struse;
		str=m_list.GetItemText(1,0);
		str1=m_list.GetItemText(1,6);
		str2=m_list.GetItemText(1,2);
		str3=m_list.GetItemText(1,3);
		struse=m_list.GetItemText(1,1);
		struser=struse;
		dd();
		if (la==false)
		{
			return;
		}
		AfxExtractSubString(str4,(LPCTSTR)str3,0,' ');//分割字符串获取出发日期

		strsql.Format("delete from Oder where OrderID='%s' ",str);
		if (!ExeSQL(strsql))
		{
			return;
		}

		strsql.Format("select YT  from Train where trainID='%s' and banci='%s' ",str2,str4);
		stryp=selectt(strsql);
		strsql.Format("update Train set YT='%d' where trainID='%s' and banci='%s' ",atoi(stryp)+1,str2,str4);//余票加一
		if (!ExeSQL(strsql))
		{
			return;
		}
		strsql.Format("select money from [User] where UseZH='%s' " ,struse);
		strmoney=selectt(strsql);
		strmoney.Format("%d",atoi(strmoney)+atoi(str1));
		strsql.Format("update [User] set money ='%s' where UseZH='%s' ",strmoney,struse);//余额加
		if (!ExeSQL(strsql))
		{
			return;
		}
		
		
		
	}

	AfxMessageBox("改签成功");
	CDialogEx::OnOK();
}
CString GQDLG::selectt(CString strsql)//查询
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

BOOL GQDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitList(MAXROW);//初始gridctr信息显示
	InitList1(MAXROW1);//初始gridctr信息显示
	strsqll.Format("select * from Oder where OrderID='%s' ",m_dd);
	ShowData(strsqll);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void GQDLG::InitList(int MaxRow)//动态显示列名
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
void GQDLG::InitList1(int MaxRow1)//动态显示列名
{
	TRY 
	{
		m_list1.SetRowCount(MaxRow1);
		m_list1.SetColumnCount(ColNum2);
		m_list1.SetFixedRowCount(1);//m_nFixRows=1
		m_list1.SetFixedColumnCount(0);
		m_list1.SetSingleRowSelection(0);
		m_list1.SetEditable(TRUE);

		m_list1.SetFixedBkColor(RGB(228,243,249));//标题背景颜色
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return ;
	}
	END_CATCH

		// fill rows/cols with text
		for (int row = 0; row <m_list1.GetRowCount(); row++)
		{

			for (int col = 0; col < ColNum2; col++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = col;

				if(row < m_list1.GetFixedRowCount())
				{
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("%s"),ColsName2[col]);
				}
				m_list1.SetItem(&Item);
			}
		}

		//设置列宽
		for (int i = 0; i < ColNum2; i++)
		{
			m_list1.SetColumnWidth(i,ColWidth1[i]);
		}
		for (int i = 1; i < MAXROW1; i++)
		{
			m_list1.SetRowHeight(i,24);//设置行高
		}
		m_list1.ExpandLastColumn();
		m_list1.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

		m_list1.Invalidate();		

}
void GQDLG::ShowData(CString strsql)
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
void GQDLG::ShowData1(CString strsql)
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
					m_list1.SetItemText(i+1,j,output);//输出

				}
			}
		}
	}
	m_list1.Invalidate();
}
bool GQDLG::ExeSQL(CString strsql)
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

int GQDLG::GetRecCount(CString strsql)
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
void GQDLG::Clear(){
	for(int i=0;i<51;i++)
	{
		for(int j=0;j<7;j++)
		{
			m_list1.SetItemText(i+1,j," ");//输出

		}
	}
}

void GQDLG::OnBnClickedButton1()
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
	if(month=atoi(month1)&&day<atoi(day1))
	{
		AfxMessageBox("请选择当前日期之后的车次");
		return;
	}
	CString strsql;
	strsql.Format("select [trainID],[Start],[Finish],[GoTime],[OverTime],[ZT],[YT],[Price],[banci]  from Train where Start='%s' and Finish='%s' and banci='%s' ",m_Begin,m_End,str1);
	if (GetRecCount(strsql)==0)
	{
		AfxMessageBox("没有该列车");
	}
	Clear();
	ShowData1(strsql);
}
void GQDLG::dd(){
	la=true;
	CString strmoney;CString str,strsql,str1,str2,str3,str4,str5,str6,stryp,str7,str8;
	int nTemp=m_list1.GetFocusCell().row;

	str=m_list1.GetItemText(nTemp,0);
	str2=m_list1.GetItemText(nTemp,3);
	str4=m_list1.GetItemText(nTemp,4);
	stryp=m_list1.GetItemText(nTemp,5);
	str8=m_list1.GetItemText(nTemp,6);
	strsql.Format("select money from [User] where UseZH='%s' " ,struser);
	strmoney=selectt(strsql);
	AfxMessageBox(strmoney);
	strmoney.Format("%d",atoi(strmoney)-atoi(str8));
	if (atoi(strmoney)-atoi(str8)<0)
	{
		la=false;
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
	strsql.Format("select * from Oder where [UserZH]='%s' and [trainID]='%s' and [trainBegin]='%s' and [trainEnd]='%s'",struser,str,str1,str3);
	if (GetRecCount(strsql)!=0)
	{
		AfxMessageBox("您已购买该票。请勿重复购票");
		la=false;
		return;
	}
	strsql.Format("insert into Oder ([UserZH],[trainID],[trainBegin],[trainEnd],[OrderTime],[Price])values('%s','%s','%s','%s','%s','%s') ",struser,str,str1,str3,curdata,str6);//订单
	if (!ExeSQL(strsql))
	{
		la=false;
		return;
	}
	strsql.Format("update Train set YT='%d' where trainID='%s' and banci='%s' ",atoi(stryp)-1,str,str7);//余票减一
	if (!ExeSQL(strsql))
	{
		la=false;
		return;
	}
	strsql.Format("update [User] set money ='%s' where UseZH='%s' ",strmoney,m_user);//余额减
	if (!ExeSQL(strsql))
	{
		la=false;
		return;
	}

}


BOOL GQDLG::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\6.jpg");
	Image img(bkimg.AllocSysString(), TRUE);
	//输出图片
	bmpGraphics.DrawImage(&img, 0, 0, rect.Width(), rect.Height());
	//拷贝输出到屏幕
	Graphics graphics(pDC->m_hDC);
	CachedBitmap cachedBmp(&bmp, &graphics);
	graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
	return true;
	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH GQDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(   CTLCOLOR_STATIC   ==   nCtlColor   )  
	{ 
		//pDC->SetBkColor(RGB(255,0,0));  //需要指定背景色的时候用 
		pDC->SetTextColor(RGB(0,   0,   0));    //文字前景色
		pDC->SetBkMode(TRANSPARENT);    //设置透明
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //获取画笔颜色混合后的画笔，完成透明 
	}  
	return hbr;
}
