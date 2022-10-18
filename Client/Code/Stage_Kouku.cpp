#include "stdafx.h"
#include "..\Header\Stage_Kouku.h"

#include "StaticCamera.h"
#include "Terrain.h"
#include "Player.h"
#include "AbstFactory.h"

#include "DynamicCamera.h"
#include "Particle.h"
#include "Arrow.h"
#include "ArrowCubeMgr.h"
#include "Box.h"
#include "Dynamite.h"
#include "BossHPUI.h"
#include "StatComponent.h"
#include "PlayerUI.h"

//monster
#include "Monster.h"
#include "Geomancer.h"
#include "Zombie.h"
#include "Creeper.h"
#include "Skeleton.h"
#include "Enchanter.h"
#include "RedStoneCube.h"
#include "RedStoneMonstrosity.h"
#include "UI.h"
#include "CoolTimeUI.h"
#include "BatchTool.h"
#include "DamageFontMgr.h"
#include "Kouku.h"
#include "Saton.h"

// object
#include "Birds.h"
#include "BirdsBrown.h"
CStage_Kouku::CStage_Kouku(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}


CStage_Kouku::~CStage_Kouku()
{
}

HRESULT CStage_Kouku::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);
	return S_OK;
}

_int CStage_Kouku::Update_Scene(const _float & fTimeDelta)
{
	if(m_pPlayer != nullptr)
	{
		if (m_pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)->IsDead())
		{
			if (m_fDeadTime > 3.f)
				m_pPlayer->PlayerSpawn();
			m_fDeadTime += fTimeDelta;

			if (m_bPlayerAlive)
			{
				m_pPlayerUI = CUIFactory::Create<CPlayerUI>("PlayerUI", L"PlayerDead", 0, WINCX * 0.5f, WINCY * 0.5f, WINCX, WINCY);
				m_pPlayerUI->Open();
				m_pPlayerUI->SetUITexture(25);
			}
			m_bPlayerAlive = false;
			
		}
		else
		{
			if (m_pPlayerUI != nullptr)
			{
				m_pPlayerUI->Close();
				m_pPlayerUI = nullptr;
			}
			m_bPlayerAlive = true;
			m_fDeadTime = 0.f;
		}
	}
	
	Engine::GetFont();

	CSoundMgr::GetInstance()->Update_Listener(LAYER_ENV, L"StaticCamera");
	CDamageFontMgr::GetInstance()->Update_DamageFontMgr(fTimeDelta);
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage_Kouku::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CStage_Kouku::Render_Scene(void)
{
	CArrowCubeMgr::GetInst().Render_Buffer(); // todo : 렌더러에서 동작하게 바꾸기
	CDamageFontMgr::GetInstance()->Render_DamageFontMgr();
}

HRESULT CStage_Kouku::Ready_Layer_Environment()
{
	CGameObject*		pGameObject = nullptr;

	CStaticCamera* pCAm = CEnvFactory::Create<CStaticCamera>("StaticCamera", L"StaticCamera");
	pCAm->Set_Mode();
	
	// Terrain
	CEnvFactory::Create<CTerrainWater>("WaterTerrain", L"WaterTerrain");

	// TerrainCubeMap
	CTerrainCubeMap* pMap;
	pGameObject = pMap =  CTerrainCubeMap::Create(m_pGraphicDev, L"../Bin/Resource/Map/koukusaton_test.map");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"TerrainCubeMap", pGameObject), E_FAIL);

	pMap->SetHeight(60, 25, 25.000000);
	pMap->SetHeight(60, 26, 25.000000);
	pMap->SetHeight(60, 27, 25.000000);
	pMap->SetHeight(60, 28, 25.000000);
	pMap->SetHeight(60, 29, 25.000000);
	pMap->SetHeight(59, 30, 25.000000);
	pMap->SetHeight(60, 30, 25.000000);
	pMap->SetHeight(63, 30, 25.000000);
	pMap->SetHeight(49, 31, 25.000000);
	pMap->SetHeight(60, 31, 25.000000);
	pMap->SetHeight(61, 31, 25.000000);
	pMap->SetHeight(62, 32, 25.000000);
	pMap->SetHeight(72, 32, 25.000000);
	pMap->SetHeight(53, 33, 25.000000);
	pMap->SetHeight(58, 34, 25.000000);
	pMap->SetHeight(60, 34, 25.000000);
	pMap->SetHeight(63, 34, 25.000000);
	pMap->SetHeight(64, 34, 25.000000);
	pMap->SetHeight(56, 35, 25.000000);
	pMap->SetHeight(62, 35, 25.000000);
	pMap->SetHeight(39, 36, 25.000000);
	pMap->SetHeight(57, 36, 25.000000);
	pMap->SetHeight(66, 36, 25.000000);
	pMap->SetHeight(50, 37, 25.000000);
	pMap->SetHeight(56, 37, 25.000000);
	pMap->SetHeight(58, 37, 25.000000);
	pMap->SetHeight(60, 37, 25.000000);
	pMap->SetHeight(62, 37, 25.000000);
	pMap->SetHeight(64, 37, 25.000000);
	pMap->SetHeight(66, 37, 25.000000);
	pMap->SetHeight(72, 38, 25.000000);
	pMap->SetHeight(74, 38, 25.000000);
	pMap->SetHeight(79, 38, 25.000000);
	pMap->SetHeight(62, 39, 25.000000);
	pMap->SetHeight(71, 39, 25.000000);
	pMap->SetHeight(72, 39, 25.000000);
	pMap->SetHeight(75, 41, 25.000000);
	pMap->SetHeight(41, 42, 25.000000);
	pMap->SetHeight(37, 43, 25.000000);
	pMap->SetHeight(54, 43, 25.000000);
	pMap->SetHeight(60, 43, 25.000000);
	pMap->SetHeight(83, 43, 25.000000);
	pMap->SetHeight(74, 45, 25.000000);
	pMap->SetHeight(38, 47, 25.000000);
	pMap->SetHeight(66, 47, 25.000000);
	pMap->SetHeight(82, 49, 25.000000);

	// for (int z = 49; z >= 0; --z)
	// 	pMap->SetHeight(z, 49, 0.f);

	for(int z = 33; z< VTXCNTZ; ++z) // 왼쪽
	{
		for(int x = 40; x>=0; --x)
		{
			pMap->SetHeight(x, z, 0.f);
		}
	}
	for (int z = 33; z< VTXCNTZ; ++z) // 오른쪽
	{
		for (int x = 84; x < VTXCNTX; ++x)
		{
			pMap->SetHeight(x, z, 0.f);
		}
	}

	for (int x = 41; x < VTXCNTX; ++x) // 아래
	{
		for (int z = 32; z >= 0; --z)
		{
			pMap->SetHeight(x, z, 0.f);
		}
	}

	for (int x = 41; x < VTXCNTX; ++x) // 위
	{
		for (int z = 49; z < VTXCNTZ; ++z)
		{
			pMap->SetHeight(x, z, 0.f);
		}
	}
	for (int z = 37; z >= 0; --z) // 왼쪽 아래 
	{
		for (int x = 48; x >= 0; --x)
		{
			pMap->SetHeight(x, z, 0.f);
		}
	}
	for (int z = 39; z >= 0; --z) // 오른쪽 아래 
	{
		for (int x = 78; x < VTXCNTX; ++x)
		{
			pMap->SetHeight(x, z, 0.f);
		}
	}
	//  Birds

	for (int i = 0; i < 10; ++i)
	{
		CBirdsBrown* bird = CObjectFactory::Create<CBirdsBrown>("BirdsBrown", L"BirdsWhite");
		bird->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->Set_Pos(3.f, 9.5f, 18.f + i);

	}
	
	return S_OK;
}

