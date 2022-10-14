#pragma once
#include "Rune.h"
class CMultiShotRune : public CRune
{
private:
	explicit CMultiShotRune(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMultiShotRune() override;

public:
	virtual void Use() override;
	virtual void EquipRune(CWeapon* pWeapon) override;
	static CMultiShotRune* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_uint m_iWeaponDamage = 0;
};

