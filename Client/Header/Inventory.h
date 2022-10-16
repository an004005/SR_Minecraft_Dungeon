#pragma once
#include "EquipItem.h"
#include "GameObject.h"

#define COL 4
#define ROW 3

class CEquipItem;
class SkeletalCube;
class CConsumeItem;
class CItemSpaceUI;
class CInventoryUI;
class CItemUI;
class CPlayer;

class CInventory :
	public CGameObject
{
public:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInventory();

public:
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;

	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	// 인벤 창 생성
	void OpenInventory();
	// 인벤토리에 아이템 넣기
	void Put(CEquipItem* pItem);
	void Put(CConsumeItem* pItem);
	void TakeOut(CEquipItem* pItem);

	void UseArrow(_uint iArrowCnt) { m_iArrow -= iArrowCnt;}
	void SetOwner(CPlayer* pOwner) { m_pOwner = pOwner; NULL_CHECK(m_pOwner);}

	void AddDefaultItems();


	//아이템 장착
	void Equip_Item(SkeletalPart* pSkeletalPart, ITEMTYPE eIT);

	CEquipItem* CurWeapon(ITEMTYPE eIT);

	_uint GetEmeraldCnt() const { return m_iEmerald; }
	_uint GetArrowCnt() const { return m_iArrow; }

	_bool InputLock() { return m_bLock; }
	_float GetLegacyCoolTime(ITEMTYPE eType) const
	{
		if (m_arrEquip[eType] == nullptr)
			return 1.f;
		return m_arrEquip[eType]->GetCoolTime();
	}
	_uint GetItemUITexNum(ITEMTYPE eType) const
	{
		if (m_arrEquip[eType] == nullptr)
			return 0;
		return m_arrEquip[eType]->GetUITexNum();
	}

private:
	CPlayer* m_pOwner = nullptr;
	array<CEquipItem*, (COL * ROW)> m_arrItem{}; //  아이템 스페이스
	array<CEquipItem*, IT_END> m_arrEquip{};  //  장비 스페이스
	
	CInventoryUI* m_pInventoryUI = nullptr;

	array<CItemSpaceUI*, COL * ROW> m_arrItemSpace{};  // 아이템 스페이스 (빈 공간 UI)
	array<CItemSpaceUI*, IT_END> m_arrEquipSpace{};// 장비 스페이스(빈공간UI)

	//arrItem 이면 equipstate = flase, arrEquip이면 equipstate = true
	void MouseEvent(CItemSpaceUI* pItemSpaceUI, CEquipItem* pEquipItem, _int index, _bool bEquipState);
	_uint m_iEmerald = 0;
	_uint m_iArrow = 0;
	_bool m_bLock = false;

	CItemSpaceUI* pCurCollsionIconUI = nullptr;

};

