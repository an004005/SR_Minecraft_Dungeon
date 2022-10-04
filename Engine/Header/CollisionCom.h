#pragma once
#include "Component.h"
#include <functional>

#include "Transform.h"

BEGIN(Engine)
	class CGameObject;
class CTransform;

enum COLLISION_TYPE
{
	COLL_PLAYER,
	COLL_ENEMY,
	COLL_PLAYER_BULLET,
	COLL_ENEMY_BULLET,
	COLL_END
};

class ENGINE_DLL CCollisionCom : public CComponent
{
private:
	explicit CCollisionCom();
	explicit CCollisionCom(const CCollisionCom& rhs);
	virtual ~CCollisionCom() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	virtual CComponent* Clone() override;
	virtual void Free() override;

	static CCollisionCom* Create();

	virtual void CollisionDynamic(CCollisionCom* pOther);
	virtual void CollisionStatic(const _vec3& vCenter, _float fRadius);

	// collision사용시 꼭 호출하기
	void SetOwner(CGameObject* pOwner) { m_pOwner = pOwner; }
	void SetOwnerTransform(CTransform* pOwnerTrans) { m_pOwnerTrans = pOwnerTrans; }
	void SetRadius(_float fRadius) { m_fRadius = fRadius; }
	void SetCollType(COLLISION_TYPE eType) { m_eType = eType; }
	//

	CGameObject* GetOwner() const { return m_pOwner; }
	CTransform* GetTransform() const { return m_pOwnerTrans; }
	_float GetRadius() const { return m_fRadius; }
	void SetCollOffset(const _vec3& vOffset) { m_vOffset = vOffset; }
	_vec3 GetCollPos() const { return m_pOwnerTrans->m_vInfo[INFO_POS] + m_vOffset; }
	COLLISION_TYPE GetType() const { return m_eType; }
	

	void SetStop() { m_bStop = true; }

protected:
	CGameObject* m_pOwner = nullptr;
	CTransform* m_pOwnerTrans = nullptr;
	_vec3 m_vOffset{0.f, 0.f, 0.f};
	_float m_fRadius = 1.f;
	COLLISION_TYPE m_eType = COLL_END;

	std::function<void(CCollisionCom*)> m_pCollisionDynamic = nullptr;
	std::function<void(_vec3, _float)> m_pCollisionStatic = nullptr;

	bool m_bStop = false;
};
END
