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
	int m_Column;//列个数
	int m_num;
	int m_pageall; //一共多少页
	int m_dwFileSize;//文件
	int m_start;
	float m_total;
public:
	CMessg();
public:
	virtual ~CMessg();
	virtual void Serialize(CArchive& ar);
};


#endif