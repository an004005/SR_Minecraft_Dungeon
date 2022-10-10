#include "stdafx.h"
#include "AbstFactory.h"
#include "Player.h"
#include "Terrain.h"
#include "Particle.h"
#include "StaticCamera.h"
#include "Monster.h"
#include "Geomancer.h"
#include "Zombie.h"
#include "Creeper.h"
#include "Skeleton.h"
#include "Enchanter.h"
#include "RedStoneCube.h"
#include "RedStoneMonstrosity.h"
#include "GeomancerWall.h"
#include "SphereEffect.h"
#include "RedStoneMonstrosityBullet.h"
#include "Crossbow.h"
#include "Sword.h"
#include "Glaive.h"
#include "Arrow.h"
#include "Axe.h"
#include "Box.h"
#include "Inventory.h"
#include "Dynamite.h"
#include "UI.h"
#include "Stage.h"
#include "AnimationTool.h"
#include "Loading.h"
#include "MapTool.h"
#include "TerrainCubeMap.h"
#include "Emerald.h"
#include "Apple.h"
#include "Bread.h"
#include "Posion.h"
#include "ArrowBundle.h"
#include "HPUI.h"
#include "CoolTimeUI.h"
#include "CameraTool.h"
#include "BatchTool.h"

LPDIRECT3DDEVICE9 CAbstFactory::s_pGraphicDev = nullptr;

map<string, std::function<CGameObject*()>> CPlayerFactory::s_mapPlayerSpawner;
map<string, std::function<CGameObject*()>> CEnemyFactory::s_mapEnemySpawner;
map<string, std::function<CGameObject*()>> CEffectFactory::s_mapEffectSpawner;
map<string, std::function<CGameObject*()>> CEnvFactory::s_mapEnvSpawner;
map<string, std::function<CGameObject*(_float)>> CBulletFactory::s_mapBulletSpawner;
map<string, std::function<CGameObject*()>> CObjectFactory::s_mapObjectSpawner;
map<string, std::function<CGameObject*()>> CItemFactory::s_mapItemSpawner;
map<string, std::function<CGameObject*(_uint)>> CUIFactory::s_mapUISpawner;

map<string, std::function<CScene*()>> CSceneFactory::s_mapLoadingSpawner;
map<string, std::function<CScene*()>> CSceneFactory::s_mapSceneSpawner;



void CAbstFactory::Ready_Factories(LPDIRECT3DDEVICE9 pGraphicDev)
{
	s_pGraphicDev = pGraphicDev;
	s_pGraphicDev->AddRef();

	CPlayerFactory::Ready_PlayerFactory();
	CEnemyFactory::Ready_EnemyFactory();
	CEffectFactory::Ready_EffectFactory();
	CEnvFactory::Ready_EnvFactory();
	CBulletFactory::Ready_BulletFactory();
	CObjectFactory::Ready_ObjectFactory();
	CItemFactory::Ready_ItemFactory();
	CUIFactory::Ready_UIFactory();
	CSceneFactory::Ready_SceneFactory();
}

void CPlayerFactory::Ready_PlayerFactory()
{
	s_mapPlayerSpawner.insert({"Steve", []()
	{
		return CPlayer::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/CubeMan/Steve.cube");
	}});
	s_mapPlayerSpawner.insert({"Pride", []()
	{
		return CPlayer::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/CubeMan/Pride.cube");
	}});
	s_mapPlayerSpawner.insert({"Eshe", []()
	{
		return CPlayer::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/CubeMan/Eshe.cube");
	}});
	s_mapPlayerSpawner.insert({"Copper", []()
	{
		return CPlayer::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/CubeMan/Copper.cube");
	}});
}

void CEnemyFactory::Ready_EnemyFactory()
{
	s_mapEnemySpawner.insert({"Zombie", []()
	{
		return CZombie::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/Zombie.cube");
	} });
	s_mapEnemySpawner.insert({ "Geomancer", []()
	{
	return CGeomancer::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/Geomancer.cube");
	} });
	s_mapEnemySpawner.insert({ "Creeper", []()
	{
		return CCreeper::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/Creeper.cube");
	} });
	s_mapEnemySpawner.insert({ "Skeleton", []()
	{
		return CSkeleton::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/Skeleton.cube");
	} });
	s_mapEnemySpawner.insert({ "Enchanter", []()
	{
		return CEnchanter::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/Enchanter.cube");
	} });
	s_mapEnemySpawner.insert({ "RedStoneCube", []()
	{
		return CRedStoneCube::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/RedStoneCube.cube");
	} });
	s_mapEnemySpawner.insert({ "RedStoneMonstrosity", []()
	{
		return CRedStoneMonstrosity::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/RedStoneMonstrosity.cube");
	} });
}

