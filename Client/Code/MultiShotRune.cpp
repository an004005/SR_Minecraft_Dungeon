#include "stdafx.h"
#include "..\Header\MultiShotRune.h"
#include "Player.h"
#include "Crossbow.h"

CMultiShotRune::CMultiShotRune(LPDIRECT3DDEVICE9 pGraphicDev): CRune(pGraphicDev)
{
	m_eTargetType = WEAPON_CROSSBOW;
	m_iUItexNum = 16;
	m_eItemType = IT_RUNE;
}

CMultiShotRune::~CMultiShotRune()
{
}

void CMultiShotRune::Use()
{
	if (CCrossbow* pCrossbow = dynamic_cast<CCrossbow*>(m_pWeapon))
	{
		if (pCrossbow->IsFireWork())
		{
			m_pOwner->SpawnArrow(50, PlayerArrowType::MULTISHOT, false, ARROW_FIREWORK);
			return;
		}
	}

	m_pOwner->SpawnArrow(m_iWeaponDamage, PlayerArrowType::MULTISHOT);
}

void CMultiShotRune::EquipRune(CWeapon* pWeapon)
{
	m_pItemUI = CUIFactory::Create<CItemUI>("ItemUI", L"MultiShotRuneUI", 0);
	m_pItemUI->SetUITexture(m_iUItexNum);

	m_iWeaponDamage = pWeapon->GetDamage();
	m_pWeapon = pWeapon;
}

void CMultiShotRune::UnEquipRune(CWeapon* pWeapon)
{
	m_pWeapon = nullptr;
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
