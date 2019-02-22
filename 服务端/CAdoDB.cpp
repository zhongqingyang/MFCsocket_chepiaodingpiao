
/*	��CAdoDB�Ķ���,��������˶����ݿ�Ļ������� */

#include "StdAfx.h"
#include "CAdoDB.h"

// -- CAdoDB�๹�캯��
CAdoDB::CAdoDB( CString DBName,
			    CString strUserID,
				CString strUserPassword,
				CString strDataSource )
{
	TRACE( "Construction Of Class CAdoDB\n" ) ;
	// ��Ա������ʼ��
	m_pConnection = NULL ;
	m_pRecordset = NULL ;
	m_DBName = DBName ;
	m_strUserID = strUserID ;
	m_strUserPassword = strUserPassword ;
	m_strDataSource = strDataSource ;
	
	// �������ݿ�
	OnInitDBConnection() ;
}

// -- CAdoDB����������
CAdoDB::~CAdoDB()
{
	TRACE( "Destruction Of Class CAdoDB\n" ) ;
	
	// CAdoDB������Чʱ�Ͽ������ݿ������
	ExitConnect() ;
}

// -- ��ʼ�����ݿ�����
BOOL CAdoDB::OnInitDBConnection()
{
	TRACE( "OnInitDBConnection Of Class CAdoDB\n" ) ;

	try
	{
		// ��ʼ��OLE/COM�⻷��
		::CoInitialize( NULL ) ;
			// ���������ַ���
		//SQL Server 
		// �������ݿ����Ӷ���
		m_pConnection.CreateInstance( TEXT( "AdoDB.Connection") ) ;
        
		_bstr_t strConnection =  TEXT( "Provider = SQLOLEDB.1 ; Persist Security Info = True ; Data Source =" ) ;
		if ( m_strDataSource.IsEmpty() )
		{
			strConnection += TEXT( "(local)" ) ;
			//strConnection += TEXT( "PC-1\\SQLEXPRESS" ) ;
		}
		
		else
		{
			strConnection += _bstr_t( m_strDataSource ) ;
		}
		strConnection += _bstr_t( m_strDataSource ) ;
		strConnection += TEXT( "; Initial Catalog =" ) ;
		strConnection += _bstr_t( m_DBName ) ;
		strConnection += TEXT( "; User ID =" ) ;
		strConnection += _bstr_t( m_strUserID ) ;
		strConnection += TEXT( "; Password =" ) ;
		strConnection += _bstr_t( m_strUserPassword ) ;
		strConnection += TEXT( ";" ) ;
		// �����ʹ���û������������������Windows��֤����
		if ( m_strUserID.IsEmpty() )
		{
			strConnection += TEXT( " Integrated Security = SSPI ;" ) ;
		}
		
		TRACE( "\n%s\n", (LPCTSTR)strConnection ) ;
		// ���ó�ʱΪ5��
		m_pConnection->CommandTimeout = 2 ;

		// ��������
		m_pConnection->Open( strConnection, TEXT( "" ), TEXT( "" ), adModeUnknown ) ;
				
		// ���ó�ʱΪ5��
		m_pConnection->CommandTimeout = 2 ;
		
		//ACCESS
		// ��������
		/*_bstr_t strConnection =  TEXT( "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" ) ;
		strConnection += _bstr_t( m_DBName ) ;
		strConnection += TEXT( ".mdb;Persist Security Info=False;" ) ;
		m_pConnection.CreateInstance("ADODB.Connection");//����Connection����
		m_pConnection->Open(strConnection,"","",adModeUnknown);///�������ݿ�

		// ���ó�ʱΪ5��
		m_pConnection->CommandTimeout = 2 ;*/
	
		return TRUE ;
	}

	catch ( _com_error e )
	{
		TRACE( "Exceptions In OnInitDBConnection Of Class CAdoDB\n" ) ;
		TRACE( "%s\n", (LPCTSTR)e.Description() ) ;
		m_pConnection = NULL ;

		return FALSE ;
	}

}

// -- �Ͽ������ݿ������
void CAdoDB::ExitConnect()
{
	TRACE( "ExitConnect Of Class CAdoDB\n" ) ;

	// �Ͽ���¼������
	if ( m_pRecordset != NULL )
	{
		m_pRecordset->Close() ;
	}
    
	// �Ͽ����ݿ�����
	if ( m_pConnection != NULL )
	{
		m_pConnection->Close() ;
	}
	
	// �ͷŻ�����Դ
    ::CoUninitialize() ;
}

// -- ���ݿ������Ƿ���Ч
BOOL CAdoDB::IsConnected()
{
	if ( m_pConnection == NULL )
	{
		
		return FALSE ;
	}
	return TRUE ;
}

// -- ִ��SQL��� INSERT UPDATE DELETE ��
BOOL CAdoDB::ExecuteSQL( LPCTSTR vSQL )
{
	TRACE( "ExecuteSQL Of Class CAdoDB\n" ) ;
    
	try
	{
	   // �����δ�������ݿ������������ݿ�
	   if( !IsConnected() )
	   {
		   if ( !OnInitDBConnection() )
		   {
			   return FALSE ;
		   }
	   }
	   
	   //TRACE( "\nSQL ��� : %s\n\n", vSQL ) ;
	   
	   // ִ��SQL���
	   m_pConnection->Execute( vSQL, NULL, adCmdText ) ;

       return TRUE ;
	}

    catch ( _com_error e )
	{
		//TRACE( "Exceptions In ExecuteSQL Of Class CAdoDB\n" ) ;
		//TRACE( "%s\n", (LPCTSTR)e.Description() ) ;
		//AfxMessageBox( vSQL);
		//AfxMessageBox(e.Description());

	    return FALSE ;
	}

	catch ( ... )
	{
		TRACE( "Unknown Exceptions In ExecuteSQL Of Class CAdoDB\n" ) ;
		
		return FALSE ;
	}
}

