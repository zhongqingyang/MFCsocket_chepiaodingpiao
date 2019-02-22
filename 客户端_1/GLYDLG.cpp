// GLYDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CPDB.h"
#include "GLYDLG.h"
#include "TRDLG.h"
#include "MainFrm.h"
#include "DDGLDLG.h"
#include "YHGLDLG.h"
#include "DADLG.h"
#include "CMessg.h"
#include "YHDLG.h"
// GLYDLG

IMPLEMENT_DYNCREATE(GLYDLG, CFormView)

	GLYDLG::GLYDLG()
	: CFormView(GLYDLG::IDD)
{

}

GLYDLG::~GLYDLG()
{
}

void GLYDLG::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_BUTTON4 ,m_btn);
	CFormView::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_TREE1, m_webTree);
}

BEGIN_MESSAGE_MAP(GLYDLG, CFormView)
	ON_BN_CLICKED(IDC_BUTTON4, &GLYDLG::OnBnClickedButton4)
//	ON_WM_ERASEBKGND()
//	ON_WM_CTLCOLOR()
//	ON_WM_SIZE()
ON_WM_CTLCOLOR()
ON_WM_ERASEBKGND()
ON_BN_CLICKED(IDC_BUTTON1, &GLYDLG::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &GLYDLG::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON3, &GLYDLG::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON5, &GLYDLG::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON6, &GLYDLG::OnBnClickedButton6)
END_MESSAGE_MAP()


// GLYDLG ���

#ifdef _DEBUG
void GLYDLG::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void GLYDLG::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// GLYDLG ��Ϣ�������


void GLYDLG::OnBnClickedButton4()
{
	if (MessageBox("��Ҫ�˳���ע������Ҫ���棡","��ʾ��Ϣ",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
	{
		AfxGetApp()-> m_pMainWnd->DestroyWindow();
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void GLYDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	quanxian();
	m_btn.SetToolTipText(_T("�˳�"));
	m_btn.SetSkin(IDB_BITMAP4,IDB_BITMAP5,IDB_BITMAP6,0,0,NULL,0,0,4);
	m_btn.SizeToContent();
	// TODO: �ڴ����ר�ô����/����û���
}

CString GLYDLG::selectt(CString strsql){
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
void GLYDLG::quanxian(){
	CString strsql,str,output;
	CString strsz[5];
	strsql.Format("select [UserQX] from [User] where UseZH='%s' ",m_user);
	str=selectt(strsql);
	for(int i=0;i<5;i++)
	{
		AfxExtractSubString(output,str,i,',');
		strsz[i]=output;
	}
	if (strsz[1]=="1")
	{
		GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	if (strsz[3]=="1"&&strsz[2]=="1"&&strsz[4]=="1")
	{
		GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
}

void GLYDLG::ChangeSize()
{

}

HBRUSH GLYDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if(   CTLCOLOR_STATIC   ==   nCtlColor   )  
	{ 
		//pDC->SetBkColor(RGB(255,0,0));  //��Ҫָ������ɫ��ʱ���� 
		pDC->SetTextColor(RGB(0,   0,   0));    //����ǰ��ɫ
		pDC->SetBkMode(TRANSPARENT);    //����͸��
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //��ȡ������ɫ��Ϻ�Ļ��ʣ����͸�� 
	}  
	return hbr;
}


BOOL GLYDLG::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\8.jpg");
	Image img(bkimg.AllocSysString(), TRUE);
	//���ͼƬ
	bmpGraphics.DrawImage(&img, 0, 0, rect.Width(), rect.Height());
	//�����������Ļ
	Graphics graphics(pDC->m_hDC);
	CachedBitmap cachedBmp(&bmp, &graphics);
	graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
	return true;

	return CFormView::OnEraseBkgnd(pDC);
}


void GLYDLG::OnBnClickedButton1()
{
	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
	pFram->ReplaceView(0,1,RUNTIME_CLASS(TRDLG));
}


void GLYDLG::OnBnClickedButton2()
{
	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
	pFram->ReplaceView(0,1,RUNTIME_CLASS(DDGLDLG));
}


void GLYDLG::OnBnClickedButton3()
{
	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
	pFram->ReplaceView(0,1,RUNTIME_CLASS(YHGLDLG));
}


void GLYDLG::OnBnClickedButton5()
{
	DADLG mydlg;
	mydlg.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void GLYDLG::OnBnClickedButton6()
{
	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
	pFram->ReplaceView(0,1,RUNTIME_CLASS(YHDLG));
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
