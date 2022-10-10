#pragma once
#include "SkeletalCube.h"



class CController;
class CStatComponent;
class CInventory;

class CPlayer : public CSkeletalCube
{
protected:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer() override;

private:
	enum PlayerState
	{
		IDLE,
		WALK,
		ATTACK,
		STUN,
		ROLL,
		LEGACY,
		DEAD,
		STATE_END
	};

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

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	virtual void AnimationEvent(const string& strEvent) override;

	virtual void AttackState();
	void StateChange();
	_vec3 GetInfo(INFOID eID) { return m_pRootPart->pTrans->m_vInfo[eID]; }

	// controller 입력함수
	void SetMoveDir(_float fX, _float fZ);
	void RangeAttackPress(bool bOn) { m_bRangeAttack = bOn; }
	void MeleeAttackPress(bool bOn) { m_bMeleeAttack = bOn; }
	void RollPress() { m_bRoll = true; }
	void Legacy1Press() { m_bLegacy1 = true; }
	void Legacy2Press() { m_bLegacy2 = true; }
	void Legacy3Press() { m_bLegacy3 = true; }
	void Legacy4Press();

	void UsePotion();
	//

	// UI용 함수
	_float GetRollCoolTime() const { return m_CurRollCoolTime / s_RollCoolTime; }
	_float GetPotionCoolTime() const { return m_CurPotionCoolTime / s_PotionCollTime; }
	//

	//아이템 변경(임시)
	void WeaponChange(ITEMTYPE eIT);

	CInventory* GetInventory() { return m_pInventory; }
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

private:
	void RotateToCursor();
	void RotateToMove();
	CInventory* m_pInventory = nullptr;

protected:
	SkeletalPart* m_pWeaponPart = nullptr;
	CStatComponent* m_pStat = nullptr;
	CCollisionCom* m_pColl = nullptr;

	PlayerState m_eState = STATE_END;
	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	const static _float s_RollCoolTime;
	_float m_CurRollCoolTime;
	const static _float s_PotionCollTime;
	_float m_CurPotionCoolTime;
	_float m_fSpeed; // 속도
	_float m_fRollSpeed; // 구르기 속도

	_int  m_iAttackCnt = 0;

	_vec3 m_vMoveDirNormal{0.f, 0.f, 0.f}; // 이동 방향

	// true : PlayAnimationOnce 사용 가능 상태(동작 애니메이션 실행 가능), false: 다른 애니메이션 실행중
	_bool m_bCanPlayAnim = true; // 현재 실행중인 애니메이션 끊고 애니메이션 실행 가능 여부

	_bool m_bRoll = false; // controller 입력
	_bool m_bMeleeAttack = false; // controller 입력
	_bool m_bRangeAttack = false; // controller 입력
	_bool m_bMove = false; // controller 입력

	_bool m_bLegacy1 = false;
	_bool m_bLegacy2 = false;
	_bool m_bLegacy3 = false;

	_bool m_bApplyMeleeAttack = false;
	_bool m_bApplyMeleeAttackNext = false;

	DWORD m_dwWalkDust;
	DWORD m_dwRollDust;

	// 원거리에서 근거리 무기로 다시 돌아올 때 1프레임동안 근거리 무기 위치가 이상한 현상을 막기 위함.
	_bool m_bDelay = false;

	
};



