#pragma once
#include "Base.h"
#include "Logo.h"

class CObjectStoreMgr : public CBase
{
	DECLARE_SINGLETON(CObjectStoreMgr)

private:
	explicit CObjectStoreMgr();
	virtual ~CObjectStoreMgr() override;

public:
	void SetPlayerName(const string& strPlayerName) { m_strPlayerName = strPlayerName; }
	void SetPlayerSkin(CharacterSelect eSkin);
	virtual void Free() override;

private:
	string m_strPlayerName;
	Protocol::PlayerSkin m_eSkin = Protocol::PLAYER_TYPE_STEVE;
	
};

