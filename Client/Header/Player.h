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
	enum LoopAnim
	{
		WALK,
		IDLE,
		DEAD,
		LA_END
	};
	enum OnceAnim
	{
		ATTACK1,
		ATTACK2, // 무기에서 가져와서 실행하게 구현
		ATTACK3,
		ROLL,
		TELEPORT,
		RESCUE,

		OA_END
	};


public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	virtual void AnimationEvent(const string& strEvent) override;

	virtual void SetMove(_float fX, _float fZ);
	void MeleeAttackOn(bool bOn) { m_bMeleeAttack = bOn; }
	virtual void MeleeAttack();
	virtual void Roll();


	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

private:
	void RotateToCursor();


protected:
	CController* m_pController;
	CStatComponent* m_pStat;

	array<CubeAnimFrame, LA_END> m_arrLoopAnim;
	array<CubeAnimFrame, OA_END> m_arrOnceAnim;

	_float m_fVelocity;
	_float m_fRollSpeed;

	_uint m_iAttackCnt = 0;

	_vec3 m_vMoveDir{0.f, 0.f, 0.f};
	_vec3 m_vMoveDirNormal{0.f, 0.f, 0.f};


	_bool m_bAction = false;
	_bool m_bRoll = false;
	_bool m_bMeleeAttack = false;

	_bool m_bApplyMeleeAttack = false;
	_bool m_bApplyMeleeAttackNext = false;


};



