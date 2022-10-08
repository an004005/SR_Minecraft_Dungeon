#pragma once
#include "GameObject.h"

class CEquipItem;


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
	void TakeOut(CEquipItem* pItem);

private:
	vector<CEquipItem*> m_vecItem;

};

