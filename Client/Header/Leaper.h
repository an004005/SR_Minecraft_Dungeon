#pragma once
#include "Monster.h"
class CLeaper :
	public CMonster
{
private:

	enum class State : _int
	{
		IDLE,
		WALK,
		CHOP,
		ATTACK,
		JUMP,
		STUN,
		DEAD,
		STATE_END
	};

	enum Animation
	{
		ANIM_WALK,
		ANIM_IDLE,
		ANIM_DEAD,
		ANIM_ATTACK,
		ANIM_JUMP,
		ANIM_END
	};
	explicit CLeaper(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLeaper(const CMonster& rhs);
	virtual ~CLeaper();

public:
	virtual HRESULT Ready_Object() override;
	virtual void AnimationEvent(const string& strEvent) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	static CLeaper* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath, _bool bRemote = false);

	virtual void StateChange();
	void RotateToTargetSlow(const _vec3& vTargetPos);
	// controller ���� �Լ�
	void WalkToTarget() { m_bMove = true; }
	void DoAttack() { m_bAttack = true; }
	void DoJump() { m_bJump = true; }
	void DoIdle() { m_bMove = false; }
	void SetTarget(_vec3 vTargetPos) { m_vTargetPos = vTargetPos; }

	//

private:
	State m_eState = State::STATE_END;
	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	// true : PlayAnimationOnce ��� ���� ����(���� �ִϸ��̼� ���� ����), false: �ٸ� �ִϸ��̼� ������
	_bool m_bCanPlayAnim = true; // ���� �������� �ִϸ��̼� ���� �ִϸ��̼� ���� ���� ����

	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero; // controller �Է�

	_bool m_bMove = false; // controller �Է�
	_bool m_bAttack = false; // controller �Է�
	_bool m_bJump = false; // controller �Է�
	_bool m_bAttackFire = false; // anim event �Է�

	void JumpToPlayer(const _float& fTimeDelta);
	_float m_fJumpTime = 0.1f;
	_bool m_bLanding = true;
	_vec3 m_vJumpTargetPos;
	_bool m_bJumpFire = false;

};

