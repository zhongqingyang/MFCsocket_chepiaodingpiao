// DLDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CPDB.h"
#include "DLDLG.h"
#include "afxdialogex.h"
#include "CMessg.h"
#include "DDGLDLG.h"
#include "MainFrm.h"
#include "CPDBView.h"
#define DEFADDR TEXT("127.0.0.1" )//��������Ĭ��IP��ַ
#define DEFPORT 34567             //�������Ŀ��Ŷ˿�
COLORREF		MENU_BOTTOM_COL = RGB(10,130,210);//������ɫ
/////////////////////////////////////////////////////////////////////////////
// DLDLG �Ի���

IMPLEMENT_DYNAMIC(DLDLG, CDialogEx)

	DLDLG::DLDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(DLDLG::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_pwd = _T("");
	m_user = _T("");
	m_nPort = 0;
}

DLDLG::~DLDLG()
{
}

void DLDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Control(pDX, IDC_BUTTON1, m_btn);
	DDX_Control(pDX, IDC_BUTTON2, m_btn1);
	DDX_Control(pDX, IDCANCEL, m_btn2);
	DDX_Text(pDX, IDC_EDIT2, m_pwd);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT4, m_nPort);
}


BEGIN_MESSAGE_MAP(DLDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DLDLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &DLDLG::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &DLDLG::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON4, &DLDLG::OnBnClickedButton4)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// DLDLG ��Ϣ�������


