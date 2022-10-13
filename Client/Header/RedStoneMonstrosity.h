#pragma once
#include "Monster.h"
#include <functional>

//struct Time77
//{
//public:
//	_float fTime = 0.f;
//
//private:
//	_float fCurTime = 0.f;
//	std::function<void()> callBack;
//
//public:
//	void Tick(_float fTimeDelta)
//	{
//		if (fCurTime > 0.f)
//			fCurTime -= fTimeDelta;
//		else
//		{
//			fCurTime = fTime;
//			callBack();
//		}
//	}
//
//	Time77(_float fTime, std::function<void()> callBack)
//		: fTime(fTime), callBack(callBack) {}
//};

class CController;

class CRedStoneMonstrosity : public CMonster
{
private:
	enum CRedStoneMonstrosityState
	{
		INTRO,
		WALK,
		CHOP,
		SPIT,
		SUMMON,
		WINDMILL,
		DEAD,
		STATE_END
	};


private:
	explicit CRedStoneMonstrosity(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRedStoneMonstrosity(const CMonster& rhs);
	virtual ~CRedStoneMonstrosity() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual void AnimationEvent(const string& strEvent) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	static CRedStoneMonstrosity* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

	virtual void StateChange();

	// controller 조종 함수
	_vec3 Get_TargetPos() { return m_vTargetPos; }
	void WalkToTarget(const _vec3& vTargetPos) { m_vTargetPos = vTargetPos; }
	void SetStart() { m_bStartPlay = true; }
	void Chop(const _vec3& vTargetPos)
	{
		m_bChop = true;
		m_vTargetPos = vTargetPos;
	}
	void Spit(const _vec3& vTargetPos)
	{
		m_bSpit = true;
		m_vTargetPos = vTargetPos;
	}
	void Summon(const _vec3& vTargetPos)
	{
		m_bSummon = true;
		m_vTargetPos = vTargetPos;
	}
	void Windmill(const _vec3& vTargetPos)
	{
		m_bWindmill = true;
		m_vTargetPos = vTargetPos;
	}



private:
	CRedStoneMonstrosityState m_eState = STATE_END;
	array<CubeAnimFrame, STATE_END> m_arrAnim;

	void	SetOff() 
	{
		m_bChop = false;
		m_bSpit = false;
		m_bSummon = false;
		m_bWindmill = false;
	}
	// true : PlayAnimationOnce 사용 가능 상태(동작 애니메이션 실행 가능), false: 다른 애니메이션 실행중
	_bool m_bCanPlayAnim = true; // 현재 실행중인 애니메이션 끊고 애니메이션 실행 가능 여부

	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero; // controller 입력

	_bool m_bChop = false; // controller 입력
	_bool m_bSpit = false; // controller 입력
	_bool m_bSummon = false; // controller 입력
	_bool m_bWindmill = false; // controller 입력


	_bool m_bChopFire = false; // anim event 입력
	_bool m_bWindmillFire = false; // anim event 입력
	_matrix m_matSommonWorld[6];

	ULONG m_dwWindTime = 0;

	_bool m_bStartPlay = false;
	_bool m_bIntroPlay = false;
};

