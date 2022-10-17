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
#include "PowerRune.h"
#include "StunRune.h"
#include "MultiShotRune.h"
#include "LightningRune.h"
#include "LaserShotRune.h"
#include "ItemTexUI.h"
#include "StormRune.h"

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
	//UI ����
	{
		//��� ����
		m_pBackGround = CUIFactory::Create<CInventoryUI>("InventoryUI", L"BackGround", 0, WINCX * 0.5f, WINCY * 0.5f, WINCX, WINCY);
		m_pBackGround->SetRenderPriority(9);
		// �κ� â ����
		_float fItemSpaceSize = WINCX * 0.08f;
		for (_int i = 0; i < 4; ++i)
		{
			for (_int j = 0; j < 3; ++j)
			{
				_int iIdx = i * 3 + j;
				wstring wstrItemName = L"InvenSpaceUI" + to_wstring(iIdx);
				m_arrItemSlot[iIdx] = CUIFactory::Create<CInventoryUI>("InventoryUI", wstrItemName, 0, WINCX * 0.4f + (fItemSpaceSize * 0.55f  * j * 2), WINCY*0.25f + (fItemSpaceSize * 0.55f  * i * 2), fItemSpaceSize, fItemSpaceSize);
				m_arrItemSlot[iIdx]->SetUITexture(1);
			}
		}


		// ���� ���� ĭ ����
		_float fEquipSlotSize = WINCX * 0.07f;
		m_pMeleeSlot = CUIFactory::Create<CInventoryUI>("InventoryUI", L"MeleeSlot", 0, WINCX * 0.08f, WINCY*0.23f, fEquipSlotSize, fEquipSlotSize);
		m_pMeleeSlot->SetUITexture(2);

		//���Ÿ� ���� ĭ ����
		m_pRangeSlot = CUIFactory::Create<CInventoryUI>("InventoryUI", L"RangeSlot", 0, WINCX * 0.28f, WINCY*0.23f, fEquipSlotSize, fEquipSlotSize);
		m_pRangeSlot->SetUITexture(2);

		// ����ĭ ����
		m_arrLegacySlot[0] = CUIFactory::Create<CInventoryUI>("InventoryUI", L"LegacySlot1", 0, WINCX * 0.1f, WINCY * 0.85f, fEquipSlotSize, fEquipSlotSize);
		m_arrLegacySlot[1] = CUIFactory::Create<CInventoryUI>("InventoryUI", L"LegacySlot2", 0, WINCX * 0.1f + fEquipSlotSize * 0.55f * 2.f, WINCY * 0.85f, fEquipSlotSize, fEquipSlotSize);
		m_arrLegacySlot[2] = CUIFactory::Create<CInventoryUI>("InventoryUI", L"LegacySlot3", 0, WINCX * 0.1f + fEquipSlotSize * 0.55f * 4.f, WINCY * 0.85f, fEquipSlotSize, fEquipSlotSize);
		
		m_arrLegacySlot[0]->SetUITexture(6);
		m_arrLegacySlot[1]->SetUITexture(6);
		m_arrLegacySlot[2]->SetUITexture(6);
	
		// ��ĭ ����
		m_pRuneSlot = CUIFactory::Create<CInventoryUI>("InventoryUI", L"RuneSlot", 0, WINCX * 0.8f, WINCY*0.85f, fEquipSlotSize, fEquipSlotSize);
		m_pRuneSlot->SetUITexture(3);

		// ������ ����
		m_pClickFrame = CUIFactory::Create<CItemUI>("ItemUI", L"ClickFrame", 0);
		m_pClickFrame->SetUITexture(5);

		m_pCollFrame = CUIFactory::Create<CItemUI>("ItemUI", L"CollFrame", 0);
		m_pCollFrame->SetUITexture(4);

		// �ؽ���
		fEquipSlotSize = WINCX * 0.09f;
		m_pItemTexUI = CUIFactory::Create<CItemTexUI>("ItemTexUI", L"ItemTexUI", 0, WINCX * 0.9f, WINCY*0.2f, fEquipSlotSize, fEquipSlotSize);
	}
	m_iArrow = 90;
	m_iEmerald = 0;

	return S_OK;
}

