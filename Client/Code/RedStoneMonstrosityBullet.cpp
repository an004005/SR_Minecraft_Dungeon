#include "stdafx.h"
#include "..\Header\RedStoneMonstrosityBullet.h"
#include "Transform.h"
#include "AbstFactory.h"
#include "TerrainCubeMap.h"
#include "RedStoneMonstrosity.h"
#include "Player.h"
#include "StatComponent.h"

CRedStoneMonstrosityBullet::CRedStoneMonstrosityBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CRedStoneMonstrosityBullet::~CRedStoneMonstrosityBullet()
{
}

HRESULT CRedStoneMonstrosityBullet::Ready_Object()
{
	m_pTransCom = Add_Component<Engine::CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);

	CCollisionCom* m_pCollCom = Add_Component<CCollisionCom>(L"Proto_CollisionCom", L"Proto_CollisionCom", ID_DYNAMIC);
	m_pCollCom->SetOwner(this);
	m_pCollCom->SetOwnerTransform(m_pTransCom);
	m_pCollCom->SetRadius(1.5f);
	m_pCollCom->SetCollOffset(_vec3{ 0.f, 0.f, 0.f });
	m_pCollCom->SetCollType(COLL_ENEMY_BULLET);

	return S_OK;
}

_int CRedStoneMonstrosityBullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	//플레이어 위치로 포물선으로 날아가게 함

	_vec3 vTargetPos = Get_GameObject<CRedStoneMonstrosity>(LAYER_ENEMY, L"RedStoneMonstrosity")->Get_TargetPos();

	_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
	_vec3 vDist = vTargetPos - vPos;
	D3DXVec3Normalize(&vDist, &vDist);

	m_pTransCom->m_vInfo[INFO_POS] += vDist * 7.f * fTimeDelta;

	return 0;
}

void CRedStoneMonstrosityBullet::LateUpdate_Object()
{
	//바닥에 부딫히면 터지면 사망.
	CTerrainCubeMap* pCubeMap = Get_GameObject<CTerrainCubeMap>(LAYER_ENV, L"TerrainCubeMap");
	
	_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
	
	if (pCubeMap->GetHeight(vPos.x, vPos.z) >= vPos.y)
		m_bDead = true;

	if (m_bDead)
	{
		_vec3& vPos = m_pTransCom->m_vInfo[INFO_POS];

		set<CGameObject*> setObj;
		Engine::GetOverlappedObject(setObj, vPos, 3.5f);

		for (auto& obj : setObj)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
			{
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
					->TakeDamage(1, vPos, this, DT_KNOCK_BACK);
				IM_LOG("damage");
				break;
			}
		}

		DEBUG_SPHERE(m_pTransCom->m_vInfo[INFO_POS], 3.5f, 1.f);

		return;
	}

	DEBUG_SPHERE(m_pTransCom->m_vInfo[INFO_POS], 1.f, 0.1f);

}

void CRedStoneMonstrosityBullet::Render_Object()
{
	//구체가 랜더되게 함.
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

CRedStoneMonstrosityBullet * CRedStoneMonstrosityBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRedStoneMonstrosityBullet* pInstance = new CRedStoneMonstrosityBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRedStoneMonstrosityBullet::Free()
{
	CGameObject::Free();
}
