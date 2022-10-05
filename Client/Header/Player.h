#pragma once
#include "SkeletalCube.h"

class CController;
class CStatComponent;
class CPlayer : public CSkeletalCube
{
protected:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer() override;

private:
	enum LoopState
	{
		STATE_IDLE,
		STATE_WALK,
		STATE_STUN,
		STATE_END
	};

	enum Animation
	{
		ANIM_WALK,
		ANIM_IDLE,
		ANIM_DEAD,
		ANIM_ATTACK1,
		ANIM_ATTACK2, // 무기에서 가져와서 실행하게 구현
		ANIM_ATTACK3,
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

	virtual void SetMove(_float fX, _float fZ);
	void MeleeAttackOn(bool bOn) { m_bMeleeAttack = bOn; }
	virtual void MeleeAttack();
	virtual void Roll();


	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

private:
	void RotateToCursor();


protected:
	CController* m_pController;
	CStatComponent* m_pStat;

	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	_float m_fVelocity;
	_float m_fRollSpeed;

	_uint m_iAttackCnt = 0;

	_vec3 m_vMoveDir{0.f, 0.f, 0.f};
	_vec3 m_vMoveDirNormal{0.f, 0.f, 0.f};


	_bool m_bAction = false;
	_bool m_bRoll = false;
	_bool m_bMeleeAttack = false;

	_bool m_bApplyMeleeAttack = false;
	_bool m_bApplyMeleeAttackNext = false;


};



