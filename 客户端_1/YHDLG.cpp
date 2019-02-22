// YHDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CPDB.h"
#include "YHDLG.h"
#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"
#include "CMessg.h"

// YHDLG
static int CurSelRow=0,CurSelCol=0;
const int MAXROW	=10;	//�������
const int INFO_LEN	=300;	//ÿ����󳤶�

static int ColNum1=8;
static CString ColsName1[8]={"�˻���","�Ա�","�ֻ���","����","���","����","���֤","Ȩ��"};

static int ColWidth[8]={150,150,150,150,150,150,150,150};//�п�
IMPLEMENT_DYNCREATE(YHDLG, CFormView)

YHDLG::YHDLG()
	: CFormView(YHDLG::IDD)
{

}

YHDLG::~YHDLG()
{
}

void YHDLG::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_CUSTOM1, m_list);
	DDX_GridControl(pDX, IDC_CUSTOM2, m_list2);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
}

BEGIN_MESSAGE_MAP(YHDLG, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &YHDLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &YHDLG::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK,IDC_CUSTOM1, OnClickList1)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// YHDLG ���

#ifdef _DEBUG
void YHDLG::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void YHDLG::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// YHDLG ��Ϣ�������


void YHDLG::InitList(int MaxRow)
{
	TRY 
	{
		m_list.SetRowCount(MaxRow);
		m_list.SetColumnCount(ColNum1);
		m_list.SetFixedRowCount(1);//m_nFixRows=1
		m_list.SetFixedColumnCount(0);
		m_list.SetEditable(FALSE);
		m_list.SetFixedBkColor(RGB(228,243,249));//���ⱳ����ɫ
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return ;
	}
	END_CATCH

		// fill rows/cols with text
		for (int row = 0; row <m_list.GetRowCount(); row++)//�����У������̶��У�����Ŀ
		{

			for (int col = 0; col < ColNum1; col++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = col;

				if(row < m_list.GetFixedRowCount())//���ع̶��е���Ŀ
				{
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("%s"),ColsName1[col]);
				}
				m_list.SetItem(&Item);

				//���ڸ�ʽ��Ԫ��
				/*if(col==8)
				{
					m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellDateTime));
				}

				//���ָ�ʽ��Ԫ��
				if(col==0||col==3)
				{
					m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellNumeric));
				}
				//��Ͽ��ʽ��Ԫ���ʼ��
				if( col==2)
				{
					m_list.SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));

					CGridCellCombo *pCell=(CGridCellCombo *)m_list.GetCell(row,col);
					CStringArray ar;
					ar.SetSize(2);
					ar.SetAt(0,"��");
					ar.SetAt(1,"Ů");
					pCell->SetOptions(ar);
				}*/
			}

			m_list.ExpandLastColumn();
			m_list.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

			//�����п�
			for (int i = 0; i < ColNum1; i++)
			{
				m_list.SetColumnWidth(i,ColWidth[i]);
			}
			for (int i = 1; i < MAXROW; i++)
			{
				m_list.SetRowHeight(i,24);//�����и�
			}

			m_list.Invalidate();		
		}
}

void YHDLG::InitList2(int MaxRow)//��̬��ʾ����
{
	TRY 
	{
		m_list2.SetRowCount(MaxRow);
		m_list2.SetColumnCount(ColNum1);
		m_list2.SetFixedRowCount(1);//m_nFixRows=1
		m_list2.SetFixedColumnCount(0);
		m_list2.SetFixedBkColor(RGB(228,243,249));//���ⱳ����ɫ
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return ;
	}
	END_CATCH

		// fill rows/cols with text
		for (int row = 0; row <m_list2.GetRowCount(); row++)
		{

			for (int col = 0; col < ColNum1; col++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = col;

				if(row < m_list2.GetFixedRowCount())
				{
					Item.nFormat = DT_CENTER|DT_WORDBREAK;
					Item.strText.Format(_T("%s"),ColsName1[col]);
				}
				m_list2.SetItem(&Item);

				//���ڸ�ʽ��Ԫ��
				/*if(col==5 || col==7  )
				{
					m_list2.SetCellType(row,col, RUNTIME_CLASS(CGridCellDateTime));
				}

				//���ָ�ʽ��Ԫ��
				if(col==0 || col==3)
				{
					m_list2.SetCellType(row,col, RUNTIME_CLASS(CGridCellNumeric));
				}*/
				//��Ͽ��ʽ��Ԫ���ʼ��
				if( col==1)
				{
					m_list2.SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));

					CGridCellCombo *pCell=(CGridCellCombo *)m_list2.GetCell(row,col);
					CStringArray ar;
					ar.SetSize(2);
					ar.SetAt(0,"��");
					ar.SetAt(1,"Ů");
					pCell->SetOptions(ar);
				}
			}
		}

		m_list2.ExpandLastColumn();
		m_list2.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

		//�����п�
		for (int i = 0; i < ColNum1; i++)
		{
			m_list2.SetColumnWidth(i,ColWidth[i]);
		}
		for (int i = 1; i < MAXROW; i++)
		{
			m_list2.SetRowHeight(i,24);//�����и�
		}

		m_list2.Invalidate();

}

void YHDLG::ShowData(CString strsql)
{
	CMessg msg;
	//��ʾ��������
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
					m_list.SetItemText(i+1,j,output);//���

				}
			}
		}
	}
	m_list.Invalidate();
}

void YHDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//GDI+��ʼ��
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	InitList(MAXROW);//��ʼgridctr��Ϣ��ʾ
	InitList2(MAXROW);//��ʼgridctr��Ϣ��ʾ
	CString strsql;
	CString str="�û�";
	strsql.Format("select [UseZH],[UseSex] ,[UsePhone],[UsePwd],[money],[UseName],[UseID],[UseQX] from [User] where UseQX='%s'",str);
	ShowData(strsql);
	// TODO: �ڴ����ר�ô����/����û���
}
void YHDLG::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW*) pNMHDR;
	int selno = pNmgv->iRow;
	if(selno==-1) 
	{ 
		return; 
	} 
	CString strno,strst;
	strno = m_list.GetItemText(selno,0);
	//��ѯ���ݣ�����ʾ����
	//ShowAllInfo(strno);
	strst.Format("select [UseZH],[UseSex] ,[UsePhone],[UsePwd],[money],[UseName],[UseID],[UseQX] from [User] where [UseZH]='%s'",strno);
	ShowData1(strst);
	*pResult = 0;
}

void YHDLG::ShowData1(CString strsql)
{
	CMessg msg;
	//��ʾ��������
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
					m_list2.SetItemText(i+1,j,output);//���

				}
			}
		}
	}
	m_list2.Invalidate();
}

void YHDLG::OnBnClickedButton1()
{
	m_list2.EndEdit();//ȷ�ϱ༭�Ľ��
	CString strtp1,strsql,strtp2;
	vector<int> SaveRow;
	for(int i=1;i<20;i++)//
	{
		for(int j=0;j<2;j++)
		{
			strtp1 = m_list2.GetItemText(i,0);
			strtp1.TrimRight();//�����ұ߿ո��ַ�
			strtp2 = m_list2.GetItemText(i,1);
			strtp2.TrimRight();
		}
		if( strtp1 ==""&&strtp2!="")//��ֹ�鵽����Ŀհ���
		{
			AfxMessageBox("�˻�������Ϊ��");
			return;
		}
		else if( strtp1 !=""&&strtp2!="")
			SaveRow.push_back(i);//�����ݵ���
	}
	//!=0����ա�����
	for(int i=0;i<SaveRow.size();i++)
	{
		strsql.Format("delete from  [User] where UseZH='%s' ",m_list2.GetItemText(SaveRow[i],0));//ɾ����������
		if(!ExeSQL(strsql))
		{
			AfxMessageBox("����ʧ��");
			return;
		}
	}

	for(int i=0;i<SaveRow.size();i++)
	{
		CString strtemp;
		strsql.Format("insert into [User] (UseZH,UseSex,UsePhone,UsePwd,money,UseName,UseID,UseQX) values(");//����������и��µ�����
		for(int j=0;j<ColNum1;j++)
		{
			if(j==0)
				strtemp.Format("'%s'",m_list2.GetItemText(SaveRow[i],j));
			else
				strtemp.Format(",'%s'",m_list2.GetItemText(SaveRow[i],j));
			strsql+=strtemp;

		}
		strsql+=")";
		if(!ExeSQL(strsql))
		{
			AfxMessageBox("����ʧ��");
			return;
		}
	}
	Clear();
	//ShowAllTeam();
	AfxMessageBox("�������ݳɹ���");
	CString strsql1;
	CString str="�û�";
	strsql1.Format("select [UseZH],[UseSex] ,[UsePhone],[UsePwd],[money],[UseName],[UseID],[UseQX] from [User] where UseQX='%s'",str);
	ShowData(strsql1);
}

bool YHDLG::ExeSQL(CString strsql)
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
void YHDLG::OnBnClickedButton2()
{
	CString strsql,str1,str2;
	int nTemp=m_list.GetFocusCell().row;
	str1=m_list.GetItemText(nTemp,0);
	if (str1=="")
	{
		AfxMessageBox("��ѡ���û�");
		return;
	}
	if (MessageBox("ȷ��Ҫɾ�����û���","��ʾ��Ϣ",MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2)==IDYES)
	{
		
		//str2=m_list.GetItemText(nTemp,8);
		strsql.Format("delete from [User] where UseZH='%s'",str1);
		ExeSQL(strsql);
		AfxMessageBox("ɾ���ɹ���");
		Clear();
		CString strsql1;
		CString str="�û�";
		strsql1.Format("select [UseZH],[UseSex] ,[UsePhone],[UsePwd],[money],[UseName],[UseID],[UseQX] from [User] where UseQX='%s'",str);
		ShowData(strsql1);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void YHDLG::Clear()
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<8;j++)
		{
			m_list.SetItemText(i+1,j,"");
			m_list2.SetItemText(i+1,j,"");//���
		}
	}
	m_list.Invalidate();
	m_list2.Invalidate();
}




void YHDLG::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ChangeSize();
	CFormView::ShowScrollBar(SB_BOTH, false);
	// TODO: �ڴ˴������Ϣ����������
}


HBRUSH YHDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL YHDLG::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\6.jpg");
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
void YHDLG::ChangeSize()
{
	if(m_list.m_hWnd) 
	{
		CRect rect,rectts;
		GetClientRect(&rect);

		rectts.left=rect.left;
		rectts.right=rect.right;
		rectts.top=rect.bottom-300;
		rectts.bottom=rect.bottom-100;
		m_list.MoveWindow(rectts,true);

		rectts.left=rect.left;
		rectts.right=rect.right;
		rectts.top=rect.bottom-500;
		rectts.bottom=rect.bottom-300;
		m_list2.MoveWindow(rectts,true);

		rectts.left=rect.left+200;
		rectts.right=rect.left+300;
		rectts.top=rect.bottom-650;
		rectts.bottom=rect.bottom-600;
		m_button1.MoveWindow(rectts,true);

		rectts.left=rect.left+500;
		rectts.right=rect.left+600;
		rectts.top=rect.bottom-650;
		rectts.bottom=rect.bottom-600;
		m_button2.MoveWindow(rectts,true);


		m_list.ShowWindow(TRUE);
	}
}