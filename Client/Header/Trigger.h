#pragma once
#include "GameObject.h"

enum TriggerEvent
{
	TRIGGER_REDSTONE_CAM_START,
	TRIGGER_END
};

class CTrigger : public CGameObject
{
private:
	explicit CTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrigger(const CGameObject& rhs);
	virtual ~CTrigger() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	static CTrigger* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CTransform* m_pTransform = nullptr;
	_float m_fTriggerFreq = 0.3f;
	_float m_fCurTriggerFreq = 0.3f;

	_float m_fRadius = 4.f;

	std::function<void(set<CGameObject*>&)> m_pTriggerFunction;
};

