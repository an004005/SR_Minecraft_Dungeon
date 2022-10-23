#include "stdafx.h"
#include "..\Header\Portrait.h"

CPortrait::CPortrait(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}


CPortrait::~CPortrait()
{
}


HRESULT CPortrait::Ready_Object()
{
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_pBufferCom = Add_Component<CRcTex>(L"Proto_RcTexCom", L"Proto_RcTexCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pTargetTexture = CTargetTexture::Create(m_pGraphicDev);
	return S_OK;
}

_int CPortrait::Update_Object(const _float& fTimeDelta)
{
	return CUI::Update_Object(fTimeDelta);
}

void CPortrait::Render_Object()
{
	if (m_bVisible == false) return;

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &CGameUtilMgr::s_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTargetTexture->Set_Texture();
	m_pBufferCom->Render_Buffer();

	if (m_wstrText.empty() == false)
	{
		const _vec3& vPos = m_pTransCom->m_vInfo[INFO_POS];
		const _float fX = vPos.x + WINCX / 2;
		const _float fY = -vPos.y + WINCY / 2;

		Render_Font(m_wstrFont, m_wstrText.c_str(), &_vec2{fX + m_vTextOffset.x, fY + m_vTextOffset.y}, m_TextColor);
	}
}

CPortrait* CPortrait::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum)
{
	return nullptr;
}

void CPortrait::Free()
{
	CUI::Free();
}