void CEffectFactory::Ready_EffectFactory()
{
	/**********************
	*	-ParticleSystem
	**********************/
	s_mapEffectSpawner.insert({ "3D_Base", []()
	{
		return C3DBaseTexture::Create(s_pGraphicDev, L"../Bin/Resource/Texture/JJH/3DView_Spotlight_Pattern.png");
	} });


	s_mapEffectSpawner.insert({ "Attack_Basic", []()
	{
		return CAttack_P::Create(s_pGraphicDev, L"../Bin/Resource/Texture/JJH/flare_alpha.dds");
	} });

	
	s_mapEffectSpawner.insert({ "Speed_Boots", []()
	{
		return CSpeedBoots::Create(s_pGraphicDev, L"../Bin/Resource/Texture/JJH/SpeedBoots_Particle.bmp");
	} });

	s_mapEffectSpawner.insert({ "Speed_Boots_Particle", []()
	{
		return CSpeedBoots_Particle::Create(s_pGraphicDev, L"../Bin/Resource/Texture/JJH/SpeedBoots_Particle.bmp");
	} });

	s_mapEffectSpawner.insert({ "FireWork_Fuze", []()
	{
		return CFireWork_Fuze::Create(s_pGraphicDev, L"../Bin/Resource/Texture/JJH/T_LinearGradient.png");
	} });

	s_mapEffectSpawner.insert({ "FireWork", []()
	{
		return CFireWork::Create(s_pGraphicDev, L"../Bin/Resource/Texture/JJH/bump.png");
	} });

	/**********************
	*		-Shader
	**********************/
	s_mapEffectSpawner.insert({ "Shock_Powder", []()
	{
		return CShock_Powder::Create(s_pGraphicDev);
	} });

	s_mapEffectSpawner.insert({ "ShockPowder_Cloud", []()
	{
		return CCloud::Create(s_pGraphicDev,1.3f,SHOCKPOWDER);
	} });

	s_mapEffectSpawner.insert({ "Creeper_Cloud", []()
	{
		return CCloud::Create(s_pGraphicDev,1.3f,CREEPEREX);
	} });

	s_mapEffectSpawner.insert({ "Walk_Cloud", []()
	{
		return CCloud::Create(s_pGraphicDev,0.4f,WALK);
	} });

	s_mapEffectSpawner.insert({ "Decal_Cloud", []()
	{
		return CCloud::Create(s_pGraphicDev,0.4f,DECAL);
	} });

	s_mapEffectSpawner.insert({ "Roll_Cloud", []()
	{
		return CCloud::Create(s_pGraphicDev,0.7f,ROLL);
	} });

	//

	s_mapEffectSpawner.insert({ "Golem_Spit_Sphere",[]()
	{
		return CSphereEffect::Create(s_pGraphicDev, 0.03f, GOLEM_SPIT);
	} });

	s_mapEffectSpawner.insert({ "Golem_Melee_L",[]()
	{
		return CSphereEffect::Create(s_pGraphicDev, 0.03f, GOLEM_MELEE_L);
	} });

	s_mapEffectSpawner.insert({ "Golem_Melee_M",[]()
	{
		return CSphereEffect::Create(s_pGraphicDev, 0.025f, GOLEM_MELEE_M);
	} });

	s_mapEffectSpawner.insert({ "Golem_Melee_S",[]()
	{
		return CSphereEffect::Create(s_pGraphicDev, 0.015f, GOLEM_MELEE_S);
	} });

	s_mapEffectSpawner.insert({ "Golem_Melee_Shpere_L",[]()
	{
		return CSphereEffect::Create(s_pGraphicDev, 0.02f, SPHERE_L);
	} });
	s_mapEffectSpawner.insert({ "Golem_Melee_Shpere_M",[]()
	{
		return CSphereEffect::Create(s_pGraphicDev, 0.0185f, SPHERE_M);
	} });

	//

	s_mapEffectSpawner.insert({ "Golem_Spit",[]()
	{
		return CGolemSpit::Create(s_pGraphicDev, 1.f);
	} });

	s_mapEffectSpawner.insert({ "Shock_Circle", []()
	{
		return CUVCircle::Create(s_pGraphicDev,3.f, SHOCK);
	} });

	s_mapEffectSpawner.insert({ "Firwork_Circle", []()
	{
		return CUVCircle::Create(s_pGraphicDev, 7.f, FIREWORK);
	} });

	s_mapEffectSpawner.insert({ "Creeper_Explosion", []()
	{
		return CUVCircle::Create(s_pGraphicDev, 3.7f, CREEPER);
	} });

	s_mapEffectSpawner.insert({ "Golem_Explosion", []()
	{
		return CUVCircle::Create(s_pGraphicDev, 7.f, GOLEM);
	} });

	s_mapEffectSpawner.insert({ "Red_Cube_Crack", []()
	{
		return CCrack::Create(s_pGraphicDev, 1.f, GOLEM_SPIT_CRACK);
	} });

	s_mapEffectSpawner.insert({ "Monster_Stun",[]()
	{
		return CStun::Create(s_pGraphicDev, 1.f);
	} });

	s_mapEffectSpawner.insert({ "Heal_Circle_R",[]()
	{
		return CHealCircle::Create(s_pGraphicDev, 1.4f, 90.f);
	} });

	s_mapEffectSpawner.insert({ "Heal_Circle_L",[]()
	{
		return CHealCircle::Create(s_pGraphicDev, 1.4f, 90.f);
	} });

	s_mapEffectSpawner.insert({ "Lava_Particle",[]()
	{
		return CLava_Particle::Create(s_pGraphicDev, 1.f, FALLINLAVA);
	} });

	s_mapEffectSpawner.insert({ "Fuze_Particle",[]()
	{
		return CLava_Particle::Create(s_pGraphicDev, 1.f, FUZEPARTICLE);
	} });

	s_mapEffectSpawner.insert({ "Exe_Decal",[]()
	{
		return CCrack::Create(s_pGraphicDev, 2.f, EXE_DECAL);
	} });

	s_mapEffectSpawner.insert({ "HeartParticle",[]()
	{
		return CHeartParticle::Create(s_pGraphicDev, 1.f);
	} });
}

