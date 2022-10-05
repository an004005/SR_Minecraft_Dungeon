#pragma once
#include "Controller.h"

class CSkeletalCube;
class CPlayer;


class CPlayerController : public CController
{
private:
	explicit CPlayerController(void);
	explicit CPlayerController(const CPlayerController& rhs);
	virtual ~CPlayerController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
public:
	static CPlayerController* Create();

private:
	_vec3 m_vPressDir = CGameUtilMgr::s_vZero;
	_vec3 m_vPrevPressDir = CGameUtilMgr::s_vZero;

	_vec3 m_vMoveDir = CGameUtilMgr::s_vZero;

};

