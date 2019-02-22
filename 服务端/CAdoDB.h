
// ����ADO�ӿ�
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename( "BOF", "adoBOF" ) rename( "EOF", "adoEOF" ) rename( "LockTypeEnum", "NewLockTypeEnum" ) rename( "DataTypeEnum", "NewDataTypeEnum" ) rename( "FieldAttributeEnum", "NewFieldAttributeEnum" ) rename( "EditModeEnum", "NewEditModeEnum" ) rename( "RecordStatusEnum", "NewRecordStatusEnum" ) rename( "ParameterDirectionEnum", "NewParameterDirectionEnum" )

class CAdoDB
{

public:

	_ConnectionPtr m_pConnection ; // ���ݿ����Ӷ����ָ��
	_RecordsetPtr m_pRecordset ; // ���ݿ��¼�������ָ��

    CString m_DBName ; // ��¼���ݿ���
	CString m_strUserID ; // ��¼����SQL Serverʱ��ʹ�õ��û���
	CString m_strUserPassword ; // ��¼����SQL Serverʱ��ʹ�õ�����
	CString m_strDataSource ; // ��¼����Դ��ַ

public:

	CAdoDB( CString DBName,
			CString strUserID = _T( "" ),
			CString strPassword = _T( "" ),
			CString strDataSource = _T( "" ) ) ;
	
	virtual ~CAdoDB() ;

	BOOL OnInitDBConnection() ; // ��ʼ�����ݿ�����
	void ExitConnect() ; // �Ͽ������ݿ������

	BOOL IsConnected() ; // ���ݿ������Ƿ���Ч
	
	BOOL ExecuteSQL( LPCTSTR vSQL ) ; // ִ��SQL��� INSERT UPDATE DELETE ��
	BOOL ExecuteSQL( LPCTSTR vSQL, CString& strException ) ; // ִ��SQL��� INSERT UPDATE DELETE ��
	BOOL ExecuteSQLSelect( LPCTSTR vSQL ) ; // ִ��SELECT���

	CString GetRecordResult( LPCTSTR lpszField ) ; // �õ���Ӧ�ֶε�����
	BOOL ResultExist() ; // ����¼�����Ƿ��м�¼
	BOOL NextRecord() ; // ��ת����¼���е���һ����¼
	int GetRecCount();

	CString GetFieldValue( long index ); 
};
