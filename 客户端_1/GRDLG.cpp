// GRDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CPDB.h"
#include "GRDLG.h"

#include "CMessg.h"
// GRDLG
IMPLEMENT_DYNCREATE(GRDLG, CFormView)
	bool bold=false;
GRDLG::GRDLG()
	: CFormView(GRDLG::IDD)
{

	//  m_edit5 = _T("");
}

GRDLG::~GRDLG()
{
}

void GRDLG::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo1);
	DDX_Control(pDX, IDC_COMBO2, m_combo2);
	//  DDX_Text(pDX, IDC_EDIT5, m_edit5);
}

BEGIN_MESSAGE_MAP(GRDLG, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &GRDLG::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &GRDLG::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON3, &GRDLG::OnBnClickedButton3)
END_MESSAGE_MAP()


// GRDLG ���

#ifdef _DEBUG
void GRDLG::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void GRDLG::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// GRDLG ��Ϣ�������


void GRDLG::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
	m_combo1.InsertString(0,"��");
	m_combo1.InsertString(1,"Ů");
	m_combo1.SetCurSel(0);
	m_combo2.InsertString(0,"50");
	m_combo2.InsertString(1,"100");
	m_combo2.InsertString(1,"200");
	m_combo2.InsertString(1,"300");
	m_combo2.SetCurSel(0);
	CString strsql;
	strsql.Format("select * from [User]  where UseZH='%s'",m_user);
	
	ShowData(strsql);

	
	// TODO: �ڴ����ר�ô����/����û���
}
void GRDLG::ShowData(CString strsql)
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
		if (recmsg->m_OperState==0)
			return;
		else
		{
			CString output,stemp;
					AfxExtractSubString(output,recmsg->m_strText,0,'|');
					//AfxMessageBox(output);
					GetDlgItem(IDC_EDIT1)->SetWindowText(output);
					AfxExtractSubString(output,recmsg->m_strText,1,'|');
					GetDlgItem(IDC_COMBO1)->SetWindowText(output);
					AfxExtractSubString(output,recmsg->m_strText,2,'|');
					GetDlgItem(IDC_EDIT2)->SetWindowText(output);
					AfxExtractSubString(output,recmsg->m_strText,4,'|');
					GetDlgItem(ID_YUE)->SetWindowText(output);
					AfxExtractSubString(output,recmsg->m_strText,5,'|');
					GetDlgItem(IDC_EDIT6)->SetWindowText(output);
					AfxExtractSubString(output,recmsg->m_strText,6,'|');
					GetDlgItem(IDC_EDIT7)->SetWindowText(output);

		}
	}
}
void GRDLG::OnBnClickedButton2()
{
	
	if(bold==false){
	GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
	bold=true;
	}else{
		GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
		bold=false;
	}
}
void GRDLG::OnBnClickedButton1()
{
	CString str1,str2,str3;
	GetDlgItem(IDC_EDIT8)->GetWindowText(str1);
	GetDlgItem(IDC_EDIT9)->GetWindowText(str2);
	GetDlgItem(IDC_EDIT10)->GetWindowText(str3);
	CString strn1,strn2,strn3,strn4;
	GetDlgItem(IDC_COMBO1)->GetWindowText(strn1);
	GetDlgItem(IDC_EDIT2)->GetWindowText(strn2);
	GetDlgItem(IDC_EDIT6)->GetWindowText(strn3);
	GetDlgItem(IDC_EDIT7)->GetWindowText(strn4);
	if (str1!=""&&str2!=""&&str3!="")
	{
		if (str1!=m_pwd)
		{
			AfxMessageBox("ԭ���벻��ȷ��������ȷ��");
			return;
		}else{
			if(str2!=str3)
			{
				AfxMessageBox("���벻һ��");
				return;
			}else{
				CString strsql;
				strsql.Format("update [User] set [UseSex]='%s' , [UsePhone]='%s'  ,[UsePwd]='%s' , [UseName]='%s' , [UseID]='%s' where [UseZH]='%s'",strn1,strn2,str3,strn3,strn4,m_user);
				ExeSQL(strsql);
			}
			AfxMessageBox("����ɹ�!");
			GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT8)->SetWindowText("");
			GetDlgItem(IDC_EDIT9)->SetWindowText("");
			GetDlgItem(IDC_EDIT10)->SetWindowText("");
		}
			//AfxMessageBox("����д����");
		//	return;
	}else{
		CString strsql;
		strsql.Format("update [User] set [UseSex]='%s',[UsePhone]='%s',[UseName]='%s',[UseID]='%s' where [UseZH]='%s'",strn1,strn2,strn3,strn4,m_user);
		ExeSQL(strsql);
		AfxMessageBox("����ɹ�!");
		GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
	}
}
bool GRDLG::ExeSQL(CString strsql)
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


HBRUSH GRDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if(   CTLCOLOR_STATIC   ==   nCtlColor   )  
	{ 
		//pDC->SetBkColor(RGB(255,0,0));  //��Ҫָ������ɫ��ʱ���� 
		pDC->SetTextColor(RGB(0,   200,   0));    //����ǰ��ɫ
		pDC->SetBkMode(TRANSPARENT);    //����͸��
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //��ȡ������ɫ��Ϻ�Ļ��ʣ����͸�� 
	}  
	return hbr;
}


BOOL GRDLG::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	Bitmap bmp(rect.Width(), rect.Height());
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CString bkimg, bkimg1;
	bkimg.Format(".\\10.jpg");
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


void GRDLG::OnBnClickedButton3()
{
	CString sm1,sm2;
	GetDlgItem(ID_YUE)->GetWindowText(sm1);
	GetDlgItem(ID_YUE)->SetWindowText("");
	GetDlgItem(IDC_COMBO2)->GetWindowText(sm2);
	CString strsql;
	int strtemp=atoi(sm1)+atoi(sm2);
	strsql.Format("update [User] set [money]='%d' where [UseZH]='%s'",strtemp,m_user);
	ExeSQL(strsql);
	MessageBox("��ֵ�ɹ�");
	CString strsql1;
	strsql1.Format("%d",strtemp);
	GetDlgItem(ID_YUE)->SetWindowTextA(strsql1);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
