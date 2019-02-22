// FGDLG.cpp : 实现文件
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


// FGDLG 诊断

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


// FGDLG 消息处理程序


void FGDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_btn.SetToolTipText(_T("退出"));
	m_btn.SetSkin(IDB_BITMAP4,IDB_BITMAP5,IDB_BITMAP6,0,0,NULL,0,0,4);
	m_btn.SizeToContent();
	CRect rect;
	GetClientRect(rect);
	m_webTree.MoveWindow(0,0,rect.Width(),rect.Height());
	//m_webTree.SetBKImage("IDB_BITMAP8"); 
	HICON hIcon[4]; // 图标句柄数组
	HTREEITEM hRoot[4]; // 树的根节点的句柄
	HTREEITEM hCataItem; // 可表示任一分类节点的句柄
	HTREEITEM hArtItem; // 可表示任一文章节点的句柄

	m_webTree.SetTextColor(RGB(0,0,220));
	m_webTree.SetBkColor(RGB(228,243,249));

	// 加载三个图标，并将它们的句柄保存到数组
	hIcon[0] = theApp.LoadIcon(IDI_ICON1);
	hIcon[1] = theApp.LoadIcon(IDI_ICON2);
	hIcon[2] = theApp.LoadIcon(IDI_ICON3);
	hIcon[3] = theApp.LoadIcon(IDI_ICON4);
	// 创建图像序列 CImageList 对象
	m_imageList.Create( 32,32, ILC_COLOR32|TRUE, 3, 3);
	//CImageList *m_imagelist=new CImageList;
	//m_imageList.Create(13,13,ILC_COLOR32|TRUE,4,4); //透明显示 32bit颜色值图标
	// 将图标添加到图像序列
	for (int i=0; i<4; i++)
	{
		m_imageList.Add(hIcon[i]);
	}

	// 为树形控件设置图像序列
	m_webTree.SetImageList(&m_imageList, TVSIL_NORMAL);

	// 插入根节点
	HTREEITEM hItem=m_webTree.InsertItem(_TEXT("用户"),0,0,TVI_ROOT);

	HTREEITEM hSub=m_webTree.InsertItem(_TEXT("订票系统"),1,1,hItem);


	hSub=m_webTree.InsertItem(_TEXT("订单查询"),2,2,hItem);
	//m_webTree.InsertItem(_TEXT("武汉"),4,6,hSub);

	hSub=m_webTree.InsertItem(_TEXT("个人信息"),3,3,hItem);
	//m_webTree.InsertItem(_TEXT("经济改革"),5,6,hSub);
	//m_editFont.CreatePointFont(500, "宋体"); 
    //m_editPlace.SetFont(&m_editFont); // 设置新字体
	// TODO: 在此添加专用代码和/或调用基类
}


//void FGDLG::OnSize(UINT nType, int cx, int cy)
//{
//	CFormView::OnSize(nType, cx, cy);
//    ChangeSize();
//	CFormView::ShowScrollBar(SB_BOTH, false);
//	// TODO: 在此处添加消息处理程序代码
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
			if (MessageBox("真要退出吗，注意数据要保存！","提示信息",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
		{
			AfxGetApp()-> m_pMainWnd->DestroyWindow();
		}
	// TODO: 在此添加控件通知处理程序代码
}


//BOOL FGDLG::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
//	//输出图片
//	bmpGraphics.DrawImage(&img, 0, 0, rect.Width(), rect.Height());
//	//拷贝输出到屏幕
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
		//pDC->SetBkColor(RGB(255,0,0));  //需要指定背景色的时候用 
		pDC->SetTextColor(RGB(0,   255,   255));    //文字前景色
		pDC->SetBkMode(TRANSPARENT);    //设置透明
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //获取画笔颜色混合后的画笔，完成透明 
	}  
	return hbr;
}


//void FGDLG::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
//	pFram->ReplaceView(0,1,RUNTIME_CLASS(CCPDBView));
//}


//void FGDLG::OnBnClickedButton2()
//{
//	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString strText; // 树节点的标签文本字符串

	// 获取当前选中节点的句柄
	HTREEITEM hItem = m_webTree.GetSelectedItem();
	// 获取选中节点的标签文本字符串
	strText = m_webTree.GetItemText(hItem);//



	if(strText=="订票系统")
	{
		CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
		pFram->ReplaceView(0,1,RUNTIME_CLASS(CCPDBView));
	}
	else if(strText=="订单查询")
	{
		CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
		pFram->ReplaceView(0,1,RUNTIME_CLASS(DDDLG));
	}
	else if(strText=="个人信息")
	{
		CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd();
		pFram->ReplaceView(0,1,RUNTIME_CLASS(GRDLG));
	}
	else return;

}

//void FGDLG::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