// -- ִ��SQL��� INSERT UPDATE DELETE ��
BOOL CAdoDB::ExecuteSQL( LPCTSTR vSQL, CString& strException )
{
	TRACE( "ExecuteSQL Of Class CAdoDB\n" ) ;
    
	try
	{
		// �����δ�������ݿ������������ݿ�
		if( !IsConnected() )
		{
			if ( !OnInitDBConnection() )
			{
				return FALSE ;
			}
		}
		
		//TRACE( "\nSQL ��� : %s\n\n", vSQL ) ;
		
		// ִ��SQL���
		m_pConnection->Execute( vSQL, NULL, adCmdText ) ;
		
		return TRUE ;
	}
	
    catch ( _com_error e )
	{
		TRACE( "Exceptions In ExecuteSQL Of Class CAdoDB\n" ) ;
		TRACE( "%s\n", (LPCTSTR)e.Description() ) ;
		strException = (LPCTSTR)e.Description() ;

		return FALSE ;
	}
	
	catch ( ... )
	{
		TRACE( "Unknown Exceptions In ExecuteSQL Of Class CAdoDB\n" ) ;
		strException = TEXT( "Unknown Exceptions" ) ;
		
		return FALSE ;
	}
}

// -- ִ��SELECT���
BOOL CAdoDB::ExecuteSQLSelect( LPCTSTR vSQL )
{
	TRACE( "ExecuteSQLSelect Of Class CAdoDB\n" ) ;
	try
	{
		// �Ͽ���ǰ�ļ�¼������
		if ( m_pRecordset != NULL )
		{
			m_pRecordset->Close() ;
		}
		// �����δ�������ݿ������������ݿ�
		if ( !IsConnected() )
		{
			if ( !OnInitDBConnection() )
			{
				m_pRecordset = NULL ;
				return FALSE ;
			}
		}
		//TRACE( "\nThe SQL is: %s\n\n", vSQL ) ;

		m_pRecordset.CreateInstance(TEXT("ADODB.Recordset"));

		m_pRecordset->Open(vSQL,
							m_pConnection.GetInterfacePtr(),
							adOpenDynamic,
							adLockOptimistic,
							adCmdText );
		return TRUE ;
	}
	
	catch ( _com_error e )
	{
		TRACE( "Exceptions In ExecuteSQLSelect Of Class CAdoDB\n" ) ;
		TRACE( "%s\n", (LPCTSTR)e.Description() ) ;
		m_pRecordset = NULL ;
		
		return FALSE ;
	}
	
	catch ( ... )
	{
		TRACE( "Unknown Exceptions In ExecuteSQLSelect Of Class CAdoDB\n" ) ;
		m_pRecordset = NULL ;
		
		return FALSE ;
	}
}

// -- �õ���Ӧ�ֶε�����
CString CAdoDB::GetRecordResult( LPCTSTR lpszField )
{
	TRACE( "GetRecordResult Of Class CAdoDB\n" ) ;

	try
	{
		return (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect( lpszField ) ;
	}

	catch ( _com_error e )
	{
		TRACE( "Exceptions In GetRecordResult Of Class CAdoDB\n" ) ;
		TRACE( "%s\n", (LPCTSTR)e.Description() ) ;
		
		CString strRet ;
		strRet.Empty() ;
		
		return strRet ;
	}
	
	catch ( ... )
	{
		TRACE( "Unknown Exceptions In GetRecordResult Of Class CAdoDB\n" ) ;
		
		CString strRet ;
		strRet.Empty() ;
		
		return strRet ;
	}
}

// -- ����¼�����Ƿ��м�¼
BOOL CAdoDB::ResultExist()
{
	if ( m_pRecordset == NULL || m_pRecordset->adoEOF )
	{
		return FALSE ;
	}
	
	return TRUE ;
}

// -- ��ת����¼���е���һ����¼
BOOL CAdoDB::NextRecord()
{
	if ( !ResultExist() )
	{
		return FALSE ;
	}
	
	m_pRecordset->MoveNext() ;
	
	return TRUE ;
}
int CAdoDB::GetRecCount()
{
	if ( !ResultExist() )
	{
		return FALSE ;
	}
	int count=0;
	m_pRecordset->MoveFirst();
	while(!m_pRecordset->adoEOF)
	{
		count++;
		m_pRecordset->MoveNext();
	}
	m_pRecordset->MoveFirst();
	return count;
}

CString CAdoDB::GetFieldValue( long index ) 
{
	_variant_t vValue;
	_variant_t vIndex;
	vIndex = (long)index;
	vValue = m_pRecordset->GetCollect( vIndex );
	if (vValue.vt == VT_NULL)
	{
		return "";
	}
	return  (LPCTSTR)(_bstr_t)vValue;
}