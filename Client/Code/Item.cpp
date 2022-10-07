#include "stdafx.h"
#include "..\Header\Item.h"


CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_Object()
{
	return E_NOTIMPL;
}

_int CItem::Update_Object(const _float & fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CItem::LateUpdate_Object()
{
	Engine::CGameObject::LateUpdate_Object();
}

void CItem::Render_Object()
{
}

//CItem * CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
//{
//	CItem* pInstance = new CItem(pGraphicDev);
//
//	if (FAILED(pInstance->Ready_Object()))
//	{
//		Safe_Release(pInstance);
//		return nullptr;
//	}
//
//	return pInstance;
//}

void CItem::Free()
{
	CGameObject::Free();
}
