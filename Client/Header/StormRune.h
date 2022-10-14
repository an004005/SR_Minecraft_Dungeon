#pragma once
#include "Rune.h"
class CStormRune : public CRune
{
private:
	explicit CStormRune(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStormRune() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;
	virtual void Collision() override;
	virtual void Use() override;
	static CStormRune* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

