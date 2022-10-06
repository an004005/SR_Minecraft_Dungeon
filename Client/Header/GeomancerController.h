#pragma once
#include "Controller.h"

class CGeomancerController : public CController
{
private:
	explicit CGeomancerController(void);
	explicit CGeomancerController(const CGeomancerController& rhs);
	virtual ~CGeomancerController() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
public:
	static CGeomancerController* Create();

private:
	_float m_fCurAttackCoolTime = 0.f;
	_float m_fAttackCoolTime = 3.f;

	_float m_fAttackRange = 15.f; // 사정거리
	_float m_fRunDist = 7.f;
};

