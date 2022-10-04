#include "stdafx.h"
#include "..\Header\Arrow.h"


CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev): CGameObject(pGraphicDev)
{
}

CArrow::CArrow(const CArrow& rhs): CGameObject(rhs)
{
}

CArrow::~CArrow()
{
}

HRESULT CArrow::Ready_Object()
{
	CGameObject::Ready_Object();

	m_pTexture = Add_Component<CTexture>(L"Proto_ArrowTexture", L"Proto_ArrowTexture", ID_STATIC);
	m_pTransform = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pBuffer = Add_Component<CArrowCube>(L"Proto_ArrowTex", L"Proto_ArrowTex", ID_STATIC);
	return S_OK;
}

_int CArrow::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_PRIORITY, this);
	
	return OBJ_NOEVENT;
}

void CArrow::Render_Object()
{
	m_pTexture->Set_Texture();
	m_pBuffer->Render_Buffer();
}

void CArrow::Free()
{
	CGameObject::Free();
}

CArrow* CArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CArrow* pInstance = new CArrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}
