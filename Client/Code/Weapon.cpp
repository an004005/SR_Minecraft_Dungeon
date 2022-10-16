#include "stdafx.h"
#include "..\Header\Weapon.h"
#include "Rune.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev): CEquipItem(pGraphicDev)
{
}

CWeapon::~CWeapon()
{
}

bool CWeapon::SetRune(CRune* pRune)
{
	NULL_CHECK_RETURN(pRune, false);

	if (m_pRune)
	{
		m_pRune->UnEquipRune(this);
		Safe_Release(m_pRune);
	}

	if (pRune->GetTargetTypeMask() & m_eType)
	{
		m_pRune = pRune;
		m_pRune->EquipRune(this);
		m_pRune->AddRef();
		return true;
	}

	return false;
}

void CWeapon::Free()
{
	Safe_Release(m_pRune);
	CEquipItem::Free();
}
