#pragma once

#include "GameObject.h"

class CStaticCamera : public CGameObject
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera();

public:
	HRESULT Ready_Object() override;

	virtual _int Update_Object(const _float& fTimeDelta) override;
	void SetMatProj(const _float& fFov = D3DXToRadian(60.f),
	                const _float& fAspect = (float)WINCX / WINCY,
	                const _float& fNear = 0.1f,
	                const _float& fFar = 1000.f);

	void SetTarget(CGameObject* pTarget);
	void LerpDistanceTo(_float fDistance);

private:
	void Update_DefaultFollow(const _float& fTimeDelta);

private:
	_matrix m_matView, m_matProj;
	CTransform* m_pTransform = nullptr;

	CGameObject* m_pTarget = nullptr;
	CTransform* m_pTargetTrans = nullptr;

	// normal mode
	_float m_fDistance;
	// normal mode

public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};