HRESULT CStage_Kouku::Ready_Layer_GameLogic()
{
	_matrix matWorld;

	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 62.5f, 0.f ,42.7f });
	m_pPlayer = CPlayerFactory::Create<CPlayer>("Steve", L"Player", matWorld);
	m_pPlayer->PlayerSpawn();
	
	CEffectFactory::Create<C3DBaseTexture>("3D_Base", L"3D_Base");
	CEffectFactory::Create<CAttack_P>("Attack_Basic", L"Attack_Basic");
	CEffectFactory::Create<CFireWork_Fuze>("FireWork_Fuze", L"FireWork_Fuze");
	CEffectFactory::Create<CFireWork>("FireWork", L"FireWork");
	CEffectFactory::Create<CSpeedBoots>("Speed_Boots", L"Speed_Boots");
	CEffectFactory::Create<CSpeedBoots_Particle>("Speed_Boots_Particle", L"Speed_Boots_Particle");
	CEffectFactory::Create<CMoonParticle>("MoonParticle", L"MoonParticle");
	CEffectFactory::Create<CFascinated_Effect>("Fascinate_Effect", L"Fascinate_Effect");

	
	//monsters
	{	
		CGameUtilMgr::MatWorldComposeEuler(matWorld, { 3.f, 3.f, 3.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 62.5f, 21.5f ,47.8f });
		CEnemyFactory::Create<CSaton>("Saton", L"Saton", matWorld);

		CGameUtilMgr::MatWorldComposeEuler(matWorld, { 0.7f, 0.7f, 0.7f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 62.5f, 25.f ,44.8f });
		CEnemyFactory::Create<CKouku>("Kouku", L"Kouku", matWorld);
	}
	return S_OK;
}

HRESULT CStage_Kouku::Ready_Layer_UI()
{
	// 플레이어 생성하고 생성하기
	CUIFactory::Create<CUI>("HPUI", L"HPUI", -1, WINCX/2, WINCY - 50, 100, 80);
	CUIFactory::Create<CCoolTimeUI>("PotionCoolTime", L"PotionCoolTime", -1, WINCX/2 + 90, WINCY - 40, 50, 50);
	CUIFactory::Create<CCoolTimeUI>("RollCoolTime", L"RollCoolTime", -1, WINCX/2 + 140, WINCY - 30, 30, 30);


	CUIFactory::Create<CCoolTimeUI>("Legacy1CoolTime", L"Legacy1CoolTime", -1, WINCX/2 - 210, WINCY - 40, 50, 50);
	CUIFactory::Create<CCoolTimeUI>("Legacy2CoolTime", L"Legacy2CoolTime", -1, WINCX/2 - 150, WINCY - 40, 50, 50);
	CUIFactory::Create<CCoolTimeUI>("Legacy3CoolTime", L"Legacy3CoolTime", -1, WINCX/2 - 90, WINCY - 40, 50, 50);

	CUIFactory::Create<CCountUI>("ArrowUI", L"ArrowUI", -1, WINCX/2 + 190, WINCY - 30, 50, 50);
	CUIFactory::Create<CCountUI>("EmeraldUI", L"EmeraldUI", -1, WINCX/2 + 250, WINCY - 30, 20, 30);

	// 플레이어 생성하고 생성하기

	return S_OK;
}



CStage_Kouku * CStage_Kouku::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage_Kouku *	pInstance = new CStage_Kouku(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CStage_Kouku::Free(void)
{


	CScene::Free();
}
