#include "stdafx.h"
#include "AbstFactory.h"
#include "Player.h"
#include "Terrain.h"
#include "Particle.h"
#include "StaticCamera.h"
#include "Monster.h"
#include "Geomancer.h"
#include "UI.h"

LPDIRECT3DDEVICE9 CAbstFactory::s_pGraphicDev = nullptr;

map<string, std::function<CGameObject*()>> CPlayerFactory::s_mapPlayerSpawner;
map<string, std::function<CGameObject*()>> CEnemyFactory::s_mapEnemySpawner;
map<string, std::function<CGameObject*()>> CEffectFactory::s_mapEffectSpawner;
map<string, std::function<CGameObject*()>> CEnvFactory::s_mapEnvSpawner;
map<string, std::function<CGameObject*()>> CBulletFactory::s_mapBulletSpawner;
map<string, std::function<CGameObject*()>> CUIFactory::s_mapUISpawner;

void CAbstFactory::Ready_Factories(LPDIRECT3DDEVICE9 pGraphicDev)
{
	s_pGraphicDev = pGraphicDev;
	s_pGraphicDev->AddRef();

	CPlayerFactory::Ready_PlayerFactory();
	CEnemyFactory::Ready_EnemyFactory();
	CEffectFactory::Ready_EffectFactory();
	CEnvFactory::Ready_EnvFactory();
	CBulletFactory::Ready_BulletFactory();
	CUIFactory::Ready_UIFactory();
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
	s_mapEnemySpawner.insert({"TestZombie", []()
	{
		return CMonster::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/Zombie.cube");
	}});
		s_mapEnemySpawner.insert({"Geomancer", []()
	{
		return CGeomancer::Create(s_pGraphicDev, L"../Bin/Resource/SkeletalCube/Monster/Geomancer.cube");
	}});
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
		return CAttack_P::Create(s_pGraphicDev, L"../Bin/Resource/Texture/JJH/aac1e6-corona.png");
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

	s_mapEffectSpawner.insert({ "Texture_Cloud", []()
	{
		return CCloud::Create(s_pGraphicDev);
	} });

	s_mapEffectSpawner.insert({ "UV_Circle", []()
	{
		return CUVCircle::Create(s_pGraphicDev,3.f, SHOCK);
	} });

	s_mapEffectSpawner.insert({ "Firwork_Circle", []()
	{
		return CUVCircle::Create(s_pGraphicDev, 7.f, FIREWORK);
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

void CUIFactory::Ready_UIFactory()
{
	s_mapUISpawner.insert({ "UITexture", []()
	{
		return CUI::Create(s_pGraphicDev, 0);
	} });

	s_mapUISpawner.insert({ "UI_HP", []()
	{
		return CUI::Create(s_pGraphicDev, 1);
	} });

	s_mapUISpawner.insert({ "UI_MinusHP", []()
	{
		return CUI::Create(s_pGraphicDev, 1);
	} });

	s_mapUISpawner.insert({ "Inven", []()
	{
		return CUI::Create(s_pGraphicDev, 2);
	} });

	s_mapUISpawner.insert({ "InvenTool", []()
	{
		return CUI::Create(s_pGraphicDev, 4);
	} });

	s_mapUISpawner.insert({ "InvenToolPos", []()
	{
		return CUI::Create(s_pGraphicDev, 13);
	} });

	s_mapUISpawner.insert({ "arrowTool", []()
	{
		return CUI::Create(s_pGraphicDev, 3);
	} });

	s_mapUISpawner.insert({ "arrow", []()
	{
		return CUI::Create(s_pGraphicDev, 9);
	} });

	s_mapUISpawner.insert({ "Buffe1", []()
	{
		return CUI::Create(s_pGraphicDev, 4);
	} });

	s_mapUISpawner.insert({ "Buffe2", []()
	{
		return CUI::Create(s_pGraphicDev, 4);
	} });

	s_mapUISpawner.insert({ "Buffe3", []()
	{
		return CUI::Create(s_pGraphicDev, 4);
	} });

	s_mapUISpawner.insert({ "Buffe4", []()
	{
		return CUI::Create(s_pGraphicDev, 4);
	} });

	s_mapUISpawner.insert({ "RollTool", []()
	{
		return CUI::Create(s_pGraphicDev,10);
	} });

	s_mapUISpawner.insert({ "Roll", []()
	{
		return CUI::Create(s_pGraphicDev,5);
	} });

	s_mapUISpawner.insert({ "map", []()
	{
		return CUI::Create(s_pGraphicDev, 6);
	} });

	s_mapUISpawner.insert({ "mapTool", []()
	{
		return CUI::Create(s_pGraphicDev, 4);
	} });

	s_mapUISpawner.insert({ "emerald", []()
	{
		return CUI::Create(s_pGraphicDev, 8);
	} });

	s_mapUISpawner.insert({ "enchant", []()
	{
		return CUI::Create(s_pGraphicDev, 11);
	} });

}
