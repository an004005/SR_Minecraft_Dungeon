#include "..\..\Header\CubeTex6.h"


D3DVERTEXELEMENT9 BoxVTXInstance_Elements3[] =
{
	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
	{1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 1},
	{1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 2},
	{1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 3},
	{1, 64, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	D3DDECL_END()
};

CCubeTex6::CCubeTex6(LPDIRECT3DDEVICE9 pGraphicDev): CVIBuffer(pGraphicDev)
{
}

CCubeTex6::CCubeTex6(const CCubeTex6& rhs): CVIBuffer(rhs)
{
	m_pEffect = rhs.m_pEffect;
	m_pVtxDeclare = rhs.m_pVtxDeclare;
	m_pVBMatrix = rhs.m_pVBMatrix;
}

CCubeTex6::~CCubeTex6()
{
}

CComponent* CCubeTex6::Clone()
{
	return new CCubeTex6(*this);
}

HRESULT CCubeTex6::Ready_Buffer()
{
	ID3DXBuffer* pCompilationErrors = 0;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(m_pGraphicDev,
	                         L"../Bin/Resource/Shader/Instancing_cube6.fx", 0, 0, dwShaderFlags, 0, &m_pEffect,
	                         &pCompilationErrors);

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
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexDeclaration(BoxVTXInstance_Elements3, &m_pVtxDeclare), E_FAIL);

	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwFVF = 0;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	// 버텍스 초기화
	{
		VTXCUBE* pVertex = nullptr;

		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		pVertex[0].vPos = {-0.5f, 0.5f, -0.5f};
		pVertex[0].vTexUV = pVertex[0].vPos;

		pVertex[1].vPos = {0.5f, 0.5f, -0.5f};
		pVertex[1].vTexUV = pVertex[1].vPos;

		pVertex[2].vPos = {0.5f, -0.5f, -0.5f};
		pVertex[2].vTexUV = pVertex[2].vPos;

		pVertex[3].vPos = {-0.5f, -0.5f, -0.5f};
		pVertex[3].vTexUV = pVertex[3].vPos;

		// 뒷면
		pVertex[4].vPos = {-0.5f, 0.5f, 0.5f};
		pVertex[4].vTexUV = pVertex[4].vPos;

		pVertex[5].vPos = {0.5f, 0.5f, 0.5f};
		pVertex[5].vTexUV = pVertex[5].vPos;

		pVertex[6].vPos = {0.5f, -0.5f, 0.5f};
		pVertex[6].vTexUV = pVertex[6].vPos;

		pVertex[7].vPos = {-0.5f, -0.5f, 0.5f};
		pVertex[7].vTexUV = pVertex[7].vPos;
		m_pVB->Unlock();


		INDEX32* pIndex = nullptr;

		m_pIB->Lock(0, 0, (void**)&pIndex, 0);

		// X+

		pIndex[0]._0 = 1;
		pIndex[0]._1 = 5;
		pIndex[0]._2 = 6;

		pIndex[1]._0 = 1;
		pIndex[1]._1 = 6;
		pIndex[1]._2 = 2;

		// X-
		pIndex[2]._0 = 4;
		pIndex[2]._1 = 0;
		pIndex[2]._2 = 3;

		pIndex[3]._0 = 4;
		pIndex[3]._1 = 3;
		pIndex[3]._2 = 7;

		// Y+
		pIndex[4]._0 = 4;
		pIndex[4]._1 = 5;
		pIndex[4]._2 = 1;

		pIndex[5]._0 = 4;
		pIndex[5]._1 = 1;
		pIndex[5]._2 = 0;

		// Y-
		pIndex[6]._0 = 3;
		pIndex[6]._1 = 2;
		pIndex[6]._2 = 6;

		pIndex[7]._0 = 3;
		pIndex[7]._1 = 6;
		pIndex[7]._2 = 7;

		// Z+
		pIndex[8]._0 = 7;
		pIndex[8]._1 = 6;
		pIndex[8]._2 = 5;

		pIndex[9]._0 = 7;
		pIndex[9]._1 = 5;
		pIndex[9]._2 = 4;

		// Z-
		pIndex[10]._0 = 0;
		pIndex[10]._1 = 1;
		pIndex[10]._2 = 2;

		pIndex[11]._0 = 0;
		pIndex[11]._1 = 2;
		pIndex[11]._2 = 3;

		m_pIB->Unlock();
	}

	m_pGraphicDev->CreateVertexBuffer( 6 * sizeof(OrderMatrix), 0, 0,
                                              D3DPOOL_MANAGED, &m_pVBMatrix, 0 );

	return S_OK;
}

void CCubeTex6::Render_Buffer()
{
	_matrix* pIPos;
	m_pVBMatrix->Lock(0, NULL, ( void** )&pIPos, 0);
	memcpy(pIPos, m_arrPartWorld.data(), sizeof(OrderMatrix) * PART_END);
	m_pVBMatrix->Unlock();

	_matrix view, proj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &view);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &proj);
	m_pEffect->SetMatrix("g_mView", &view);
	m_pEffect->SetMatrix("g_mProj", &proj);

	m_pEffect->SetTexture("g_txHead", m_arrTexture[HEAD]);
	m_pEffect->SetTexture("g_txBody", m_arrTexture[BODY]);
	m_pEffect->SetTexture("g_txArmL", m_arrTexture[ARM_L]);
	m_pEffect->SetTexture("g_txArmR", m_arrTexture[ARM_R]);
	m_pEffect->SetTexture("g_txLegL", m_arrTexture[LEG_L]);
	m_pEffect->SetTexture("g_txLegR", m_arrTexture[LEG_R]);

	m_pGraphicDev->SetVertexDeclaration(m_pVtxDeclare);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetStreamSourceFreq( 0, D3DSTREAMSOURCE_INDEXEDDATA | PART_END );

	m_pGraphicDev->SetStreamSource( 1, m_pVBMatrix, 0, sizeof(OrderMatrix));
	m_pGraphicDev->SetStreamSourceFreq( 1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

	m_pGraphicDev->SetIndices(m_pIB);

	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pEffect->BeginPass(i);
			{
				m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
			}
			m_pEffect->EndPass();
		}
	}
	m_pEffect->End();

	m_pGraphicDev->SetStreamSourceFreq( 0, 1 );
	m_pGraphicDev->SetStreamSourceFreq( 1, 1 );
}

void CCubeTex6::Free()
{
	// Safe_Release(m_pTexture);
	for(auto& pTexture : m_arrTexture)
		Safe_Release(pTexture);
	Safe_Release(m_pEffect);
	Safe_Release(m_pVtxDeclare);
	Safe_Release(m_pVBMatrix);

	CVIBuffer::Free();
}

CCubeTex6* CCubeTex6::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeTex6* pInstance = new CCubeTex6(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCubeTex6::SetPartTexture(IDirect3DBaseTexture9* pTexture, PartOrder eOrder)
{
	m_arrTexture[eOrder] = pTexture;
	m_arrTexture[eOrder]->AddRef();
}

void CCubeTex6::SetPartWorldMatrix(const _matrix& matPartWorld, PartOrder eOrder)
{
	m_arrPartWorld[eOrder] = {matPartWorld, eOrder};
}
