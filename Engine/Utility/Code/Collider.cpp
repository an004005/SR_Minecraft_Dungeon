#include "..\..\Header\Collider.h"
#include "CollisionCom.h"
#include "Transform.h"
#include "GameObject.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollider)

_float fdx[8]{};
_float fdz[8]{};

CCollider::CCollider()
{
	Ready_Collider();
}


CCollider::~CCollider()
{
	Free();
}

void CCollider::Ready_Collider()
{
	m_vecGrid.resize(COLL_GRID_Z);
	for (auto& e : m_vecGrid)
		e.resize(COLL_GRID_X);

	m_fGridCZ = (_float)VTXCNTZ / COLL_GRID_Z;
	m_fGridCX = (_float)VTXCNTX / COLL_GRID_X;
}

void CCollider::Add_CollisionCom(CCollisionCom* pCollision)
{
	NULL_CHECK(pCollision);

	const _vec3 vPos = pCollision->GetTransform()->m_vInfo[INFO_POS];
	const _float fRadius = pCollision->GetRadius();

	_int iX_start = (_int)(vPos.x - fRadius / m_fGridCX);
	_int iZ_start = (_int)(vPos.z - fRadius / m_fGridCZ);
	if (iX_start < 0) iX_start = 0;
	if (iZ_start < 0) iZ_start = 0;

	_int iX_end = (_int)(vPos.x + fRadius / m_fGridCX);
	_int iZ_end = (_int)(vPos.z + fRadius / m_fGridCZ);
	if (iX_end >= COLL_GRID_X) iX_end = COLL_GRID_X - 1;
	if (iZ_end >= COLL_GRID_X) iZ_end = COLL_GRID_Z - 1;

	for (int i = iZ_start; i <= iZ_end; ++i)
	{
		for (int j = iX_start; j <= iX_end; ++j)
		{
			m_vecGrid[i][j].dynamicList.push_back(pCollision);
		}
	}
}

void CCollider::Add_StaticCollision(const _vec3& vCenter, _float fRadius)
{

	_int iX_start = (_int)(vCenter.x - fRadius / m_fGridCX);
	_int iZ_start = (_int)(vCenter.z - fRadius / m_fGridCZ);
	if (iX_start < 0) iX_start = 0;
	if (iZ_start < 0) iZ_start = 0;

	_int iX_end = (_int)(vCenter.x + fRadius / m_fGridCX);
	_int iZ_end = (_int)(vCenter.z + fRadius / m_fGridCZ);
	if (iX_end >= COLL_GRID_X) iX_end = COLL_GRID_X - 1;
	if (iZ_end >= COLL_GRID_X) iZ_end = COLL_GRID_Z - 1;

	for (int i = iZ_start; i <= iZ_end; ++i)
	{
		for (int j = iX_start; j <= iX_end; ++j)
		{
			m_vecGrid[i][j].staticList.push_back({vCenter, fRadius});
		}
	}
}

void CCollider::GetOverlappedObject(vector<CGameObject*>& vecObj, const _vec3& vPos, _float fRadius)
{
}

void CCollider::Check_Blocking()
{
	for(auto& vecRow : m_vecGrid)
	{
		for (auto& cell : vecRow)
		{
			for(auto& coll_1 : cell.dynamicList)
			{
				for(auto& coll_2 : cell.dynamicList)
				{
					if (coll_1->GetOwner() == coll_2->GetOwner()) continue;
					if (IsCollided(
						coll_1->GetTransform()->m_vInfo[INFO_POS], coll_1->GetRadius(),
						coll_2->GetTransform()->m_vInfo[INFO_POS], coll_2->GetRadius()))
					{
						coll_1->CollisionDynamic(coll_2);
						coll_2->CollisionDynamic(coll_1);
					}
				}

				for (auto& coll_static : cell.staticList)
				{
					if (IsCollided(
						coll_1->GetTransform()->m_vInfo[INFO_POS], coll_1->GetRadius(),
						coll_static.first, coll_static.second))
					{
						coll_1->CollisionStatic(coll_static.first, coll_static.second);
					}
				}
			}
		}
	}
}

void CCollider::Clear_Dynamic()
{
	for(auto& vecRow : m_vecGrid)
	{
		for (auto& cell : vecRow)
		{
			cell.dynamicList.clear();
		}
	}
}

void CCollider::Clear_All()
{
	for(auto& vecRow : m_vecGrid)
	{
		for (auto& cell : vecRow)
		{
			cell.dynamicList.clear();
			cell.staticList.clear();
		}
	}
}

void CCollider::Free()
{
	Clear_All();
}

bool CCollider::IsCollided(const _vec3& vPos1, _float fRadius1, const _vec3& vPos2, _float fRadius2)
{
	const _vec3 vDiff = vPos1 - vPos2;
	return D3DXVec3Length(&vDiff) <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2);
}	
