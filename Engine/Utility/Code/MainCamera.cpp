#include "MainCamera.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameUtilMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CMainCamera)

CMainCamera::CMainCamera()
{
	D3DXVec3Normalize(&m_vCamAwayDir, &m_vCamAwayDir);
}


CMainCamera::~CMainCamera()
{
	Free();
}

void CMainCamera::UpdateCamera(const _float& fTimeDelta)
{
	UpdateCameraNormal(fTimeDelta);
}

void CMainCamera::UpdateCameraNormal(const _float& fTimeDelta)
{
	if (m_pTargetObj == nullptr || m_pTargetTrans == nullptr || m_pGraphicDev == nullptr) return;

	m_vAt = m_pTargetTrans->m_vInfo[INFO_POS];
	m_vPrePos = m_vAt;
	m_vEye = m_vAt + (m_vCamAwayDir * m_fDist);
	m_vEye = _vec3(0.f, 10.f, -10.f);
	m_vAt = _vec3{0.f, 0.f, 0.f};

	_matrix world;
	CGameUtilMgr::MatWorldComposeEuler(world, {1.f, 1.f, 1.f}, {45.f, 0.f ,0.f }, {0.f, 10.f, -10.f});
	D3DXMatrixInverse(&world, nullptr, &world);


	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &world);
}

void CMainCamera::Free()
{
	m_pTargetObj = nullptr;
	m_pTargetTrans = nullptr;
	Safe_Release(m_pGraphicDev);
}

void CMainCamera::SetTargetObj(CGameObject* pTargetObj)
{
	NULL_CHECK(pTargetObj);
	m_pTargetObj = pTargetObj;
	m_pTargetTrans = pTargetObj->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC);

	m_vAt = m_pTargetTrans->m_vInfo[INFO_POS];
	m_vPrePos = m_vAt;
	m_vEye = m_vAt + (m_vCamAwayDir * m_fDist);
}

void CMainCamera::SetMatProj(_float fNear, _float fFar, _float fFov, _float fAspect)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

