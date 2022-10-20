#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_ENTER_GAME = 1002,
	PKT_S_ENTER_GAME = 1003,
	PKT_S_OTHER_PLAYER = 1004,
	PKT_C_PLAYER_INPUT = 1005,
	PKT_S_PLAYER_INPUT = 1006,
	PKT_C_PLAYER_WORLD = 1007,
	PKT_S_PLAYER_WORLD = 1008,
	PKT_C_PLAYER_YAW_ACTION = 1009,
	PKT_S_PLAYER_YAW_ACTION = 1010,
	PKT_C_PLAYER_ACTION = 1011,
	PKT_S_PLAYER_ACTION = 1012,
	PKT_C_PLAYER_ARROW = 1013,
	PKT_S_PLAYER_ARROW = 1014,
	PKT_C_PLAYER_EQUIP = 1015,
	PKT_S_PLAYER_EQUIP = 1016,
	PKT_C_SPAWN_MONSTER = 1017,
	PKT_S_SPAWN_MONSTER = 1018,
	PKT_C_MONSTER_SET_TARGET = 1019,
	PKT_S_MONSTER_SET_TARGET = 1020,
	PKT_C_MONSTER_WORLD = 1021,
	PKT_S_MONSTER_WORLD = 1022,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt);
bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt);
bool Handle_C_PLAYER_INPUT(PacketSessionRef& session, Protocol::C_PLAYER_INPUT& pkt);
bool Handle_C_PLAYER_WORLD(PacketSessionRef& session, Protocol::C_PLAYER_WORLD& pkt);
bool Handle_C_PLAYER_YAW_ACTION(PacketSessionRef& session, Protocol::C_PLAYER_YAW_ACTION& pkt);
bool Handle_C_PLAYER_ACTION(PacketSessionRef& session, Protocol::C_PLAYER_ACTION& pkt);
bool Handle_C_PLAYER_ARROW(PacketSessionRef& session, Protocol::C_PLAYER_ARROW& pkt);
bool Handle_C_PLAYER_EQUIP(PacketSessionRef& session, Protocol::C_PLAYER_EQUIP& pkt);
bool Handle_C_SPAWN_MONSTER(PacketSessionRef& session, Protocol::C_SPAWN_MONSTER& pkt);
bool Handle_C_MONSTER_SET_TARGET(PacketSessionRef& session, Protocol::C_MONSTER_SET_TARGET& pkt);
bool Handle_C_MONSTER_WORLD(PacketSessionRef& session, Protocol::C_MONSTER_WORLD& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LOGIN>(Handle_C_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_C_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ENTER_GAME>(Handle_C_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_C_PLAYER_INPUT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PLAYER_INPUT>(Handle_C_PLAYER_INPUT, session, buffer, len); };
		GPacketHandler[PKT_C_PLAYER_WORLD] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PLAYER_WORLD>(Handle_C_PLAYER_WORLD, session, buffer, len); };
		GPacketHandler[PKT_C_PLAYER_YAW_ACTION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PLAYER_YAW_ACTION>(Handle_C_PLAYER_YAW_ACTION, session, buffer, len); };
		GPacketHandler[PKT_C_PLAYER_ACTION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PLAYER_ACTION>(Handle_C_PLAYER_ACTION, session, buffer, len); };
		GPacketHandler[PKT_C_PLAYER_ARROW] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PLAYER_ARROW>(Handle_C_PLAYER_ARROW, session, buffer, len); };
		GPacketHandler[PKT_C_PLAYER_EQUIP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PLAYER_EQUIP>(Handle_C_PLAYER_EQUIP, session, buffer, len); };
		GPacketHandler[PKT_C_SPAWN_MONSTER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SPAWN_MONSTER>(Handle_C_SPAWN_MONSTER, session, buffer, len); };
		GPacketHandler[PKT_C_MONSTER_SET_TARGET] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MONSTER_SET_TARGET>(Handle_C_MONSTER_SET_TARGET, session, buffer, len); };
		GPacketHandler[PKT_C_MONSTER_WORLD] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MONSTER_WORLD>(Handle_C_MONSTER_WORLD, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::S_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_S_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_OTHER_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_OTHER_PLAYER); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYER_INPUT& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYER_INPUT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYER_WORLD& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYER_WORLD); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYER_YAW_ACTION& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYER_YAW_ACTION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYER_ACTION& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYER_ACTION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYER_ARROW& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYER_ARROW); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYER_EQUIP& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYER_EQUIP); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SPAWN_MONSTER& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN_MONSTER); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MONSTER_SET_TARGET& pkt) { return MakeSendBuffer(pkt, PKT_S_MONSTER_SET_TARGET); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MONSTER_WORLD& pkt) { return MakeSendBuffer(pkt, PKT_S_MONSTER_WORLD); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};