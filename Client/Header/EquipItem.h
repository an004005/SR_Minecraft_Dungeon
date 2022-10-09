#pragma once
#include "Item.h"
#include "SkeletalCube.h"

class CTransform;

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



class CEquipItem :
	public CItem
{
public:
	explicit CEquipItem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEquipItem();

public:
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	static CEquipItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;
	virtual void Equipment(SkeletalPart* pSkeletalPart) {}
	virtual void Collision() {}
	//현제 플레이어 한명일때만 가정해서 구현. 서버되면 수정
	virtual _int Attack() { return 0; }


public:
	const array<CubeAnimFrame, ANIM_END>& SetarrAnim(){ return m_arrAnim; }
	ITEMTYPE GetItemType() { return m_eItemType; }

protected:
	Engine::CTexture*	m_pTextureCom = nullptr;
	Engine::CVoxelTex*	m_pBufferCom = nullptr;
	Engine::CTransform*	m_pTransCom = nullptr;
	
	array<CubeAnimFrame, ANIM_END> m_arrAnim{};
	_uint m_iAttackCnt = 0; // 콤보 번호
	ITEMTYPE	m_eItemType;
	
};

