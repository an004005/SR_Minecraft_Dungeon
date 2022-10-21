#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	// 싱글쓰레드 환경인마냥 코딩
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);
	void Dead(uint64 iID);

	map<uint64, PlayerRef> _players;

	uint64 _playerNum;
};

extern shared_ptr<Room> GRoom;