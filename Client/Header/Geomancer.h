#pragma once
#include "Monster.h"

class CController;

class CGeomancer : public CMonster
{
	enum LoopAnim
	{
		WALK,
		IDLE,
		DEAD,
		LA_END
	};

	enum OnceAnim
	{
		ATTACK,
		OA_END
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

	void SetIdle();
	void SetAttack(CPlayer* pPlayer);
	void Run(CPlayer* pPlayer);

	bool CanAttack() { return m_fCurAttackCoolTime >= m_fAttackCoolTime; }


private:
	array<CubeAnimFrame, LA_END> m_arrLoopAnim;
	array<CubeAnimFrame, OA_END> m_arrOnceAnim;

	_float m_fCurAttackCoolTime = 0.f;
	_float m_fAttackCoolTime = 4.f;

	_vec3 m_vMoveDir = CGameUtilMgr::s_vZero;

	_vec3 m_vAttackDest;
	_bool m_bAttack = false;
};
