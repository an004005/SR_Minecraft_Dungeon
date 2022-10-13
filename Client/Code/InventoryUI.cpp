#include "stdafx.h"
#include "..\Header\InventoryUI.h"
#include "AbstFactory.h"
#include "Inventory.h"
/*------------------------
* BackGround
------------------------*/
CInventoryUI::CInventoryUI(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CInventoryUI::~CInventoryUI()
{
}

HRESULT CInventoryUI::Ready_Object()
{

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_pBufferCom = Add_Component<CRcTex>(L"Proto_RcTexCom", L"Proto_RcTexCom", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_InventoryUI_Texture", L"Proto_InventoryUI_Texture", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);

	m_iRenderPriority = 9;
	return S_OK;
}

_int CInventoryUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bClose)
		return OBJ_NOEVENT;

	CUI::Update_Object(fTimeDelta);

	m_pTransCom->Set_Scale(WINCX, WINCY, 0.f);

	

	return OBJ_NOEVENT;
}

void CInventoryUI::Render_Object()
{
	CUI::Render_Object();
}

void CInventoryUI::Free()
{
	CUI::Free();
}

CInventoryUI * CInventoryUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum)
{
	CInventoryUI* pInstance = new CInventoryUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

/*------------------------
* ItemSpace
------------------------*/
CItemSpaceUI::CItemSpaceUI(LPDIRECT3DDEVICE9 pGraphicDev) : CInventoryUI(pGraphicDev)
{
}

CItemSpaceUI::~CItemSpaceUI()
{
}

HRESULT CItemSpaceUI::Ready_Object()
{
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_pBufferCom = Add_Component<CRcTex>(L"Proto_RcTexCom", L"Proto_RcTexCom", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_InventoryUI_Texture", L"Proto_InventoryUI_Texture", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);

	

	m_iRenderPriority = 8;
	return S_OK;
}

_int CItemSpaceUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bWorldSet == false)
	{
		_vec3 vScale = m_pTransCom->m_vScale;
		_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
		_vec3 vAngle = m_pTransCom->m_vAngle;
		vScale.x *= 0.8f;
		vScale.y *= 0.8f;
		CGameUtilMgr::MatWorldComposeEuler(m_matIconWorld, vScale, vAngle, vPos);

		m_bWorldSet = true;
	}

	if (m_MouseCollFrameWorldSet == false)
	{
		_vec3 vScale = m_pTransCom->m_vScale;
		_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
		_vec3 vAngle = m_pTransCom->m_vAngle;
		vScale.x *= 1.1f;
		vScale.y *= 1.1f;
		CGameUtilMgr::MatWorldComposeEuler(m_matMouseCollFrameWorld, vScale, vAngle, vPos);

		m_MouseCollFrameWorldSet = true;
	}

	if (m_CurClickFrameWorldSet == false)
	{
		_vec3 vScale = m_pTransCom->m_vScale;
		_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
		_vec3 vAngle = m_pTransCom->m_vAngle;
		vScale.x *= 1.1f;
		vScale.y *= 1.1f;
		CGameUtilMgr::MatWorldComposeEuler(m_matCurClickFrameWorld, vScale, vAngle, vPos);

		m_CurClickFrameWorldSet = true;
	}
	
	if (m_bClose)
		return OBJ_NOEVENT;


	CUI::Update_Object(fTimeDelta);
	return OBJ_NOEVENT;
}

void CItemSpaceUI::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &CGameUtilMgr::s_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	switch (m_eRenderType)
	{
	case SLOT_BASE:
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		m_pTextureCom->Set_Texture(m_iBaseSlot);
		m_pBufferCom->Render_Buffer();
	}
	break;
	case SLOT_WEAPON:
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		m_pTextureCom->Set_Texture(m_iWeaponSlot);
		m_pBufferCom->Render_Buffer();
	}
	break;
	case SLOT_LEGACY:
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		m_pTextureCom->Set_Texture(m_iLegacySlot);
		m_pBufferCom->Render_Buffer();
	}
	break;
	case SLOT_TYPE_END:
		break;
	}

	if (m_iIconTexNum != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matIconWorld);
		m_pTextureCom->Set_Texture(m_iIconTexNum);
		m_pBufferCom->Render_Buffer();
		m_iIconTexNum = 0;
	}

	if (m_bMouseCollFrame)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matMouseCollFrameWorld);
		m_pTextureCom->Set_Texture(4);
		m_pBufferCom->Render_Buffer();
		m_bMouseCollFrame = false;
	}

	if (m_bCurClickFrame)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matCurClickFrameWorld);
		m_pTextureCom->Set_Texture(5);
		m_pBufferCom->Render_Buffer();
	}
}

void CItemSpaceUI::Free()
{
	
	CInventoryUI::Free();
}

CItemSpaceUI * CItemSpaceUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum)
{
	CItemSpaceUI* pInstance = new CItemSpaceUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}