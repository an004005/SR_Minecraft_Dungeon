#include "stdafx.h"
#include "..\Header\StaticCamera.h"
#include "Player.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matProj);
}


CStaticCamera::~CStaticCamera()
{
}

HRESULT CStaticCamera::Ready_Object()
{
	m_fDistance = 16.f;
	m_pTransform = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 10.f, -10.f), &_vec3{0.f, 0.f, 0.f}, &_vec3{0.f, 1.f, 0.f});
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	SetMatProj();

	// m_pTransform->Rotation(ROT_Y, D3DXToRadian(45.f));
	// m_pTransform->Rotation(ROT_X, D3DXToRadian(45.f));

	return S_OK;
}

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	Update_DefaultFollow(fTimeDelta);

	// CTransform*	pSkyBoxTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"SkyBox", L"Proto_TransformCom", ID_DYNAMIC));
	// NULL_CHECK_RETURN(pSkyBoxTransform, -1);

	// pSkyBoxTransform->m_vInfo[INFO_POS] = m_vEye;

	
	// m_pTransform->m_vAngle.y += D3DXToRadian(Get_DIMouseMove(DIMS_X) / 10.f);
	// m_pTransform->m_vAngle.x += D3DXToRadian(Get_DIMouseMove(DIMS_Y) / 10.f);

	m_pTransform->m_vInfo[INFO_POS] = {0.f, 10.f, -10.f};

	D3DXMatrixInverse(&m_matView, nullptr, &m_pTransform->m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

		return OBJ_NOEVENT;
}

void CStaticCamera::SetMatProj(const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}


void CStaticCamera::SetTarget(CGameObject* pTarget)
{
	Safe_Release(m_pTarget);
	Safe_Release(m_pTargetTrans);

	m_pTarget = pTarget;
	m_pTarget->AddRef();
	m_pTargetTrans = m_pTarget->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC);
	m_pTargetTrans->AddRef();


	// m_pTransform->m_vInfo[INFO_POS] = m_pTargetTrans->m_vInfo[INFO_POS] + m_pTransform->m_vInfo[INFO_LOOK] * -m_fDistance;
	m_pTransform->m_vInfo[INFO_POS] = {-5.f, 10.f, -5.f};


	// m_vAt = m_pTargetTrans->m_vInfo[INFO_POS];
	// m_vEye = m_vAt + (m_vCamAway * m_fDistance);
	// m_vPreTargetPos = m_pTargetTrans->m_vInfo[INFO_POS];
}

void CStaticCamera::LerpDistanceTo(_float fDistance)
{
}

void CStaticCamera::Free(void)
{
	Safe_Release(m_pTarget);
	Safe_Release(m_pTargetTrans);
	CGameObject::Free();
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticCamera*		pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CStaticCamera::Update_DefaultFollow(const _float& fTimeDelta)
{
	if (m_pTargetTrans == nullptr) return;

	// const _vec3& vCurTargetPos = m_pTargetTrans->m_vInfo[INFO_POS];
	// const _vec3 vCurEyeDest = vCurTargetPos + (m_vCamAway * m_fDistance);
	//
	// if (CGameUtilMgr::Vec3Cmp(m_pTargetTrans->m_vInfo[INFO_POS], m_vPreTargetPos)) // 정지 하면
	// {
	// 	m_fTimeAcc += fTimeDelta;
	// 	if (m_fTimeAcc > 1.f)
	// 		m_fTimeAcc = 1.f;
	// }
	// else // 이동하면
	// {
	// 	m_fTimeAcc = 0.f;
	// 	m_fTimeAcc += fTimeDelta;
	// }
	//
	// D3DXVec3Lerp(&m_vEye, &m_vEye, &vCurEyeDest, m_fTimeAcc);
	// m_vAt = m_vEye + (m_fDistance * m_vCamAway);
	//
	// m_vPreTargetPos = m_pTargetTrans->m_vInfo[INFO_POS];
}
