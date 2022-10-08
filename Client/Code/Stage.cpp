#include "stdafx.h"
#include "..\Header\Stage.h"

#include "StaticCamera.h"
#include "Terrain.h"
#include "Player.h"
#include "AbstFactory.h"

#include "StatComponent.h"
#include "DynamicCamera.h"
#include "Particle.h"

//controller
#include "PlayerController.h"
#include "GeomancerController.h"
#include "ZombieController.h"
#include "CreeperController.h"
#include "SkeletonController.h"
#include "EnchanterController.h"

//monster
#include "Monster.h"
#include "Geomancer.h"
#include "Zombie.h"
#include "Creeper.h"
#include "Skeleton.h"
#include "Enchanter.h"

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
	pGameObject = CTerrainCubeMap::Create(m_pGraphicDev, L"../Bin/Resource/Map/Stage2.map");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"TerrainCubeMap", pGameObject), E_FAIL);
	
		
	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic()
{
	_matrix matWorld;

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
		/*CGameUtilMgr::MatWorldComposeEuler(matWorld, {1.f, 1.f, 1.f}, {0.f, D3DXToRadian(90.f) ,0.f }, {3.f, 0.f ,3.f});
		CEnemyFactory::Create<CGeomancer>("Geomancer", L"Geomancer", matWorld);*/

		/*CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 3.f, 0.f ,6.f });
		CEnemyFactory::Create<CZombie>("Zombie", L"Zombie", matWorld);*/

		// CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 3.f, 0.f ,13.f });
		// CEnemyFactory::Create<CCreeper>("Creeper", L"Creeper", matWorld);

		/*CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 3.f, 0.f ,13.f });
		CEnemyFactory::Create<CSkeleton>("Skeleton", L"Skeleton", matWorld);*/

		// CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, D3DXToRadian(90.f) ,0.f }, { 3.f, 0.f ,13.f });
		// CEnemyFactory::Create<CEnchanter>("Enchanter", L"Enchanter", matWorld);
	}
	
	CGameUtilMgr::MatWorldComposeEuler(matWorld, {1.f, 1.f, 1.f}, {0.f, D3DXToRadian(90.f) ,0.f }, {6.f, 0.f ,6.f});
	CEnemyFactory::Create<CGeomancer>("Geomancer", L"Geomancer", matWorld);
	// CEnemyFactory::Create<CMonster>("TestZombie", L"TestZombie", matWorld);




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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 155)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossCubeTile", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/BossCubeTile/boss_%d.dds", TEX_CUBE, 12)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/weapon/weapon_%d.png", TEX_NORMAL, 3)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Electric", CTexture::Create(m_pGraphicDev, L"T_ElectricArcs.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cloud", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/T_Smoke_deformedBall_Small.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Circle", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/T_fullcircle_explosion_124_spritesheet.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Spit", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/3DView_Spotlight_Pattern.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Crack", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Crack1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Stun", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Stun.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Heal", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Heal_Circle1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Heal_Particle", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Heart_Particle.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Lava_Particle", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Lava_Particlel.png", TEX_NORMAL)), E_FAIL);

	// FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcShaderCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.0f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcShaderCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx",_vec2(0.0f,0.0f),_vec2(0.25f,0.f),_vec2(0.25f,0.25f),_vec2(0.0f,0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShockPowderCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/ShockPowder_Cloud.fx",_vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CreeperExCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/CreeperEx_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WalkCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Walk_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RollCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Roll_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereCom_Ring", CSphereMesh::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Half_Shpere.fx", L"../Bin/Resource/Texture/JJH/Test.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereCom_Lava", CSphereMesh::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Half_Shpere.fx", L"../Bin/Resource/Texture/JJH/Test.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereCom_L", CSphereMesh::Create(m_pGraphicDev, L"../Bin/Resource/Shader/TextureMapping.fx", L"../Bin/Resource/Texture/JJH/YellowSphere.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereCom_M", CSphereMesh::Create(m_pGraphicDev, L"../Bin/Resource/Shader/TextureMapping.fx", L"../Bin/Resource/Texture/JJH/YellowSphere.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StunCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx", _vec2(0.0f, 0.0f), _vec2(1.f, 0.0f), _vec2(1.f, 1.0f), _vec2(0.0f, 1.f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HealCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Crack.fx", _vec2(0.0f, 0.0f), _vec2(1.f, 0.0f), _vec2(1.f, 1.0f), _vec2(0.0f, 1.f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HeartParticleCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Crack.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 1.0f), _vec2(0.0f, 1.f))), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Lava_ParticleCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Crack.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.0f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CrackCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Crack.fx", _vec2(0.0f, 0.0f), _vec2(0.5f, 0.0f), _vec2(0.5f, 0.5f), _vec2(0.0f, 0.5f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Crack_DecalCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Decal.fx", _vec2(0.0f, 0.0f), _vec2(0.5f, 0.0f), _vec2(0.5f, 0.5f), _vec2(0.0f, 0.5f))), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderCircleCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Circle.fx",_vec2(0.0f, 0.0f), _vec2(0.25f, 0.0f), _vec2(0.25f, 0.5f), _vec2(0.0f, 0.5f))), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpitCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Golem_Spit.fx", _vec2(0.0f, 0.0f), _vec2(1.f, 0.0f), _vec2(1.f, 1.f), _vec2(0.0f, 1.f))), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Sword", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_0.png", 0.08f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Crossbow", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_2.png", 0.12f)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainCubeTexCom",CTerrainCubeTex::Create(m_pGraphicDev, L"../Bin/Resource/Map/MapTest.dat", 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CollisionCom", CCollisionCom::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StatCom", CStatComponent::Create()), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlantTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Plant/plant_%d.png", TEX_NORMAL, 3)), E_FAIL);


	// controller
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerController", CPlayerController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GeomancerController", CGeomancerController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ZombieController", CZombieController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CreeperController", CCreeperController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkeletonController", CSkeletonController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EnchanterController", CEnchanterController::Create()), E_FAIL);




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
