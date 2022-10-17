#pragma once
#include "Controller.h"
class CPlayer;

class CSatonController :public CController
{
private:
	explicit CSatonController(void);
	explicit CSatonController(const CSatonController& rhs);
	virtual ~CSatonController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
public:
	static CSatonController* Create();

private:
	CPlayer* m_pTargetPlayer = nullptr;



	_float m_fCurFirstHammerCoolTime = 0.f;
	_float m_fCurSatonBirdCoolTime = 0.f;
	_float m_fCurSatonGrapCoolTime = 0.f;
	_float m_fCurSatonFascinateCoolTime = 0.f;
	_float m_fCurSatonDrawMoonCoolTime = 0.f;
	_float m_fCurTargetingCoolTime = 3.f;

	_float m_fFirstHammerCoolTime = 15.f;
	_float m_fSatonBirdCoolTime = 25.f;
	_float m_fSatonGrapCoolTime = 30.f;
	_float m_fSatonFascinateCoolTime = 5.f;
	_float m_fSatonDrawMoonCoolTime = 2.f;
	_float m_fTargetingCoolTime = 3.f;

	_float m_fFirstHammerDist = 20.f;
	_float m_fSatonBirdkDist = 20.f;
	_float m_fSatonGrapkDist = 20.f;
	_float m_fSatonFascinateDist = 30.f;

	_vec3 m_vLookFront;

	_bool m_bIsGimmick_On = false;
	_bool m_bIsDrawMoon = false;
	_bool m_bIsKoukuSymbol = false;

	_float m_fDist = 5.f;
};

