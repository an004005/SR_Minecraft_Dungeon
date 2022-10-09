#include "stdafx.h"
#include "..\Header\Dynamite.h"

CDynamite::CDynamite(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CDynamite::~CDynamite()
{
}

HRESULT CDynamite::Ready_Object()
{
	m_pTransCom = Add_Component<Engine::CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pBufferCom = Add_Component<Engine::CCubeTex>(L"Proto_CubeTexCom", L"Proto_CubeTexCom", ID_STATIC);
	m_pTextureCom = Add_Component<Engine::CTexture>(L"Proto_MinecraftCubeTexture", L"Proto_MinecraftCubeTexture", ID_STATIC);

	m_pTransCom->m_vInfo[INFO_POS] = { 3.f, 7.5f, 4.f };
	return E_NOTIMPL;
}

_int CDynamite::Update_Object(const _float & fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CDynamite::LateUpdate_Object()
{
}

void CDynamite::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(104);
	m_pBufferCom->Render_Buffer();
}

void CDynamite::Free()
{
	CGameObject::Free();
}

CDynamite * CDynamite::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamite* pInstance = new CDynamite(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
