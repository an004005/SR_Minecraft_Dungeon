#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTexture;
	class CRcTex;
	class CTransform;
}
class CMapDeco :
	public CGameObject
{
public:
	explicit CMapDeco(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapDeco();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT				Add_Component(void);
	CTexture*			m_pTextureCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;

public:
	static CMapDeco*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);

};

