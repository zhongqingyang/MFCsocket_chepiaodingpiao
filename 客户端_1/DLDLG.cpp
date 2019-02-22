// DLDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDB.h"
#include "DLDLG.h"
#include "afxdialogex.h"
#include "CMessg.h"
#include "DDGLDLG.h"
#include "MainFrm.h"
#include "CPDBView.h"
#define DEFADDR TEXT("127.0.0.1" )//服务器的默认IP地址
#define DEFPORT 34567             //服务器的开放端口
COLORREF		MENU_BOTTOM_COL = RGB(10,130,210);//背景颜色
/////////////////////////////////////////////////////////////////////////////
// DLDLG 对话框

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


// DLDLG 消息处理程序


void DLDLG::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString jiemipwd,str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11;	
	if(!theApp.m_HCSocket.OnLine)//链接服务器
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
			AfxMessageBox("请检查用户名和密码");
			theApp.m_HCSocket.Close();
			theApp.m_HCSocket.OnLine=false;
			return;
		}
		else
		{
			CMessg msg;
			msg.m_strText.Format("select   *  from [User]    where UseZH='%s' and UseQX like '%%%s'",m_user,"管理员");
			msg.m_type = 'a';
			if(!theApp.m_HCSocket.SendMessage(msg))  
				return;
			CMessg *recmsg = new CMessg();
			if(theApp.m_HCSocket.RecMessage(recmsg))
			{
				if (recmsg->m_OperState==0)
		      {
				  m_qx="用户";
				}else{
					m_qx="管理员";
				}
			}
				theApp.m_name=m_user;
				EndDialog(IDOK);
				//AfxMessageBox("登陆成功,请稍后...");
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
			SetDlgItemText( IDC_BUTTON2, TEXT( "网络设置↑" ) ) ;
			SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight, SWP_NOMOVE ) ;
			//GetOwner()->SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight, SWP_NOMOVE ) ;
		}

		else
		{
			SetDlgItemText( IDC_BUTTON2,TEXT( "网络设置↓" ) ) ;
			SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight * 2 / 3-5, SWP_NOMOVE ) ;
			//GetOwner()->SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight * 2 / 3, SWP_NOMOVE ) ;

		}

		// 重新创建状态栏
		int nPart = -1 ;	
	}


	BOOL DLDLG::OnInitDialog()
	{
		CDialogEx::OnInitDialog();
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
		m_btn.SetToolTipText(_T("登录"));
		m_btn.SetSkin(IDB_BITMAP2,IDB_BITMAP1,IDB_BITMAP3,0,0,NULL,0,0,4);
		m_btn.SizeToContent();
		m_btn1.SetToolTipText(_T("网络设置"));
		m_btn1.SetSkin(IDB_BITMAP2,IDB_BITMAP1,IDB_BITMAP3,0,0,NULL,0,0,4);
		m_btn1.SizeToContent();
		m_btn2.SetToolTipText(_T("取消"));
		m_btn2.SetSkin(IDB_BITMAP2,IDB_BITMAP1,IDB_BITMAP3,0,0,NULL,0,0,4);
		m_btn2.SizeToContent();
		CRect rcDlg ;
		GetWindowRect( rcDlg ) ;
		m_nDlgWidth = rcDlg.Width() ;
		m_nDlgHight = rcDlg.Height() ;
		((CButton*)GetDlgItem( IDC_CHECK1) )->SetCheck(1);
		OnSelectdefault();
		m_bIsExpand=true;

		ReadCfgFile();//读取配置文件，显示

		InitNet() ;// 初始化网络设置
		// 收缩窗口
		OnInternet();
		//默认显示
		m_user="admin";
		m_pwd="1";
		UpdateData(FALSE);

		return TRUE;  // return TRUE unless you set the focus to a control
		// 异常: OCX 属性页应返回 FALSE
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
	BOOL DLDLG::Connect()//连接服务器
	{
		//获取输入的IP和PORT
		UpdateData(TRUE);
		strIP = GetAddr();
		if ( strIP.IsEmpty() )
		{
			AfxMessageBox("请输入服务器IP地址");	
			return false;
		}
		//strIP="hccczu.eicp.net";
		// 获得服务器端口
		nPort = GetPort() ;
		if ( !nPort )
		{
			return false;
		}
		theApp.m_ips=strIP;//获取IP
		theApp.ports=nPort;//获取端口

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
			//AfxMessageBox("连接服务器失败！", MB_USERDEFINE);
			AfxMessageBox("连接失败！请先【网络设置】配置IP地址");
			theApp.m_HCSocket.Close();
			theApp.m_HCSocket.OnLine=false;
			return false;
		}

	}
	int DLDLG::GetPort()//获取端口
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
			AfxMessageBox("请填写服务器端口" );
			lpPortEdit->SetFocus() ;
			return 0 ;
		}

		if ( atoi( strPort ) < 1 || atoi( strPort ) > 65535 )
		{
			AfxMessageBox("服务器端口范围应该在1-65535之间");
			return 0 ;
		}

		return atoi( strPort ) ;
	}
	CString DLDLG::GetAddr()//获取IP
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
			AfxMessageBox("请输入服务器IP地址");
			return ;
		}

		// 获得服务器端口
		int inptPort = GetPort() ;

		CString strPort;
		strPort.Format("%d",inptPort);
		Inifile.SetKeyValue("ServerInfo", "Ip",strAddr);
		Inifile.SetKeyValue("ServerInfo", "Port",strPort);
		AfxMessageBox("恭喜！保存配置信息成功！");
	}
	void DLDLG::InitNet()
	{
		//读取配置文件中的IP PORT 显示,如果没有配置文件就显示常量
		CIPAddressCtrl* lpIpEdit = (CIPAddressCtrl*)GetDlgItem( IDC_IPADDRESS1 ) ;
		lpIpEdit->SetWindowText(strIP);//默认IP
		m_nPort=nPort;//默认端口
		UpdateData(FALSE);

		//默认check选中
		( (CButton*)GetDlgItem( IDC_BUTTON2) )->SetCheck( TRUE ) ;
		OnInternet() ;
	}
	BOOL DLDLG::ReadCfgFile()
	{
		//获得ini文件的绝对路径
		CFileFind finder;
		BOOL bWorking = finder.FindFile(theApp.configpath);//寻找文件
		if(!bWorking)
		{
			//MessageBox("没有找到登录配置文件！","提示信息",MB_ICONINFORMATION);
			strIP = DEFADDR;
			nPort = DEFPORT;
			return false;
		}
		Inifile.SetPath(theApp.configpath);
		//读取IP PORT
		strIP = Inifile.GetKeyValue("ServerInfo", "IP");
		nPort = atoi(Inifile.GetKeyValue("ServerInfo", "Port"));
		return true;
	}
	void DLDLG::OnInternet() //网络配置伸缩控制
	{
		m_bIsExpand = !m_bIsExpand ;
		//	CDealDlg* lpMainDlg = (CDealDlg*)GetOwner() ;

		if (! m_bIsExpand )
		{
			SetDlgItemText( IDC_BUTTON2, TEXT( "网络设置↑" ) ) ;
			SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight, SWP_NOMOVE ) ;
			//GetOwner()->SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight, SWP_NOMOVE ) ;
		}

		else
		{
			SetDlgItemText( IDC_BUTTON2,TEXT( "网络设置↓" ) ) ;
			SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight * 2 / 3-5, SWP_NOMOVE ) ;
			//GetOwner()->SetWindowPos( &wndTop, 0, 0, m_nDlgWidth, m_nDlgHight * 2 / 3, SWP_NOMOVE ) ;

		}

		// 重新创建状态栏
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
		//输出图片
		bmpGraphics.DrawImage(&img, 0, 0, rect.Width(), rect.Height());
		//拷贝输出到屏幕
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
			//pDC->SetBkColor(RGB(255,0,0));  //需要指定背景色的时候用 
			pDC->SetTextColor(RGB(255,   255,   255));    //文字前景色
			pDC->SetBkMode(TRANSPARENT);    //设置透明
			hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //获取画笔颜色混合后的画笔，完成透明 
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
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		return (HCURSOR) m_hIcon;
		return CDialogEx::OnQueryDragIcon();
	}
	
	CString DLDLG::selectt(CString strsql)//查询
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