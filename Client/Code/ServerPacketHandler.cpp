#include "stdafx.h"
#include "ServerPacketHandler.h"
#include "ClientServiceMgr.h"
#include "AbstFactory.h"
#include "Player.h"
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
	{
		Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player")->PlayerSpawn();
		return true;
	}

	return true;
}

bool Handle_S_OTHER_PLAYER(PacketSessionRef& session, Protocol::S_OTHER_PLAYER& pkt)
{
	for (int i = 0; i < pkt.player_size(); ++i)
	{
		Protocol::Player* pPlayer = pkt.mutable_player(i);
		if (pPlayer->id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
			continue;

		CPlayerFactory::Create<CPlayer>("Steve_Remote", L"Player_Remote_" + to_wstring(pPlayer->id()), CGameUtilMgr::s_matIdentity)
			->PlayerSpawn();
	}

	return true;
}

bool Handle_S_PLAYER_INPUT(PacketSessionRef& session, Protocol::S_PLAYER_INPUT& pkt)
{
	if (pkt.player().id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
		return true;

	Engine::Get_Component<CPlayerRemoteController>(LAYER_PLAYER, L"Player_Remote_" + to_wstring(pkt.player().id()), L"Proto_PlayerRemoteController",  ID_DYNAMIC)
		->SetInputMask(pkt.inputbit());

	return true;
}

bool Handle_S_PLAYER_WORLD(PacketSessionRef& session, Protocol::S_PLAYER_WORLD& pkt)
{
	if (pkt.success() == false)
		return true;
	if (pkt.player().id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
		return true;


	_matrix matWorld;
	{
		matWorld._11 = pkt.matworld().vright().x();
		matWorld._12 = pkt.matworld().vright().y();
		matWorld._13 = pkt.matworld().vright().z();
		matWorld._14 = pkt.matworld().vright().w();

		matWorld._21 = pkt.matworld().vup().x();
		matWorld._22 = pkt.matworld().vup().y();
		matWorld._23 = pkt.matworld().vup().z();
		matWorld._24 = pkt.matworld().vup().w();

		matWorld._31 = pkt.matworld().vlook().x();
		matWorld._32 = pkt.matworld().vlook().y();
		matWorld._33 = pkt.matworld().vlook().z();
		matWorld._34 = pkt.matworld().vlook().w();

		matWorld._41 = pkt.matworld().vpos().x();
		matWorld._42 = pkt.matworld().vpos().y();
		matWorld._43 = pkt.matworld().vpos().z();
		matWorld._44 = pkt.matworld().vpos().w();
	}
	Engine::Get_Component<CPlayerRemoteController>(LAYER_PLAYER, L"Player_Remote_" + to_wstring(pkt.player().id()), L"Proto_PlayerRemoteController",  ID_DYNAMIC)
		->SetWorld(matWorld);

	return true;
}

bool Handle_S_PLAYER_YAW_ACTION(PacketSessionRef& session, Protocol::S_PLAYER_YAW_ACTION& pkt)
{
	if (pkt.success() == false)
		return true;
	if (pkt.player().id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
		return true;

	Engine::Get_Component<CPlayerRemoteController>(LAYER_PLAYER, L"Player_Remote_" + to_wstring(pkt.player().id()), L"Proto_PlayerRemoteController",  ID_DYNAMIC)
		->SetYawAction(pkt.yaw(), pkt.actionbit());
	return true;
}

bool Handle_S_PLAYER_ACTION(PacketSessionRef& session, Protocol::S_PLAYER_ACTION& pkt)
{
	if (pkt.success() == false)
		return true;
	if (pkt.player().id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
		return true;

	Engine::Get_Component<CPlayerRemoteController>(LAYER_PLAYER, L"Player_Remote_" + to_wstring(pkt.player().id()), L"Proto_PlayerRemoteController",  ID_DYNAMIC)
		->SetAction(pkt.actionbit());

	return true;
}

bool Handle_S_PLAYER_ARROW(PacketSessionRef& session, Protocol::S_PLAYER_ARROW& pkt)
{
	if (pkt.success() == false)
		return true;
	if (pkt.player().id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
		return true;

	_vec3 vLookAt;
	vLookAt.x =  pkt.vlookat().x();
	vLookAt.y =  pkt.vlookat().y();
	vLookAt.z =  pkt.vlookat().z();

	Engine::Get_Component<CPlayerRemoteController>(LAYER_PLAYER, L"Player_Remote_" + to_wstring(pkt.player().id()), L"Proto_PlayerRemoteController",  ID_DYNAMIC)
		->SetArrow(pkt.yaw(), vLookAt, pkt.actionbit());

	return true;
}

bool Handle_S_PLAYER_EQUIP(PacketSessionRef& session, Protocol::S_PLAYER_EQUIP& pkt)
{
	return true;
}

bool Handle_S_SPAWN_MONSTER(PacketSessionRef& session, Protocol::S_SPAWN_MONSTER& pkt)
{
	return true;
}

bool Handle_S_MONSTER_SET_TARGET(PacketSessionRef& session, Protocol::S_MONSTER_SET_TARGET& pkt)
{
	return true;
}

bool Handle_S_MONSTER_WORLD(PacketSessionRef& session, Protocol::S_MONSTER_WORLD& pkt)
{
	return true;
}

bool Handle_S_MONSTER_ATTACK(PacketSessionRef& session, Protocol::S_MONSTER_ATTACK& pkt)
{
	return true;
}
