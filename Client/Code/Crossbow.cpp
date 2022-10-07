#include "stdafx.h"
#include "..\Header\Crossbow.h"
#include "SkeletalCube.h"

CCrossbow::CCrossbow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipItem(pGraphicDev)
{
}

CCrossbow::~CCrossbow()
{
}

HRESULT CCrossbow::Ready_Object()
{
	m_pBufferCom = Add_Component<CVoxelTex>(L"Proto_VoxelTex_Crossbow", L"Proto_VoxelTex_Crossbow", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_WeaponTexture", L"Proto_WeaponTexture", ID_STATIC);
	return S_OK;
}

_int CCrossbow::Update_Object(const _float & fTimeDelta)
{
	CEquipItem::Update_Object(fTimeDelta);
	return 0;
}

void CCrossbow::LateUpdate_Object()
{
	CEquipItem::LateUpdate_Object();
}

void CCrossbow::Render_Object()
{
	/*m_pTextureCom->Set_Texture(2);
	m_pBufferCom->Render_Buffer();*/
}

CCrossbow * CCrossbow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossbow* pInstance = new CCrossbow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCrossbow::Free()
{
	CEquipItem::Free();
}

void CCrossbow::Equipment(SkeletalPart* pSkeletalPart)
{
	pSkeletalPart->pBuf = m_pBufferCom;
	pSkeletalPart->pTex = m_pTextureCom;
	pSkeletalPart->iTexIdx = 2;
}
