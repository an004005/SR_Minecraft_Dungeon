#include "stdafx.h"
#include "..\Header\Weapon.h"
#include "Rune.h"
#include "Inventory.h"
CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev): CEquipItem(pGraphicDev)
, m_pInventory(nullptr) , m_pRune(nullptr)
{
}

CWeapon::~CWeapon()
{
}

HRESULT CWeapon::Ready_Object()
{
	m_pInventory = Get_GameObject<CInventory>(LAYER_GAMEOBJ, L"Inventory");
	return S_OK;
}

_int CWeapon::Update_Object(const _float & fTimeDelta)
{
	CEquipItem::Update_Object(fTimeDelta);

	

	return OBJ_NOEVENT;
}

bool CWeapon::SetRune(CRune* pRune)
{
	if (pRune == nullptr) // �� ����
	{
		if (m_pRune)
			m_pRune->UnEquipRune(this);
		//Safe_Release(m_pRune);
		m_pRune = nullptr;
		return true;
	}

	// �� ����

	if (pRune->GetTargetTypeMask() & m_eType) // ���� �������� Ȯ��
	{
		// ���� ����
		if (m_pRune) // ���� ���� �̹� �ִٸ� ����
		{
			m_pRune->UnEquipRune(this);
			//Safe_Release(m_pRune);
			m_pRune = nullptr;
		}

		// �״��� ����
		m_pRune = pRune;
		m_pRune->EquipRune(this);
		//m_pRune->AddRef();
		return true;
	}

	// ���� �Ұ�
	return false;
}

void CWeapon::Free()
{
	Safe_Release(m_pRune);
	CEquipItem::Free();
}
