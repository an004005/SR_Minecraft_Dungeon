#pragma once
#include "GameObject.h"

class CRedStoneMonstrosityBullet :
	public CGameObject
{
public:
	explicit CRedStoneMonstrosityBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRedStoneMonstrosityBullet();

public:
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	static CRedStoneMonstrosityBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;

private:
	_bool m_bDead = false;
	Engine::CTransform* m_pTransCom = nullptr;

};

