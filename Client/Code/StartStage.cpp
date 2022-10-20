#include "stdafx.h"
#include "..\Header\StartStage.h"
#include "AbstFactory.h"
#include "StaticCamera.h"
#include "Terrain.h"
#include "Player.h"
#include "ObjectStoreMgr.h"
#include "Trigger.h"
#include "Zombie.h"
#include "MapUI.h"
#include "MapTable.h"

CStartStage::CStartStage(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CStartStage::~CStartStage()
{
}

HRESULT CStartStage::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);

	return S_OK;
}

_int CStartStage::Update_Scene(const _float & fTimeDelta)
{
	CSoundMgr::GetInstance()->Update_Listener(LAYER_ENV, L"StaticCamera");
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStartStage::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CStartStage::Render_Scene(void)
{
}

HRESULT CStartStage::Ready_Layer_Environment()
{
	CGameObject*		pGameObject = nullptr;

	CEnvFactory::Create<CStaticCamera>("StaticCamera", L"StaticCamera");

	// Terrain
	CEnvFactory::Create<CTerrainWater>("WaterTerrain", L"WaterTerrain");

	// TerrainCubeMap
	pGameObject = CTerrainCubeMap::Create(m_pGraphicDev, L"../Bin/Resource/Map/StartMap.map");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"TerrainCubeMap", pGameObject), E_FAIL);

	return S_OK;
}

HRESULT CStartStage::Ready_Layer_GameLogic()
{
	_matrix matWorld;

	// CObjectFactory::Create<CBox>("Box", L"Box" , { 2.f, 7.5f, 6.f });
	//CObjectFactory::Create<CBox>("Box", L"Box2", { 4.f, 9.f, 15.f });
	// CObjectFactory::Create<CDynamite>("Dynamite", L"Dynamite");


	

	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 30.f, 2.f ,10.f });

	switch (CObjectStoreMgr::GetInstance()->GetPlayerSkin())
	{
	case Protocol::PLAYER_TYPE_STEVE:
		m_pPlayer = CPlayerFactory::Create<CPlayer>("Steve", L"Player", matWorld);
		break;
	case Protocol::PLAYER_TYPE_PRIDE:
		m_pPlayer = CPlayerFactory::Create<CPlayer>("Pride", L"Player", matWorld);
		break;
	case Protocol::PLAYER_TYPE_ESHE:
		m_pPlayer = CPlayerFactory::Create<CPlayer>("Eshe", L"Player", matWorld);
		break;
	case Protocol::PLAYER_TYPE_COPPER:
		m_pPlayer = CPlayerFactory::Create<CPlayer>("Copper", L"Player", matWorld);
		break;
	case Protocol::PlayerSkin_INT_MIN_SENTINEL_DO_NOT_USE_: break;
	case Protocol::PlayerSkin_INT_MAX_SENTINEL_DO_NOT_USE_: break;
	default:;
	}
	m_pPlayer->SetName(CObjectStoreMgr::GetInstance()->GetPlayerName());
	m_pPlayer->PlayerSpawn();

	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 2.f, 0.5f, 2.5f }, { 0.f, 0 ,0.f }, { 33.2f, 3.5f ,17.2f });

	CObjectFactory::Create<CMapTable>("MapTable", L"MapTable", matWorld);
	return S_OK;
}

HRESULT CStartStage::Ready_Layer_UI()
{
	_matrix matWorld;

	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 38.f, 1.f, 15.f });

	m_pMapUI = CUIFactory::Create<CMapUI>("MapUI", L"MapUI", 0, WINCX * 0.5f, WINCY * 0.5f, WINCX, WINCY);
	m_pMapUI->Close();
	
	return S_OK;
}

CStartStage * CStartStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStartStage* pInstance = new CStartStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStartStage::Free(void)
{
	CScene::Free();
}