_int CInventory::Update_Object(const _float & fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	// ItemUI Rend
	if (!m_pBackGround->IsClosen())
	{
		
		for (int i = 0; i < COL * ROW; ++i)
		{
			if (m_arrItem[i] != nullptr)
			{
				CItemUI* pItemUI = m_arrItem[i]->GetItemUI();

				if (!pItemUI->IsPick())
				{
					CTransform* pTrans = m_arrItemSlot[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
					_vec3 vPos = pTrans->m_vInfo[INFO_POS];
					_vec3 vSize = pTrans->m_vScale;
					pItemUI->SetPos({ vPos.x, vPos.y, 0.f });
					pItemUI->SetSize(vSize);
					pItemUI->Open();
				}
				MouseEvent(m_arrItem[i], pItemUI, i);
			}
			
		}

		if (m_pMelee != nullptr)
		{
			CItemUI* pItemUI = m_pMelee->GetItemUI();
			if (!m_pMelee->GetItemUI()->IsPick())
			{
				CTransform* pTrans = m_pMeleeSlot->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
				_vec3 vPos = pTrans->m_vInfo[INFO_POS];
				_vec3 vSize = pTrans->m_vScale;
				pItemUI->SetPos({ vPos.x, vPos.y, 0.f });
				pItemUI->SetSize(vSize);
				pItemUI->Open();
			}
			MouseEvent(m_pMelee, pItemUI, 0);
		}

		if (m_pRange != nullptr)
		{
			CItemUI* pItemUI = m_pRange->GetItemUI();
			if (!m_pRange->GetItemUI()->IsPick())
			{				
				CTransform* pTrans = m_pRangeSlot->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
				_vec3 vPos = pTrans->m_vInfo[INFO_POS];
				_vec3 vSize = pTrans->m_vScale;
				pItemUI->SetPos({ vPos.x, vPos.y, 0.f });
				pItemUI->SetSize(vSize);
				pItemUI->Open();
			}
			MouseEvent(m_pRange, pItemUI, 0);
		}

		
		
		for (_int i = 0; i < LEGACY_SLOT_END; ++i)
		{
			if (m_arrLegacy[i] != nullptr && m_arrLegacy[i]->GetItemUI() != nullptr)
			{
				CItemUI* pItemUI = m_arrLegacy[i]->GetItemUI();

				if (!m_arrLegacy[i]->GetItemUI()->IsPick())
				{
					CTransform* pTrans = m_arrLegacySlot[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
					_vec3 vPos = pTrans->m_vInfo[INFO_POS];
					_vec3 vSize = pTrans->m_vScale;
					pItemUI->SetPos({ vPos.x, vPos.y, 0.f });
					pItemUI->SetSize(vSize);
					pItemUI->Open();
				}
					MouseEvent(m_arrLegacy[i], pItemUI, i);
				
			}
		}

		if (m_pRune != nullptr)
		{
			CItemUI* pItemUI = m_pRune->GetItemUI();
			CTransform* pTrans = m_pRuneSlot->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
			_vec3 vPos = pTrans->m_vInfo[INFO_POS];
			_vec3 vSize = pTrans->m_vScale;
			pItemUI->SetPos({ vPos.x, vPos.y, 0.f });
			pItemUI->SetSize(vSize);
			m_pRuneSlot->Open();
			MouseEvent(m_pRune, pItemUI, 0);
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
	
	Safe_Release(m_pMelee);
	Safe_Release(m_pRange);

	for (auto& item : m_arrLegacy)
	{
		Safe_Release(item);
	}

	//Safe_Release(m_pRune);
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
		m_pBackGround->Open();
		m_bLock = true;
		for (int i = 0; i < COL; ++i)
		{
			for (int j = 0; j < ROW; ++j)
			{
				_int iIdx = i * 3 + j;
				m_arrItemSlot[iIdx]->Open();
			}
		}
		m_pMeleeSlot->Open();
		m_pRangeSlot->Open();

		for (auto i : m_arrLegacySlot)
			i->Open();

	}
	else
	{
		m_pBackGround->Close();
		m_bLock = false;
		for (int i = 0; i < COL; ++i)
		{
			for (int j = 0; j < ROW; ++j)
			{
				_int iIdx = i * 3 + j;
				m_arrItemSlot[iIdx]->Close();

				if (m_arrItem[iIdx] != nullptr)
				{
					if (m_arrItem[iIdx]->GetItemUI() != nullptr)
					{
						m_arrItem[iIdx]->GetItemUI()->Close();
					}
				}
			}
		}

		m_pMeleeSlot->Close();
		if (m_pMelee != nullptr)
		{
			m_pMelee->GetItemUI()->Close();
		}
	
		m_pRangeSlot->Close();
		if (m_pRange != nullptr)
		{
			m_pRange->GetItemUI()->Close();
		}

		for (_int i = 0;i < LEGACY_SLOT_END; ++i)
		{
			m_arrLegacySlot[i]->Close();
			if (m_arrLegacy[i] != nullptr)
			{
				if (m_arrLegacy[i]->GetItemUI() != nullptr)
				{
					m_arrLegacy[i]->GetItemUI()->Close();
				}
			}
		}
		pCurClickUI = nullptr;
		m_pPreClickItem = nullptr;
		m_pCurClickItem = nullptr;
		m_pRuneSlot->Close();
		m_pCollFrame->Close();
		m_pClickFrame->Close();
		m_pItemTexUI->Close();
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
			m_arrItem[i]->SetOwner(m_pOwner);
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
		// CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
		m_pOwner->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
			->ModifyHP(15);
	}
		break;
	case IE_POSION:
		//�÷��̾� ����
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
	m_pOwner = nullptr;
}

void CInventory::AddDefaultItems()
{
	m_pRange = CItemFactory::Create<CCrossbow>("Crossbow", L"Crossbow", IS_TAKE);
	m_pRange->AddRef();
	m_pRange->SetOwner(m_pOwner);
	

	m_pMelee = CItemFactory::Create<CSword>("Sword", L"Sword", IS_TAKE);
	m_pMelee->AddRef();
	m_pMelee->SetOwner(m_pOwner);

	m_arrItem[0] = CItemFactory::Create<CGlaive>("Glaive", L"Glaive", IS_TAKE);
	m_arrItem[0]->AddRef();
	m_arrItem[0]->SetOwner(m_pOwner);

	m_arrItem[1] = CItemFactory::Create<CLaserShotRune>("LaserShotRune", L"LaserShotRune", IS_TAKE);
	m_arrItem[1]->AddRef();
	m_arrItem[1]->SetOwner(m_pOwner);

	m_arrItem[2] = CItemFactory::Create<CStormRune>("StormRune", L"StormRune", IS_TAKE);
	m_arrItem[2]->AddRef();
	m_arrItem[2]->SetOwner(m_pOwner);

	m_arrItem[3] = CItemFactory::Create<CAxe>("Axe", L"Axe", IS_TAKE);
	m_arrItem[3]->AddRef();
	m_arrItem[3]->SetOwner(m_pOwner);

	m_arrItem[4] = CItemFactory::Create<CStunRune>("StunRune", L"StunRune", IS_TAKE);
	m_arrItem[4]->AddRef();
	m_arrItem[4]->SetOwner(m_pOwner);

	m_arrItem[5] = CItemFactory::Create<CFireworksArrow>("FireworksArrow", L"FireworksArrow", IS_TAKE);
	m_arrItem[5]->AddRef();
	m_arrItem[5]->SetOwner(m_pOwner);

	m_arrItem[6] = CItemFactory::Create<CPowerRune>("PowerRune", L"PowerRune", IS_TAKE);
	m_arrItem[6]->AddRef();
	m_arrItem[6]->SetOwner(m_pOwner);

	m_arrLegacy[LEGACY_SLOT1] = CItemFactory::Create<CShockPowder>("ShockPowder", L"ShockPowder", IS_TAKE);
	m_arrLegacy[LEGACY_SLOT1]->AddRef();
	m_arrLegacy[LEGACY_SLOT1]->SetOwner(m_pOwner);
}

void CInventory::Equip_Item(SkeletalPart* pSkeletalPart, ITEMTYPE eIT)
{	
	if (eIT >= IT_END)
	{
		_CRASH("wrong Item Type");
	}

	switch (eIT)
	{
	case IT_MELEE:
		m_pMelee->Equipment(pSkeletalPart);
		break;
	case IT_RANGE:
		m_pRange->Equipment(pSkeletalPart);
		break;
	case IT_LEGACY1:
		m_arrLegacy[LEGACY_SLOT1]->Equipment(pSkeletalPart);
		break;
	case IT_LEGACY2:
		m_arrLegacy[LEGACY_SLOT2]->Equipment(pSkeletalPart);
		break;
	case IT_LEGACY3:
		m_arrLegacy[LEGACY_SLOT3]->Equipment(pSkeletalPart);
		break;
	}
	
}

CEquipItem * CInventory::CurWeapon(ITEMTYPE eIT)
{
	if (eIT >= IT_END)
	{
		_CRASH("wrong Item Type");
	}

	switch (eIT)
	{
	case IT_MELEE:
		return m_pMelee;
	case IT_RANGE:
		return m_pRange;
	case IT_LEGACY1:
		return m_arrLegacy[LEGACY_SLOT1];
	case IT_LEGACY2:
		return m_arrLegacy[LEGACY_SLOT2];
	case IT_LEGACY3:
		return m_arrLegacy[LEGACY_SLOT3];
	case IT_RUNE:
		return nullptr;
	}

	return nullptr;
	
}

void CInventory::CreateClickFrame()
{
	m_pClickFrame->Open();

	for (_int i = 0; i < COL * ROW; ++i)
	{
		if (m_arrItem[i] == m_pCurClickItem)
		{
			CTransform* pTrans = m_arrItemSlot[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
			_vec3 vPos = pTrans->m_vInfo[INFO_POS];
			_vec3 vSize = pTrans->m_vScale;
			vSize *= 1.1f;
			m_pClickFrame->SetPos({ vPos.x, vPos.y, 0.f });
			m_pClickFrame->SetSize(vSize);
			return;
		}
	}

	if (m_pMelee == m_pCurClickItem)
	{
		CTransform* pTrans = m_pMeleeSlot->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
		_vec3 vPos = pTrans->m_vInfo[INFO_POS];
		_vec3 vSize = pTrans->m_vScale;
		vSize *= 1.1f;
		m_pClickFrame->SetPos({ vPos.x, vPos.y, 0.f });
		m_pClickFrame->SetSize(vSize);
		return;
	}

	if (m_pRange == m_pCurClickItem)
	{
		CTransform* pTrans = m_pRangeSlot->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
		_vec3 vPos = pTrans->m_vInfo[INFO_POS];
		_vec3 vSize = pTrans->m_vScale;
		vSize *= 1.1f;
		m_pClickFrame->SetPos({ vPos.x, vPos.y, 0.f });
		m_pClickFrame->SetSize(vSize);
		return;
	}

	for (_int i = 0; i < LEGACY_SLOT_END; ++i)
	{
		if (m_arrLegacy[i] == m_pCurClickItem)
		{
			CTransform* pTrans = m_arrLegacySlot[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
			_vec3 vPos = pTrans->m_vInfo[INFO_POS];
			_vec3 vSize = pTrans->m_vScale;
			vSize *= 1.1f;
			m_pClickFrame->SetPos({ vPos.x, vPos.y, 0.f });
			m_pClickFrame->SetSize(vSize);
			return;
		}
	}
	
}

void CInventory::CreateCollFrame(CEquipItem* pCurCollItem)
{
	for (_int i = 0; i < COL * ROW; ++i)
	{
		if (m_arrItem[i] == pCurCollItem)
		{
			CTransform* pTrans = m_arrItemSlot[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
			_vec3 vPos = pTrans->m_vInfo[INFO_POS];
			_vec3 vSize = pTrans->m_vScale;
			vSize *= 1.1f;
			m_pCollFrame->SetPos({ vPos.x, vPos.y, 0.f });
			m_pCollFrame->SetSize(vSize);
			return;
		}
	}

	if (m_pMelee == pCurCollItem)
	{
		CTransform* pTrans = m_pMeleeSlot->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
		_vec3 vPos = pTrans->m_vInfo[INFO_POS];
		_vec3 vSize = pTrans->m_vScale;
		vSize *= 1.1f;
		m_pCollFrame->SetPos({ vPos.x, vPos.y, 0.f });
		m_pCollFrame->SetSize(vSize);
		return;
	}

	if (m_pRange == pCurCollItem)
	{
		CTransform* pTrans = m_pRangeSlot->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
		_vec3 vPos = pTrans->m_vInfo[INFO_POS];
		_vec3 vSize = pTrans->m_vScale;
		vSize *= 1.1f;
		m_pCollFrame->SetPos({ vPos.x, vPos.y, 0.f });
		m_pCollFrame->SetSize(vSize);
		return;
	}

	for (_int i = 0; i < LEGACY_SLOT_END; ++i)
	{
		if (m_arrLegacy[i] == pCurCollItem)
		{
			CTransform* pTrans = m_arrLegacySlot[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
			_vec3 vPos = pTrans->m_vInfo[INFO_POS];
			_vec3 vSize = pTrans->m_vScale;
			vSize *= 1.1f;
			m_pCollFrame->SetPos({ vPos.x, vPos.y, 0.f });
			m_pCollFrame->SetSize(vSize);
			return;
		}
	}

}

void CInventory::MouseEvent(CEquipItem * pCurCollItem, CItemUI * pCurCollUI, _int iSlotIndex)
{
	//for sound
	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom", ID_DYNAMIC);
	_vec3 vSoundPos = pPlayerTransform->m_vInfo[INFO_POS];

	_vec3& vIconPos = pCurCollUI->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
	_vec3& vIconSize = pCurCollUI->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vScale;
	_float fX = vIconPos.x + WINCX / 2;
	_float fY = -vIconPos.y + WINCY / 2;

	
	
	RECT	rcUI = { LONG(fX - vIconSize.x * 0.5f), LONG(fY - vIconSize.y * 0.5f), LONG(fX + vIconSize.x * 0.5f), LONG(fY + vIconSize.y * 0.5f) };
	
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	

	if (PtInRect(&rcUI, ptMouse))
	{
		m_pCurCollItem = pCurCollItem;
		CreateCollFrame(pCurCollItem);
		m_pCollFrame->Open();

		// check mouse click event
		if (MouseKeyDown(DIM_LB))
		{
			//sound play
			CSoundMgr::GetInstance()->PlaySound(L"inven_click_001.ogg", vSoundPos);
			m_pCurClickItem = pCurCollItem;
			m_iSlotIndex = iSlotIndex;
			// �� ���� ���� üũ
			if (CWeapon* pWeapon = dynamic_cast<CWeapon*>(m_pCurClickItem))
			{
				m_pRuneSlot->Open();
				m_pRune = pWeapon->GetRune();
			}
			else
				m_pRuneSlot->Close();			

		
			pCurCollUI->SetPick(true);
			//Ŭ���� ������ ����		
			CreateClickFrame();
			//�ؽ��� ���
			m_pItemTexUI->SetUITexture(pCurCollItem->GetUITexNum());
			m_pItemTexUI->Open();
		}
		else if (MouseKeyPress(DIM_LB))
		{
			if (!pCurCollUI->IsPick())
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

			vIconPos.x = vPointAt.x;
			vIconPos.y = vPointAt.y;

			if (vIconSize.x < WINCX * 0.1f && vIconSize.y < WINCX * 0.1f)
			{
				vIconSize.x *= 1.1f;
				vIconSize.y *= 1.1f;
			}
		
			
		}
		else if (MouseKeyUp(DIM_LB))
		{
			CSoundMgr::GetInstance()->PlaySound(L"inven_click_end-001.ogg", vSoundPos);
			m_pPreClickItem = m_pCurClickItem;
			pCurCollUI->SetPick(false);

			for (int i = 0; i < COL * ROW; ++i)
			{
				if (m_arrItem[i] != m_pCurClickItem && m_arrItem[i] == pCurCollItem)
				{
					m_arrItem[m_iSlotIndex] = m_arrItem[i];
					m_arrItem[i] = m_pCurClickItem;
					return;
				}

			}
		}
		else if (MouseKeyDown(DIM_RB))
		{
			CSoundMgr::GetInstance()->PlaySoundRandom({
								L"inven_equip-001.ogg",
								L"inven_equip-002.ogg",
								L"inven_equip-003.ogg",
								L"inven_equip-004.ogg" }, vSoundPos);


			//1. �κ����� ������Ű�� ������ �´� ĭ�� ���� �ؾ���.
			for (_int i = 0; i < COL * ROW; ++i)
			{

				if (m_arrItem[i] == pCurCollItem)
				{
					switch (m_arrItem[i]->GetItemType())
					{
					case IT_MELEE:
					{
						CEquipItem* pWeapon = m_pMelee;
						m_pMelee = pCurCollItem;
						m_arrItem[i] = pWeapon;
						return;
					}
					case IT_RANGE:
						m_arrItem[i] = m_pRange;
						m_pRange = pCurCollItem;
						return;
						//�����϶�
					case IT_END:
						for (_int j = 0; j < LEGACY_SLOT_END; ++j)
						{
							if (m_arrLegacy[j] == nullptr)
							{
								m_arrItem[i] = m_arrLegacy[j];
								m_arrLegacy[j] = pCurCollItem;
								return;
							}
						}
						return;
					case IT_RUNE:
					{
						CWeapon* pWeapon = dynamic_cast<CWeapon*>(m_pCurClickItem);
						CRune* pRune = dynamic_cast<CRune*>(pCurCollItem);
						if (pWeapon && pWeapon->GetRune() == nullptr)
						{
							if (pRune && pWeapon->SetRune(pRune))
							{
								m_arrItem[i] = nullptr;
								m_pRune = pRune;
								//Safe_Release(pRune);
								return;
							}
						}				
						else if(pWeapon && pWeapon->GetRune() != nullptr)
						{
							if (pRune && pWeapon->SetRune(pRune))
							{
								m_arrItem[i] = m_pRune;
								m_pRune = pRune;
								return;
							}
							
						}
				
						return;
					}
					default:
						MSG_BOX("�����ΰ�?");
						break;
					}

				}
			}
			
			if (m_pMelee == pCurCollItem)
			{
				return;
				//�������� ���� ���� �ȵ�
			}
			if (m_pRange == pCurCollItem)
			{
				return;
				//�������� ���� ���� �ȵ�
			}

			for (_int i = 0; i < LEGACY_SLOT_END; ++i)
			{
				if (m_arrLegacy[i] == pCurCollItem)
				{
					for (_int j = 0; j < COL * ROW; ++j)
					{
						if (m_arrItem[j] == nullptr)
						{
							m_arrLegacy[i] = m_arrItem[j];
							m_arrItem[j] = pCurCollItem;
							return;
						}
					}
				}
			}

			if (CRune* pRune = dynamic_cast<CRune*>(pCurCollItem))
			{
				if (dynamic_cast<CWeapon*>(m_pCurClickItem)->SetRune(nullptr))
				{
					m_pRune = nullptr;
					for (_int i = 0; i < COL * ROW; ++i)
					{
						if (m_arrItem[i] == nullptr)
						{				
							m_arrItem[i] = pRune;
							return;
						}
					}
				}
				
				return;
			}

		}
	}
	else if (m_pCurCollItem == pCurCollItem)
	{
		m_pCollFrame->Close();
	}
	
}

//void CInventory::MouseEvent(CItemSpaceUI* pItemSpaceUI, CEquipItem* pEquipItem, _int index , _bool bEquipState)
//{
//	//for sound
//	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom", ID_DYNAMIC);
//	_vec3 vSoundPos = pPlayerTransform->m_vInfo[INFO_POS];
//
//	_matrix& matIconWorld = pItemSpaceUI->GetIconWorld();
//	_float fX = matIconWorld._41 + WINCX / 2;
//	_float fY = -matIconWorld._42 + WINCY / 2;
//
//	RECT	rcUI = { LONG(fX - matIconWorld._11 * 0.5f), LONG(fY - matIconWorld._22 * 0.5f), LONG(fX + matIconWorld._11 * 0.5f), LONG(fY + matIconWorld._22 * 0.5f) };
//
//	POINT		ptMouse;
//	GetCursorPos(&ptMouse);
//	ScreenToClient(g_hWnd, &ptMouse);
//
//
//	// check mouse collison
//	if (PtInRect(&rcUI, ptMouse))
//	{	
//		//sound play	
//		if (pCurCollsionIconUI == nullptr || (pCurCollsionIconUI != pItemSpaceUI && !pCurCollsionIconUI->GetPick()))
//		{		
//			pCurCollsionIconUI = pItemSpaceUI;
//			CSoundMgr::GetInstance()->PlaySound(L"sfx_multi_clickLong-001.ogg", vSoundPos);
//		}
//
//
//		// check mouse click event
//		if (MouseKeyDown(DIM_LB))
//		{
//			//sound play
//			CSoundMgr::GetInstance()->PlaySound(L"inven_click_001.ogg", vSoundPos);
//			// occur clickFrame Event
//			for (int i = 0; i < COL * ROW; ++i)
//			{
//				if (m_arrItem[i] != nullptr)
//					m_arrItemSpace[i]->SetCurClickFrame(false);
//			}
//			for (int i = 0; i < IT_END; ++i)
//			{
//				if (m_arrEquip[i] != nullptr)
//					m_arrEquipSpace[i]->SetCurClickFrame(false);
//			}
//			pItemSpaceUI->SetCurClickFrame(true);
//
//			//if (pEquipItem->GetItemType() != IT_MELEE && pEquipItem->GetItemType() != IT_RANGE)
//			//	pItemSpaceUI->SetRuneIcon(false);
//		
//			pCurClickItem = pEquipItem;
//
//			if (CWeapon* pweapon = dynamic_cast<CWeapon*>(pCurClickItem))
//			{
//				if (m_arrEquipSpace[IT_RUNE])
//				{
//					m_arrEquipSpace[IT_RUNE]->Close();
//				}
//				
//				m_arrEquipSpace[IT_RUNE] = pweapon->GetRuneSlot();
//				m_arrEquipSpace[IT_RUNE]->Open();
//			}
//		
//			//������ Ư���� ���� UI �߰�
//			m_pInventoryUI->SetUITexture(pEquipItem->GetUITexNum());
//
//		}
//		else if (MouseKeyPress(DIM_LB))
//		{
//			// if other Icon has picked
//			if (pItemSpaceUI->GetPick())
//				return;
//
//			// chasing mouse point
//			_vec3 vPointAt;
//
//			D3DVIEWPORT9		ViewPort;
//			ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
//			m_pGraphicDev->GetViewport(&ViewPort);
//
//
//			vPointAt.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
//			vPointAt.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
//			vPointAt.z = 1.f;
//
//			_matrix matProj;
//			D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.1f, 1.f);
//			D3DXMatrixInverse(&matProj, nullptr, &matProj);
//			D3DXVec3TransformCoord(&vPointAt, &vPointAt, &matProj);
//
//			matIconWorld._41 = vPointAt.x;
//			matIconWorld._42 = vPointAt.y;
//
//			if (matIconWorld._11 < WINCX * 0.1f && matIconWorld._22 < WINCX * 0.1f)
//			{
//				matIconWorld._11 *= 1.1f;
//				matIconWorld._22 *= 1.1f;
//			}
//			//cant collision other Icon		
//			for (int i = 0; i < COL * ROW; ++i)
//			{
//				m_arrItemSpace[i]->Do_OtherIcon_Picking(true);
//			}
//
//			for (int i = 0; i < IT_END; ++i)
//			{
//				m_arrEquipSpace[i]->Do_OtherIcon_Picking(true);
//			}
//			
//			pItemSpaceUI->Do_OtherIcon_Picking(false);
//		}
//		else if (MouseKeyUp(DIM_LB))
//		{
//			// set worldmatrix at immediate black space by check collision
//			// clickFrame on
//
//			//sound play
//			CSoundMgr::GetInstance()->PlaySound(L"inven_click_end-001.ogg", vSoundPos);
//
//			ITEMTYPE eType = pEquipItem->GetItemType();
//			
//			for (int i = 0; i < COL * ROW; ++i)
//			{
//				if (i != index)
//				{
//					CTransform* pTrans = m_arrItemSpace[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
//					_vec3 vPos = pTrans->m_vInfo[INFO_POS];
//					_vec3 vSize = pTrans->m_vScale;
//					_float fX = vPos.x + WINCX / 2;
//					_float fY = -vPos.y + WINCY / 2;
//
//					RECT	rcUI = { LONG(fX - vSize.x * 0.5f), LONG(fY - vSize.y * 0.5f), LONG(fX + vSize.x * 0.5f), LONG(fY + vSize.y * 0.5f) };
//				
//					POINT		ptMouse;
//					GetCursorPos(&ptMouse);
//					ScreenToClient(g_hWnd, &ptMouse);
//
//					// check mouse collison
//					if (PtInRect(&rcUI, ptMouse))
//					{
//						//inven->inven
//						if (!bEquipState)
//						{
//							m_arrItem[index] = m_arrItem[i];
//							m_arrItem[i] = pEquipItem;
//							break;
//						}
//						//equip->inven
//						else
//						{
//							if (m_arrItem[i] == nullptr)
//							{
//								m_arrEquip[index] = m_arrItem[i];
//								m_arrItem[i] = pEquipItem;
//								break;
//							}
//							else
//							{
//								if (eType == IT_MELEE || eType == IT_RANGE ||
//									m_arrItem[i]->GetItemType() == IT_MELEE || m_arrItem[i]->GetItemType() == IT_RANGE)
//									break;
//
//								m_arrEquip[index] = m_arrItem[i];
//								m_arrItem[i] = pEquipItem;
//								break;
//							}				
//						}
//					}
//				}
//			}
//
//			
//			for (int i = 0; i < IT_END; ++i)
//			{
//				if (i != index)
//				{
//					CTransform* pTrans = m_arrEquipSpace[i]->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
//					_vec3 vPos = pTrans->m_vInfo[INFO_POS];
//					_vec3 vSize = pTrans->m_vScale;
//					_float fX = vPos.x + WINCX / 2;
//					_float fY = -vPos.y + WINCY / 2;
//
//					RECT	rcUI = { LONG(fX - vSize.x * 0.5f), LONG(fY - vSize.y * 0.5f), LONG(fX + vSize.x * 0.5f), LONG(fY + vSize.y * 0.5f) };
//
//					POINT		ptMouse;
//					GetCursorPos(&ptMouse);
//					ScreenToClient(g_hWnd, &ptMouse);
//
//					// check mouse collison
//					if (PtInRect(&rcUI, ptMouse))
//					{
//						//equip->equip
//						if (bEquipState)
//						{
//							m_arrEquip[index] = m_arrEquip[i];
//							m_arrEquip[i] = pEquipItem;
//							break;
//						}
//						//inven->equip
//						else
//						{
//							//����� nullptr�� �� �� ����
//							if (m_arrEquip[i] == nullptr)
//							{
//								if (eType != IT_END)
//									break;
//
//								m_arrItem[index] = m_arrEquip[i];
//								m_arrEquip[i] = pEquipItem;
//								break;
//							}
//							else
//							{
//								if (eType != m_arrEquip[i]->GetItemType())
//									break;
//
//								m_arrItem[index] = m_arrEquip[i];
//								m_arrEquip[i] = pEquipItem;
//								break;
//							}
//						}						
//					}
//				}
//			}
//			
//
//			//if not changed
//			pItemSpaceUI->InitIconWorldSet();
//
//			// finish picking
//			for (int i = 0; i < COL * ROW; ++i)
//			{
//				m_arrItemSpace[i]->Do_OtherIcon_Picking(false);
//			}
//
//			for (int i = 0; i < IT_END; ++i)
//			{
//				m_arrEquipSpace[i]->Do_OtherIcon_Picking(false);
//			}
//
//		}
//		else if (MouseKeyDown(DIM_RB))
//		{
//			//sound play
//			CSoundMgr::GetInstance()->PlaySoundRandom({
//				L"inven_equip-001.ogg",
//				L"inven_equip-002.ogg",
//				L"inven_equip-003.ogg",
//				L"inven_equip-004.ogg" }, vSoundPos);
//			// Change Equip item
//			ITEMTYPE eType = pEquipItem->GetItemType();
//			
//			// in itemspace
//			if (!bEquipState)
//			{
//				if (eType == IT_MELEE || eType == IT_RANGE)
//				{
//					m_arrItem[index] = m_arrEquip[eType];
//					m_arrEquip[eType] = pEquipItem;
//			
//				}
//				//����
//				else if(eType == IT_END)
//				{
//					for (_int i = IT_LEGACY1; i <= IT_LEGACY3; ++i)
//					{
//						if (m_arrEquip[i] == nullptr)
//						{
//							m_arrItem[index] = m_arrEquip[i];
//							m_arrEquip[i] = pEquipItem;
//							return;
//						}
//					}
//
//					// full space
//					m_arrItem[index] = m_arrEquip[IT_LEGACY3];
//					m_arrEquip[IT_LEGACY3] = pEquipItem;
//
//				}
//				else if (eType == IT_RUNE)
//				{
//					// �������� Ŭ�� �Ǿ� �־���ϰ�, ������ Ÿ�Կ� �¾ƾ� ��.
//					CWeapon* pWeapon = dynamic_cast<CWeapon*>(pCurClickItem);
//					CRune* pRune = dynamic_cast<CRune*>(pEquipItem);
//
//					if (pWeapon && pRune && pWeapon->SetRune(pRune))
//					{
//						m_arrItem[index] = m_arrEquip[IT_RUNE];
//						m_arrEquip[IT_RUNE] = pEquipItem;
//					}
//				}
//			}
//			// in equipspace
//			else
//			{
//				for (_int i = 0; i < COL * ROW; ++i)
//				{
//					if (m_arrItem[i] == nullptr)
//					{
//						m_arrEquip[index] = m_arrItem[i];
//						m_arrItem[i] = pEquipItem;
//						break;
//					}
//				}
//			}
//		}
//		else
//		{	
//			pItemSpaceUI->SetMouseCollFrame();
//		}
//	}
//	else if (pCurCollsionIconUI == pItemSpaceUI)
//	{
//		pCurCollsionIconUI = nullptr;
//	}
//}


