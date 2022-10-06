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
		ANIM_ATTACK2, // ���⿡�� �����ͼ� �����ϰ� ����
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

	virtual void MeleeAttack();
	void StateChange();

	// controller �Է��Լ�
	void SetMoveDir(_float fX, _float fZ);
	void MeleeAttackPress(bool bOn) { m_bMeleeAttack = bOn; }
	void RollPress() { m_bRoll = true; }
	//

	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

private:
	void RotateToCursor();
	void RotateToMove();


protected:
	CStatComponent* m_pStat;

	PlayerState m_eState = STATE_END;
	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	_float m_fSpeed; // �ӵ�
	_float m_fRollSpeed; // ������ �ӵ�

	_uint m_iAttackCnt = 0; // �޺� ��ȣ

	_vec3 m_vMoveDirNormal{0.f, 0.f, 0.f}; // �̵� ����

	// true : PlayAnimationOnce ��� ���� ����(���� �ִϸ��̼� ���� ����), false: �ٸ� �ִϸ��̼� ������
	_bool m_bCanPlayAnim = true; // ���� �������� �ִϸ��̼� ���� �ִϸ��̼� ���� ���� ����

	_bool m_bRoll = false; // controller �Է�
	_bool m_bMeleeAttack = false; // controller �Է�
	_bool m_bRangeAttack = false; // controller �Է�
	_bool m_bMove = false; // controller �Է�

	_bool m_bApplyMeleeAttack = false;
	_bool m_bApplyMeleeAttackNext = false;
};