void CEnvFactory::Ready_EnvFactory()
{
	s_mapEnvSpawner.insert({"DefaultTerrain", []()
	{
		return CTerrain::Create(s_pGraphicDev);
	}});
	s_mapEnvSpawner.insert({ "WaterTerrain", []()
	{
		return CTerrainWater::Create(s_pGraphicDev);
	} });

	s_mapEnvSpawner.insert({"StaticCamera", []()
	{
		return CStaticCamera::Create(s_pGraphicDev);
	}});
	
}

void CBulletFactory::Ready_BulletFactory()
{
	s_mapBulletSpawner.insert({"PlayerNormalArrow", [](_float fDamage)
	{
		return CArrow::Create(s_pGraphicDev, fDamage, COLL_PLAYER_BULLET);
	}});
	s_mapBulletSpawner.insert({"EnemyNormalArrow", [](_float fDamage)
	{
		return CArrow::Create(s_pGraphicDev, fDamage, COLL_ENEMY_BULLET);
	}});
	s_mapBulletSpawner.insert({"PlayerFireWorkArrow", [](_float fDamage)
	{
		return CArrow::Create(s_pGraphicDev, fDamage, COLL_PLAYER_BULLET, ARROW_FIREWORK);
	}});
}

void CObjectFactory::Ready_ObjectFactory()
{
	s_mapObjectSpawner.insert({"GeoWall_Normal", []()
	{
		return CGeomancerWall::Create(s_pGraphicDev, false);
	}});
	s_mapObjectSpawner.insert({"GeoWall_Boom", []()
	{
		return CGeomancerWall::Create(s_pGraphicDev, true);
	}});
	s_mapObjectSpawner.insert({ "RedStoneMonstrosityBullet", []()
	{	
		return CRedStoneMonstrosityBullet::Create(s_pGraphicDev);
	} });
	s_mapObjectSpawner.insert({ "Box", []()
	{
		return CBox::Create(s_pGraphicDev);
	} });
	s_mapObjectSpawner.insert({ "Inventory", []()
	{
		return CInventory::Create(s_pGraphicDev);
	} });
	s_mapObjectSpawner.insert({ "Dynamite", []()
	{
		return CDynamite::Create(s_pGraphicDev);
	} });

}

