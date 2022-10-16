#pragma once
#include "Monster.h"
class CController;

class CSaton :	public CMonster
{
private:
	enum CStatonState
	{
		INTRO,
		IDLE,
		FIRSTATTACK,
		SATON_BIRD,
		SATON_GRAP,
		SATON_SYMBOL,
		SATON_FASCINATE,
		DEAD,
		STATE_END
	};

private:
	explicit CSaton(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSaton(const CMonster& rhs);
	virtual ~CSaton()override;

public:
	virtual HRESULT Ready_Object() override;
	virtual void AnimationEvent(const string& strEvent) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;

	static CSaton* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

	virtual void StateChange();

	void FirstAttack(const _vec3& vTargetPos)
	{
		m_bFirstHammerAttack = true;
		m_vTargetPos = vTargetPos;
	}
	
	void SatonGrap(const _vec3& vTargetPos)
	{
		m_bSatonGrap = true;
		m_vTargetPos = vTargetPos;
	}

	void SatonBird(const _vec3& vTargetPos)
	{
		m_bSatonBird = true;
		m_vTargetPos = vTargetPos;
	}

	void SatonSymbolAnim(const _vec3& vTargetPos)
	{
		m_bSatonSymbolAnim = true;
		m_vTargetPos = vTargetPos;
	}

	void SatonFascinate(const _vec3& vTargetPos)
	{
		m_bSatonFascinate = true;
		m_vTargetPos = vTargetPos;
	}

	// controller 조종 함수
	_vec3 Get_TargetPos() { return m_vTargetPos; }
	void WalkToTarget(const _vec3& vTargetPos) { m_vTargetPos = vTargetPos; }


	//m_bMove = true;
private:
	CStatonState m_eState = STATE_END;
	array<CubeAnimFrame, STATE_END> m_arrAnim;

	void	SetOff()
	{
		m_bFirstHammerAttack = false;
		m_bSatonGrap = false;
		m_bSatonBird = false;
		m_bSatonSymbolAnim = false;
		m_bSatonFascinate = false;
	}

	// true : PlayAnimationOnce 사용 가능 상태(동작 애니메이션 실행 가능), false: 다른 애니메이션 실행중
	_bool m_bCanPlayAnim = true; // 현재 실행중인 애니메이션 끊고 애니메이션 실행 가능 여부

	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero; // controller 입력

	_bool m_bFirstHammerAttack = false; // controller 입력
	_bool m_bSatonGrap = false;
	_bool m_bSatonBird = false;
	_bool m_bSatonSymbolAnim = false;
	_bool m_bSatonFascinate = false;

	_float m_fTime;
	_float m_fCurTime;
	// _bool m_bHammerReady = false;
};

