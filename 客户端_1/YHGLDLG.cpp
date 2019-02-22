// YHGLDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDB.h"
#include "YHGLDLG.h"
#include "CMessg.h"

// YHGLDLG

IMPLEMENT_DYNCREATE(YHGLDLG, CFormView)

	YHGLDLG::YHGLDLG()
	: CFormView(YHGLDLG::IDD)
	, m_sex(_T(""))
	, m_phone(_T(""))
{

	m_oldpwn = _T("");
	m_newpwn = _T("");
	m_zh = _T("");
	m_pass = _T("");
	m_pass1 = _T("");
	//  m_shenfen = _T("");
}

YHGLDLG::~YHGLDLG()
{
}

void YHGLDLG::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_oldpwn);
	DDX_Text(pDX, IDC_EDIT6, m_newpwn);
	DDX_Text(pDX, IDC_EDIT4, m_zh);
	DDX_Text(pDX, IDC_EDIT9, m_pass);
	DDX_Text(pDX, IDC_EDIT10, m_pass1);
	//  DDX_CBString(pDX, IDC_COMBO1, m_shenfen);
	DDX_Text(pDX, IDC_EDIT13, m_phone);
	DDX_Control(pDX, IDC_COMBO1, m_shenfen);
	DDX_Control(pDX, IDC_COMBO2, m_sex1);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_list1);
}

BEGIN_MESSAGE_MAP(YHGLDLG, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &YHGLDLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &YHGLDLG::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST1, &YHGLDLG::OnSelchangeList1)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &YHGLDLG::OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &YHGLDLG::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &YHGLDLG::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &YHGLDLG::OnBnClickedButton5)
END_MESSAGE_MAP()


// YHGLDLG 诊断

#ifdef _DEBUG
void YHGLDLG::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void YHGLDLG::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// YHGLDLG 消息处理程序


void YHGLDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//GDI+初始化
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	GetDlgItem(IDC_EDIT1)->SetWindowText(m_user);
	quanxian();
	m_sex1.InsertString(0,"男");
	m_sex1.InsertString(1,"女");
	m_sex1.SetCurSel(0);
	m_shenfen.InsertString(0,"高级管理员");
	m_shenfen.InsertString(1,"订单管理员");
	m_shenfen.InsertString(2,"车辆管理员");
	m_shenfen.InsertString(3,"用户管理员");

	m_shenfen.SetCurSel(-1);
	CString strsql,str1;
	str1="用户";
	strsql.Format("select [UseZH] from [User] where UseQX!='%s' ",str1);
	quanxian();
	CMessg msg;
	msg.m_type = 'a';
	msg.m_strText = strsql;
	if(!theApp.m_HCSocket.SendMessage(msg))
		return;
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
			int i =0,j=0,k=0;
			CString output,stemp;
			for(i=0;i<rowcount;i++)
			{
				for(j=0;j<column;j++)
				{
					AfxExtractSubString(output,recmsg->m_strText,i*column+j,'|');
					m_list.InsertString(k,output);
					k++;
				}
			}
		}
	}
}


void YHGLDLG::OnBnClickedButton1()
{
	UpdateData();
	CString strsql,strpwd,strpwd1;
	strsql.Format("select [UsePwd] from [User] where UseZH='%s' ",m_user);
	strpwd=selectt(strsql);
	GetDlgItem(IDC_EDIT2)->GetWindowText(strpwd1);
	if (strpwd!=strpwd1)
	{
		AfxMessageBox("密码错误");
		return;
	}
	if (m_oldpwn!=m_newpwn)
	{
		AfxMessageBox("请输入相同的密码");
		return;
	}
	strsql.Format("update [User] set UsePwd='%s' where UseZH='%s' ",m_newpwn,m_user);
	ExeSQL(strsql);
	AfxMessageBox("修改成功");

}

CString YHGLDLG::selectt(CString strsql)//查询
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
bool YHGLDLG::ExeSQL(CString strsql)
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
int YHGLDLG::GetRecCount(CString strsql)
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

