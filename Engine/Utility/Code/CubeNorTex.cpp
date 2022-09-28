#include "..\..\Header\CubeNorTex.h"


CCubeNorTex::CCubeNorTex(LPDIRECT3DDEVICE9 pGraphicDev): CVIBuffer(pGraphicDev)
{
}

CCubeNorTex::CCubeNorTex(const CCubeNorTex& rhs): CVIBuffer(rhs)
{
}

CCubeNorTex::~CCubeNorTex()
{
}

HRESULT CCubeNorTex::Ready_Buffer()
{
	m_dwVtxCnt = 24;
	m_dwTriCnt = 12;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX; // for use vtx declareation

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	_float w2 = 0.5f;
	_float h2 = 0.5f;
	_float d2 = 0.5f;

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	{
		// Back
		pVertex[0] = {_vec3(-w2, -h2, -d2), _vec2(0.0f, 1.0f), _vec3(0.0f, 0.0f, -1.0f)};
		pVertex[1] = {_vec3(-w2, +h2, -d2), _vec2(0.0f, 0.0f), _vec3(0.0f, 0.0f, -1.0f)};
		pVertex[2] = {_vec3(+w2, +h2, -d2), _vec2(1.0f, 0.0f), _vec3(0.0f, 0.0f, -1.0f)};
		pVertex[3] = {_vec3(+w2, -h2, -d2), _vec2(1.0f, 1.0f), _vec3(0.0f, 0.0f, -1.0f)};
		// Look
		pVertex[4] = {_vec3(-w2, -h2, +d2), _vec2(1.0f, 1.0f), _vec3(0.0f, 0.0f, 1.0f)};
		pVertex[5] = {_vec3(+w2, -h2, +d2), _vec2(0.0f, 1.0f), _vec3(0.0f, 0.0f, 1.0f)};
		pVertex[6] = {_vec3(+w2, +h2, +d2), _vec2(0.0f, 0.0f), _vec3(0.0f, 0.0f, 1.0f)};
		pVertex[7] = {_vec3(-w2, +h2, +d2), _vec2(1.0f, 0.0f), _vec3(0.0f, 0.0f, 1.0f)};
		// Up
		pVertex[8] = {_vec3(-w2, +h2, -d2), _vec2(0.0f, 1.0f), _vec3(0.0f, 1.0f, 0.0f)};
		pVertex[9] = {_vec3(-w2, +h2, +d2), _vec2(0.0f, 0.0f), _vec3(0.0f, 1.0f, 0.0f)};
		pVertex[10] = {_vec3(+w2, +h2, +d2), _vec2(1.0f, 0.0f), _vec3(0.0f, 1.0f, 0.0f)};
		pVertex[11] = {_vec3(+w2, +h2, -d2), _vec2(1.0f, 1.0f), _vec3(0.0f, 1.0f, 0.0f)};
		// Down
		pVertex[12] = {_vec3(-w2, -h2, -d2), _vec2(1.0f, 1.0f), _vec3(0.0f, -1.0f, 0.0f)};
		pVertex[13] = {_vec3(+w2, -h2, -d2), _vec2(0.0f, 1.0f), _vec3(0.0f, -1.0f, 0.0f)};
		pVertex[14] = {_vec3(+w2, -h2, +d2), _vec2(0.0f, 0.0f), _vec3(0.0f, -1.0f, 0.0f)};
		pVertex[15] = {_vec3(-w2, -h2, +d2), _vec2(1.0f, 0.0f), _vec3(0.0f, -1.0f, 0.0f)};
		// left
		pVertex[16] = {_vec3(-w2, -h2, +d2), _vec2(0.0f, 1.0f), _vec3(-1.0f, 0.0f, 0.0f)};
		pVertex[17] = {_vec3(-w2, +h2, +d2), _vec2(0.0f, 0.0f), _vec3(-1.0f, 0.0f, 0.0f)};
		pVertex[18] = {_vec3(-w2, +h2, -d2), _vec2(1.0f, 0.0f), _vec3(-1.0f, 0.0f, 0.0f)};
		pVertex[19] = {_vec3(-w2, -h2, -d2), _vec2(1.0f, 1.0f), _vec3(-1.0f, 0.0f, 0.0f)};
		// right
		pVertex[20] = {_vec3(+w2, -h2, -d2), _vec2(0.0f, 1.0f), _vec3(1.0f, 0.0f, 0.0f)};
		pVertex[21] = {_vec3(+w2, +h2, -d2), _vec2(0.0f, 0.0f), _vec3(1.0f, 0.0f, 0.0f)};
		pVertex[22] = {_vec3(+w2, +h2, +d2), _vec2(1.0f, 0.0f), _vec3(1.0f, 0.0f, 0.0f)};
		pVertex[23] = {_vec3(+w2, -h2, +d2), _vec2(1.0f, 1.0f), _vec3(1.0f, 0.0f, 0.0f)};
	}

	m_pVB->Unlock();


	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 5;
	pIndex[2]._2 = 6;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 6;
	pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 8;
	pIndex[4]._1 = 9;
	pIndex[4]._2 = 10;

	pIndex[5]._0 = 8;
	pIndex[5]._1 = 10;
	pIndex[5]._2 = 11;

	// Y-
	pIndex[6]._0 = 12;
	pIndex[6]._1 = 13;
	pIndex[6]._2 = 14;

	pIndex[7]._0 = 12;
	pIndex[7]._1 = 14;
	pIndex[7]._2 = 15;

	// Z+
	pIndex[8]._0 = 16;
	pIndex[8]._1 = 17;
	pIndex[8]._2 = 18;

	pIndex[9]._0 = 16;
	pIndex[9]._1 = 18;
	pIndex[9]._2 = 19;

	// Z-
	pIndex[10]._0 = 20;
	pIndex[10]._1 = 21;
	pIndex[10]._2 = 22;

	pIndex[11]._0 = 20;
	pIndex[11]._1 = 22;
	pIndex[11]._2 = 23;

	m_pIB->Unlock();

	return S_OK;
}

CComponent* CCubeNorTex::Clone()
{
	return new CCubeNorTex(*this);
}

CCubeNorTex* CCubeNorTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeNorTex*	pInstance = new CCubeNorTex(pGraphicDev);
	
	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
