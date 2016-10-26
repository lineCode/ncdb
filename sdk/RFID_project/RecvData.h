#pragma once
class CTcpDB;
class CDataDeal;
class CShowData;
struct TrackShow;
class CADODataBase;


class CRecvData
{
public:
	CRecvData(void);
	~CRecvData(void);

public:

	bool Init(void* pAdd = 0);
	bool UnInit();

	bool Work();
	bool ShowToUI(TrackShow* ts);
private:

	CDataDeal*		m_pDataDeal;
	CTcpDB*			m_pTcp;
	CShowData*		m_pShowData;


public:
	CWnd*			m_pAdd;
	CADODataBase*	m_pDB;
};
