
// 导入ADO接口
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename( "BOF", "adoBOF" ) rename( "EOF", "adoEOF" ) rename( "LockTypeEnum", "NewLockTypeEnum" ) rename( "DataTypeEnum", "NewDataTypeEnum" ) rename( "FieldAttributeEnum", "NewFieldAttributeEnum" ) rename( "EditModeEnum", "NewEditModeEnum" ) rename( "RecordStatusEnum", "NewRecordStatusEnum" ) rename( "ParameterDirectionEnum", "NewParameterDirectionEnum" )

class CAdoDB
{

public:

	_ConnectionPtr m_pConnection ; // 数据库连接对象的指针
	_RecordsetPtr m_pRecordset ; // 数据库记录集对象的指针

    CString m_DBName ; // 记录数据库名
	CString m_strUserID ; // 记录连接SQL Server时所使用的用户名
	CString m_strUserPassword ; // 记录连接SQL Server时所使用的密码
	CString m_strDataSource ; // 记录数据源地址

public:

	CAdoDB( CString DBName,
			CString strUserID = _T( "" ),
			CString strPassword = _T( "" ),
			CString strDataSource = _T( "" ) ) ;
	
	virtual ~CAdoDB() ;

	BOOL OnInitDBConnection() ; // 初始化数据库连接
	void ExitConnect() ; // 断开与数据库的连接

	BOOL IsConnected() ; // 数据库连接是否有效
	
	BOOL ExecuteSQL( LPCTSTR vSQL ) ; // 执行SQL语句 INSERT UPDATE DELETE 等
	BOOL ExecuteSQL( LPCTSTR vSQL, CString& strException ) ; // 执行SQL语句 INSERT UPDATE DELETE 等
	BOOL ExecuteSQLSelect( LPCTSTR vSQL ) ; // 执行SELECT语句

	CString GetRecordResult( LPCTSTR lpszField ) ; // 得到相应字段的内容
	BOOL ResultExist() ; // 检查记录集中是否有记录
	BOOL NextRecord() ; // 跳转到记录集中的下一条记录
	int GetRecCount();

	CString GetFieldValue( long index ); 
};
