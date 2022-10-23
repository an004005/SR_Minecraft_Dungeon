#include "..\..\Header\TargetTexture.h"
#include "Shader.h"

CTargetTexture::CTargetTexture(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev)
{
}

CTargetTexture::CTargetTexture(const CTargetTexture& rhs) : CComponent(rhs)
{
	m_pTexture = rhs.m_pTexture;
	m_pTexture->AddRef();
	m_pRender2Surface = rhs.m_pRender2Surface;
	m_pRender2Surface->AddRef();
}

CTargetTexture::~CTargetTexture()
{
}

HRESULT CTargetTexture::Ready_Texture()
{
	FAILED_CHECK(m_pGraphicDev->CreateTexture(WINCX, WINCY, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture, nullptr));

	FAILED_CHECK(D3DXCreateRenderToSurface(m_pGraphicDev, WINCX, WINCY, D3DFMT_X8R8G8B8, FALSE, D3DFMT_UNKNOWN, &m_pRender2Surface));


	return S_OK;
}

void CTargetTexture::Set_Texture(const _uint& iIndex)
{
	m_pGraphicDev->SetTexture(0, m_pTexture);
}

void CTargetTexture::Set_Texture(CShader* pShaderCom, D3DXHANDLE hHandle, const _uint& iIndex)
{
	pShaderCom->Set_Texture(hHandle, m_pTexture);
}

void CTargetTexture::RenderOnSurface()
{
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	IDirect3DSurface9* pSurface;
	FAILED_CHECK_RETURN(m_pTexture->GetSurfaceLevel(0, &pSurface));

	m_pRender2Surface->BeginScene(pSurface, &ViewPort);
	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);

	for (auto e : m_RenderList)
		e->Render_Object();

	m_pRender2Surface->EndScene(D3DX_FILTER_LINEAR);

	Safe_Release(pSurface);
}

CTargetTexture* CTargetTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTargetTexture* pInstance = new CTargetTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture()))
	{
		MSG_BOX("Texture Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* CTargetTexture::Clone()
{
	_CRASH("Dont Clone This");
	return nullptr;
}

void CTargetTexture::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pRender2Surface);
	CComponent::Free();
}


