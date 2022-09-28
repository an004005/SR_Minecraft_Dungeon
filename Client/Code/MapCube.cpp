#include "stdafx.h"
#include "..\Header\MapCube.h"
#include "MapTool.h"
#include "Export_Function.h"

CMapCube::CMapCube(LPDIRECT3DDEVICE9 pGraphicDev, MapTool& tMapTool)
	:CGameObject(pGraphicDev)
	, m_tMapTool(tMapTool)
{
	Initalize_vFaceVtx();
}

CMapCube::CMapCube(LPDIRECT3DDEVICE9 pGraphicDev , MapTool maptool, _vec3 PickPos)
	: Engine::CGameObject(pGraphicDev)
	, m_tMapTool(maptool)
	, m_vPickPos(PickPos)
{
	Initalize_vFaceVtx();
}

CMapCube::~CMapCube()
{
}

HRESULT CMapCube::Ready_Object(void)
{
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//maptool 에서 피킹 옵션 선택
	if (m_tMapTool.iPickingOption == PICK_TERRAIN)
	{
		_vec3	vPickPos = PickUp_OnTerrain();

		if (vPickPos == _vec3(0.f, 0.f, 0.f))
			return E_FAIL;

		m_pTransCom->Set_Pos(vPickPos.x, vPickPos.y, vPickPos.z);
		m_pTransCom->Update_Component(0.f);
	}
	else
	{
		m_pTransCom->Set_Pos(m_vPickPos.x, m_vPickPos.y, m_vPickPos.z);
		m_pTransCom->Set_ScaleY(m_tMapTool.fHeight);
		m_pTransCom->Update_Component(0.f);
	}

	
	for (int i = 0; i < FACE_END; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			D3DXVec3TransformCoord(&vFaceVtx[i][j], &vFaceVtx[i][j], &m_pTransCom->m_matWorld);
		}
	}
	D3DXVec3TransformCoord(&vCenter, &vCenter, &m_pTransCom->m_matWorld);

	return S_OK;
}

_int CMapCube::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);
	return 0;
}

void CMapCube::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();


}

void CMapCube::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	Render_State();
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CMapCube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MinecraftCubeTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MinecraftCubeTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });


	return S_OK;
}



CMapCube * CMapCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, MapTool maptool, _vec3 PickPos)
{
	CMapCube*	pInstance = new CMapCube(pGraphicDev, maptool, PickPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CMapCube * CMapCube::Create(LPDIRECT3DDEVICE9 pGrahpicDev, _matrix & CubeWorld, MapTool& tMapTool)
{
	CMapCube*	pInstance = new CMapCube(pGrahpicDev, tMapTool);

	pInstance->Add_Component();
	pInstance->m_pTransCom->Set_WorldMatrix(&CubeWorld);
	pInstance->m_wstrName = L"MapCube_" + to_wstring(tMapTool.iCubeCount);
	
	for (int i = 0; i < FACE_END; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			D3DXVec3TransformCoord(&pInstance->vFaceVtx[i][j], &pInstance->vFaceVtx[i][j], &pInstance->m_pTransCom->m_matWorld);
		}
	}
	D3DXVec3TransformCoord(&pInstance->vCenter, &pInstance->vCenter, &pInstance->m_pTransCom->m_matWorld);

	return pInstance;
}

void CMapCube::Free(void)
{
	CGameObject::Free();
}


Engine::_vec3 CMapCube::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->PickingOnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}


void CMapCube::Initalize_vFaceVtx(void)
{
	_float fP = 0.5f;
	_float fM = -0.5f;

	vFaceVtx[FACE_LOOK][0] = { fM, fP, fP };
	vFaceVtx[FACE_LOOK][1] = { fP, fP, fP };
	vFaceVtx[FACE_LOOK][2] = { fP, fM, fP };
	vFaceVtx[FACE_LOOK][3] = { fM, fM, fP };

	vFaceVtx[FACE_BACK][0] = { fM, fP, fM };
	vFaceVtx[FACE_BACK][1] = { fP, fP, fM };
	vFaceVtx[FACE_BACK][2] = { fP, fM, fM };
	vFaceVtx[FACE_BACK][3] = { fM, fM, fM };

	vFaceVtx[FACE_RIGHT][0] = { fP, fP, fP };
	vFaceVtx[FACE_RIGHT][1] = { fP, fM, fP };
	vFaceVtx[FACE_RIGHT][2] = { fP, fM, fM };
	vFaceVtx[FACE_RIGHT][3] = { fP, fP, fM };

	vFaceVtx[FACE_LEFT][0] = { fM, fP,  fP };
	vFaceVtx[FACE_LEFT][1] = { fM, fM,  fP };
	vFaceVtx[FACE_LEFT][2] = { fM, fM,  fM };
	vFaceVtx[FACE_LEFT][3] = { fM, fP,  fM };

	vFaceVtx[FACE_UP][0] = { fP, fP, fP };
	vFaceVtx[FACE_UP][1] = { fP, fP, fM };
	vFaceVtx[FACE_UP][2] = { fM, fP, fM };
	vFaceVtx[FACE_UP][3] = { fM, fP, fP };

	vFaceVtx[FACE_DOWN][0] = { fP, fM, fP };
	vFaceVtx[FACE_DOWN][1] = { fP, fM, fM };
	vFaceVtx[FACE_DOWN][2] = { fM, fM, fM };
	vFaceVtx[FACE_DOWN][3] = { fM, fM, fP };
}

void CMapCube::Render_State(void)
{
	if(m_tMapTool.bRendState)
		m_pTextureCom->Set_Texture(m_tMapTool.iCubeType);
	else
		m_pTextureCom->Set_Texture(m_tMapTool.iTexIdx);
}

