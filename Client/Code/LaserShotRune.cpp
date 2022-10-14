#include "stdafx.h"
#include "..\Header\LaserShotRune.h"


CLaserShotRune::CLaserShotRune(LPDIRECT3DDEVICE9 pGraphicDev): CRune(pGraphicDev)
{
}

CLaserShotRune::~CLaserShotRune()
{
}

CLaserShotRune* CLaserShotRune::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLaserShotRune* pInstance = new CLaserShotRune(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
