// DinDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDB.h"
#include "DinDLG.h"
#include "afxdialogex.h"
#include "CMessg.h"

// DinDLG 对话框

IMPLEMENT_DYNAMIC(DinDLG, CDialogEx)

DinDLG::DinDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(DinDLG::IDD, pParent)
{

}

DinDLG::~DinDLG()
{
}

void DinDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DinDLG, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// DinDLG 消息处理程序


BOOL DinDLG::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\12.jpg");
	Image img(bkimg.AllocSysString(), TRUE);
	//输出图片
	bmpGraphics.DrawImage(&img, 0, 0, rect.Width(), rect.Height());
	//拷贝输出到屏幕
	Graphics graphics(pDC->m_hDC);
	CachedBitmap cachedBmp(&bmp, &graphics);
	graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
	return true;
}


BOOL DinDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strsql,stry,strm,strd,strdd,strt,strc,strh;
	strsql.Format("select [Start] from Train where [trainID]='%s'",m_trainID);
	GetDlgItem(IDC_EDIT1)->SetWindowText(selectt(strsql));
	strsql.Format("select [Finish] from Train where [trainID]='%s'",m_trainID);
	GetDlgItem(IDC_EDIT2)->SetWindowText(selectt(strsql));
	AfxExtractSubString(stry,m_begin,0,'-');
	GetDlgItem(IDC_EDIT3)->SetWindowText(stry);
	AfxExtractSubString(strm,m_begin,1,'-');
	GetDlgItem(IDC_EDIT4)->SetWindowText(strm);
	AfxExtractSubString(strd,m_begin,2,'-');
	AfxExtractSubString(strdd,strd,0,' ');
	GetDlgItem(IDC_EDIT5)->SetWindowText(strdd);
	AfxExtractSubString(strt,m_begin,1,' ');
	GetDlgItem(IDC_EDIT7)->SetWindowText(strt);
	
		AfxExtractSubString(strc,m_seat,0,',');
	GetDlgItem(IDC_EDIT9)->SetWindowText(strc);
	AfxExtractSubString(strh,m_seat,1,',');
	GetDlgItem(IDC_EDIT10)->SetWindowText(strh);
	strsql.Format("select [Price] from Train where [trainID]='%s'",m_trainID);
	GetDlgItem(IDC_EDIT11)->SetWindowText(selectt(strsql));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
CString DinDLG::selectt(CString strsql)//查询
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


HBRUSH DinDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(   CTLCOLOR_EDIT   ==   nCtlColor   )  
	{ 
		//pDC->SetBkColor(RGB(255,0,0));  //需要指定背景色的时候用 
		pDC->SetTextColor(RGB(0,  0,  0));    //文字前景色
		pDC->SetBkMode(TRANSPARENT);    //设置透明
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //获取画笔颜色混合后的画笔，完成透明 
	}  
	return hbr;
}