void CItemFactory::Ready_ItemFactory()
{
	s_mapItemSpawner.insert({ "Crossbow", []()
	{
		return CCrossbow::Create(s_pGraphicDev);
	} });
	s_mapItemSpawner.insert({ "Sword", []()
	{
		return CSword::Create(s_pGraphicDev);
	} });
	s_mapItemSpawner.insert({ "Glaive", []()
	{
		return CGlaive::Create(s_pGraphicDev);
	} });
	s_mapItemSpawner.insert({ "Axe", []()
	{
		return CAxe::Create(s_pGraphicDev);
	} });
	s_mapItemSpawner.insert({ "Emerald", []()
	{
		return CEmerald::Create(s_pGraphicDev);
	} });
	s_mapItemSpawner.insert({ "Apple", []()
	{
		return CApple::Create(s_pGraphicDev);
	} });
	s_mapItemSpawner.insert({ "Bread", []()
	{
		return CBread::Create(s_pGraphicDev);
	} });
	s_mapItemSpawner.insert({ "ArrowBundle", []()
	{
		return CArrowBundle::Create(s_pGraphicDev);
	} });
	s_mapItemSpawner.insert({ "Posion", []()
	{
		return CPosion::Create(s_pGraphicDev);
	} });
}

void CUIFactory::Ready_UIFactory()
{
	 s_mapUISpawner.insert({ "DefaultUI", [](_uint iTexNum)
	 {
	 	return CUI::Create(s_pGraphicDev, iTexNum);
	 } });
	 s_mapUISpawner.insert({ "HPUI", [](_uint iTexNum)
	 {
	 	return CHPUI::Create(s_pGraphicDev, -1);//not used
	 } });
	 s_mapUISpawner.insert({ "PotionCoolTime", [](_uint iTexNum)
	 {
	 	return CCoolTimeUI::Create(s_pGraphicDev, -1, CoolTimeTarget::POTION);
	 } });
	 s_mapUISpawner.insert({ "RollCoolTime", [](_uint iTexNum)
	 {
	 	return CCoolTimeUI::Create(s_pGraphicDev, -1, CoolTimeTarget::ROLL);
	 } });

	 s_mapUISpawner.insert({ "Legacy1CoolTime", [](_uint iTexNum)
	 {
	 	return CCoolTimeUI::Create(s_pGraphicDev, -1, CoolTimeTarget::LEGACY1);
	 } });
	 s_mapUISpawner.insert({ "Legacy2CoolTime", [](_uint iTexNum)
	 {
	 	return CCoolTimeUI::Create(s_pGraphicDev, -1, CoolTimeTarget::LEGACY2);
	 } });
	 s_mapUISpawner.insert({ "Legacy3CoolTime", [](_uint iTexNum)
	 {
	 	return CCoolTimeUI::Create(s_pGraphicDev, -1, CoolTimeTarget::LEGACY3);
	 } });

	 s_mapUISpawner.insert({ "ArrowUI", [](_uint iTexNum)
	 {
	 	return CCountUI::Create(s_pGraphicDev, CNT_ARROW);
	 } });
	 s_mapUISpawner.insert({ "EmeraldUI", [](_uint iTexNum)
	 {
	 	return CCountUI::Create(s_pGraphicDev, CNT_EMERALD);
	 } });
}

void CSceneFactory::Ready_SceneFactory()
{
	// loading scene
	{
		s_mapLoadingSpawner.insert({"Loading1", []()
		{
			return CLoading::Create(s_pGraphicDev);
		}});
	}

	// scene
	{
		s_mapSceneSpawner.insert({"Stage_Default", []()
		{
			return CStage::Create(s_pGraphicDev);
		}});
		s_mapSceneSpawner.insert({"Animation Tool", []()
		{
			return CAnimationTool::Create(s_pGraphicDev);
		}});
		s_mapSceneSpawner.insert({"Map Tool", []()
		{
			return CMapTool::Create(s_pGraphicDev);
		}});

		s_mapSceneSpawner.insert({ "Camera Tool", []()
		{
			return CCameraTool::Create(s_pGraphicDev);
		} });
		s_mapSceneSpawner.insert({"Batch Tool", []()
		{
			return CBatchTool::Create(s_pGraphicDev);
		}});
	}
}
