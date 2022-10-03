#include "stdafx.h"
#include "Player.h"
#include "Controller.h"
#include "GameUtilMgr.h"
#include "TerrainCubeMap.h"

/*-----------------------
 *    CCharacter
 ----------------------*/
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev) : CSkeletalCube(pGraphicDev)
{
	m_vDest = {0.f, 0.f, 0.f};
	m_fVelocity = 5.f;
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object()
{
	CSkeletalCube::Ready_Object();
	m_pController = CPlayerController::Create();

	m_arrLoopAnim[IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_idle.anim");
	m_arrLoopAnim[WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_walk.anim");
	m_arrOnceAnim[ATTACK1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_attack_a.anim");
	// m_arrOnceAnim[ATTACK1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/JK/Intro.anim");
	m_arrOnceAnim[ATTACK2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_attack_b.anim");
	m_arrOnceAnim[ATTACK3] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_attack_c.anim");
	m_pIdleAnim = &m_arrLoopAnim[IDLE];

	CCollisionCom* pColl = Add_Component<CCollisionCom>(L"Proto_CollisionCom", L"Proto_CollisionCom", ID_DYNAMIC);
	pColl->SetOwner(this);
	pColl->SetOwnerTransform(m_pRootPart->pTrans);
	
	return S_OK;
}

_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	CSkeletalCube::Update_Object(fTimeDelta);
	m_pController->Update(this);

	if (m_vDest != _vec3(0.f, 0.f, 0.f))
	{
		_vec3& vCurPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
		_vec3  vToDest =  m_vDest - vCurPos;
		_float fLengthXZ = CGameUtilMgr::Vec3LenXZ(vToDest);

		D3DXVec3Normalize(&vToDest, &vToDest);

		vCurPos += vToDest * fTimeDelta * m_fVelocity;
		if (fLengthXZ < 0.1f)
		{
			m_vDest = _vec3(0.f, 0.f, 0.f);
			m_pCurAnim = &m_arrLoopAnim[IDLE];
		}
	}

	CTerrainCubeMap* pTerrain = Get_GameObject<CTerrainCubeMap>(LAYER_ENV, L"TerrainCubeMap");
	NULL_CHECK_RETURN(pTerrain, -1);
	_vec3& vPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
	vPos.y = pTerrain->GetHeight(vPos.x, vPos.z);

	//collision check
	//if (pTerrain->IsCollision(vPos.x, vPos.z))
	//	IM_LOG("COLLISION!!");

	return 0;
}

void CPlayer::Free()
{
	CSkeletalCube::Free();
	Safe_Release(m_pController);
}

void CPlayer::AnimationEvent(const string& strEvent)
{
	IM_LOG(strEvent.c_str());
	CSkeletalCube::AnimationEvent(strEvent);
}

void CPlayer::CheckCursor()
{
	const CTerrainTex* pTerrainBufferCom = Engine::Get_Component<CTerrainTex>(LAYER_ENV, L"Terrain", L"Proto_TerrainTexCom", ID_STATIC);
	const CTransform* pTerrainTransformCom = Engine::Get_Component<CTransform>(LAYER_ENV, L"Terrain", L"Proto_TransformCom", ID_DYNAMIC);

	m_vDest = PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
	if (m_vDest != _vec3(0.f, 0.f, 0.f))
	{
		_vec3& vCurPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
		_vec3  vToDest =  m_vDest - vCurPos;
		D3DXVec3Normalize(&vToDest, &vToDest);

		_vec2 v2Look{0.f, 1.f};
		_vec2 v2ToDest{vToDest.x, vToDest.z};
		_float fDot = D3DXVec2Dot(&v2Look, &v2ToDest);

		m_pRootPart->pTrans->m_vAngle.y = acosf(fDot);
		if (m_vDest.x < vCurPos.x)
			m_pRootPart->pTrans->m_vAngle.y *= -1.f;

		m_pCurAnim = &m_arrLoopAnim[WALK];
	}
}

void CPlayer::SetMove(const _vec3& vPos)
{
	m_pCurAnim = &m_arrLoopAnim[WALK];
	m_vDest = vPos;
}

void CPlayer::SetTarget(CSkeletalCube* pTarget)
{
	// m_pTarget = pTarget;
}

void CPlayer::Attack()
{
	if (m_iAttackCnt == 0)
	{
		PlayAnimationOnce(&m_arrOnceAnim[ATTACK1]);
	}
	else if (m_iAttackCnt == 1)
	{
		PlayAnimationOnce(&m_arrOnceAnim[ATTACK2]);
	}
	else
	{
		PlayAnimationOnce(&m_arrOnceAnim[ATTACK3]);
	}
	m_iAttackCnt = (m_iAttackCnt + 1) % 3;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

_vec3 CPlayer::PickingOnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	//vPoint 는 위치 벡터. 그래서 위치값을 저장할 수 있게 w = 1을 만들어주는 D3DXCoord 함수를 사용해야 한다.

	// 투영 -> 뷰 스페이스
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3	vRayDir, vRayPos;		// 뷰 스페이스 영역에 있는 상태

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vPoint - vRayPos;

	// 뷰 스페이스 -> 월드

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorld;

	pTerrainTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].x - 0.5f/*+ (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU*/,
					0.5f,
					pTerrainVtx[dwVtxIdx[1]].z - 0.5f /*+ (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV*/);
			}

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].x + 0.5f/*+  (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU*/,
					0.5f,
					pTerrainVtx[dwVtxIdx[2]].z + 0.5f/*+ (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV*/);
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

