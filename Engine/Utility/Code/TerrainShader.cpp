#include "..\..\Header\TerrainShader.h"

#include "GameUtilMgr.h"


CTerrainShader::CTerrainShader(LPDIRECT3DDEVICE9 pGraphicDev):CVIBuffer(pGraphicDev)
{
}

CTerrainShader::CTerrainShader(const CTerrainShader& rhs) : CVIBuffer(rhs)
{
	// m_pVB = rhs.m_pVB;
	// m_pIB = rhs.m_pIB;
	// m_pGraphicDev = rhs.m_pGraphicDev;
	m_pEffect = rhs.m_pEffect;
	m_pVtxDeclare = rhs.m_pVtxDeclare;
	// m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
	m_pVtxDeclare->AddRef();
	// m_pVB->AddRef();
	// m_pIB->AddRef();
}
CTerrainShader::~CTerrainShader() = default;

HRESULT CTerrainShader::Ready_Buffer(const wstring& _shaderfile, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv,
	_vec2 _uv0, _vec2 _uv1, _vec2 _uv2, _vec2 _uv3)
{
	DWORD dwShaderFlags = 0;
#ifdef _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif


	ID3DXBuffer *pCompilationErrors = 0;
	HRESULT hr = D3DXCreateEffectFromFile(m_pGraphicDev, L"../Bin/Resource/Shader/Water_Terrain_Test1.fx", 0, 0,
		dwShaderFlags, 0, &m_pEffect, &pCompilationErrors);

	NULL_CHECK_RETURN(m_pEffect, E_FAIL);


	if (!m_pEffect && pCompilationErrors)
	{
		int size = pCompilationErrors->GetBufferSize();
		void* ack = pCompilationErrors->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf_s(str, size, (const char*)ack, size);
			OutputDebugStringA(str);
			_CRASH(str);
			delete[] str;
		}
	}

	D3DVERTEXELEMENT9 VTXCUBEEX_Elements[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0,  12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		// { 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		D3DDECL_END()
	};

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexDeclaration(VTXCUBEEX_Elements, &m_pVtxDeclare), E_FAIL);


	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxSize = sizeof(VTXUVTERRAIN);
	m_dwFVF = 0; // for use vtx declareation

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	_ulong	dwIndex = 0;

	VTXUVTERRAIN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPos = { _float(j) * dwVtxItv, 0.f,  _float(i) * dwVtxItv };
			pVertex[dwIndex].vUV = { _float(j) / (dwCntX - 1) * 20.f, _float(i) / (dwCntZ - 1)* 20.f};
		}
	}

	// pVertex[0] = { _vec3(-1.f, 1.f, 0.f), _uv0 };
	// pVertex[1] = { _vec3(1.f, 1.f, 0.f), _uv1 };
	// pVertex[2] = { _vec3(1.f, -1.f, 0.f), _uv2 };
	// pVertex[3] = { _vec3(-1.f, -1.f, 0.f), _uv3 };

	m_pVB->Unlock();

	_ulong dwTriCnt = 0;

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);


	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			++dwTriCnt;

			// 왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			++dwTriCnt;
		}
	}


	// 오른쪽 위
	// pIndex[0]._0 = 0;
	// pIndex[0]._1 = 1;
	// pIndex[0]._2 = 2;
	//
	// // 왼쪽 아래
	// pIndex[1]._0 = 0;
	// pIndex[1]._1 = 2;
	// pIndex[1]._2 = 3;

	m_pIB->Unlock();

	//LoadTexture(L"T_ElectricArcs.png");

	return S_OK;
}

void CTerrainShader::Render_Buffer()
{

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_iPlayOnFrameCnt++;

	if (m_iPlayOnFrameCnt == m_iFrameCnt)
	{
		m_iWidthTextureCnt++;

		if (m_iWidthTextureCnt == m_iTextureCnt_W)
		{
			m_iWidthTextureCnt = 0;
			m_iHeightTextureCnt++;
		}

		if (m_iHeightTextureCnt == m_iTextureCnt_H)
		{
			m_iHeightTextureCnt = 0;
		}
		m_iPlayOnFrameCnt = 0;
	}



	//Matrix
	m_pEffect->SetMatrix("gWorldMatrix", &CGameUtilMgr::s_matIdentity);

	_matrix view, proj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &view);
	m_pEffect->SetMatrix("gViewMatrix", &view);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &proj);
	m_pEffect->SetMatrix("gProjectionMatrix", &proj);
	//~Matrix
	m_pEffect->SetTexture("DiffuseMap_Tex", m_pTexture);

	// m_pEffect->SetFloat("gTime", CGameUtilMgr::s_fTimeDelta);


	// static float timeSpeed = 1.f;
	m_fTime += CGameUtilMgr::s_fTimeDelta * 1;
	m_pEffect->SetFloat("gTime", m_fTime);
	// CGameUtilMgr::s_fTimeDelta
	m_pEffect->SetFloat("gSpeed", 2.16f);
	m_pEffect->SetFloat("gWaveHeight", 1.5f);
	m_pEffect->SetFloat("gWaveFrequency", 1.5f);
	m_pEffect->SetFloat("gUVSpeed", 0.6f);
	// m_pEffect->SetFloat("gTime", tick / 10000.f);
	// // CGameUtilMgr::s_fTimeDelta
	// m_pEffect->SetFloat("gSpeed", 2.f);
	// m_pEffect->SetFloat("gWaveHeight", 0.8f);
	// m_pEffect->SetFloat("gWaveFrequency", 3.f);
	// m_pEffect->SetFloat("gUVSpeed", 1.f);
	

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

CTerrainShader* CTerrainShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& _shaderfile, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv,
	_vec2 _uv0, _vec2 _uv1, _vec2 _uv2, _vec2 _uv3)
{
	CTerrainShader*	pInstance = new CTerrainShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(_shaderfile, dwCntX, dwCntZ, dwVtxItv, _uv0, _uv1, _uv2, _uv3)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* CTerrainShader::Clone()
{
	return new CTerrainShader(*this);
}

void CTerrainShader::Free()
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pVtxDeclare);
	Safe_Release(m_pTexture);
	CVIBuffer::Free();
}



