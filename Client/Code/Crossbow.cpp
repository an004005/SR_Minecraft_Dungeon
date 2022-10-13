#include "stdafx.h"
#include "..\Header\Crossbow.h"
#include "AbstFactory.h"
#include "Player.h"
#include "TerrainCubeMap.h"

CCrossbow::CCrossbow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipItem(pGraphicDev)
	
{
}


CCrossbow::~CCrossbow()
{
}

HRESULT CCrossbow::Ready_Object()
{
	m_pTransCom = Add_Component<Engine::CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pBufferCom = Add_Component<CVoxelTex>(L"Proto_VoxelTex_Crossbow", L"Proto_VoxelTex_Crossbow", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_WeaponTexture", L"Proto_WeaponTexture", ID_STATIC);

	//Attack_start
	m_arrAnim[ANIM_ATTACK1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_start.anim");
	//Attack_roof
	m_arrAnim[ANIM_ATTACK2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_loop.anim");
	//Attack_end
	m_arrAnim[ANIM_ATTACK3] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_end.anim");
	m_arrAnim[ANIM_ROLL] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/roll.anim");
	m_arrAnim[ANIM_RANGE_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_start.anim");

	m_eItemType = IT_RANGE;
	m_iUItexNum = 8;
	return S_OK;
}

_int CCrossbow::Update_Object(const _float & fTimeDelta)
{
	if (m_eItemState == IS_TAKE)
		return 0;

	_vec3& vPos = m_pTransCom->m_vInfo[INFO_POS];
	CTerrainCubeMap* pCubeMap = Get_GameObject<CTerrainCubeMap>(LAYER_ENV, L"TerrainCubeMap");
	_float fHeight = pCubeMap->GetHeight(vPos.x, vPos.z);

	Parabola(vPos, fHeight, fTimeDelta);

	CEquipItem::Update_Object(fTimeDelta);
	return 0;
}

void CCrossbow::LateUpdate_Object()
{
	CEquipItem::LateUpdate_Object();
}

void CCrossbow::Render_Object()
{
	if (m_eItemState == IS_TAKE)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(2);
	m_pBufferCom->Render_Buffer();
}

CCrossbow * CCrossbow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossbow* pInstance = new CCrossbow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCrossbow::Free()
{
	CEquipItem::Free();
}

_int CCrossbow::Attack()
{
	CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
	if (pPlayer == nullptr)
		return 0;

	pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK1]);

	// if (m_iAttackCnt == 0)
	// {
	//
	// }
	// else
	// {
	// 	pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK1]);
	// }

	// todo : 임시 설정, 이후 피킹한 몬스터 방향으로 쏘게 하기
	Engine::CTransform* pPlayerTrans = pPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC);
	const _vec3 vPos = pPlayerTrans->m_vInfo[INFO_POS] + _vec3{0.f, 1.3f, 0.f};
	_vec3 vLookAt;
	if (PickTargetEnemy(OUT vLookAt) == false)
	{
		vLookAt = vPos + pPlayerTrans->m_vInfo[INFO_LOOK];
	}
	
	if (m_bFireWork)
	{
		CBulletFactory::Create<CGameObject>("PlayerFireWorkArrow", L"PlayerFireWorkArrow", 10.f, vPos, vLookAt);
		m_bFireWork = false;
	}
	else
		CBulletFactory::Create<CGameObject>("PlayerNormalArrow", L"PlayerNormalArrow", 10.f, vPos, vLookAt);
	

	return m_iAttackCnt;
}

void CCrossbow::Equipment(SkeletalPart* pSkeletalPart)
{
	pSkeletalPart->pBuf = m_pBufferCom;
	pSkeletalPart->pTex = m_pTextureCom;
	pSkeletalPart->iTexIdx = 2;
}

_bool CCrossbow::PickTargetEnemy(_vec3& vLookAt)
{
	// https://gohen.tistory.com/79 참조(광선과 직선 교차판정)

	_vec3 vOrigin, vRayDir;
	_matrix matView, matProj;
	D3DVIEWPORT9 ViewPort;

	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	CGameUtilMgr::GetPickingRay(vOrigin, vRayDir, g_hWnd, matView, matProj, ViewPort);

	for (const auto& enemy : Engine::Get_Layer(LAYER_ENEMY)->Get_MapObject())
	{
		const auto pColl = enemy.second->Get_Component<CCollisionCom>(L"Proto_CollisionCom", ID_DYNAMIC);
		const _vec3 vSubject = vOrigin - pColl->GetCollPos();
		const _float fB = D3DXVec3Dot(&vRayDir, &vSubject);
		const _float fC = D3DXVec3Dot(&vSubject, &vSubject) - pColl->GetRadius();
		if (fB * fB  - fC >= 0.f)
		{
			vLookAt = pColl->GetCollPos();
			return true;
		}
	}
	return false;
}
