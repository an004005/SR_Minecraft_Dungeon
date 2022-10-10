#include "stdafx.h"
#include "..\Header\FireworksArrow.h"


CFireworksArrow::CFireworksArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipItem(pGraphicDev)
{
}


CFireworksArrow::~CFireworksArrow()
{
}

HRESULT CFireworksArrow::Ready_Object()
{

	return S_OK;
}

_int CFireworksArrow::Update_Object(const _float & fTimeDelta)
{
	return OBJ_DEAD;




	return OBJ_NOEVENT;
	CEquipItem::Update_Object(fTimeDelta);
}

void CFireworksArrow::LateUpdate_Object()
{
	CEquipItem::LateUpdate_Object();
}

void CFireworksArrow::Render_Object()
{
}

void CFireworksArrow::Free()
{
	CEquipItem::Free();
}

CFireworksArrow * CFireworksArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFireworksArrow* pInstance = new CFireworksArrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFireworksArrow::Equipment(SkeletalPart * pSkeletalPart)
{
}
