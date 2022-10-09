#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
}

class CUI :
	public CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUI();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

private: /* 직교투영행렬. */
	_matrix				m_ProjMatrix;

public:
	static CUI*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum);
	virtual void		Free(void);

private:
	_uint m_iTexNum = 0;
};

