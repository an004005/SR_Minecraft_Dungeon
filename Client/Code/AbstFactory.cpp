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
#include "RedStoneMonstrosityBullet.h"
#include "Crossbow.h"
#include "Sword.h"
#include "Glaive.h"
#include "Arrow.h"
#include "Axe.h"



LPDIRECT3DDEVICE9 CAbstFactory::s_pGraphicDev = nullptr;

map<string, std::function<CGameObject*()>> CPlayerFactory::s_mapPlayerSpawner;
map<string, std::function<CGameObject*()>> CEnemyFactory::s_mapEnemySpawner;
map<string, std::function<CGameObject*()>> CEffectFactory::s_mapEffectSpawner;
map<string, std::function<CGameObject*()>> CEnvFactory::s_mapEnvSpawner;
map<string, std::function<CGameObject*(_float)>> CBulletFactory::s_mapBulletSpawner;
map<string, std::function<CGameObject*()>> CObjectFactory::s_mapObjectSpawner;
map<string, std::function<CGameObject*()>> CItemFactory::s_mapItemSpawner;


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

	s_mapEffectSpawner.insert({ "Roll_Cloud", []()
	{
		return CCloud::Create(s_pGraphicDev,0.7f,ROLL);
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
		return CUVCircle::Create(s_pGraphicDev, 3.5f, CREEPER);
	} });

	s_mapEffectSpawner.insert({ "Golem_Explosion", []()
	{
		return CUVCircle::Create(s_pGraphicDev, 7.f, GOLEM);
	} });

}

void CEnvFactory::Ready_EnvFactory()
{
	s_mapEnvSpawner.insert({"DefaultTerrain", []()
	{
		return CTerrain::Create(s_pGraphicDev);
	}});
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
}
