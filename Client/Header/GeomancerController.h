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
public:
	static CGeomancerController* Create();

private:
};

