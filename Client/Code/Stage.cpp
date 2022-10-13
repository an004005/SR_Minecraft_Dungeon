#include "stdafx.h"
#include "..\Header\Stage.h"

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

//item
#include "Crossbow.h"
#include "Sword.h"
#include "Glaive.h"
#include "Axe.h"

// object
#include "Birds.h"
#include "BirdsBrown.h"
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

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);
	
	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	// m_pTransform->Get_Info(INFO_POS, &_vec3(0.f, -0.5f, 0.f));
	// m_pTransform->Set_Scale(0.f, 1.f, 0.f);
	//CUIFactory::Create<CUI>("UI_HP", L"UI", 600.f, 650.f - fY, 55.f, 40.f);
	//Engine::Get_Component<CTransform>(LAYER_UI, L"UI_HP", L"Proto_TransformCom", ID_DYNAMIC)
	//	->m_vAngle.y += D3DXToRadian(40.f) * fTimeDelta;

	Engine::GetFont();

	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{
	CArrowCubeMgr::GetInst().Render_Buffer(); // todo : 렌더러에서 동작하게 바꾸기

}

HRESULT CStage::Ready_Layer_Environment()
{
	CGameObject*		pGameObject = nullptr;

	CEnvFactory::Create<CStaticCamera>("StaticCamera", L"StaticCamera");

	// Terrain
	CEnvFactory::Create<CTerrain>("DefaultTerrain", L"Terrain");

	// TerrainCubeMap
	pGameObject = CTerrainCubeMap::Create(m_pGraphicDev, L"../Bin/Resource/Map/Stage1.map");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"TerrainCubeMap", pGameObject), E_FAIL);

	//  Birds

	for (int i = 0; i < 10; ++i)
	{
		CBirds* bird = CEnvFactory::Create<CBirds>("BirdsWhite", L"BirdsWhite");
		bird->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->Set_Pos(3.f, 9.5f, 18.f + i);

		CBirdsBrown* Brwon = CEnvFactory::Create<CBirdsBrown>("BirdsBrown", L"BirdsBrown");
		Brwon->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->Set_Pos(6.f, 9.5f, 18.f + i);
	}
	
	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic()
{
	_matrix matWorld;

	CItemFactory::Create<CAxe>("Axe", L"Axe");

	//CObjectFactory::Create<CBox>("Box", L"Box");
	//CObjectFactory::Create<CDynamite>("Dynamite", L"Dynamite");

	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 1.f, 0.f ,3.f });
	CPlayerFactory::Create<CPlayer>("Steve", L"Player", matWorld);

	
	CEffectFactory::Create<C3DBaseTexture>("3D_Base", L"3D_Base");
	CEffectFactory::Create<CAttack_P>("Attack_Basic", L"Attack_Basic");
	CEffectFactory::Create<CFireWork_Fuze>("FireWork_Fuze", L"FireWork_Fuze");
	CEffectFactory::Create<CFireWork>("FireWork", L"FireWork");
	CEffectFactory::Create<CSpeedBoots>("Speed_Boots", L"Speed_Boots");
	CEffectFactory::Create<CSpeedBoots_Particle>("Speed_Boots_Particle", L"Speed_Boots_Particle");

	
	//monsters
	{	
		CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 50.f, 0.f ,26.f});
		CEnemyFactory::Create<CZombie>("Zombie", L"Zombie", matWorld);
	
		//
		//CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 55.f, 0.f ,28.f });
		//CEnemyFactory::Create<CGeomancer>("Geomancer", L"Geomancer", matWorld);


		// CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 43.f, 0.f , 21.f });
		// CEnemyFactory::Create<CCreeper>("Creeper", L"Creeper", matWorld);

		CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 3.f, 0.f ,13.f });
		CEnemyFactory::Create<CSkeleton>("Skeleton", L"Skeleton", matWorld);

		//CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 47.f, 0.f ,17.f });
		//CEnemyFactory::Create<CEnchanter>("Enchanter", L"Enchanter", matWorld);

		// CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(180.f) ,0.f }, { 3.f, 0.f ,13.f });
		// CEnemyFactory::Create<CRedStoneMonstrosity>("RedStoneMonstrosity", L"RedStoneMonstrosity", matWorld);
	}
	
	CGameUtilMgr::MatWorldComposeEuler(matWorld, {1.f, 1.f, 1.f}, {0.f, D3DXToRadian(90.f) ,0.f }, {6.f, 0.f ,6.f});
	// CEnemyFactory::Create<CGeomancer>("Geomancer", L"Geomancer", matWorld);
	// CEnemyFactory::Create<CMonster>("Zombie", L"TestZombie", matWorld);


	return S_OK;
}

HRESULT CStage::Ready_Layer_UI()
{
	// CGameObject*		pGameObject = nullptr;
	
	//CUIFactory::Create<CUI>("Buffe4", L"UI", 700.f, 650.f, 40.f, 40.f);
	//CUIFactory::Create<CUI>("Buffe3", L"UI", 503.f, 650.f, 40.f, 40.f);
	CUIFactory::Create<CUI>("UITexture", L"UI", 600.f, 650.f, 130.f, 110.f);
	CUIFactory::Create<CUI>("UI_HP", L"UI_HP", 600.f, 650.f, 110.f, 85.f);
	//CUIFactory::Create<CUI>("InvenTool", L"UI", 230.f, 655.f, 35.f, 35.f);
	//CUIFactory::Create<CUI>("Inven", L"UI", 230.f, 655.f, 25.f, 25.f);
	//CUIFactory::Create<CUI>("Buffe1", L"UI", 325.f, 650.f, 40.f, 40.f);
	//CUIFactory::Create<CUI>("Buffe2", L"UI", 415.f, 650.f, 40.f, 40.f);
	//CUIFactory::Create<CUI>("mapTool", L"UI", 800.f, 655.f, 35.f, 35.f);
	//CUIFactory::Create<CUI>("map", L"UI", 800.f, 655.f, 35.f, 35.f);
	//CUIFactory::Create<CUI>("RollTool", L"UI", 880.f, 655.f, 30.f, 30.f);
	//CUIFactory::Create<CUI>("Roll", L"UI", 880.f, 657.f, 25.f, 20.f);
	CUIFactory::Create<CUI>("arrowTool", L"UI", 980.f, 655.f, 40.f, 50.f);
	CUIFactory::Create<CUI>("arrow", L"UI", 980.f, 655.f, 30.f, 30.f);
	CUIFactory::Create<CUI>("emerald", L"UI", 1100.f, 655.f, 20.f, 25.f);
	//CUIFactory::Create<CUI>("enchant", L"UI", 150.f, 655.f, 20.f, 20.f);



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


	CScene::Free();
}
