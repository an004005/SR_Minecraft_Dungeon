#include "stdafx.h"
#include "..\Header\UI.h"
#include "Inventory.h"

/*------------------------
 * CUI
 ------------------------*/
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

_int CUI::Update_Object(const _float& fTimeDelta)
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
	// Render_Font(L"Font_Jinji", m_wstrText.c_str(), &_vec2(955.f, 630.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	// Render_Font(L"Font_Jinji", L"0", &_vec2(1110.f, 640.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	// old view, proj ������ renderer�� ����

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &CGameUtilMgr::s_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iTexNum);
	m_pBufferCom->Render_Buffer();
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum)
{
	CUI* pInstance = new CUI(pGraphicDev);
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

/*------------------------
 * CCountUI
 ------------------------*/
CCountUI::CCountUI(LPDIRECT3DDEVICE9 pGraphicDev): CUI(pGraphicDev)
{
}

CCountUI::~CCountUI()
{
}

HRESULT CCountUI::Ready_Object()
{
	FAILED_CHECK_RETURN(CUI::Ready_Object(), E_FAIL);


	return S_OK;
}

_int CCountUI::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);

	switch (m_eType)
	{
	case CNT_EMERALD:
		m_iCnt = Get_GameObject<CInventory>(LAYER_GAMEOBJ, L"Inventory")->GetEmeraldCnt();
		m_iTexNum = 8;
		break;
	case CNT_ARROW:
		m_iCnt = Get_GameObject<CInventory>(LAYER_GAMEOBJ, L"Inventory")->GetArrowCnt();
		m_iTexNum = 9;
		break;
	case CNT_END:
		_CRASH("Init CountUI failed?");
		break;
	default: ;
	}


	return OBJ_NOEVENT;
}

void CCountUI::Render_Object()
{
	CUI::Render_Object();

	const _vec3& vPos = m_pTransCom->m_vInfo[INFO_POS];
	Render_Font(L"Font_Default_Normal", to_wstring(m_iCnt).c_str(), 
		&_vec2(vPos.x + WINCX/2 + 10, -vPos.y + WINCY/2 - 20), 
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CCountUI* CCountUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, CountUIType eType)
{
	CCountUI* pInstance = new CCountUI(pGraphicDev);
	pInstance->m_eType = eType;

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
