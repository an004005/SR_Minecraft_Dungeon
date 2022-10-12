#pragma once
#include "Controller.h"

class CSkeletalCube;
class CPlayer;
class CDynamite;

enum PlayerInputMask
{
	PLAYER_W = 1,
	PLAYER_A = 1 << 1,
	PLAYER_S = 1 << 2,
	PLAYER_D = 1 << 3,
	PLAYER_ML = 1 << 4,
	PLAYER_MR = 1 << 5,
	PLAYER_1 = 1 << 6,
	PLAYER_2 = 1 << 7,
	PLAYER_3 = 1 << 8,
};

class CPlayerController : public CController
{
protected:
	explicit CPlayerController(void);
	explicit CPlayerController(const CPlayerController& rhs);
	virtual ~CPlayerController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	static CPlayerController* Create();

protected:
	_vec3 m_vPressDir = CGameUtilMgr::s_vZero;
	_vec3 m_vPrevPressDir = CGameUtilMgr::s_vZero;

	_vec3 m_vMoveDir = CGameUtilMgr::s_vZero;

	void putItem(CPlayer* pPlayer,const  _vec3& vTargetPos);
	void pickGameObj(CPlayer* pPlayer, const  _vec3& vTargetPos);
	CDynamite* m_pDynamite = nullptr;

	_uint m_iPlayerInputMask = 0;
};

/*----------------------
 * Remote
 -------------------*/
class CPlayerRemoteController : public CPlayerController
{
protected:
	explicit CPlayerRemoteController(void);
	explicit CPlayerRemoteController(const CPlayerRemoteController& rhs);
	virtual ~CPlayerRemoteController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	static CPlayerRemoteController* Create();

	void SetInputMask(_uint iPlayerInputMask) { m_iPlayerInputMask = iPlayerInputMask; }
};