#ifndef _CMESSG_
#define _CMESSG_

class CMessg:public CObject
{
protected:
	DECLARE_DYNCREATE(CMessg)
public:
	CString m_strText;
	char m_type;
	int m_OperState;
	int m_Column;//�и���
	int m_num;
	int m_pageall; //һ������ҳ
	int m_dwFileSize;//�ļ�
	int m_start;
	float m_total;
public:
	CMessg();
public:
	virtual ~CMessg();
	virtual void Serialize(CArchive& ar);
};


#endif