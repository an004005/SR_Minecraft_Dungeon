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
#include "Player.h"
#include "StatComponent.h"
#include "ShockPowder.h"
#include "FireworksArrow.h"
#include "BootsOfSwiftness.h"
#include "InventoryUI.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	//ZeroMemory(&m_arrRendSpace, sizeof(_bool) * 12);
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_Object()
{
	m_arrEquip[IT_RANGE] = CItemFactory::Create<CCrossbow>("Crossbow", L"Crossbow", IS_TAKE);
	m_arrEquip[IT_RANGE]->AddRef();

	m_arrEquip[IT_MELEE] = CItemFactory::Create<CSword>("Sword", L"Sword", IS_TAKE);
	m_arrEquip[IT_MELEE]->AddRef();

	m_arrItem[0] = CItemFactory::Create<CGlaive>("Glaive", L"Glaive", IS_TAKE);
	m_arrItem[0]->AddRef();

	m_arrItem[3] = CItemFactory::Create<CAxe>("Axe", L"Axe", IS_TAKE);
	m_arrItem[3]->AddRef();

	m_arrEquip[IT_LEGACY1] = CItemFactory::Create<CShockPowder>("ShockPowder", L"ShockPowder", IS_TAKE);
	m_arrEquip[IT_LEGACY1]->AddRef();


	m_arrItem[5] = CItemFactory::Create<CFireworksArrow>("FireworksArrow", L"FireworksArrow", IS_TAKE);
	m_arrItem[5]->AddRef();


	//m_pLegacy2 = CItemFactory::Create<CBootsOfSwiftness>("BootsOfSwiftness", L"BootsOfSwiftness", IS_TAKE);
	//m_pLegacy2->AddRef();
	//m_arrItem[6] = m_pLegacy3;

	//UI 생성
	{
		//배경 생성
		m_pInventoryUI = CUIFactory::Create<CInventoryUI>("InventoryUI", L"InventoryUI", 0);

		// 인벤 창 생성
		_float fItemSpaceSize = WINCX * 0.08f;
		for (_int i = 0; i < 4; ++i)
		{
			for (_int j = 0; j < 3; ++j)
			{
				_int iIdx = i * 3 + j;
				wstring wstrItemName = L"ItemSpaceUI" + to_wstring(iIdx);
				CItemSpaceUI* pItemSpace = CUIFactory::Create<CItemSpaceUI>("ItemSpaceUI", wstrItemName, 0, WINCX * 0.4f + (fItemSpaceSize * 0.55f  * j * 2), WINCY*0.25f + (fItemSpaceSize * 0.55f  * i * 2), fItemSpaceSize, fItemSpaceSize);
				pItemSpace->SetRenderType(SLOT_BASE);
				m_arrItemSpace[iIdx] = pItemSpace;

			}
		}

	
		// 착용중인 장비 생성
		_float fEquipSlotSize = WINCX * 0.07f;
		CItemSpaceUI* pWeaponSlot = CUIFactory::Create<CItemSpaceUI>("ItemSpaceUI", L"MeleeSlot", 0, WINCX * 0.08f, WINCY*0.23f, fEquipSlotSize, fEquipSlotSize);
		pWeaponSlot->SetRenderType(SLOT_WEAPON);
		m_arrEquipSpace[IT_MELEE] = pWeaponSlot;

		pWeaponSlot = CUIFactory::Create<CItemSpaceUI>("ItemSpaceUI", L"RangeSlot", 0, WINCX * 0.28f, WINCY*0.23f, fEquipSlotSize, fEquipSlotSize);
		pWeaponSlot->SetRenderType(SLOT_WEAPON);
		m_arrEquipSpace[IT_RANGE] = pWeaponSlot;

		// 착용중인 유물 생성
		CItemSpaceUI* pLegacySlot = CUIFactory::Create<CItemSpaceUI>("ItemSpaceUI", L"LegacySlot1", 0, WINCX * 0.1f, WINCY*0.85f, fEquipSlotSize, fEquipSlotSize);
		pLegacySlot->SetRenderType(SLOT_LEGACY);
		m_arrEquipSpace[IT_LEGACY1] = pLegacySlot;

		pLegacySlot = CUIFactory::Create<CItemSpaceUI>("ItemSpaceUI", L"LegacySlot2", 0, WINCX * 0.1f + fEquipSlotSize * 0.55f * 2.f, WINCY*0.85f, fEquipSlotSize, fEquipSlotSize);
		pLegacySlot->SetRenderType(SLOT_LEGACY);
		m_arrEquipSpace[IT_LEGACY2] = pLegacySlot;

		pLegacySlot = CUIFactory::Create<CItemSpaceUI>("ItemSpaceUI", L"LegacySlot3", 0, WINCX * 0.1f + fEquipSlotSize * 0.55f * 4.f, WINCY*0.85f, fEquipSlotSize, fEquipSlotSize);
		pLegacySlot->SetRenderType(SLOT_LEGACY);
		m_arrEquipSpace[IT_LEGACY3] = pLegacySlot;
	
	}

	m_iArrow = 90;
	m_iEmerald = 0;

	return S_OK;
}