void DLDLG::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString jiemipwd,str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11;	
	if(!theApp.m_HCSocket.OnLine)//���ӷ�����
		if(!Connect())
			return;

	CMessg msg;
	msg.m_strText.Format("select * from [User] where UseZH='%s' and UsePwd ='%s'",m_user,m_pwd);
	msg.m_type = 'a';
	if(!theApp.m_HCSocket.SendMessage(msg))  
		return;
	CMessg *recmsg = new CMessg();
	if(theApp.m_HCSocket.RecMessage(recmsg))
	{
		if (recmsg->m_OperState==0)
		{
			AfxMessageBox("�����û���������");
			theApp.m_HCSocket.Close();
			theApp.m_HCSocket.OnLine=false;
			return;
		}
		else
		{
			CMessg msg;
			msg.m_strText.Format("select   *  from [User]    where UseZH='%s' and UseQX like '%%%s'",m_user,"����Ա");
			msg.m_type = 'a';
			if(!theApp.m_HCSocket.SendMessage(msg))  
				return;
			CMessg *recmsg = new CMessg();
			if(theApp.m_HCSocket.RecMessage(recmsg))
			{
				if (recmsg->m_OperState==0)
		      {
				  m_qx="�û�";
				}else{
					m_qx="����Ա";
				}
			}
				theApp.m_name=m_user;
				EndDialog(IDOK);
				//AfxMessageBox("��½�ɹ�,���Ժ�...");
				theApp.m_HCSocket.OnLine=true;	
			}
		}
	//AfxMessageBox(m_qx);
	}


	void DLDLG::OnBnClickedButton2()
	{
		m_bIsExpand = !m_bIsExpand ;
		//	CDealDlg* lpMainDlg = (CDealDlg*)GetOwner() ;

		if (! m_bIsExpand )
		{
			SetDlgItemText( IDC_BUTTON2, TEXT( "�������á�" ) ) ;
			SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight, SWP_NOMOVE ) ;
			//GetOwner()->SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight, SWP_NOMOVE ) ;
		}

		else
		{
			SetDlgItemText( IDC_BUTTON2,TEXT( "�������á�" ) ) ;
			SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight * 2 / 3-5, SWP_NOMOVE ) ;
			//GetOwner()->SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight * 2 / 3, SWP_NOMOVE ) ;

		}

		// ���´���״̬��
		int nPart = -1 ;	
	}


	BOOL DLDLG::OnInitDialog()
	{
		CDialogEx::OnInitDialog();
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
		m_btn.SetToolTipText(_T("��¼"));
		m_btn.SetSkin(IDB_BITMAP2,IDB_BITMAP1,IDB_BITMAP3,0,0,NULL,0,0,4);
		m_btn.SizeToContent();
		m_btn1.SetToolTipText(_T("��������"));
		m_btn1.SetSkin(IDB_BITMAP2,IDB_BITMAP1,IDB_BITMAP3,0,0,NULL,0,0,4);
		m_btn1.SizeToContent();
		m_btn2.SetToolTipText(_T("ȡ��"));
		m_btn2.SetSkin(IDB_BITMAP2,IDB_BITMAP1,IDB_BITMAP3,0,0,NULL,0,0,4);
		m_btn2.SizeToContent();
		CRect rcDlg ;
		GetWindowRect( rcDlg ) ;
		m_nDlgWidth = rcDlg.Width() ;
		m_nDlgHight = rcDlg.Height() ;
		((CButton*)GetDlgItem( IDC_CHECK1) )->SetCheck(1);
		OnSelectdefault();
		m_bIsExpand=true;

		ReadCfgFile();//��ȡ�����ļ�����ʾ

		InitNet() ;// ��ʼ����������
		// ��������
		OnInternet();
		//Ĭ����ʾ
		m_user="admin";
		m_pwd="1";
		UpdateData(FALSE);

		return TRUE;  // return TRUE unless you set the focus to a control
		// �쳣: OCX ����ҳӦ���� FALSE
	}


	void DLDLG::OnBnClickedCheck1()
	{
		if ( ( (CButton*)GetDlgItem( IDC_CHECK1 ) )->GetCheck() )
		{
			GetDlgItem( IDC_IPADDRESS1 )->EnableWindow( FALSE ) ;
			GetDlgItem( IDC_EDIT4 )->EnableWindow( FALSE ) ;
		}

		else
		{
			GetDlgItem( IDC_IPADDRESS1 )->EnableWindow( TRUE ) ;
			GetDlgItem( IDC_EDIT4 )->EnableWindow( TRUE ) ;
			( (CIPAddressCtrl*)GetDlgItem( IDC_IPADDRESS1 ) )->SetFieldFocus( 4 ) ;
		}	
	}
	BOOL DLDLG::Connect()//���ӷ�����
	{
		//��ȡ�����IP��PORT
		UpdateData(TRUE);
		strIP = GetAddr();
		if ( strIP.IsEmpty() )
		{
			AfxMessageBox("�����������IP��ַ");	
			return false;
		}
		//strIP="hccczu.eicp.net";
		// ��÷������˿�
		nPort = GetPort() ;
		if ( !nPort )
		{
			return false;
		}
		theApp.m_ips=strIP;//��ȡIP
		theApp.ports=nPort;//��ȡ�˿�

		theApp.m_HCSocket.Create();
		theApp.m_HCSocket.SetTimeOut(500);
		if(theApp.m_HCSocket.Connect(strIP,nPort))
		{	
			theApp.m_HCSocket.Init();
			theApp.m_HCSocket.KillTimeOut();
			theApp.m_HCSocket.OnLine=true;
			return true;
		}
		else
		{
			GetDlgItem( IDC_BUTTON2 )->SetFocus() ;
			theApp.m_HCSocket.KillTimeOut();
			//AfxMessageBox("���ӷ�����ʧ�ܣ�", MB_USERDEFINE);
			AfxMessageBox("����ʧ�ܣ����ȡ��������á�����IP��ַ");
			theApp.m_HCSocket.Close();
			theApp.m_HCSocket.OnLine=false;
			return false;
		}

	}
	int DLDLG::GetPort()//��ȡ�˿�
	{
		if ( ( (CButton*)GetDlgItem( IDC_BUTTON2) )->GetCheck() )
		{
			return nPort ;
		}

		CString strPort ;
		CEdit* lpPortEdit = (CEdit*)GetDlgItem( IDC_EDIT4 ) ;
		lpPortEdit->GetWindowText( strPort ) ;

		if (strPort.IsEmpty() )
		{
			AfxMessageBox("����д�������˿�" );
			lpPortEdit->SetFocus() ;
			return 0 ;
		}

		if ( atoi( strPort ) < 1 || atoi( strPort ) > 65535 )
		{
			AfxMessageBox("�������˿ڷ�ΧӦ����1-65535֮��");
			return 0 ;
		}

		return atoi( strPort ) ;
	}
	CString DLDLG::GetAddr()//��ȡIP
	{
		if ( ( (CButton*)GetDlgItem( IDC_BUTTON2 ) )->GetCheck() )
		{
			return strIP ;
		}

		CString strAddr ;
		BYTE bit1, bit2, bit3, bit4 ;
		CIPAddressCtrl* lpIpEdit = (CIPAddressCtrl*)GetDlgItem( IDC_IPADDRESS1 ) ;
		lpIpEdit->GetAddress( bit1, bit2, bit3, bit4 ) ;


		strAddr.Format( TEXT( "%d.%d.%d.%d" ), bit1, bit2, bit3, bit4 ) ;
		return strAddr ;
	}


	void DLDLG::OnBnClickedButton4()
	{
		CString strAddr = GetAddr() ;
		if ( strAddr.IsEmpty() )
		{
			AfxMessageBox("�����������IP��ַ");
			return ;
		}

		// ��÷������˿�
		int inptPort = GetPort() ;

		CString strPort;
		strPort.Format("%d",inptPort);
		Inifile.SetKeyValue("ServerInfo", "Ip",strAddr);
		Inifile.SetKeyValue("ServerInfo", "Port",strPort);
		AfxMessageBox("��ϲ������������Ϣ�ɹ���");
	}
	void DLDLG::InitNet()
	{
		//��ȡ�����ļ��е�IP PORT ��ʾ,���û�������ļ�����ʾ����
		CIPAddressCtrl* lpIpEdit = (CIPAddressCtrl*)GetDlgItem( IDC_IPADDRESS1 ) ;
		lpIpEdit->SetWindowText(strIP);//Ĭ��IP
		m_nPort=nPort;//Ĭ�϶˿�
		UpdateData(FALSE);

		//Ĭ��checkѡ��
		( (CButton*)GetDlgItem( IDC_BUTTON2) )->SetCheck( TRUE ) ;
		OnInternet() ;
	}
	BOOL DLDLG::ReadCfgFile()
	{
		//���ini�ļ��ľ���·��
		CFileFind finder;
		BOOL bWorking = finder.FindFile(theApp.configpath);//Ѱ���ļ�
		if(!bWorking)
		{
			//MessageBox("û���ҵ���¼�����ļ���","��ʾ��Ϣ",MB_ICONINFORMATION);
			strIP = DEFADDR;
			nPort = DEFPORT;
			return false;
		}
		Inifile.SetPath(theApp.configpath);
		//��ȡIP PORT
		strIP = Inifile.GetKeyValue("ServerInfo", "IP");
		nPort = atoi(Inifile.GetKeyValue("ServerInfo", "Port"));
		return true;
	}
	void DLDLG::OnInternet() //����������������
	{
		m_bIsExpand = !m_bIsExpand ;
		//	CDealDlg* lpMainDlg = (CDealDlg*)GetOwner() ;

		if (! m_bIsExpand )
		{
			SetDlgItemText( IDC_BUTTON2, TEXT( "�������á�" ) ) ;
			SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight, SWP_NOMOVE ) ;
			//GetOwner()->SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight, SWP_NOMOVE ) ;
		}

		else
		{
			SetDlgItemText( IDC_BUTTON2,TEXT( "�������á�" ) ) ;
			SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight * 2 / 3-5, SWP_NOMOVE ) ;
			//GetOwner()->SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight * 2 / 3, SWP_NOMOVE ) ;

		}

		// ���´���״̬��
		int nPart = -1 ;	
	}
	void DLDLG::OnSelectdefault() 
	{
		if ( ( (CButton*)GetDlgItem( IDC_CHECK1 ) )->GetCheck() )
		{
			GetDlgItem( IDC_IPADDRESS1 )->EnableWindow( FALSE ) ;
			GetDlgItem( IDC_EDIT4 )->EnableWindow( FALSE ) ;
		}

		else
		{
			GetDlgItem( IDC_IPADDRESS1 )->EnableWindow( TRUE ) ;
			GetDlgItem( IDC_EDIT4)->EnableWindow( TRUE ) ;
			( (CIPAddressCtrl*)GetDlgItem( IDC_IPADDRESS1) )->SetFieldFocus( 4 ) ;
		}	
	}

	BOOL DLDLG::OnEraseBkgnd(CDC* pDC)
	{
		CRect rect;
		GetClientRect(rect);
		Bitmap bmp(rect.Width(), rect.Height());
		Graphics bmpGraphics(&bmp);
		bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

		CString bkimg, bkimg1;
		bkimg.Format(".\\11.jpg");
		Image img(bkimg.AllocSysString(), TRUE);
		//���ͼƬ
		bmpGraphics.DrawImage(&img, 0, 0, rect.Width(), rect.Height());
		//�����������Ļ
		Graphics graphics(pDC->m_hDC);
		CachedBitmap cachedBmp(&bmp, &graphics);
		graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
		return true;
	}


	HBRUSH DLDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
	{
		HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
		if(   CTLCOLOR_STATIC   ==   nCtlColor   )  
		{ 
			//pDC->SetBkColor(RGB(255,0,0));  //��Ҫָ������ɫ��ʱ���� 
			pDC->SetTextColor(RGB(255,   255,   255));    //����ǰ��ɫ
			pDC->SetBkMode(TRANSPARENT);    //����͸��
			hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //��ȡ������ɫ��Ϻ�Ļ��ʣ����͸�� 
		}  
		return hbr;
	}


	void DLDLG::OnPaint()
	{
		if (IsIconic())
		{
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			CDialog::OnPaint();
		}
	}


	HCURSOR DLDLG::OnQueryDragIcon()
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		return (HCURSOR) m_hIcon;
		return CDialogEx::OnQueryDragIcon();
	}
	
	CString DLDLG::selectt(CString strsql)//��ѯ
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