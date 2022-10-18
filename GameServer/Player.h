#pragma once

class Player
{
public:

	uint32					playerId = 0;
	string					name;
	Protocol::PlayerSkin	type = Protocol::PLAYER_TYPE_STEVE;
	weak_ptr<GameSession>			ownerSession; // Cycle
	array<float, 3> vPos;
};

