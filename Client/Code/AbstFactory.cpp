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
#include "GeomancerWall.h"
#include "SphereEffect.h"

LPDIRECT3DDEVICE9 CAbstFactory::s_pGraphicDev = nullptr;

map<string, std::function<CGameObject*()>> CPlayerFactory::s_mapPlayerSpawner;
map<string, std::function<CGameObject*()>> CEnemyFactory::s_mapEnemySpawner;
map<string, std::function<CGameObject*()>> CEffectFactory::s_mapEffectSpawner;
map<string, std::function<CGameObject*()>> CEnvFactory::s_mapEnvSpawner;
map<string, std::function<CGameObject*()>> CBulletFactory::s_mapBulletSpawner;
map<string, std::function<CGameObject*()>> CObjectFactory::s_mapObjectSpawner;

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
	s_mapEnvSpawner.insert({"StaticCamera", []()
	{
		return CStaticCamera::Create(s_pGraphicDev);
	}});
}

void CBulletFactory::Ready_BulletFactory()
{
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
}
