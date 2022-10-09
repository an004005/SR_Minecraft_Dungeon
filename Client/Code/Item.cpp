#include "stdafx.h"
#include "..\Header\Item.h"


CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_fPower(0.f)
	, m_fTime(0.f)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_Object()
{
	return S_OK;
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


void CItem::Free()
{
	CGameObject::Free();
}

void CItem::Parabola(_vec3 & vPos,const _float& fHeight,const _float& fTimeDelta)
{
	if (vPos.y <= fHeight)
	{
		vPos.y = fHeight;
	}
	else
	{
		vPos.y += 15.f * m_fTime * fTimeDelta - (9.8f * m_fTime * m_fTime * fTimeDelta * 0.5f);
		vPos.x += 3.f * m_vVelocity.x * fTimeDelta;
		vPos.z += 3.f * m_vVelocity.z * fTimeDelta;
		m_fTime += 7.f * fTimeDelta;
	}
}
