#pragma once
#include "GameObject.h"

class CEquipItem;
class SkeletalCube;
class CConsumeItem;

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

	// �κ��丮�� ������ �ֱ�
	void Put(CEquipItem* pItem);
	void Put(CConsumeItem* pItem);
	void TakeOut(CEquipItem* pItem);

	//������ ����(�ӽ÷� ���� �ֱٿ� ���� ������ �����ǰ� ��)
	void Equip_Item(SkeletalPart* pSkeletalPart, ITEMTYPE eIT);
	CEquipItem* CurWeapon(ITEMTYPE eIT);

private:
	vector<CEquipItem*> m_vecItem;
	CEquipItem* m_pCurMelee = nullptr;
	CEquipItem* m_pCurRange = nullptr;
	CEquipItem* m_pCurLegacy = nullptr;
	_uint m_iEmerald = 0;
	_uint m_iArrow = 0;
};

