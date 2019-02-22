#ifndef _CMESSG_
#define _CMESSG_

class CMessg:public CObject
{
protected:
	DECLARE_DYNCREATE(CMessg)
public:
	CString m_strText;
	char m_type;//操作类型 字符
	int m_OperState;
	int m_Column;//列个数
	int m_num; //查询第几页  
	int m_pageall; //一共多少页
    int m_start;
	float m_total;
	//int m_nType; //操作类型文件处理类型
	
	int m_dwFileSize;//文件大小

public:
	CMessg();
	//CMessg(int nType);
	//CMessg(int nType,CString strFileName,DWORD dwFileSize);
public:
	virtual ~CMessg();
	virtual void Serialize(CArchive& ar);
};


#endif