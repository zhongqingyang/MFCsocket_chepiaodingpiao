#ifndef _CMESSG_
#define _CMESSG_

class CMessg:public CObject
{
protected:
	DECLARE_DYNCREATE(CMessg)
public:
	CString m_strText;
	char m_type;//�������� �ַ�
	int m_OperState;
	int m_Column;//�и���
	int m_num; //��ѯ�ڼ�ҳ  
	int m_pageall; //һ������ҳ
    int m_start;
	float m_total;
	//int m_nType; //���������ļ���������
	
	int m_dwFileSize;//�ļ���С

public:
	CMessg();
	//CMessg(int nType);
	//CMessg(int nType,CString strFileName,DWORD dwFileSize);
public:
	virtual ~CMessg();
	virtual void Serialize(CArchive& ar);
};


#endif