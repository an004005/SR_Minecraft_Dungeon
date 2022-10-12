#include "stdafx.h"
#include "ServerPacketHandler.h"
#include "ClientServiceMgr.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
		return true;

	CClientServiceMgr::GetInstance()->m_iPlayerID = pkt.playerid();


	// // 입장 UI 버튼 눌러서 게임 입장
	Protocol::C_ENTER_GAME enterGamePkt;
	// enterGamePkt.set_playerindex(0); // 첫번째 캐릭터로 입장
	// auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	// session->Send(sendBuffer);

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (pkt.success() == false)
		return true;

	if (pkt.playerid() == CClientServiceMgr::GetInstance()->m_iPlayerID)
	{
		// auto pCamera = CCamera::Create(m_pGraphicDev);
		// NULL_CHECK_RETURN(pGameObject, E_FAIL);
		// FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", pGameObject), E_FAIL);
	}
	else
	{
		
	}

	return true;
}

bool Handle_S_POSITION(PacketSessionRef& session, Protocol::S_POSITION& pkt)
{
	return true;
}
