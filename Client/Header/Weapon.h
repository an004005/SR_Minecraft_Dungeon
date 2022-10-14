#pragma once
#include "EquipItem.h"

enum WeaponType
{
	WEAPON_SWORD = 1,
	WEAPON_GLAIVE = 1 << 1,
	WEAPON_AXE = 1 << 2,
	WEAPON_CROSSBOW = 1 << 3,
	WEAPON_END
};

class CRune;
class CWeapon : public CEquipItem
{
protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeapon() override;

public:
	bool SetRune(CRune* pRune);
	CRune* GetRune() { return m_pRune; }

	void SetDamage(_uint iDamage) { m_iDamage = iDamage;}
	_uint GetDamage() const { return m_iDamage; }

	virtual void Free() override;

protected:
	CRune* m_pRune = nullptr;
	WeaponType m_eType = WEAPON_END;
	_uint m_iDamage = 0;
};

