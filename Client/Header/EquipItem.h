#pragma once
#include "Item.h"
#include "SkeletalCube.h"

class CEquipItem :
	public CItem
{
protected:

	enum Animation
	{
		ANIM_WALK,
		ANIM_IDLE,
		ANIM_DEAD,
		ANIM_ATTACK1,
		ANIM_ATTACK2,
		ANIM_ATTACK3,
		ANIM_RANGE_ATTACK,
		ANIM_LEGACY1,
		ANIM_LEGACY2,
		ANIM_LEGACY3,
		ANIM_ROLL,
		ANIM_TELEPORT,
		ANIM_RESCUE,
		ANIM_END
	};

	explicit CEquipItem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEquipItem();

public:
	//gameobj function
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;

	// equipitem function
	virtual void Equipment(SkeletalPart* pSkeletalPart) {}
	virtual void Collision() {}
	//���� �÷��̾� �Ѹ��϶��� �����ؼ� ����. �����Ǹ� ����
	virtual _int Attack() { return 0; }
	virtual void Use() { m_bUse = true; }
	_float GetCoolTime() const { return m_fCurCoolTime / m_fCoolTime; }

	
public:
	const array<CubeAnimFrame, ANIM_END>& SetarrAnim(){ return m_arrAnim; }
	ITEMTYPE GetItemType() { return m_eItemType; }
	_int GetUITexNum() const { return m_iUItexNum; }

protected:
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CVoxelTex*	m_pBufferCom = nullptr;
	

	array<CubeAnimFrame, ANIM_END> m_arrAnim{};
	_uint m_iAttackCnt; // �޺� ��ȣ
	_bool m_bUse;
	ITEMTYPE	m_eItemType;
	_uint m_iUItexNum;

	// ���� ��Ÿ��(Legacy�� ��Ӵܰ� �߰��߾�� �ߴµ�....)
	_float m_fCoolTime = 1.f;
	_float m_fCurCoolTime = 1.f; 
};

