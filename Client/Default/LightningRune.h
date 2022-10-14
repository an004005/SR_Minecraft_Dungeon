#pragma once
#include "Rune.h"
class CLightningRune : public CRune
{
private:
	explicit CLightningRune(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningRune() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	virtual void Use() override;
	static CLightningRune* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

