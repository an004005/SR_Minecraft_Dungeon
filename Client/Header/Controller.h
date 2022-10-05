#pragma once
#include "Component.h"

class CController : public CComponent
{
protected:
	explicit CController(void);
	explicit CController(const CController& rhs);
	virtual ~CController();

public:
	virtual CComponent* Clone() override;
	void SetOwner(CGameObject* pOwner) { m_pOwner = pOwner; m_pOwner->AddRef(); }
	virtual void Free() override;

protected:
	_uint m_iTick = 0;
	CGameObject* m_pOwner = nullptr;
};

