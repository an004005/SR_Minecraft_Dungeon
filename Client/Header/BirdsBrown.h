#pragma once

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
}

class CBirdsBrown :
	public CGameObject
{
public:
	explicit CBirdsBrown(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBirdsBrown();

public:
	virtual HRESULT	Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	CRcTex*			m_pBufferCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;

	_float			m_fFrame2 = 0.f;
	_float			m_fSpeed = 10.f;
	
	_bool			m_bBrown = false;
	_vec3			m_vDir;
	_bool			m_bReverese = false;

	_vec3 m_vPlayerPos;
	_float m_fTriggerFreq = 0.3f;
	_float m_fCurTriggerFreq = 0.3f;


public:
	static CBirdsBrown*		Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJTYPE eType);
	virtual void			Free(void);
};

