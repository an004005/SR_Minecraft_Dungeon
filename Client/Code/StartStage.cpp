#include "stdafx.h"
#include "..\Header\StartStage.h"
#include "AbstFactory.h"
#include "StaticCamera.h"
#include "Terrain.h"
#include "Player.h"
#include "ObjectStoreMgr.h"
#include "Trigger.h"
#include "MapUI.h"
#include "MapTable.h"
#include "CoolTimeUI.h"
#include "DamageFontMgr.h"
#include "ArrowCubeMgr.h"

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
	// 임시 조치


	CSoundMgr::GetInstance()->Update_Listener(LAYER_ENV, L"StaticCamera");
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStartStage::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
	if (DIKeyDown(DIK_0))
	{
		CSceneFactory::LoadScene("Loading1", "NetStage_Start", true ,500);
	}
}

void CStartStage::Render_Scene(void)
{
	CArrowCubeMgr::GetInst().Render_Buffer(); 
	CDamageFontMgr::GetInstance()->Render_DamageFontMgr();
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

	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 2.f, 1.f, 7.f });

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


	CEffectFactory::Create<C3DBaseTexture>("3D_Base", L"3D_Base");
	CEffectFactory::Create<CAttack_P>("Attack_Basic", L"Attack_Basic");
	CEffectFactory::Create<CFireWork_Fuze>("FireWork_Fuze", L"FireWork_Fuze");
	CEffectFactory::Create<CFireWork>("FireWork", L"FireWork");
	CEffectFactory::Create<CSpeedBoots>("Speed_Boots", L"Speed_Boots");
	CEffectFactory::Create<CSpeedBoots_Particle>("Speed_Boots_Particle", L"Speed_Boots_Particle");
	return S_OK;
}

HRESULT CStartStage::Ready_Layer_UI()
{
	_matrix matWorld;

	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 38.f, 1.f, 15.f });

	m_pMapUI = CUIFactory::Create<CMapUI>("MapUI", L"MapUI", 0, WINCX * 0.5f, WINCY * 0.5f, WINCX, WINCY);
	m_pMapUI->Close();
	
	// 플레이어 생성하고 생성하기
	CUIFactory::Create<CUI>("HPUI", L"HPUI", -1, WINCX / 2, WINCY - 50, 100, 80);
	CUIFactory::Create<CCoolTimeUI>("PotionCoolTime", L"PotionCoolTime", -1, WINCX / 2 + 90, WINCY - 40, 50, 50);
	CUIFactory::Create<CCoolTimeUI>("RollCoolTime", L"RollCoolTime", -1, WINCX / 2 + 140, WINCY - 30, 30, 30);


	CUIFactory::Create<CCoolTimeUI>("Legacy1CoolTime", L"Legacy1CoolTime", -1, WINCX / 2 - 210, WINCY - 40, 50, 50);
	CUIFactory::Create<CCoolTimeUI>("Legacy2CoolTime", L"Legacy2CoolTime", -1, WINCX / 2 - 150, WINCY - 40, 50, 50);
	CUIFactory::Create<CCoolTimeUI>("Legacy3CoolTime", L"Legacy3CoolTime", -1, WINCX / 2 - 90, WINCY - 40, 50, 50);

	CUIFactory::Create<CCountUI>("ArrowUI", L"ArrowUI", -1, WINCX / 2 + 190, WINCY - 30, 50, 50);
	CUIFactory::Create<CCountUI>("EmeraldUI", L"EmeraldUI", -1, WINCX / 2 + 250, WINCY - 30, 20, 30);
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
