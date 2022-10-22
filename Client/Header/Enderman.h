#pragma once
#include "Monster.h"

class CBossHPUI;

class CEnderman :
	public CMonster
{
private:
	enum CEndermanState
	{
		IDLE,
		WALK,
		CHOP,
		SMASH,
		CLOCKING,
		ARMATTACK,
		STUN,
		DEAD,
		STATE_END
	};

	enum Animation
	{
		ANIM_WALK,
		ANIM_IDLE,
		ANIM_DEAD,
		ANIM_CHOP,
		ANIM_SMASH,
		ANIM_ARMATTACK,
		ANIM_END
	};

private:
	explicit CEnderman(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEnderman(const CMonster& rhs);
	virtual ~CEnderman();

public:
	virtual HRESULT Ready_Object() override;
	virtual void AnimationEvent(const string& strEvent) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	static CEnderman* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath, _bool bRemote = false);

	virtual void StateChange();

	// controller ���� �Լ�
	void WalkToTarget() { m_bMove = true;}
	void DoChop() { m_bChop = true; }
	void DoSmash() { m_bSmash = true; }
	void DoIdle() { m_bMove = false; }
	void DoArmAttack() { m_bArmAttack = true; }
	void DoClocking(_vec3 vLook)
	{
		m_bClocking = true;
		m_fCurClockingCollTime = 0.f;
		m_vBeforTargetLook = vLook;
	}
	void SetTarget(_vec3 vTargetPos){ m_vTargetPos = vTargetPos; }

	//

private:
	CEndermanState m_eState = STATE_END;
	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	// true : PlayAnimationOnce ��� ���� ����(���� �ִϸ��̼� ���� ����), false: �ٸ� �ִϸ��̼� ������
	_bool m_bCanPlayAnim = true; // ���� �������� �ִϸ��̼� ���� �ִϸ��̼� ���� ���� ����

	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero; // controller �Է�

	_bool m_bMove = false; // controller �Է�
	_bool m_bChop = false; // controller �Է�
	_bool m_bSmash = false; // controller �Է�
	_bool m_bAttackFire = false; // anim event �Է�
	_bool m_bClocking = false; // anim event �Է�
	_bool m_bArmAttack = false; // anim event �Է�
	_bool m_bArmAttakcFire = false;

	_float m_fClockingCollTime = 2.5f;
	_float m_fCurClockingCollTime = 0.f;

	_vec3 m_vBeforTargetLook;
	CBossHPUI* m_pBossHPUI = nullptr;
};