void YHGLDLG::OnBnClickedButton2()
{
	UpdateData();
	CString sm2,sm1;
	int Count = m_list1.GetCount();
	CString str[5],strss;
	GetDlgItem(IDC_COMBO2)->GetWindowText(sm2);
	GetDlgItem(IDC_COMBO1)->GetWindowText(sm1);
	if (m_zh==""||m_pass==""||m_pass1==""||m_phone=="")
	{
		AfxMessageBox("请输入完整信息");
		return;
	}

	CString strsql;
	strsql.Format("select * from [User] where [UseZH]='%s' ",m_zh);
	if (m_pass!=m_pass1)
	{
		AfxMessageBox("请输入相同的密码");
	}
    int index=m_shenfen.GetCurSel();
	m_shenfen.GetLBText(index,strss);

	if (GetRecCount(strsql)!=0)
	{
		AfxMessageBox("该用户已存在。请前往修改");
		return;
	}
	strsql.Format("select [UserQX] from [User] where [UseQX]='%s' ",strss);
	strss=selectt(strsql);
	strsql.Format("insert into [User] ([UseZH],[UseSex],[UsePwd],[UsePhone],[UseQX],[UserQX])values('%s','%s','%s','%s','%s','%s') ",m_zh,sm2,m_pass1,m_phone,sm1,strss);
	if (!ExeSQL(strsql))
	{
		AfxMessageBox("添加失败");
		return;
	}
	AfxMessageBox("添加用户成功");
	
}


void YHGLDLG::OnSelchangeList1()
{
	
	int index=m_list.GetCurSel();
	CString str,strsql,str1;
	m_list.GetText(index,str);
	strsql.Format("select [UseSex] from [User] where UseZH='%s' ",str);
	str1=selectt(strsql);
	GetDlgItem(IDC_COMBO2)->SetWindowText(str1);
	strsql.Format("select [UsePhone] from [User] where UseZH='%s' ",str);
	str1=selectt(strsql);
	GetDlgItem(IDC_EDIT13)->SetWindowText(str1);
	strsql.Format("select [UsePwd] from [User] where UseZH='%s' ",str);
	str1=selectt(strsql);
	GetDlgItem(IDC_EDIT9)->SetWindowText(str1);
	GetDlgItem(IDC_EDIT10)->SetWindowText(str1);
	GetDlgItem(IDC_EDIT4)->SetWindowText(str);
	strsql.Format("select [UseQX] from [User] where UseZH='%s' ",str);
	str1=selectt(strsql);
	GetDlgItem(IDC_COMBO1)->SetWindowText(str1);
	if (str1=="高级管理员")
	{
		m_shenfen.SetCurSel(0);
	}
	if (str1=="订单管理员")
	{
		m_shenfen.SetCurSel(1);
	}
	if (str1=="车辆管理员")
	{
		m_shenfen.SetCurSel(2);
	}
	if (str1=="用户管理员")
	{
		m_shenfen.SetCurSel(3);
	}
	CString output,output1;
	CString strsz[5];
	strsql.Format("select [UserQX] from [User] where UseZH='%s' ",str);
	output1=selectt(strsql);
	for(int i=0;i<5;i++)
	{
		AfxExtractSubString(output,output1,i,',');
		strsz[i]=output;
	}
	m_list1.ResetContent();
	int k=0;
	if (strsz[0]=="1")
	{
		m_list1.InsertString(k,"订单修改删除");
		k++;
	}
	if (strsz[1]=="1")
	{
		m_list1.InsertString(k,"用户修改删除");
		k++;
	}
	if (strsz[2]=="1")
	{
		m_list1.InsertString(k,"添加管理员");
		k++;
	}
	if (strsz[3]=="1")
	{
		m_list1.InsertString(k,"修改管理员");
		k++;
	}

	if (strsz[4]=="1")
	{
		m_list1.InsertString(k,"注销管理员");
	}
}
void YHGLDLG::quanxian(){

	CString strsql,str,output;
	CString strsz[5];
	strsql.Format("select [UserQX] from [User] where UseZH='%s' ",m_user);
	str=selectt(strsql);
	for(int i=0;i<5;i++)
	{
		AfxExtractSubString(output,str,i,',');
		strsz[i]=output;
	}
	if (strsz[2]=="1")
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT13)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT13)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}
	if (strsz[3]=="1")
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT13)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT13)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	}
	if (strsz[4]=="1")
	{

		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
}

BOOL YHGLDLG::OnEraseBkgnd(CDC* pDC)
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


HBRUSH YHGLDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void YHGLDLG::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ChangeSize();
	CFormView::ShowScrollBar(SB_BOTH, false);
}
void YHGLDLG::ChangeSize()
{

}

