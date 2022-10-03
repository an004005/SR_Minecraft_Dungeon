#pragma once
#include "SkeletalCube.h"

class CController;

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
		DOTGE,
		TELEPORT,
		RESCUE,

		OA_END
	};


public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Free() override;
	virtual void AnimationEvent(const string& strEvent) override;

	virtual void CheckCursor();
	virtual void SetMove(const _vec3& vPos);
	virtual void SetTarget(CSkeletalCube* pTarget);
	virtual void Attack();

	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

	_vec3 PickingOnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom);



protected:
	CController* m_pController;

	array<CubeAnimFrame, LA_END> m_arrLoopAnim;
	array<CubeAnimFrame, OA_END> m_arrOnceAnim;

	_vec3 m_vDest;
	CSkeletalCube* m_pTarget = nullptr;

	_float m_fVelocity;

	_uint m_iAttackCnt = 0;
};


