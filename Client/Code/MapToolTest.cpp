#include "stdafx.h"
#include "..\Header\MapToolTest.h"
#include "Terrain.h"
#include "MapCube.h"
#include "DynamicCamera.h"
#include "ImGuiMgr.h"
#include "Export_Function.h"

CMapToolTest::CMapToolTest(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}


CMapToolTest::~CMapToolTest()
{
}

HRESULT CMapToolTest::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);

	m_pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pLayer, E_FAIL);

	return S_OK;
}

_int CMapToolTest::Update_Scene(const _float & fTimeDelta)
{

	ImGui::ShowDemoWindow(nullptr);
	IM_BEGIN("Map Editor Window");
	CImGuiMgr::MapControl(m_fFloor, m_fHeight, m_iIndex);
	
	if (Engine::Get_DIMouseState(DIM_LB) & 0X80 && m_dwTime + 1000 < GetTickCount())
	{
		PickPos = PickingOnCube(g_hWnd);
		
		CGameObject*		pGameObject = nullptr;
		CMapCube* pCube;
		pGameObject = pCube = CMapCube::Create(m_pGraphicDev, m_fHeight, m_iIndex, PickPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(L"MapCube", pGameObject), E_FAIL);

		m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });

		pCube->m_wstrName = L"MapCube_" + m_iCubeCount;
		m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pCube->m_wstrName.c_str(), pGameObject), E_FAIL);
		m_vecCube.push_back(dynamic_cast<CMapCube*>(pGameObject));

		m_iCubeCount++;

		m_dwTime = GetTickCount();
	}
	
	IM_END;
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CMapToolTest::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CMapToolTest::Render_Scene(void)
{
}

_vec3 CMapToolTest::PickingOnCube(HWND hWnd)
{
	return _vec3();
}

HRESULT CMapToolTest::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	pGameObject = CMapCube::Create(m_pGraphicDev, m_fHeight, m_iIndex, PickPos);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MapCube", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMapToolTest::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 103)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);

	return S_OK;
}

CMapToolTest * CMapToolTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapToolTest *	pInstance = new CMapToolTest(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMapToolTest::Free(void)
{
	CScene::Free();
}
