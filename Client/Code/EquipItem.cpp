#include "stdafx.h"
#include "..\Header\EquipItem.h"
#include "Player.h"
#include "SkeletalCube.h"
#include "Transform.h"

CEquipItem::CEquipItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
	m_eItemType = IT_END;
	
}



CEquipItem::~CEquipItem()
{
}

HRESULT CEquipItem::Ready_Object()
{
	return S_OK;
}

_int CEquipItem::Update_Object(const _float & fTimeDelta)
{
	CItem::Update_Object(fTimeDelta);
	return 0;
}

void CEquipItem::LateUpdate_Object()
{
	CItem::LateUpdate_Object();
}

void CEquipItem::Render_Object()
{
}

CEquipItem * CEquipItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEquipItem* pInstance = new CEquipItem(pGraphicDev);
	
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEquipItem::Free()
{
	CItem::Free();
}

