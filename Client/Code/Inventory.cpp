#include "stdafx.h"
#include "..\Header\Inventory.h"
#include "Sword.h"
#include "Glaive.h"
#include "Crossbow.h"
#include "SkeletalCube.h"
#include "AbstFactory.h"
#include "Emerald.h"
#include "Axe.h"
#include "Glaive.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_Object()
{
	m_pCurRange = CItemFactory::Create<CCrossbow>("Crossbow", L"Crossbow", IS_TAKE);
	m_pCurRange->AddRef();
	m_vecItem.push_back(m_pCurRange);

	m_pCurMelee = CItemFactory::Create<CSword>("Sword", L"Sword", IS_TAKE);
	m_pCurMelee->AddRef();
	m_vecItem.push_back(m_pCurMelee);

	m_pCurMelee = CItemFactory::Create<CGlaive>("Glaive", L"Glaive", IS_TAKE);
	m_pCurMelee->AddRef();
	m_vecItem.push_back(m_pCurMelee);

	m_pCurMelee = CItemFactory::Create<CAxe>("Axe", L"Axe", IS_TAKE);
	m_pCurMelee->AddRef();
	m_vecItem.push_back(m_pCurMelee);

	m_pCurMelee = m_vecItem[1];
	return S_OK;
}

_int CInventory::Update_Object(const _float & fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);
	if(Engine::DIKeyDown(DIK_5))
	{
		m_pCurMelee = m_vecItem[1];
	}
	if (Engine::DIKeyDown(DIK_6))
	{
		m_pCurMelee = m_vecItem[2];
	}
	if (Engine::DIKeyDown(DIK_7))
	{
		m_pCurMelee = m_vecItem[3];
	}
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
}

void CInventory::Put(CConsumeItem * pItem)
{
	if (pItem == nullptr || pItem->GetItemState() == IS_TAKE)
		return;
	
	switch (pItem->GetItemEffect())
	{
	case IE_MONEY:
		m_iEmerald++;
		break;
	case IE_ARROW:
		m_iArrow++;
		break;
	case IE_HEAL:
		//플레이어 체력 증가
		break;
	case IE_POSION:
		//플레이어 투명
		break;
	default:
		_CRASH("worng access");
	}

	pItem->ItemDelete();

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

