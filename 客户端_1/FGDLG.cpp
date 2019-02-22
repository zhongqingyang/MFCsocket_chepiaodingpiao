// FGDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CPDB.h"
#include "FGDLG.h"
#include "MainFrm.h"
// FGDLG
#include "CPDBView.h"
#include "DDDLG.h"
#include "GRDLG.h"

IMPLEMENT_DYNCREATE(FGDLG, CFormView)

FGDLG::FGDLG()
	: CFormView(FGDLG::IDD)
{

}

FGDLG::~FGDLG()
{
}

void FGDLG::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_BUTTON4 ,m_btn);
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_webTree);
}

BEGIN_MESSAGE_MAP(FGDLG, CFormView)
//	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON4, &FGDLG::OnBnClickedButton4)
//	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_BUTTON1, &FGDLG::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON2, &FGDLG::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BUTTON3, &FGDLG::OnBnClickedButton3)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &FGDLG::OnSelchangedTree1)
//	ON_BN_CLICKED(IDC_BUTTON1, &FGDLG::OnBnClickedButton1)
END_MESSAGE_MAP()


// FGDLG ���

#ifdef _DEBUG
void FGDLG::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FGDLG::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FGDLG ��Ϣ�������


void FGDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_btn.SetToolTipText(_T("�˳�"));
	m_btn.SetSkin(IDB_BITMAP4,IDB_BITMAP5,IDB_BITMAP6,0,0,NULL,0,0,4);
	m_btn.SizeToContent();
	CRect rect;
	GetClientRect(rect);
	m_webTree.MoveWindow(0,0,rect.Width(),rect.Height());
	//m_webTree.SetBKImage("IDB_BITMAP8"); 
	HICON hIcon[4]; // ͼ��������
	HTREEITEM hRoot[4]; // ���ĸ��ڵ�ľ��
	HTREEITEM hCataItem; // �ɱ�ʾ��һ����ڵ�ľ��
	HTREEITEM hArtItem; // �ɱ�ʾ��һ���½ڵ�ľ��

	m_webTree.SetTextColor(RGB(0,0,220));
	m_webTree.SetBkColor(RGB(228,243,249));

	// ��������ͼ�꣬�������ǵľ�����浽����
	hIcon[0] = theApp.LoadIcon(IDI_ICON1);
	hIcon[1] = theApp.LoadIcon(IDI_ICON2);
	hIcon[2] = theApp.LoadIcon(IDI_ICON3);
	hIcon[3] = theApp.LoadIcon(IDI_ICON4);
	// ����ͼ������ CImageList ����
	m_imageList.Create( 32,32, ILC_COLOR32|TRUE, 3, 3);
	//CImageList *m_imagelist=new CImageList;
	//m_imageList.Create(13,13,ILC_COLOR32|TRUE,4,4); //͸����ʾ 32bit��ɫֵͼ��
	// ��ͼ����ӵ�ͼ������
	for (int i=0; i<4; i++)
	{
		m_imageList.Add(hIcon[i]);
	}

	// Ϊ���οؼ�����ͼ������
	m_webTree.SetImageList(&m_imageList, TVSIL_NORMAL);

	// ������ڵ�
	HTREEITEM hItem=m_webTree.InsertItem(_TEXT("�û�"),0,0,TVI_ROOT);

	HTREEITEM hSub=m_webTree.InsertItem(_TEXT("��Ʊϵͳ"),1,1,hItem);


	hSub=m_webTree.InsertItem(_TEXT("������ѯ"),2,2,hItem);
	//m_webTree.InsertItem(_TEXT("�人"),4,6,hSub);

	hSub=m_webTree.InsertItem(_TEXT("������Ϣ"),3,3,hItem);
	//m_webTree.InsertItem(_TEXT("���øĸ�"),5,6,hSub);
	//m_editFont.CreatePointFont(500, "����"); 
    //m_editPlace.SetFont(&m_editFont); // ����������
	// TODO: �ڴ����ר�ô����/����û���
}


//void FGDLG::OnSize(UINT nType, int cx, int cy)
//{
//	CFormView::OnSize(nType, cx, cy);
//    ChangeSize();
//	CFormView::ShowScrollBar(SB_BOTH, false);
//	// TODO: �ڴ˴������Ϣ����������
//}
/*void FGDLG::ChangeSize()
{
		CRect rect,rectts;
		GetClientRect(&rect);
		rectts.left=rect.left;
		//rectts.right=rect.left+rect.Width()*5/7;;
		//rectts.top=rect.bottom-600;
		rectts.bottom=rect.bottom;

		m_btn.MoveWindow(rectts,true);
}*/


void FGDLG::OnBnClickedButton4()
{
			if (MessageBox("��Ҫ�˳���ע������Ҫ���棡","��ʾ��Ϣ",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
		{
			AfxGetApp()-> m_pMainWnd->DestroyWindow();
		}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


//BOOL FGDLG::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	
//	CRect rect;
//	GetClientRect(rect);
//	Bitmap bmp(rect.Width(), rect.Height());
//	Graphics bmpGraphics(&bmp);
//	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
//
//	CString bkimg, bkimg1;
//	bkimg.Format(".\\9.jpg");
//	Image img(bkimg.AllocSysString(), TRUE);
//	//���ͼƬ
//	bmpGraphics.DrawImage(&img, 0, 0, rect.Width(), rect.Height());
//	//�����������Ļ
//	Graphics graphics(pDC->m_hDC);
//	CachedBitmap cachedBmp(&bmp, &graphics);
//	graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
//	return true;
//}


HBRUSH FGDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor); 
	if(   CTLCOLOR_STATIC   ==   nCtlColor   )  
	{ 
		//pDC->SetBkColor(RGB(255,0,0));  //��Ҫָ������ɫ��ʱ���� 
		pDC->SetTextColor(RGB(0,   255,   255));    //����ǰ��ɫ
		pDC->SetBkMode(TRANSPARENT);    //����͸��
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //��ȡ������ɫ��Ϻ�Ļ��ʣ����͸�� 
	}  
	return hbr;
}


//void FGDLG::OnBnClickedButton1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
//	pFram->ReplaceView(0,1,RUNTIME_CLASS(CCPDBView));
//}


//void FGDLG::OnBnClickedButton2()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
//	pFram->ReplaceView(0,1,RUNTIME_CLASS(DDDLG));
//}


//void FGDLG::OnBnClickedButton3()
//{
//	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
//	pFram->ReplaceView(0,1,RUNTIME_CLASS(GRDLG));
//}
void FGDLG::OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CString strText; // ���ڵ�ı�ǩ�ı��ַ���

	// ��ȡ��ǰѡ�нڵ�ľ��
	HTREEITEM hItem = m_webTree.GetSelectedItem();
	// ��ȡѡ�нڵ�ı�ǩ�ı��ַ���
	strText = m_webTree.GetItemText(hItem);//



	if(strText=="��Ʊϵͳ")
	{
		CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
		pFram->ReplaceView(0,1,RUNTIME_CLASS(CCPDBView));
	}
	else if(strText=="������ѯ")
	{
		CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
		pFram->ReplaceView(0,1,RUNTIME_CLASS(DDDLG));
	}
	else if(strText=="������Ϣ")
	{
		CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
		pFram->ReplaceView(0,1,RUNTIME_CLASS(GRDLG));
	}
	else return;

}

//void FGDLG::OnBnClickedButton1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}
