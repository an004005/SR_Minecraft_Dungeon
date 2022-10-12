#include "stdafx.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"
#include "Service.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO : Validation 체크

	Protocol::C_PLAYER_INPUT t;

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// DB에서 플레이 정보를 긁어온다
	// GameSession에 플레이 정보를 저장 (메모리)

	// ID 발급 (DB 아이디가 아니고, 인게임 아이디)
	static Atomic<uint64> idGenerator = 1;

	{
		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->ownerSession = gameSession;
		
		gameSession->_currentPlayer = playerRef;

		loginPkt.set_playerid(idGenerator);
		loginPkt.set_success(true);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);
	
	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
	Protocol::Player playerPkt = pkt.player();

	// uint64 index = playerPkt.id();
	gameSession->_currentPlayer->name = playerPkt.name();
	gameSession->_currentPlayer->type = pkt.playerskin();
	std::cout << "Enter Player :  " << playerPkt.id() << " " << playerPkt.name() << endl;

	Protocol::S_ENTER_GAME enterPkt;
	enterPkt.set_success(true);
	enterPkt.mutable_player()->set_id(playerPkt.id());
	enterPkt.mutable_player()->set_name(playerPkt.name());
	enterPkt.set_playerskin(pkt.playerskin());

	{
		if (GRoom->_players.size() > 0)
		{
			Protocol::S_OTHER_PLAYER otherPkt;
			for (auto& e : GRoom->_players)
			{
				auto repeatedPlayer = otherPkt.add_player();
				repeatedPlayer->set_id(e.second->playerId);
				repeatedPlayer->set_name(e.second->name);
				otherPkt.add_playerskin(e.second->type);
			}

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(otherPkt);
			gameSession->Send(sendBuffer);	
		}

	}

	GRoom->Enter(gameSession->_currentPlayer);

	{
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterPkt);
		GRoom->DoAsync(&Room::Broadcast, sendBuffer);
	}

	return true;
}

bool Handle_C_PLAYER_INPUT(PacketSessionRef& session, Protocol::C_PLAYER_INPUT& pkt)
{
	Protocol::S_PLAYER_INPUT inputPkt;
	inputPkt.set_success(true);
	inputPkt.mutable_player()->set_id(pkt.player().id());
	inputPkt.mutable_player()->set_name(pkt.player().name());
	inputPkt.set_inputbit(pkt.inputbit());
	cout << pkt.player().id() << " : " << pkt.inputbit() << endl;

	GRoom->DoAsync(&Room::Broadcast, ClientPacketHandler::MakeSendBuffer(inputPkt));

	return true;
}

bool Handle_C_PLAYER_WORLD(PacketSessionRef& session, Protocol::C_PLAYER_WORLD& pkt)
{
	return false;
}

// bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
// {
// 	// std::cout << pkt.msg() << endl;
// 	//
// 	// Protocol::S_CHAT chatPkt;
// 	// chatPkt.set_msg(pkt.msg());
// 	// auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);
// 	//
// 	// GRoom->DoAsync(&Room::Broadcast, sendBuffer);
//
// 	return true;
// }
