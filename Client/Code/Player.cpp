#include "stdafx.h"
#include "Player.h"
#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 5.f;
	m_fRotSpeed = 90.f;
	
	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	//m_pTransCom->m_vAngle.z = D3DXToRadian(45.f);

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (Engine::GetButton(KEY_TYPE::W))
	{
		_vec3 NormalUp;
		D3DXVec3Normalize(OUT &NormalUp, &m_pTransCom->m_vInfo[INFO_UP]);
		m_pTransCom->m_vInfo[INFO_POS] += NormalUp * m_fSpeed * fTimeDelta;
	}
	else if (Engine::GetButton(KEY_TYPE::S))
	{
		_vec3 NormalUp;
		D3DXVec3Normalize(OUT &NormalUp, &m_pTransCom->m_vInfo[INFO_UP]);
		m_pTransCom->m_vInfo[INFO_POS] -= NormalUp * m_fSpeed * fTimeDelta;
	}

	if (Engine::GetButton(KEY_TYPE::A))
	{
		m_pTransCom->m_vAngle.z += D3DXToRadian(m_fRotSpeed * fTimeDelta);
	}
	else if (Engine::GetButton(KEY_TYPE::D))
	{
		m_pTransCom->m_vAngle.z -= D3DXToRadian(m_fRotSpeed * fTimeDelta);
	}

	if (Engine::GetButton(KEY_TYPE::Q))
	{
		m_pTransCom->m_vAngle.y += D3DXToRadian(m_fRotSpeed * fTimeDelta);
	}
	else if (Engine::GetButton(KEY_TYPE::E))
	{
		m_pTransCom->m_vAngle.y -= D3DXToRadian(m_fRotSpeed * fTimeDelta);
	}

	if (Engine::GetButton(KEY_TYPE::Z))
	{
		m_pTransCom->m_vAngle.x += D3DXToRadian(m_fRotSpeed * fTimeDelta);
	}
	else if (Engine::GetButton(KEY_TYPE::C))
	{
		m_pTransCom->m_vAngle.x -= D3DXToRadian(m_fRotSpeed * fTimeDelta);
	}



	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CPlayer::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer *	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

void CPlayer::Free(void)
{
	CGameObject::Free();
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;
		
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	return S_OK;
}
