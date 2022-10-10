#pragma once
#include "GameObject.h"



class CDynamite :
	public CGameObject
{
private:
	explicit CDynamite(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamite();

public:
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;

	static CDynamite* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CCubeTex*		m_pBufferCom = nullptr;
	Engine::CTransform*	m_pTransCom = nullptr;
};

