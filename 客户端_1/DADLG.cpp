// DADLG.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDB.h"
#include "DADLG.h"
#include "afxdialogex.h"
#include "CMessg.h"

// DADLG 对话框

IMPLEMENT_DYNAMIC(DADLG, CDialogEx)

DADLG::DADLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(DADLG::IDD, pParent)
{

}

DADLG::~DADLG()
{
}

void DADLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_CHECK1, m_ch1);
	DDX_Control(pDX, IDC_CHECK2, m_ch2);
	DDX_Control(pDX, IDC_CHECK3, m_ch3);
	DDX_Control(pDX, IDC_CHECK4, m_ch4);
	DDX_Control(pDX, IDC_CHECK5, m_ch5);
}


BEGIN_MESSAGE_MAP(DADLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &DADLG::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &DADLG::OnSelchangeList1)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// DADLG 消息处理程序


void DADLG::OnBnClickedOk()
{
	CString str,str1;
	int k1,k2,k3,k4,k5;
	k1=m_ch1.GetCheck();
	k2=m_ch2.GetCheck();
	k3=m_ch3.GetCheck();
	k4=m_ch4.GetCheck();
	k5=m_ch5.GetCheck();
	str1.Format("%d,%d,%d,%d,%d",k1,k2,k3,k4,k5);
	int index=m_list.GetCurSel();
	m_list.GetText(index,str);
	CString strsql;
	strsql.Format("update [User] set [UserQX]='%s' where [UseQX]='%s' ",str1,str);
	if (!ExeSQL(strsql))
	{
		AfxMessageBox("修改失败");
	}
	AfxMessageBox("修改成功");
	CDialogEx::OnOK();
}


BOOL DADLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_list.InsertString(0,"高级管理员");
	m_list.InsertString(1,"车辆管理员");
	m_list.InsertString(2,"订单管理员");
	m_list.InsertString(3,"用户管理员");
	m_ch1.SetCheck(0);
	m_ch2.SetCheck(0);
	m_ch3.SetCheck(0);
	m_ch4.SetCheck(0);
	m_ch5.SetCheck(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void DADLG::OnSelchangeList1()
{
	m_ch1.SetCheck(0);
	m_ch2.SetCheck(0);
	m_ch3.SetCheck(0);
	m_ch4.SetCheck(0);
	m_ch5.SetCheck(0);
	int index=m_list.GetCurSel();
	CString str,strsql,str1,strsz[5],output1,output;
	m_list.GetText(index,str);
	strsql.Format("select UserQX from [User] where [UseQX]='%s' ",str);
	output1=selectt(strsql);
	for(int i=0;i<5;i++)
	{
		AfxExtractSubString(output,output1,i,',');
		strsz[i]=output;
	}
	if (strsz[0]=="1")
	{
		m_ch1.SetCheck(1);
	}
	if (strsz[1]=="1")
	{
		m_ch2.SetCheck(1);
	}
	if (strsz[2]=="1")
	{
		m_ch3.SetCheck(1);
	}
	if (strsz[3]=="1")
	{
		m_ch4.SetCheck(1);
	}
	if (strsz[4]=="1")
	{
		m_ch5.SetCheck(1);
	}

}
CString DADLG::selectt(CString strsql)//查询
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
bool DADLG::ExeSQL(CString strsql)
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

BOOL DADLG::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\5.jpg");
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


HBRUSH DADLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
