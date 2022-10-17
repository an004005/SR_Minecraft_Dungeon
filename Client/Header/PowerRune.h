#pragma once
#include "Rune.h"
class CPowerRune : public CRune
{
private:
	explicit CPowerRune(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPowerRune() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual void EquipRune(CWeapon* pWeapon) override;
	virtual void UnEquipRune(CWeapon* pWeapon) override;
	static CPowerRune* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_uint m_iPrevDamage = 0;
};

