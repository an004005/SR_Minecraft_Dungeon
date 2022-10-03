#include "stdafx.h"
#include "Player.h"
#include "Controller.h"
#include "GameUtilMgr.h"
#include "TerrainCubeMap.h"
#include "StaticCamera.h"

/*-----------------------
 *    CCharacter
 ----------------------*/
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev) : CSkeletalCube(pGraphicDev)
{
	m_fVelocity = 5.f;
	m_fRollSpeed = 12.f;
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
	m_arrOnceAnim[ATTACK2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_attack_b.anim");
	m_arrOnceAnim[ATTACK3] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_attack_c.anim");
	m_arrOnceAnim[ROLL] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/roll.anim");
	m_pIdleAnim = &m_arrLoopAnim[IDLE];

	m_pCurAnim = m_pIdleAnim;

	CCollisionCom* pColl = Add_Component<CCollisionCom>(L"Proto_CollisionCom", L"Proto_CollisionCom", ID_DYNAMIC);
	pColl->SetOwner(this);
	pColl->SetOwnerTransform(m_pRootPart->pTrans);


	// 항상 카메라 먼저 만들고 플레이어 만들기!
	Get_GameObject<CStaticCamera>(LAYER_ENV, L"StaticCamera")->SetTarget(this);

	return S_OK;
}

_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	CSkeletalCube::Update_Object(fTimeDelta);
	m_pController->Update(this);

	MeleeAttack();

	if (!m_bAction)
	{
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_vMoveDirNormal * m_fVelocity * fTimeDelta;
	}

	if (m_bRoll)
	{
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fRollSpeed * fTimeDelta;
	}

	// set height
	CTerrainCubeMap* pTerrain = Get_GameObject<CTerrainCubeMap>(LAYER_ENV, L"TerrainCubeMap");
	_vec3& vPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
	vPos.y = pTerrain->GetHeight(vPos.x, vPos.z);

	return OBJ_NOEVENT;
}

void CPlayer::Free()
{
	CSkeletalCube::Free();
	Safe_Release(m_pController);
}

void CPlayer::AnimationEvent(const string& strEvent)
{
	if (strEvent == "ActionEnd")
	{
		m_bAction = false;
		SetMove(0.f, 0.f);
		m_bRoll = false;
	}
}

void CPlayer::SetMove(_float fX, _float fZ)
{
	m_vMoveDir.x += fX;
	m_vMoveDir.z += fZ;
	D3DXVec3Normalize(&m_vMoveDirNormal, &m_vMoveDir);

	if (m_bAction) return;

	_float fCamYaw = Engine::Get_Component<CTransform>(LAYER_ENV, L"StaticCamera", L"Proto_TransformCom", ID_DYNAMIC)->m_vAngle.y;
	_matrix matYaw;
	D3DXMatrixRotationY(&matYaw, fCamYaw);

	D3DXVec3TransformNormal(&m_vMoveDirNormal, &m_vMoveDirNormal, &matYaw);

	if (!CGameUtilMgr::Vec3Cmp(m_vMoveDir, CGameUtilMgr::s_vZero))
	{
		const _vec2 v2Look{0.f, 1.f};
		const _vec2 v2ToDest{m_vMoveDirNormal.x, m_vMoveDirNormal.z};
		const _float fDot = D3DXVec2Dot(&v2Look, &v2ToDest);
		

		if (m_vMoveDirNormal.x < 0)
			m_pRootPart->pTrans->m_vAngle.y = -acosf(fDot);
		else
			m_pRootPart->pTrans->m_vAngle.y = acosf(fDot);

		m_pIdleAnim = &m_arrLoopAnim[WALK];
		m_pCurAnim = m_pIdleAnim;
	}
	else
	{
		m_pIdleAnim = &m_arrLoopAnim[IDLE];
		m_pCurAnim = m_pIdleAnim;
	}
}

void CPlayer::MeleeAttack()
{
	if (m_bMeleeAttack == false) return;
	if (m_bAction == true) return;

	m_bAction = true;
	RotateToCursor();

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

void CPlayer::Roll()
{
	RotateToCursor();
	m_bAction = true;
	m_bRoll = true;
	PlayAnimationOnce(&m_arrOnceAnim[ROLL]);
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

void CPlayer::RotateToCursor()
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3 vPoint;
	_vec3 vAt;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;
	vAt.x = vPoint.x;
	vAt.y = vPoint.y;
	vAt.z = 1.f;

	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);
	D3DXVec3TransformCoord(&vAt, &vAt, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matView);
	D3DXVec3TransformCoord(&vAt, &vAt, &matView);

	D3DXPLANE tmpPlane;
	D3DXPlaneFromPointNormal(&tmpPlane, &m_pRootPart->pTrans->m_vInfo[INFO_POS], &CGameUtilMgr::s_vUp);

	_vec3 vLookAt;
	D3DXPlaneIntersectLine(&vLookAt, &tmpPlane, &vPoint, &vAt);

	_vec3 vLook = vLookAt - m_pRootPart->pTrans->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vLook, &vLook);

	const _vec2 v2Look{0.f, 1.f};
	_vec2 v2ToDest{vLook.x, vLook.z};
	
	const _float fDot = D3DXVec2Dot(&v2Look, &v2ToDest);
	
	if (vLook.x < 0)
		m_pRootPart->pTrans->m_vAngle.y = -acosf(fDot);
	else
		m_pRootPart->pTrans->m_vAngle.y = acosf(fDot);

	m_pIdleAnim = &m_arrLoopAnim[WALK];
	m_pCurAnim = m_pIdleAnim;
}