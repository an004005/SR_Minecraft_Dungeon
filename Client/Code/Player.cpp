#include "stdafx.h"
#include "Player.h"
#include "TerrainCubeMap.h"
#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* VtxPos)
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
	Key_Input(fTimeDelta);
	Engine::CGameObject::Update_Object(fTimeDelta);

	// if (Engine::DIK(KEY_TYPE::W))
	// {
	// 	_vec3 NormalUp;
	// 	D3DXVec3Normalize(OUT &NormalUp, &m_pTransCom->m_vInfo[INFO_UP]);
	// 	m_pTransCom->m_vInfo[INFO_POS] += NormalUp * m_fSpeed * fTimeDelta;
	// }
	// else if (Engine::GetButton(KEY_TYPE::S))
	// {
	// 	_vec3 NormalUp;
	// 	D3DXVec3Normalize(OUT &NormalUp, &m_pTransCom->m_vInfo[INFO_UP]);
	// 	m_pTransCom->m_vInfo[INFO_POS] -= NormalUp * m_fSpeed * fTimeDelta;
	// }
	//
	// if (Engine::GetButton(KEY_TYPE::A))
	// {
	// 	m_pTransCom->m_vAngle.z += D3DXToRadian(m_fRotSpeed * fTimeDelta);
	// }
	// else if (Engine::GetButton(KEY_TYPE::D))
	// {
	// 	m_pTransCom->m_vAngle.z -= D3DXToRadian(m_fRotSpeed * fTimeDelta);
	// }
	//
	// if (Engine::GetButton(KEY_TYPE::Q))
	// {
	// 	m_pTransCom->m_vAngle.y += D3DXToRadian(m_fRotSpeed * fTimeDelta);
	// }
	// else if (Engine::GetButton(KEY_TYPE::E))
	// {
	// 	m_pTransCom->m_vAngle.y -= D3DXToRadian(m_fRotSpeed * fTimeDelta);
	// }
	//
	// if (Engine::GetButton(KEY_TYPE::Z))
	// {
	// 	m_pTransCom->m_vAngle.x += D3DXToRadian(m_fRotSpeed * fTimeDelta);
	// }
	// else if (Engine::GetButton(KEY_TYPE::C))
	// {
	// 	m_pTransCom->m_vAngle.x -= D3DXToRadian(m_fRotSpeed * fTimeDelta);
	// }



	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CPlayer::LateUpdate_Object(void)
{
	Set_OnTerrain();
	Engine::CGameObject::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);	
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * VtxPos)
{
	CPlayer *	pInstance = new CPlayer(pGraphicDev, VtxPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
void CPlayer::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fHeight = m_pTerrainMap->m_fHeight[(int)vPos.x][(int)vPos.z];

	//_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, m_pTerrainMap->Get_Pos(), VTXCNTX, VTXCNTZ);
	m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

_vec3 CPlayer::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());


	return m_pCalculatorCom->PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}

void CPlayer::Free(void)
{
	CGameObject::Free();
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	return S_OK;
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);


	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * 10.f * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * -10.f * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_pTransCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_pTransCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));

	if (Engine::Get_DIMouseState(DIM_LB) & 0X80)
	{
		_vec3	vPickPos = PickUp_OnTerrain();
		_vec3	vPlayerPos, vDir;
		m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
		vDir = vPickPos - vPlayerPos;
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransCom->Move_Pos(&(vDir * 5.f * fTimeDelta));
	}
}
