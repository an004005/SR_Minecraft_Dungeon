#include "stdafx.h"
#include "..\Header\Stage.h"

#include "StaticCamera.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "SkeletalCube.h"
#include "Player.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}


CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);


	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{

}

HRESULT CStage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	m_pTerrainMap = CTerrainCubeMap::Create();
	m_pTerrainMap->LoadMap(m_pGraphicDev, m_mapLayer, pLayer);
	
	CGameObject*		pGameObject = nullptr;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
	
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);

	static vector<wstring> vecNs;


	pGameObject = CPlayer::Create(m_pGraphicDev, L"../Bin/Resource/SkeletalCube/CubeMan/Steve.cube");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	// for (int i = 0; i < 150; ++i)
	// {
	// 	pGameObject = CPlayer::Create(m_pGraphicDev, L"../Bin/Resource/SkeletalCube/CubeMan/Steve.cube");
	// 	dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_TransformCom_root", ID_DYNAMIC))->m_vInfo[INFO_POS] = _vec3((_float)i, 0.f, 0.f);
	// 	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	// 	vecNs.push_back(L"Player_" + to_wstring(i));
	// 	// FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	// 	FAILED_CHECK_RETURN(pLayer->Add_GameObject(vecNs.back().c_str(), pGameObject), E_FAIL);
	// }

	// pGameObject = CPlayer::Create(m_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/Skeleton.cube");


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;




	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 122)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/weapon/weapon_%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Sword", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_0.png", 0.08f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);


	return S_OK;
}



CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CStage::Free(void)
{
	if (m_pTerrainMap != nullptr)
	{
		delete m_pTerrainMap;
		m_pTerrainMap = nullptr;
	}

	CScene::Free();
}
