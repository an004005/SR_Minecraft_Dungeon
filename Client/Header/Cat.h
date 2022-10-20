#pragma once

#include "SkeletalCube.h"

class CCat : public CSkeletalCube
{
private:
	explicit CCat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCat(const CCat& rhs);
	virtual ~CCat() override;

	enum CCatState
	{
		IDLE,
		WALK,
		STATE_END
	};

	enum Animation
	{
		ANIM_WALK,
		ANIM_IDLE,
		ANIM_END
	};

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Free() override;
	static CCat* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

	virtual void StateChange();

	void	WalkToTarger(const _vec3& vTargetPos) { m_bMove = true; m_vTargetPos = vTargetPos; }

	void	RotateToTargetPos(const _vec3& vTargetPos, bool bReverse = false);

private:

	CCatState m_eState = STATE_END;
	array<CubeAnimFrame, ANIM_END> m_arrAnim;

	_float		m_fSpeed = 0.f;
	// true : PlayAnimationOnce ��� ���� ����(���� �ִϸ��̼� ���� ����), false: �ٸ� �ִϸ��̼� ������

	// anim event �Է�
	_bool m_bCanPlayAnim = true; // ���� �������� �ִϸ��̼� ���� �ִϸ��̼� ���� ���� ����

	_bool m_bMove = false;
	_vec3 m_vTargetPos = CGameUtilMgr::s_vZero; // controller �Է�
	
	_float		m_fPlayerRange = 10.f;
	_float		m_fMoveDist = 2.f;

	_vec3 vEndPos = { 5.f,9.f,27.f };
	_bool m_bGo = true;

	CTerrainCubeMap* m_pCubeMap = nullptr;

};

