#include "stdafx.h"
#include "LightningRune.h"
#include "Weapon.h"

CLightningRune::CLightningRune(LPDIRECT3DDEVICE9 pGraphicDev): CRune(pGraphicDev)
{
	m_eTargetType = WEAPON_SWORD;
}

CLightningRune::~CLightningRune()
{
}

HRESULT CLightningRune::Ready_Object()
{
	return CRune::Ready_Object();
}

_int CLightningRune::Update_Object(const _float& fTimeDelta)
{
	return CRune::Update_Object(fTimeDelta);
}

void CLightningRune::LateUpdate_Object()
{
	CRune::LateUpdate_Object();
}

void CLightningRune::Free()
{
	CRune::Free();
}

void CLightningRune::Use()
{
	CRune::Use();
}

CLightningRune* CLightningRune::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightningRune* pInstance = new CLightningRune(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
