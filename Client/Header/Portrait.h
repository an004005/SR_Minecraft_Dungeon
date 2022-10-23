#pragma once
#include "UI.h"
class CPortrait : public CUI
{
protected:
	explicit CPortrait(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortrait();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(void) override;
	static CPortrait*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum);
	virtual void		Free(void);

private:
	CTargetTexture* m_pTargetTexture = nullptr;
};