void YHGLDLG::OnSelchangeCombo1()
{
	m_list1.ResetContent();
	int index=m_shenfen.GetCurSel();
	CString str,strsql,strsz[5],output;
	m_shenfen.GetLBText(index,str);
	strsql.Format("select [UserQX] from [User] where [UseQX]='%s' ",str);
	str=selectt(strsql);
	for(int i=0;i<5;i++)
	{
		AfxExtractSubString(output,str,i,',');
		strsz[i]=output;
	}
	int k=0;
	if (strsz[0]=="1")
	{
		m_list1.InsertString(k,"订单修改删除");
		k++;
	}
	if (strsz[1]=="1")
	{
		m_list1.InsertString(k,"用户修改删除");
		k++;
	}
	if (strsz[2]=="1")
	{
		m_list1.InsertString(k,"添加管理员");
		k++;
	}
	if (strsz[3]=="1")
	{
		m_list1.InsertString(k,"修改管理员");
		k++;
	}

	if (strsz[4]=="1")
	{
		m_list1.InsertString(k,"注销管理员");
	}

}


void YHGLDLG::OnBnClickedButton3()//修改
{
	UpdateData();
	CString sm2,sm1;CString str[5],strss,stry;
	int Count = m_list.GetCurSel();
	m_list.GetText(Count,stry);
	GetDlgItem(IDC_COMBO2)->GetWindowText(sm2);
	GetDlgItem(IDC_COMBO1)->GetWindowText(sm1);
	if (m_zh==""||m_pass==""||m_pass1==""||m_phone=="")
	{
		AfxMessageBox("请输入完整信息");
		return;
	}
	if (m_pass!=m_pass1)
	{
		AfxMessageBox("请输入相同的密码");
	}
	CString strsql;
	strsql.Format("select [UserQX] from [User] where [UseQX]='%s' ",sm1);
	strss=selectt(strsql);
	strsql.Format("update  [User]  set [UseZH]='%s',[UseSex]='%s',[UsePwd]='%s',[UsePhone]='%s',[UseQX]='%s',[UserQX]='%s' where [UseZH]='%s'",m_zh,sm2,m_pass1,m_phone,sm1,strss,stry);
	if (!ExeSQL(strsql))
	{
		AfxMessageBox("修改失败");
		return;
	}
	AfxMessageBox("修改用户成功");
}


void YHGLDLG::OnBnClickedButton4()
{
	CString strsql,str;
	GetDlgItem(IDC_EDIT4)->GetWindowText(str);
	strsql.Format("delete from [User] where [UseZH]='%s' ",str);
	if (!ExeSQL(strsql))
	{
		AfxMessageBox("注销失败");
		return;
	}
	AfxMessageBox("注销成功");
}


void YHGLDLG::OnBnClickedButton5()//取消操作
{
	GetDlgItem(IDC_EDIT2)->SetWindowText("");
	GetDlgItem(IDC_EDIT5)->SetWindowText("");
	GetDlgItem(IDC_EDIT6)->SetWindowText("");
	GetDlgItem(IDC_EDIT4)->SetWindowText("");
	GetDlgItem(IDC_EDIT13)->SetWindowText("");
	GetDlgItem(IDC_EDIT9)->SetWindowText("");
	GetDlgItem(IDC_EDIT10)->SetWindowText("");
	m_shenfen.SetCurSel(0);
	m_list.SetCurSel(-1);
	m_list1.ResetContent();
	int index=m_shenfen.GetCurSel();
	CString str,strsql,strsz[5],output;
	m_shenfen.GetLBText(index,str);
	strsql.Format("select [UserQX] from [User] where [UseQX]='%s' ",str);
	str=selectt(strsql);
	for(int i=0;i<5;i++)
	{
		AfxExtractSubString(output,str,i,',');
		strsz[i]=output;
	}
	int k=0;
	if (strsz[0]=="1")
	{
		m_list1.InsertString(k,"订单修改删除");
		k++;
	}
	if (strsz[1]=="1")
	{
		m_list1.InsertString(k,"用户修改删除");
		k++;
	}
	if (strsz[2]=="1")
	{
		m_list1.InsertString(k,"添加管理员");
		k++;
	}
	if (strsz[3]=="1")
	{
		m_list1.InsertString(k,"修改管理员");
		k++;
	}

	if (strsz[4]=="1")
	{
		m_list1.InsertString(k,"注销管理员");
	}
}
