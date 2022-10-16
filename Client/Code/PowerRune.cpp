#include "stdafx.h"
#include "..\Header\PowerRune.h"

CPowerRune::CPowerRune(LPDIRECT3DDEVICE9 pGraphicDev) : CRune(pGraphicDev)
{
	m_eTargetType = WEAPON_SWORD | WEAPON_AXE | WEAPON_AXE | WEAPON_CROSSBOW;
}

CPowerRune::~CPowerRune()
{
}

void CPowerRune::EquipRune(CWeapon* pWeapon)
{
	m_iPrevDamage = pWeapon->GetDamage();
	pWeapon->SetDamage(static_cast<_uint>((_float)m_iPrevDamage * 1.5f));
	pWeapon->SetCritical(true);
}

void CPowerRune::UnEquipRune(CWeapon* pWeapon)
{
	pWeapon->SetDamage(m_iPrevDamage);
	pWeapon->SetCritical(false);
	m_iPrevDamage = 0;
}

CPowerRune* CPowerRune::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPowerRune* pInstance = new CPowerRune(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

