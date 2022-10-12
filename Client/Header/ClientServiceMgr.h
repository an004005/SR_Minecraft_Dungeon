#pragma once
#include "Base.h"
#include "Session.h"

extern _bool g_bOnline;

class CClientServiceMgr : public CBase
{
	DECLARE_SINGLETON(CClientServiceMgr)
private:
	explicit CClientServiceMgr();
	virtual ~CClientServiceMgr();

public:
	HRESULT ReadyClientService();
	void Broadcast(const SendBufferRef& sendBuffer) const;
	
public:
	virtual void Free() override;


	uint32 m_iPlayerID;

private:
	ClientServiceRef _service;
	bool m_bStop = false;

};


/*-----------------
 *  ServerSession
 -----------------*/
class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		//cout << "Disconnected" << endl;
	}
};