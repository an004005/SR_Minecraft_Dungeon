#include "stdafx.h"
#include "Monster.h"
#include "Export_Utility.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMonster::CMonster(const CGameObject& rhs)
	: CGameObject(rhs)
{
}


CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_Object()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TransformCom", pComponent });

	m_fSpeed = 2.f;
	m_fRotSpeed = 180.f;

	return S_OK;
}

_int CMonster::Update_Object(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);
	return 0;
}

void CMonster::LateUpdate_Object()
{
	CTransform*		pPlayerTransformCom = Engine::Get_Component<CTransform>(LAYER_ENV, L"Player", L"Proto_TransformCom", ID_DYNAMIC);
	NULL_CHECK(pPlayerTransformCom);

	_vec3		vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	m_pTransCom->Chase_Target(&vPlayerPos, 2.f, m_fTimeDelta);

	Engine::CGameObject::LateUpdate_Object();
}

void CMonster::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// m_pBufferCom->Render_Buffer();
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	auto pInst = new CMonster(pGraphicDev);
	if (FAILED(pInst->Ready_Object()))
	{
		Safe_Release(pInst);
		return nullptr;
	}

	return pInst;
}

void CMonster::Free()
{
	CGameObject::Free();
}
