#pragma once
#include "Controller.h"

class CZombieController : public CController
{
private:
	explicit CZombieController(void);
	explicit CZombieController(const CZombieController& rhs);
	virtual ~CZombieController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
public:
	static CZombieController* Create();

private:
	_float m_fCurAttackCoolTime = 0.f;
	_float m_fAttackCoolTime = 2.f;

	_float m_fDetectRange = 12.f; // �����Ÿ�
	_float m_fAttackDist = 2.f; // ���� ��Ÿ�
};

