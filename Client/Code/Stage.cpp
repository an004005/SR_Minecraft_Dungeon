#include "stdafx.h"
#include "..\Header\Stage.h"

#include "StaticCamera.h"
#include "Terrain.h"
#include "Player.h"
#include "AbstFactory.h"

#include "StatComponent.h"
#include "DynamicCamera.h"
#include "Particle.h"
#include "Box.h"
#include "Dynamite.h"

//controller
#include "PlayerController.h"
#include "GeomancerController.h"
#include "ZombieController.h"
#include "CreeperController.h"
#include "SkeletonController.h"
#include "EnchanterController.h"
#include "RedStoneCubeController.h"
#include "RedStoneMonstrosityController.h"

//monster
#include "Monster.h"
#include "Geomancer.h"
#include "Zombie.h"
#include "Creeper.h"
#include "Skeleton.h"
#include "Enchanter.h"
#include "RedStoneCube.h"
#include "RedStoneMonstrosity.h"

//item
#include "Crossbow.h"
#include "Sword.h"
#include "Glaive.h"
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

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);

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
	
		
	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic()
{
	_matrix matWorld;


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

		//CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 3.f, 0.f ,13.f });
		//CEnemyFactory::Create<CSkeleton>("Skeleton", L"Skeleton", matWorld);

		//CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 47.f, 0.f ,17.f });
		//CEnemyFactory::Create<CEnchanter>("Enchanter", L"Enchanter", matWorld);

	
	/*	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.5f, 1.5f, 1.5f }, { 0.f, D3DXToRadian(180.f) ,0.f }, { 4.f, 0.f ,10.f });
		CEnemyFactory::Create<CRedStoneMonstrosity>("RedStoneMonstrosity", L"RedStoneMonstrosity", matWorld);*/
	}
	





	return S_OK;
}

HRESULT CStage::Ready_Layer_UI()
{
	CGameObject*		pGameObject = nullptr;

	return S_OK;
}

HRESULT CStage::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 155)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", Engine::CTransform::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossCubeTile", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/BossCubeTile/boss_%d.dds", TEX_CUBE, 12)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/weapon/weapon_%d.png", TEX_NORMAL, 3)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Electric", Engine::CTexture::Create(m_pGraphicDev, L"T_ElectricArcs.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cloud", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/T_Smoke_deformedBall_Small.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Circle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/T_fullcircle_explosion_124_spritesheet.png", TEX_NORMAL)), E_FAIL);

	// FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcShaderCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.0f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcShaderCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx",_vec2(0.0f,0.0f),_vec2(0.25f,0.f),_vec2(0.25f,0.25f),_vec2(0.0f,0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShockPowderCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/ShockPowder_Cloud.fx",_vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CreeperExCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/CreeperEx_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WalkCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Walk_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RollCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Roll_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderCircleCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Circle.fx",_vec2(0.0f, 0.0f), _vec2(0.25f, 0.0f), _vec2(0.25f, 0.5f), _vec2(0.0f, 0.5f))), E_FAIL);
	


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Sword", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_0.png", 0.08f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Glaive", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_1.png", 0.08f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Crossbow", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_2.png", 0.08f)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainCubeTexCom",CTerrainCubeTex::Create(m_pGraphicDev, L"../Bin/Resource/Map/MapTest.dat", 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CollisionCom", CCollisionCom::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StatCom", CStatComponent::Create()), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlantTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Plant/plant_%d.png", TEX_NORMAL, 3)), E_FAIL);


	// controller
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerController", CPlayerController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GeomancerController", CGeomancerController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ZombieController", CZombieController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CreeperController", CCreeperController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkeletonController", CSkeletonController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EnchanterController", CEnchanterController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RedStoneCubeController", CRedStoneCubeController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RedStoneMonstrosityController", CRedStoneMonstrosityController::Create()), E_FAIL);




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
