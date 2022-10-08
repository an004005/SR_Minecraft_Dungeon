#include "stdafx.h"
#include "..\Header\Inventory.h"
#include "Sword.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_Object()
{
	return S_OK;
}

_int CInventory::Update_Object(const _float & fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CInventory::LateUpdate_Object()
{
	Engine::CGameObject::LateUpdate_Object();
}

void CInventory::Render_Object()
{
}

void CInventory::Free()
{
	CGameObject::Free();
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory* pInstance = new CInventory(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CInventory::Put(CEquipItem * pItem)
{
	if (pItem == nullptr)
		return;

	pItem->AddRef();
	m_vecItem.push_back(pItem);
}

void CInventory::TakeOut(CEquipItem * pItem)
{
	auto itr = std::find(m_vecItem.begin(), m_vecItem.end(), pItem);
	if (itr == m_vecItem.end()) return;
	Safe_Release((*itr));
	m_vecItem.erase(itr);
}
