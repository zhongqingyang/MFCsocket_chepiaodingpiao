
/*	类CAdoDB的定义,该类包含了对数据库的基本操纵 */

#include "StdAfx.h"
#include "CAdoDB.h"

// -- CAdoDB类构造函数
CAdoDB::CAdoDB( CString DBName,
			    CString strUserID,
				CString strUserPassword,
				CString strDataSource )
{
	TRACE( "Construction Of Class CAdoDB\n" ) ;
	// 成员变量初始化
	m_pConnection = NULL ;
	m_pRecordset = NULL ;
	m_DBName = DBName ;
	m_strUserID = strUserID ;
	m_strUserPassword = strUserPassword ;
	m_strDataSource = strDataSource ;
	
	// 连接数据库
	OnInitDBConnection() ;
}

// -- CAdoDB类析构函数
CAdoDB::~CAdoDB()
{
	TRACE( "Destruction Of Class CAdoDB\n" ) ;
	
	// CAdoDB对象无效时断开与数据库的链接
	ExitConnect() ;
}

// -- 初始化数据库连接
BOOL CAdoDB::OnInitDBConnection()
{
	TRACE( "OnInitDBConnection Of Class CAdoDB\n" ) ;

	try
	{
		// 初始化OLE/COM库环境
		::CoInitialize( NULL ) ;
			// 创建连接字符串
		//SQL Server 
		// 创建数据库连接对象
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
		// 如果不使用用户名和密码连接则进行Windows验证连接
		if ( m_strUserID.IsEmpty() )
		{
			strConnection += TEXT( " Integrated Security = SSPI ;" ) ;
		}
		
		TRACE( "\n%s\n", (LPCTSTR)strConnection ) ;
		// 设置超时为5秒
		m_pConnection->CommandTimeout = 2 ;

		// 建立连接
		m_pConnection->Open( strConnection, TEXT( "" ), TEXT( "" ), adModeUnknown ) ;
				
		// 设置超时为5秒
		m_pConnection->CommandTimeout = 2 ;
		
		//ACCESS
		// 建立连接
		/*_bstr_t strConnection =  TEXT( "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" ) ;
		strConnection += _bstr_t( m_DBName ) ;
		strConnection += TEXT( ".mdb;Persist Security Info=False;" ) ;
		m_pConnection.CreateInstance("ADODB.Connection");//创建Connection对象
		m_pConnection->Open(strConnection,"","",adModeUnknown);///连接数据库

		// 设置超时为5秒
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

// -- 断开与数据库的连接
void CAdoDB::ExitConnect()
{
	TRACE( "ExitConnect Of Class CAdoDB\n" ) ;

	// 断开记录集连接
	if ( m_pRecordset != NULL )
	{
		m_pRecordset->Close() ;
	}
    
	// 断开数据库连接
	if ( m_pConnection != NULL )
	{
		m_pConnection->Close() ;
	}
	
	// 释放环境资源
    ::CoUninitialize() ;
}

// -- 数据库连接是否有效
BOOL CAdoDB::IsConnected()
{
	if ( m_pConnection == NULL )
	{
		
		return FALSE ;
	}
	return TRUE ;
}

// -- 执行SQL语句 INSERT UPDATE DELETE 等
BOOL CAdoDB::ExecuteSQL( LPCTSTR vSQL )
{
	TRACE( "ExecuteSQL Of Class CAdoDB\n" ) ;
    
	try
	{
	   // 如果还未连接数据库则先连接数据库
	   if( !IsConnected() )
	   {
		   if ( !OnInitDBConnection() )
		   {
			   return FALSE ;
		   }
	   }
	   
	   //TRACE( "\nSQL 语句 : %s\n\n", vSQL ) ;
	   
	   // 执行SQL语句
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

// -- 执行SQL语句 INSERT UPDATE DELETE 等
BOOL CAdoDB::ExecuteSQL( LPCTSTR vSQL, CString& strException )
{
	TRACE( "ExecuteSQL Of Class CAdoDB\n" ) ;
    
	try
	{
		// 如果还未连接数据库则先连接数据库
		if( !IsConnected() )
		{
			if ( !OnInitDBConnection() )
			{
				return FALSE ;
			}
		}
		
		//TRACE( "\nSQL 语句 : %s\n\n", vSQL ) ;
		
		// 执行SQL语句
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

// -- 执行SELECT语句
BOOL CAdoDB::ExecuteSQLSelect( LPCTSTR vSQL )
{
	TRACE( "ExecuteSQLSelect Of Class CAdoDB\n" ) ;
	try
	{
		// 断开先前的记录集连接
		if ( m_pRecordset != NULL )
		{
			m_pRecordset->Close() ;
		}
		// 如果还未连接数据库则先连接数据库
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

// -- 得到相应字段的内容
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

// -- 检查记录集中是否有记录
BOOL CAdoDB::ResultExist()
{
	if ( m_pRecordset == NULL || m_pRecordset->adoEOF )
	{
		return FALSE ;
	}
	
	return TRUE ;
}

// -- 跳转到记录集中的下一条记录
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