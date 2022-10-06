#pragma once
#include "Monster.h"

class CController;

class CCreeper : public CMonster
{
private:
	enum CCreeperState
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
	explicit CCreeper(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCreeper(const CMonster& rhs);
	virtual ~CCreeper() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual void AnimationEvent(const string& strEvent) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	static CCreeper* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

	virtual void StateChange();

	// controller ���� �Լ�
	void WalkToTarget(const _vec3& vTargetPos) { m_bMove = true; m_vTargetPos = vTargetPos; }
	void AttackCountStart() { m_bAttackCount = true; }
	void Explosion(const _vec3& vTargetPos)
	{
		m_bAttack = true;
		m_bMove = false;
		m_vTargetPos = vTargetPos;
	}
	//

private:
	CCreeperState m_eState = STATE_END;
	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	// true : PlayAnimationOnce ��� ���� ����(���� �ִϸ��̼� ���� ����), false: �ٸ� �ִϸ��̼� ������

	// anim event �Է�
	_bool m_bAttackFire = false; 
	_bool m_bCanPlayAnim = true; // ���� �������� �ִϸ��̼� ���� �ִϸ��̼� ���� ���� ����

	// controller �Է�
	_bool m_bMove = false; 
	_bool m_bAttack = false; 
	_bool m_bAttackCount = false; 
	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero;
};
