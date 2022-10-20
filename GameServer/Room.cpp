#include "stdafx.h"
#include "Room.h"

#include "ClientPacketHandler.h"
#include "Player.h"
#include "GameSession.h"
#include "Protocol.pb.h"

shared_ptr<Room> GRoom = make_shared<Room>();

void Room::Enter(PlayerRef player)
{
	_players[player->playerId] = player;

	if (_players.empty() == false)
	{
		Protocol::S_OTHER_PLAYER otherPkt;
		for (auto& e : _players)
		{
			auto repeatedPlayer = otherPkt.add_player();
			repeatedPlayer->set_id(e.second->playerId);
			repeatedPlayer->set_name(e.second->name);
			otherPkt.add_playerskin(e.second->type);
		}

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(otherPkt);
		DoAsync(&Room::Broadcast, sendBuffer);
	}
}

void Room::Leave(PlayerRef player)
{
	_players.erase(player->playerId);
}

void Room::Broadcast(SendBufferRef sendBuffer)
{
	for (auto& p : _players)
	{
		if (const auto locked = p.second->ownerSession.lock())
		{
			locked->Send(sendBuffer);
		}
	}
}