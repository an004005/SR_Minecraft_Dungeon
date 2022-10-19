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
	_float m_fCurBasicAttackCoolTime = 0.f;
	_float m_fCurSymbolGimmickCoolTime = 0.f;
	_float m_fCurTargetingCoolTime = 3.f;

	_float m_fBasicAttackCoolTime = 10.f;
	_float m_fDoubleHammerCoolTime = 20.f;
	// _float m_fHorrorAttackCoolTime = 20.f;
	_float m_fHorrorAttackCoolTime = 5.f;

	_float m_fSymbolGimmickCoolTime = 6.2f
	;
	_float m_fTargetingCoolTime = 3.f; 

	_float m_fDoubleHammerDist = 8.f;
	_float m_fHorrorAttackDist = 8.f;
	_float m_fBasicAttackDist = 5.f;
	_float m_fWalkDist = 3.f;
	_bool m_bIsSymbolGimmick = false;
	_bool m_bIsFirstSymbolGimmick = false;
	string m_strState;
	_float m_fDist = 5.f;

	_float m_fTargetDist = 9999.f;
};

