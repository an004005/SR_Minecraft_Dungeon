#pragma once
#include "Monster.h"

class CController;

class CGeomancer : public CMonster
{
private:
	enum GeomancerState
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
	explicit CGeomancer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGeomancer(const CMonster& rhs);
	virtual ~CGeomancer() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual void AnimationEvent(const string& strEvent) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	static CGeomancer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

	virtual void StateChange();

	// controller ���� �Լ�
	void SetMoveDir(_float fX, _float fZ);
	void Run(const _vec3& vTargetPos) {m_bMove = true; m_vTargetPos = vTargetPos;}
	void AttackPress(const _vec3& vTargetPos)
	{
		if (m_fAttackCoolTime <= m_fCurAttackCoolTime) // cool time done
		{
			m_bAttack = true;
			m_vTargetPos = vTargetPos;
		}
	}
	//

private:
	GeomancerState m_eState = STATE_END;
	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	_float m_fCurAttackCoolTime = 0.f;
	_float m_fAttackCoolTime = 3.f;

	_float m_fAttackRange = 15.f; // �����Ÿ�

	// true : PlayAnimationOnce ��� ���� ����(���� �ִϸ��̼� ���� ����), false: �ٸ� �ִϸ��̼� ������
	_bool m_bCanPlayAnim = true; // ���� �������� �ִϸ��̼� ���� �ִϸ��̼� ���� ���� ����

	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero; // controller �Է�

	_bool m_bMove = false; // controller �Է�
	_bool m_bAttack = false; // controller �Է�
	_bool m_bAttackFire = false; // anim event �Է�
};
