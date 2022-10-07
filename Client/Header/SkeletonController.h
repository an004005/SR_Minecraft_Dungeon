#pragma once
#include "Controller.h"

class CSkeletonController : public CController
{
private:
	explicit CSkeletonController(void);
	explicit CSkeletonController(const CSkeletonController& rhs);
	virtual ~CSkeletonController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
public:
	static CSkeletonController* Create();

private:
	_float m_fCurAttackCoolTime = 0.f;
	_float m_fAttackCoolTime = 3.f;

	_float m_fAttackRange = 15.f; // �����Ÿ�
	_float m_fRunDist = 7.f;
};

