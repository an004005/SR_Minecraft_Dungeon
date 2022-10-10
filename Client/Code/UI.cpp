#include "stdafx.h"
#include "..\Header\UI.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUI::~CUI()
{
}

HRESULT CUI::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_pBufferCom = Add_Component<CRcTex>(L"Proto_RcTexCom", L"Proto_RcTexCom", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_UI_Texture", L"Proto_UI_Texture", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);

	return S_OK;
}

_int CUI::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CUI::LateUpdate_Object(void)
{
}

void CUI::Render_Object(void)
{
	// Render_Font(L"Font_Jinji", L"10", &_vec2(955.f, 630.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	// Render_Font(L"Font_Jinji", L"0", &_vec2(1110.f, 640.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	// old view, proj 복원은 renderer에 있음
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &CGameUtilMgr::s_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iTexNum);
	m_pBufferCom->Render_Buffer();
}

CUI * CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum)
{
	CUI*		pInstance = new CUI(pGraphicDev);
	pInstance->m_iTexNum = iTexNum;

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUI::Free(void)
{
	CGameObject::Free();
}
