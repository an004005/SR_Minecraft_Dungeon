#include "stdafx.h"
#include "ServerPacketHandler.h"
#include "ClientServiceMgr.h"
#include "AbstFactory.h"
#include "PlayerController.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// Á÷Á¢ ÄÁÅÙÃ÷ ÀÛ¾÷ÀÚ

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

	Protocol::C_ENTER_GAME enterPkt;

	enterPkt.mutable_player()->set_id(pkt.playerid());
	enterPkt.mutable_player()->set_name("Player_test");
	enterPkt.set_playerskin(Protocol::PLAYER_TYPE_STEVE);

	auto SendBuffer = ServerPacketHandler::MakeSendBuffer(enterPkt);
	session->Send(SendBuffer);

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (pkt.success() == false)
		return true;

	if (CClientServiceMgr::GetInstance()->m_iPlayerID == pkt.player().id())
		return true;


	wstring wstrObj = L"Player_Remote_" + to_wstring(pkt.player().id());
	CPlayerFactory::Create<CGameObject>("Steve_Remote", wstrObj, CGameUtilMgr::s_matIdentity);

	return true;
}

bool Handle_S_OTHER_PLAYER(PacketSessionRef& session, Protocol::S_OTHER_PLAYER& pkt)
{
	for (int i = 0; i < pkt.player_size(); ++i)
	{
		Protocol::Player* pPlayer = pkt.mutable_player(i);
		if (pPlayer->id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
			continue;

		CPlayerFactory::Create<CGameObject>("Steve_Remote", L"Player_Remote_" + to_wstring(pPlayer->id()), CGameUtilMgr::s_matIdentity);
	}

	return true;
}

bool Handle_S_PLAYER_INPUT(PacketSessionRef& session, Protocol::S_PLAYER_INPUT& pkt)
{
	if (pkt.player().id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
		return true;

	// pkt.player().id()
	// pkt.inputbit()

	Engine::Get_Component<CPlayerRemoteController>(LAYER_PLAYER, L"Player_Remote_" + to_wstring(pkt.player().id()), L"Proto_PlayerRemoteController",  ID_DYNAMIC)
		->SetInputMask(pkt.inputbit());
	return false;
}

bool Handle_S_PLAYER_WORLD(PacketSessionRef& session, Protocol::S_PLAYER_WORLD& pkt)
{
	return false;

}
