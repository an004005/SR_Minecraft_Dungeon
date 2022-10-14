#include "stdafx.h"
#include "..\Header\MultiShotRune.h"
#include "Player.h"


CMultiShotRune::CMultiShotRune(LPDIRECT3DDEVICE9 pGraphicDev): CRune(pGraphicDev)
{
	m_eTargetType = WEAPON_CROSSBOW;
}

CMultiShotRune::~CMultiShotRune()
{
}

void CMultiShotRune::Use()
{
	m_pOwner->SpawnArrow(m_iWeaponDamage, PlayerArrowType::MULTISHOT);
}

void CMultiShotRune::EquipRune(CWeapon* pWeapon)
{
	m_iWeaponDamage = pWeapon->GetDamage();
}

CMultiShotRune* CMultiShotRune::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMultiShotRune* pInstance = new CMultiShotRune(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
