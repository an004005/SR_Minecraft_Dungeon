#pragma once

#include "GameObject.h"


class CSkyBox :	public CGameObject
{
public:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyBox();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(void) override;

private:

private:
	CTexture* m_pTextureCom = nullptr;
	CShader* m_pShader = nullptr;
	CShereBuffer* m_pBuffer = nullptr;


public:
	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

