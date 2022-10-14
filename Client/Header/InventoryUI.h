#pragma once
#include "UI.h"

/*------------------------
* BackGround
------------------------*/
class CInventoryUI :
	public CUI
{
protected:
	explicit CInventoryUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInventoryUI();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object() override;
	virtual void Free() override;
	static CInventoryUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum);

	void Close() { m_bClose = true; }
	void Open() { m_bClose = false; }
	_bool IsClosen() { return m_bClose; }
	void SetUITexture(_int TexIndex) { m_iTexNum = TexIndex; }

protected:
	_bool m_bClose = true;

private:
	_int m_iTexNum = 0;
	_matrix m_matIonUIWorld;
	_bool m_bWorldSet = false;
	wstring wstrItemName[3]{};

	void SetTexture(_int iTexNum);
};

/*------------------------
* ItemSpace
------------------------*/
enum SLOT_TYPE { SLOT_BASE, SLOT_WEAPON, SLOT_LEGACY, SLOT_TYPE_END };
class CItemSpaceUI : public CInventoryUI
{
private:
	
	explicit CItemSpaceUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItemSpaceUI();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object() override;
	virtual void Free() override;
	static CItemSpaceUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum);
	void SetRenderType(SLOT_TYPE eType) { m_eRenderType = eType; }

	void SetUITexture(_uint iTexNum)
	{
		m_iIconTexNum = iTexNum;
	}

	void SetMouseCollFrame() { m_bMouseCollFrame = true; }
	void SetCurClickFrame(_bool bCurClickFrame) { m_bCurClickFrame = bCurClickFrame; }
	_matrix& GetIconWorld() { return m_matIconWorld; }

	void Do_OtherIcon_Picking(_bool bPick) { m_bPick = bPick; }
	_bool GetPick() { return m_bPick; }
	void InitIconWorldSet() { m_bWorldSet = false; }

private:
	

	_uint m_iBaseSlot = 1;
	_uint m_iWeaponSlot = 2;
	_uint m_iLegacySlot = 6;
	SLOT_TYPE m_eRenderType = SLOT_TYPE_END;

	_matrix m_matIconWorld;
	_uint m_iIconTexNum = 0;

	_matrix m_matMouseCollFrameWorld;
	_matrix m_matCurClickFrameWorld;
	

	_bool m_bWorldSet = false;
	_bool m_MouseCollFrameWorldSet = false;
	_bool m_CurClickFrameWorldSet = false;

	_bool m_bMouseCollFrame = false;
	_bool m_bCurClickFrame = false;

	// 아이템을 선택했을 때 다른 아이템이 마우스를 따라가지 않도록 한다.
	_bool m_bPick = false;
	
};

