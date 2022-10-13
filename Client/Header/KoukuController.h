#pragma once
#include "Controller.h"
class CPlayer;

class CKoukuController :public CController
{
private:
	explicit CKoukuController(void);
	explicit CKoukuController(const CKoukuController& rhs);
	virtual ~CKoukuController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
public:
	static CKoukuController* Create();

private:
	CPlayer* m_pTargetPlayer = nullptr;



	_float m_fCurDoubleHammerCoolTime = 0.f;
	_float m_fCurHorrorAttackCoolTime = 0.f;

	_float m_fCurTargetingCoolTime = 3.f;

	_float m_fDoubleHammerCoolTime = 8.f;
	_float m_fHorrorAttackCoolTime = 15.f;
	_float m_fTargetingCoolTime = 3.f; 

	_float m_fDoubleHammerDist = 6.f;
	_float m_fHorrorAttackDist = 6.f;


	_float m_fDist = 3.f;
};

