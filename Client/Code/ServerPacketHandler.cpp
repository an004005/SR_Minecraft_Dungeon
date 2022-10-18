#include "stdafx.h"
#include "ServerPacketHandler.h"
#include "ClientServiceMgr.h"
#include "AbstFactory.h"
#include "Player.h"
#include "PlayerController.h"
#include "ZombieController.h"
#include "RemoteInventory.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];



// ���� ������ �۾���

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
		CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
		pPlayer->PlayerSpawn();
		pPlayer->SetID(CClientServiceMgr::GetInstance()->m_iPlayerID);
		return true;
	}

	// if (CClientServiceMgr::GetInstance()->m_iPlayerID == 0) // host player spawn monster
	// {
	// 	Protocol::C_SPAWN_MONSTER monPkt;
	// 	monPkt.set_id(1);
	// 	_matrix matWorld = CGameUtilMgr::s_matIdentity;
	// 	CEnemyFactory::Create<CGameObject>("Zombie", L"Zombie_1", matWorld);
	//
	// 	monPkt.set_factory("Zombie");
	// 	CClientServiceMgr::Mat2Pkt(matWorld, *monPkt.mutable_matrix());
	//
	// 	session->Send(ServerPacketHandler::MakeSendBuffer(monPkt));
	// }

	return true;
}

bool Handle_S_OTHER_PLAYER(PacketSessionRef& session, Protocol::S_OTHER_PLAYER& pkt)
{
	for (int i = 0; i < pkt.player_size(); ++i)
	{
		Protocol::Player* pPlayerPkt = pkt.mutable_player(i);
		if (pPlayerPkt->id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
			continue;

		CPlayer* pPlayer = CPlayerFactory::Create<CPlayer>("Steve_Remote", L"Player_Remote_" + to_wstring(pPlayerPkt->id()), CGameUtilMgr::s_matIdentity);
		pPlayer->SetID(pPlayerPkt->id());
		pPlayer->PlayerSpawn();
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
	CClientServiceMgr::Pkt2mat(pkt.matworld(), matWorld);
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
	if (pkt.success() == false)
		return true;
	if (pkt.player().id() == CClientServiceMgr::GetInstance()->m_iPlayerID)
		return true;

	if (CPlayer* pPlayer = Get_GameObjectUnCheck<CPlayer>(LAYER_PLAYER, L"Player_Remote_" + to_wstring(pkt.player().id())))
	{
		if (CRemoteInventory* pInven = dynamic_cast<CRemoteInventory*>(pPlayer->GetInventory()))
		{
			pInven->SetEquipStateChange(pkt.state());
		}
	}

	return true;
}

bool Handle_S_SPAWN_MONSTER(PacketSessionRef& session, Protocol::S_SPAWN_MONSTER& pkt)
{
	if (pkt.success() == false)
		return true;

	wstring tmp;
	tmp.assign(pkt.factory().begin(), pkt.factory().end());
	if (CClientServiceMgr::GetInstance()->m_iPlayerID == 0) // host 
	{
		_matrix matWorld = CGameUtilMgr::s_matIdentity;
		matWorld._41 = 3.f;
		matWorld._43 = 7.f;
		CEnemyFactory::Create<CGameObject>(pkt.factory(), tmp + L"_" + to_wstring(pkt.id()), matWorld)
			->SetID(pkt.id());
	}
	else
	{
		_matrix matWorld = CGameUtilMgr::s_matIdentity;
		CEnemyFactory::Create<CGameObject>(pkt.factory() + "_Remote", tmp + L"_Remote_" + to_wstring(pkt.id()), matWorld)
			->SetID(pkt.id());
	}

	return true;
}

bool Handle_S_MONSTER_SET_TARGET(PacketSessionRef& session, Protocol::S_MONSTER_SET_TARGET& pkt)
{
	if (pkt.success() == false)
		return true;
	if (CClientServiceMgr::GetInstance()->m_iPlayerID == 0) // host 
		return true;

	CZombieRemoteController* pCon = Engine::Get_ComponentUnCheck<CZombieRemoteController>(LAYER_ENEMY, L"Zombie_Remote_" + to_wstring(pkt.monsterid()), L"Proto_ZombieRemoteController",  ID_DYNAMIC);
	if (pCon)
	{
		if (pkt.battack())
			pCon->SetAttack();

		pCon->SetTarget(pkt.playerid());
	}

	return true;
}

bool Handle_S_MONSTER_WORLD(PacketSessionRef& session, Protocol::S_MONSTER_WORLD& pkt)
{
	if (pkt.success() == false)
		return true;
	if (CClientServiceMgr::GetInstance()->m_iPlayerID == 0) // host 
		return true;

	CZombieRemoteController* pCon = Engine::Get_ComponentUnCheck<CZombieRemoteController>(LAYER_ENEMY, L"Zombie_Remote_" + to_wstring(pkt.monsterid()), L"Proto_ZombieRemoteController",  ID_DYNAMIC);
	if (pCon)
	{
		_matrix matWorld;
		CClientServiceMgr::Pkt2mat(pkt.matworld(), matWorld);
		pCon->SetWorld(matWorld);
	}


	return true;
}

