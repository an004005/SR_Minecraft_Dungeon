#include "stdafx.h"
#include "..\Header\Inventory.h"
#include "Sword.h"
#include "Glaive.h"
#include "Crossbow.h"
#include "SkeletalCube.h"
#include "AbstFactory.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_Object()
{
	m_vecItem.push_back(CItemFactory::Create<CCrossbow>("Crossbow", L"Crossbow", IS_TAKE));
	m_vecItem.push_back(CItemFactory::Create<CSword>("Sword", L"Sword", IS_TAKE));

	m_vecItem.back()->AddRef();
	m_vecItem.front()->AddRef();

	m_pCurMelee = m_vecItem.back();
	m_pCurRange = m_vecItem.front();
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
	for_each(m_vecItem.begin(), m_vecItem.end(), Safe_Release<CEquipItem*>);
	m_vecItem.clear();
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
	if (pItem == nullptr || pItem->GetItemState() == IS_TAKE)
		return;

	pItem->AddRef();
	m_vecItem.push_back(pItem);
	pItem->SetState(IS_TAKE);
	
	switch (pItem->GetItemType())
	{
	case IT_MELEE:
		m_pCurMelee = pItem;
		break;
	case IT_RANGE:
		m_pCurRange = pItem;
		break;
	case IT_LEGACY:
		m_pCurLegacy = pItem;
		break;
	default:
		_CRASH("wrong access");
		break;
	}
}

void CInventory::TakeOut(CEquipItem * pItem)
{
	auto itr = std::find(m_vecItem.begin(), m_vecItem.end(), pItem);
	if (itr == m_vecItem.end()) return;
	Safe_Release((*itr));
	m_vecItem.erase(itr);
}

void CInventory::Equip_Item(SkeletalPart* pSkeletalPart, ITEMTYPE eIT)
{	
	switch (eIT)
	{
	case IT_MELEE:
		m_pCurMelee->Equipment(pSkeletalPart);
		break;
	case IT_RANGE:
		m_pCurRange->Equipment(pSkeletalPart);
		break;
	case IT_LEGACY:
		m_pCurLegacy->Equipment(pSkeletalPart);
		break;
	default:
		_CRASH("wrong access");
		break;
	}

}

CEquipItem * CInventory::CurWeapon(ITEMTYPE eIT)
{
	switch (eIT)
	{
	case IT_MELEE:
		return m_pCurMelee;
	case IT_RANGE:
		return m_pCurRange;
	case IT_LEGACY:
		return m_pCurLegacy;
	}

	return nullptr;
}
