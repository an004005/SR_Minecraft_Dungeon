#include "..\..\Header\RcShader.h"

D3DXVECTOR4				gLightColor(0.7f, 0.7f, 1.0f, 1.0f);

_int CRcShader::m_i = 0;
_int CRcShader::m_j = 0;
_int CRcShader::m_k = 0;


CRcShader::CRcShader(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CRcShader::CRcShader(const CRcShader& rhs) : CVIBuffer(rhs)
{
	m_pEffect = rhs.m_pEffect;
	m_pVtxDeclare = rhs.m_pVtxDeclare;
	m_pTexture = rhs.m_pTexture;

	m_pEffect->AddRef();
	m_pVtxDeclare->AddRef();
	m_pTexture->AddRef();
}

CRcShader::~CRcShader()
{
}

HRESULT CRcShader::Ready_Buffer()
{

	ID3DXBuffer *pCompilationErrors = 0;
	DWORD dwShaderFlags = 0;
	HRESULT hr = D3DXCreateEffectFromFile(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx", 0, 0,
		dwShaderFlags, 0, &m_pEffect, &pCompilationErrors);

	NULL_CHECK_RETURN(m_pEffect, E_FAIL);

	D3DVERTEXELEMENT9 VTXCUBEEX_Elements[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0,  12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		// { 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		D3DDECL_END()
	};

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexDeclaration(VTXCUBEEX_Elements, &m_pVtxDeclare), E_FAIL);

	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTXPARTICLE);
	m_dwFVF = FVF_COL; // for use vtx declareation

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXPARTICLE*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0] = { _vec3(-1.f, 1.f, 0.f), _vec2(0.0f, 0.0f) };
	pVertex[1] = { _vec3(1.f, 1.f, 0.f), _vec2(0.25f, 0.0f) };
	pVertex[2] = { _vec3(1.f, -1.f, 0.f), _vec2(0.25f, 0.5f) };
	pVertex[3] = { _vec3(-1.f, -1.f, 0.f), _vec2(0.f, 0.5f) };

	m_pVB->Unlock();


	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// ������ ��
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// ���� �Ʒ�
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	LoadTexture(L"T_fullcircle_explosion_124_spritesheet.png");

	return S_OK;
}

void CRcShader::Render_Buffer()
{

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_i++;

	if (m_i == 10)
	{
		m_j++;

		if (m_j == 4)
		{
			m_j = 0;
			m_k++;
		}

		if (m_k == 2)
		{
			m_k = 0;
		}

		m_pEffect->SetFloat("gTime", 0.25f * m_j);
		m_pEffect->SetFloat("gUVSpeed", 0.5f * m_k);
		m_i = 0;
	}

	//Matrix
	m_pEffect->SetMatrix("gWorldMatrix", &m_matWorld);

	_matrix view, proj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &view);
	m_pEffect->SetMatrix("gViewMatrix", &view);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &proj);
	m_pEffect->SetMatrix("gProjectionMatrix", &proj);
	//~Matrix
	m_pEffect->SetTexture("DiffuseMap_Tex", m_pTexture);
	m_pGraphicDev->SetVertexDeclaration(m_pVtxDeclare);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	

	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pEffect->BeginPass(i);
			{
				

				m_pGraphicDev->SetIndices(m_pIB);
				m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
				//m_pEffect->CommitChanges();

			}
			m_pEffect->EndPass();
		}
	}
	m_pEffect->End();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CComponent * CRcShader::Clone()
{
	return new CRcShader(*this);
}

void CRcShader::Free()
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pVtxDeclare);
	Safe_Release(m_pTexture);

	CVIBuffer::Free();
}

CRcShader * CRcShader::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcShader*	pInstance = new CRcShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRcShader::LoadTexture(const wstring& filename)
{
	if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, filename.c_str(), (LPDIRECT3DTEXTURE9*)&m_pTexture)))
	{
		
	}
}
