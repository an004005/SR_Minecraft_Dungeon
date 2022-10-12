#include "stdafx.h"
#include "ClientServiceMgr.h"
#include "ThreadManager.h"
#include "ServerPacketHandler.h"
#include "Service.h"

IMPLEMENT_SINGLETON(CClientServiceMgr)

CClientServiceMgr::CClientServiceMgr()
{
}


CClientServiceMgr::~CClientServiceMgr()
{
	Free();
}

HRESULT CClientServiceMgr::ReadyClientService()
{
	ServerPacketHandler::Init();
	this_thread::sleep_for(1s);

	_service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>, // TODO : SessionManager 등
		1);
	ASSERT_CRASH(_service->Start());

	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([this]()
			{
				while (m_bStop == false)
				{
					_service->GetIocpCore()->Dispatch(3000);
				}
			});
	}

	

	return S_OK;
}

void CClientServiceMgr::Broadcast(const SendBufferRef& sendBuffer) const
{
	_service->Broadcast(sendBuffer);
}


void CClientServiceMgr::Free()
{
	m_bStop = true;
	GThreadManager->Join();
}

/*-----------------
 *  ServerSession
 -----------------*/
void ServerSession::OnConnected()
{
	Protocol::C_LOGIN pkt;
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : packetId 대역 체크
	ServerPacketHandler::HandlePacket(session, buffer, len);
}
