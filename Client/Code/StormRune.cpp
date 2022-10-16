#include "stdafx.h"
#include "..\Header\StormRune.h"

CStormRune::CStormRune(LPDIRECT3DDEVICE9 pGraphicDev): CRune(pGraphicDev)
{
	m_eTargetType = WEAPON_GLAIVE;
}

CStormRune::~CStormRune()
{
}

HRESULT CStormRune::Ready_Object()
{
	return CRune::Ready_Object();
}

_int CStormRune::Update_Object(const _float& fTimeDelta)
{
	return CRune::Update_Object(fTimeDelta);
}

void CStormRune::LateUpdate_Object()
{
	CRune::LateUpdate_Object();
}

void CStormRune::Render_Object()
{
	CRune::Render_Object();
}

void CStormRune::Free()
{
	CRune::Free();
}

void CStormRune::Collision()
{
	CRune::Collision();
}

void CStormRune::Use()
{
	CRune::Use();
}

CStormRune* CStormRune::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStormRune* pInstance = new CStormRune(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
