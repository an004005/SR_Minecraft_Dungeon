#pragma once
#include "Controller.h"
class CEndermanController :
	public CController
{
private:
	explicit CEndermanController(void);
	explicit CEndermanController(const CEndermanController& rhs);
	virtual ~CEndermanController();

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
public:
	static CEndermanController* Create();

private:
	_float m_fAttakCollTime = 3.5f;
	_float m_fCurAttackCollTime = 3.f;

	_float m_fClockingCollTime = 3.f;
	_float m_fCurClockingCollTime = 0.f;

	_float m_fPistolCollTime = 12.f;
	_float m_fCurPistolCollTime = 0.f;

	_float m_fArmAttackCollTime = 8.5f;
	_float m_fCurArmAttackCollTime = 0.f;

};

