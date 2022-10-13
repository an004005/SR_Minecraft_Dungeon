#pragma once
#include "Monster.h"
class CController;

class CKouku :public CMonster
{
private:
	enum CKoukuState
	{
		INTRO,
		IDLE,
		HAMMER_IN,
		HAMMER_OUT,
		HAMMER_ATTACK,
		HORROR_ATTACK,
		WALK,
		DEAD,
		STATE_END
	};

private:
	explicit CKouku(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CKouku(const CMonster& rhs);
	virtual ~CKouku()override;

public:
	virtual HRESULT Ready_Object() override;
	virtual void AnimationEvent(const string& strEvent) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;

	static CKouku* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

	virtual void StateChange();

	void DoubleHammer(const _vec3& vTargetPos)
	{
		m_bDoubleHammer = true;
		m_vTargetPos = vTargetPos;
	}

	void HorrorAttack(const _vec3& vTargetPos)
	{
		m_bHorrorAttack = true;
		m_bCountable = true;
		m_vTargetPos = vTargetPos;
	}

	// controller 조종 함수
	_vec3 Get_TargetPos() { return m_vTargetPos; }
	void WalkToTarget(const _vec3& vTargetPos) { m_vTargetPos = vTargetPos; }
	//m_bMove = true;
private:
	CKoukuState m_eState = STATE_END;
	array<CubeAnimFrame, STATE_END> m_arrAnim;

	void	SetOff()
	{
		m_bDoubleHammer = false;
		m_bHorrorAttack = false;
		m_bCountable = false;
	}
	// true : PlayAnimationOnce 사용 가능 상태(동작 애니메이션 실행 가능), false: 다른 애니메이션 실행중
	_bool m_bCanPlayAnim = true; // 현재 실행중인 애니메이션 끊고 애니메이션 실행 가능 여부

	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero; // controller 입력

	_bool m_bDoubleHammer = false; // controller 입력
	_bool m_bHorrorAttack = false; // controller 입력
	_bool m_bCountable = false; // contriller 입력
	_bool m_bMove = false;
	_float m_fTime;
	_float m_fCurTime;
	_bool m_bHammerReady = false;

};

