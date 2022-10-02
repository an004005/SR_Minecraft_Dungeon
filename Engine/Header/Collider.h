#pragma once
#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;
class CCollisionCom;

struct ENGINE_DLL CollisionGrid
{
	vector<CCollisionCom*> dynamicList;
	list<pair<_vec3, _float>> staticList;
};

class ENGINE_DLL CCollider : public CBase
{
	DECLARE_SINGLETON(CCollider)

private:
	explicit CCollider();
	virtual ~CCollider();

public:
	void Ready_Collider();
	void Add_CollisionCom(CCollisionCom* pCollision);
	void Add_StaticCollision(const _vec3& vCenter, _float fRadius);
	// run at only late update
	void GetOverlappedObject(OUT list<CGameObject*>& objList, const _vec3& vPos, _float fRadius);
	void Check_Blocking();
	void Clear_Dynamic();
	void Clear_All();

	virtual void Free() override;

private:
	static bool IsCollided(const _vec3& vPos1, _float fRadius1, const _vec3& vPos2, _float fRadius2);

private:
	vector<vector<CollisionGrid>> m_vecGrid;

	_float m_fGridCX = 0;
	_float m_fGridCZ = 0;
};

END