_int CInventory::Update_Object(const _float & fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	//아이템 UI 출력
	if (!m_pInventoryUI->IsClosen())
	{
		for (int i = 0; i < COL * ROW; ++i)
		{
			if (m_arrItem[i] != nullptr)
			{
				m_arrItemSpace[i]->SetUITexture(m_arrItem[i]->GetUITexNum());
				MouseEvent(m_arrItemSpace[i], m_arrItem[i], i , false);
			}
		}

		for (int i = 0; i < IT_END; ++i)
		{
			if (m_arrEquip[i] != nullptr)
			{
				m_arrEquipSpace[i]->SetUITexture(m_arrEquip[i]->GetUITexNum());
				MouseEvent(m_arrEquipSpace[i] , m_arrEquip[i], i , true);
			}
		}
	}


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
	for (auto& item : m_arrItem)
	{
		Safe_Release(item);
	}
	for (auto& item : m_arrEquip)
	{
		Safe_Release(item);
	}

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

void CInventory::OpenInventory()
{
	static _int Select = 0;
	Select++;
	if (Select & 1)
	{
		m_pInventoryUI->Open();
		m_bLock = true;
		for (int i = 0; i < COL; ++i)
		{
			for (int j = 0; j < ROW; ++j)
			{
				_int iIdx = i * 3 + j;
				m_arrItemSpace[iIdx]->Open();
			}
		}

		for (auto i : m_arrEquipSpace)
			i->Open();
	}
	else
	{
		m_pInventoryUI->Close();
		m_bLock = false;
		for (int i = 0; i < COL; ++i)
		{
			for (int j = 0; j < ROW; ++j)
			{
				_int iIdx = i * 3 + j;
				m_arrItemSpace[iIdx]->Close();
			}
		}

		for (auto i : m_arrEquipSpace)
			i->Close();
	}
}

void CInventory::Put(CEquipItem * pItem)
{
	if (pItem == nullptr || pItem->GetItemState() == IS_TAKE)
		return;

	for (int i = 0; i < COL * ROW; ++i)
	{
		if (m_arrItem[i] == nullptr)
		{
			m_arrItem[i] = pItem;
			m_arrItem[i]->AddRef();
			m_arrItem[i]->SetState(IS_TAKE);
			break;
		}
	}
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
		m_iArrow += 40;
		break;
	case IE_HEAL:
	{
		CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
		pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
			->ModifyHP(15);
	}
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
	auto itr = std::find(m_arrItem.begin(), m_arrItem.end(), pItem);
	if (itr == m_arrItem.end()) return;
	Safe_Release((*itr));
	(*itr) = nullptr;
}

void CInventory::Equip_Item(SkeletalPart* pSkeletalPart, ITEMTYPE eIT)
{	
	if (eIT >= IT_END)
	{
		_CRASH("wrong Item Type");
	}

	m_arrEquip[eIT]->Equipment(pSkeletalPart);
}

CEquipItem * CInventory::CurWeapon(ITEMTYPE eIT)
{
	if (eIT >= IT_END)
	{
		_CRASH("wrong Item Type");
	}

	return m_arrEquip[eIT];
}

void CInventory::MouseEvent(CItemSpaceUI* pItemSpaceUI, CEquipItem* pEquipItem, _int index , _bool bEquipState)
{
	
	_matrix& matIconWorld = pItemSpaceUI->GetIconWorld();
	_float fX = matIconWorld._41 + WINCX / 2;
	_float fY = -matIconWorld._42 + WINCY / 2;

	RECT	rcUI = { LONG(fX - matIconWorld._11 * 0.5f), LONG(fY - matIconWorld._22 * 0.5f), LONG(fX + matIconWorld._11 * 0.5f), LONG(fY + matIconWorld._22 * 0.5f) };

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	// check mouse collison
	if (PtInRect(&rcUI, ptMouse))
	{
		// check mouse click event
		if (MouseKeyDown(DIM_LB))
		{
			// occur clickFrame Event
			for (int i = 0; i < COL * ROW; ++i)
			{
				if (m_arrItem[i] != nullptr)
					m_arrItemSpace[i]->SetCurClickFrame(false);
			}
			pItemSpaceUI->SetCurClickFrame(true);
			//아이템 특성과 설명 UI 추가
		
		}
		else if (MouseKeyPress(DIM_LB))
		{
			// if other Icon has picked
			if (pItemSpaceUI->GetPick())
				return;

			// chasing mouse point
			_vec3 vPointAt;

			D3DVIEWPORT9		ViewPort;
			ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
			m_pGraphicDev->GetViewport(&ViewPort);


			vPointAt.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
			vPointAt.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
			vPointAt.z = 1.f;

			_matrix matProj;
			D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.1f, 1.f);
			D3DXMatrixInverse(&matProj, nullptr, &matProj);
			D3DXVec3TransformCoord(&vPointAt, &vPointAt, &matProj);

			matIconWorld._41 = vPointAt.x;
			matIconWorld._42 = vPointAt.y;

			//cant collision other Icon		
			for (int i = 0; i < COL * ROW; ++i)
			{
				m_arrItemSpace[i]->Do_OtherIcon_Picking(true);
			}

			for (int i = 0; i < IT_END; ++i)
			{
				m_arrEquipSpace[i]->Do_OtherIcon_Picking(true);
			}
			
			pItemSpaceUI->Do_OtherIcon_Picking(false);
		}
		else if (MouseKeyUp(DIM_LB))
		{
			// befor blackspace clickFrame off
			// set worldmatrix at immediate black space by check collision
			// clickFrame on

			//pItemSpaceUI->SetCurClickFrame(false);
			
			ITEMTYPE eType = pEquipItem->GetItemType();
			
			for (int i = 0; i < COL * ROW; ++i)
			{
				if (i != index)
				{
					CTransform* pTrans = m_arrItemSpace[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
					_vec3 vPos = pTrans->m_vInfo[INFO_POS];
					_vec3 vSize = pTrans->m_vScale;
					_float fX = vPos.x + WINCX / 2;
					_float fY = -vPos.y + WINCY / 2;

					RECT	rcUI = { LONG(fX - vSize.x * 0.5f), LONG(fY - vSize.y * 0.5f), LONG(fX + vSize.x * 0.5f), LONG(fY + vSize.y * 0.5f) };
				
					POINT		ptMouse;
					GetCursorPos(&ptMouse);
					ScreenToClient(g_hWnd, &ptMouse);

					// check mouse collison
					if (PtInRect(&rcUI, ptMouse))
					{
						//inven->inven
						if (!bEquipState)
						{
							m_arrItem[index] = m_arrItem[i];
							m_arrItem[i] = pEquipItem;

							m_arrItemSpace[index]->SetUITexture(0);
							m_arrItemSpace[index]->SetCurClickFrame(false);
							m_arrItemSpace[index]->InitIconWorldSet();
							m_arrItemSpace[i]->SetUITexture(m_arrItem[i]->GetUITexNum());
							m_arrItemSpace[i]->SetCurClickFrame(true);
							break;
						}
						//equip->inven
						else
						{
							if (m_arrItem[i] == nullptr)
							{
								m_arrEquip[index] = m_arrItem[i];
								m_arrItem[i] = pEquipItem;

								m_arrEquipSpace[index]->SetUITexture(0);
								m_arrEquipSpace[index]->SetCurClickFrame(false);
								m_arrEquipSpace[index]->InitIconWorldSet();
								m_arrItemSpace[i]->SetUITexture(m_arrItem[i]->GetUITexNum());
								m_arrItemSpace[i]->SetCurClickFrame(true);
								break;
							}
							else
							{
								if (eType == IT_MELEE || eType == IT_RANGE ||
									m_arrItem[i]->GetItemType() == IT_MELEE || m_arrItem[i]->GetItemType() == IT_RANGE)
									break;

								m_arrEquip[index] = m_arrItem[i];
								m_arrItem[i] = pEquipItem;

								m_arrEquipSpace[index]->SetUITexture(0);
								m_arrEquipSpace[index]->SetCurClickFrame(false);
								m_arrEquipSpace[index]->InitIconWorldSet();
								m_arrItemSpace[i]->SetUITexture(m_arrItem[i]->GetUITexNum());
								m_arrItemSpace[i]->SetCurClickFrame(true);
								break;
							}				
						}
					}
				}
			}

			
			for (int i = 0; i < IT_END; ++i)
			{
				if (i != index)
				{
					CTransform* pTrans = m_arrEquipSpace[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
					_vec3 vPos = pTrans->m_vInfo[INFO_POS];
					_vec3 vSize = pTrans->m_vScale;
					_float fX = vPos.x + WINCX / 2;
					_float fY = -vPos.y + WINCY / 2;

					RECT	rcUI = { LONG(fX - vSize.x * 0.5f), LONG(fY - vSize.y * 0.5f), LONG(fX + vSize.x * 0.5f), LONG(fY + vSize.y * 0.5f) };

					POINT		ptMouse;
					GetCursorPos(&ptMouse);
					ScreenToClient(g_hWnd, &ptMouse);

					// check mouse collison
					if (PtInRect(&rcUI, ptMouse))
					{
						//equip->equip
						if (bEquipState)
						{
							m_arrEquip[index] = m_arrEquip[i];
							m_arrEquip[i] = pEquipItem;

							m_arrEquipSpace[index]->SetUITexture(0);
							m_arrEquipSpace[index]->SetCurClickFrame(false);
							m_arrEquipSpace[index]->InitIconWorldSet();
							m_arrEquipSpace[i]->SetUITexture(m_arrEquip[i]->GetUITexNum());
							m_arrEquipSpace[i]->SetCurClickFrame(true);
							break;
						}
						//inven->equip
						else
						{
							//무기는 nullptr이 될 수 없다
							if (m_arrEquip[i] == nullptr)
							{
								if (eType != IT_END)
									break;

								m_arrItem[index] = m_arrEquip[i];
								m_arrEquip[i] = pEquipItem;

								m_arrItemSpace[index]->SetUITexture(0);
								m_arrItemSpace[index]->SetCurClickFrame(false);
								m_arrItemSpace[index]->InitIconWorldSet();
								m_arrEquipSpace[i]->SetUITexture(m_arrEquip[i]->GetUITexNum());
								m_arrEquipSpace[i]->SetCurClickFrame(true);
								break;
							}
							else
							{
								if (eType != m_arrEquip[i]->GetItemType())
									break;

								m_arrItem[index] = m_arrEquip[i];
								m_arrEquip[i] = pEquipItem;

								m_arrItemSpace[index]->SetUITexture(0);
								m_arrItemSpace[index]->SetCurClickFrame(false);
								m_arrItemSpace[index]->InitIconWorldSet();
								m_arrEquipSpace[i]->SetUITexture(m_arrEquip[i]->GetUITexNum());
								m_arrEquipSpace[i]->SetCurClickFrame(true);
								break;
							}	
						}						
					}
				}
			}
			

			//if not changed
			pItemSpaceUI->InitIconWorldSet();

			// finish picking
			for (int i = 0; i < COL * ROW; ++i)
			{
				m_arrItemSpace[i]->Do_OtherIcon_Picking(false);
			}

			for (int i = 0; i < IT_END; ++i)
			{
				m_arrEquipSpace[i]->Do_OtherIcon_Picking(false);
			}

		}
		else if (MouseKeyDown(DIM_RB))
		{
			// Change Equip item
			ITEMTYPE eType = pEquipItem->GetItemType();
			
			// in itemspace
			if (!bEquipState)
			{
				if (eType == IT_MELEE || eType == IT_RANGE)
				{
					m_arrItem[index] = m_arrEquip[eType];
					m_arrEquip[eType] = pEquipItem;

					m_arrItemSpace[index]->SetUITexture(0);
					m_arrItemSpace[index]->SetCurClickFrame(false);
					m_arrItemSpace[index]->InitIconWorldSet();
					m_arrEquipSpace[eType]->SetUITexture(m_arrEquip[eType]->GetUITexNum());
					return;
				}
				else
				{
					for (_int i = IT_LEGACY1; i < IT_END; ++i)
					{
						if (m_arrEquip[i] == nullptr)
						{
							m_arrItem[index] = m_arrEquip[i];
							m_arrEquip[i] = pEquipItem;

							m_arrItemSpace[index]->SetUITexture(0);
							m_arrItemSpace[index]->SetCurClickFrame(false);
							m_arrItemSpace[index]->InitIconWorldSet();
							m_arrEquipSpace[i]->SetUITexture(m_arrEquip[i]->GetUITexNum());
							return;
						}
					}

					// full space
					m_arrItem[index] = m_arrEquip[IT_LEGACY3];
					m_arrEquip[IT_LEGACY3] = pEquipItem;

					m_arrItemSpace[index]->SetUITexture(0);
					m_arrItemSpace[index]->SetCurClickFrame(false);
					m_arrItemSpace[index]->InitIconWorldSet();
					m_arrEquipSpace[IT_LEGACY3]->SetUITexture(m_arrEquip[IT_LEGACY3]->GetUITexNum());
				}
			}
			// in equipspace
			else
			{
				for (_int i = 0; i < COL * ROW; ++i)
				{
					if (m_arrItem[i] == nullptr)
					{
						m_arrEquip[index] = m_arrItem[i];
						m_arrItem[i] = pEquipItem;

						m_arrEquipSpace[index]->SetUITexture(0);
						m_arrEquipSpace[index]->SetCurClickFrame(false);
						m_arrEquipSpace[index]->InitIconWorldSet();
						m_arrItemSpace[i]->SetUITexture(m_arrItem[i]->GetUITexNum());
						break;
					}
				}
			}

		}
		else
		{
			pItemSpaceUI->SetMouseCollFrame();
		}
	}
}

