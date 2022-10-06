#pragma once
#include "Controller.h"

class CPlayer;

class CRedStoneMonstrosityController : public CController
{
private:
	explicit CRedStoneMonstrosityController(void);
	explicit CRedStoneMonstrosityController(const CRedStoneMonstrosityController& rhs);
	virtual ~CRedStoneMonstrosityController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
public:
	static CRedStoneMonstrosityController* Create();

private:
	CPlayer* m_pTargetPlayer = nullptr;



	_float m_fCurChopCoolTime = 0.f;
	_float m_fCurSpitCoolTime = 0.f;
	_float m_fCurSummonCoolTime = 0.f;
	_float m_fCurWindmillCoolTime = 0.f;
	_float m_fCurTargetingCoolTime = 3.f;

	_float m_fChopCoolTime = 8.f;
	_float m_fSpitCoolTime = 5.f;
	_float m_fSummonCoolTime = 15.f;
	_float m_fWindmillCoolTime = 20.f;
	_float m_fTargetingCoolTime = 3.f;

	_float m_fChopDist = 5.f;
	_float m_fSpitDist = 10.f;
	_float m_fWindmillDist = 2.f;
};

