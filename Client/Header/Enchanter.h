#pragma once
#include "Monster.h"

class CController;

class CEnchanter : public CMonster
{
private:
	enum CEnchanterState
	{
		IDLE,
		WALK,
		ATTACK,
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
		ANIM_END
	};

private:
	explicit CEnchanter(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEnchanter(const CMonster& rhs);
	virtual ~CEnchanter() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual void AnimationEvent(const string& strEvent) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	static CEnchanter* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

	virtual void StateChange();

	// controller ���� �Լ�
	void WalkToTarget(const _vec3& vTargetPos) { m_bMove = true; m_vTargetPos = vTargetPos; }
	void AttackPress(const _vec3& vTargetPos)
	{
		m_bAttack = true;
		m_vTargetPos = vTargetPos;
	}
	void OutOfRange() { m_bMove = false; }

private:
	CEnchanterState m_eState = STATE_END;
	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	// true : PlayAnimationOnce ��� ���� ����(���� �ִϸ��̼� ���� ����), false: �ٸ� �ִϸ��̼� ������
	_bool m_bCanPlayAnim = true; // ���� �������� �ִϸ��̼� ���� �ִϸ��̼� ���� ���� ����

	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero; // controller �Է�

	_bool m_bMove = false; // controller �Է�
	_bool m_bAttack = false; // controller �Է�
	_bool m_bAttackFire = false; // anim event �Է�
};
