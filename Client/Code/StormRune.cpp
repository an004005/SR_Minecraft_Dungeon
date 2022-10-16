#include "stdafx.h"
#include "..\Header\StormRune.h"

CStormRune::CStormRune(LPDIRECT3DDEVICE9 pGraphicDev): CRune(pGraphicDev)
{
	m_eTargetType = WEAPON_GLAIVE;
	m_iUItexNum = 18;
	m_eItemType = IT_RUNE;
}

CStormRune::~CStormRune()
{
}

HRESULT CStormRune::Ready_Object()
{
	m_pItemUI = CUIFactory::Create<CItemUI>("ItemUI", L"StormRuneUI", 0);
	m_pItemUI->SetUITexture(m_iUItexNum);

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
