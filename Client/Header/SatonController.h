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
	_float m_fCurTargetingCoolTime = 3.f;

	_float m_fFirstHammerCoolTime = 15.f;
	_float m_fSatonBirdCoolTime = 25.f;
	_float m_fSatonGrapCoolTime = 30.f;
	_float m_fTargetingCoolTime = 3.f;
	_float m_fFirstHammerDist = 8.f;
	_float m_fSatonBirdkDist = 8.f;
	_float m_fSatonGrapkDist = 5.f;

	_float m_fDist = 5.f;
};

