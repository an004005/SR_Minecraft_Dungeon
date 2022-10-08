#pragma once
#include "GameObject.h"

enum ArrowMoveType
{
	ARROW_MOVE_NORMAL,
	ARROW_MOVE_DANGLING,
	ARROW_MOVE_STUCK,
	ARROW_MOVE_END
};

class CStatComponent;
class CArrow : public CGameObject
{
private:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CArrow(const CArrow& rhs);
	virtual ~CArrow() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	static CArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fDamage, COLLISION_TYPE eCollType, ArrowType eType = ARROW_NORMAL);
	virtual void Free() override;

	void FireWork();

	// 아래 두 함수는 Collider 실행 중에(LateUpdate) 에 실행되기 때문에 GetOverlappedObject 사용가능
	void DynamicCallBack(CCollisionCom* pOther);
	void StaticCallBack(_vec3 vCenter, _float fRadius);
	//
private:
	CTransform* m_pTransform = nullptr;
	CCollisionCom* m_pCollCom = nullptr;
	ArrowMoveType m_eMoveType = ARROW_MOVE_NORMAL;

	ArrowType m_eType = ARROW_END;
	COLLISION_TYPE m_eCollType = COLL_END;

	_float m_fDamage;

	_float m_fpSpeed;
	_float m_fLife;

	// dangling
	CTransform* m_pParentTrans = nullptr;
	CStatComponent* m_pParentStat = nullptr;
	_matrix m_matDanglingLocal;
	// dangling

	// stucked
	_float m_fStuckTime = 1.f;
	_float m_bCurWaveTime = 0.f;
	_bool m_bWaveDir = true;
	// stucked
};

