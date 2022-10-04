#include "..\..\Header\ArrowCube.h"
#include "GameUtilMgr.h"

USING(Engine)

CArrowCube::CArrowCube(LPDIRECT3DDEVICE9 pGraphicDev): CVIBuffer(pGraphicDev)
{
}

CArrowCube::CArrowCube(const CArrowCube& rhs): CVIBuffer(rhs)
{
}

CArrowCube::~CArrowCube()
{
}

HRESULT CArrowCube::Ready_Buffer()
{
	m_dwVtxCnt = 0;
	m_dwTriCnt = 0;
	m_dwVtxSize = sizeof(VTXCUBE2);
	m_dwFVF = FVF_CUBE2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	vector<VTXCUBE2> vecVtxCube2;
	vector<INDEX32> vecIdx;

	static _vec2 vHeadUV[4]
	{
		{0.f, 0.f}, {1.f/ 3.f, 0.f}, {0.f, 1.f}, {1.f/ 3.f, 1.f}
	};
	static _vec2 vBodyUV[4]
	{
		{1.f/ 3.f, 0.f}, {2.f/ 3.f, 0.f}, {1.f/ 3.f, 1.f}, {2.f/ 3.f, 1.f}
	};
	static _vec2 vTailUV[4]
	{
		{2.f/ 3.f, 0.f}, {1.f, 0.f}, {2.f/ 3.f, 1.f}, {1.f, 1.f}
	};

	VTXCUBE2 DefaultCubeVtx[FACE_END][4]{};
	_matrix matTrans;

	{ // arrow head point
		CGameUtilMgr::MatWorldComposeEuler(matTrans, {1.f, 1.f, 1.f}, {0.f, 0.f, 0.f}, {0.f,0.f,5.5f});
		for (int i = 0; i < FACE_END; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				D3DXVec3TransformCoord(&DefaultCubeVtx[i][j].vPos, &CGameUtilMgr::s_vFaceCubeVtx[i][j], &matTrans);
				DefaultCubeVtx[i][j].vTexUV = vHeadUV[j];
				vecVtxCube2.push_back(DefaultCubeVtx[i][j]);
			}
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 1, m_dwVtxCnt + 2});
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 2, m_dwVtxCnt + 3});
			m_dwVtxCnt += 4;
			m_dwTriCnt += 2;
		}
	}
	{ // arrow head
		CGameUtilMgr::MatWorldComposeEuler(matTrans, {1.f, 3.f, 1.f}, {0.f, 0.f, 0.f}, {0.f,0.f,4.5f});
		for (int i = 0; i < FACE_END; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				D3DXVec3TransformCoord(&DefaultCubeVtx[i][j].vPos, &CGameUtilMgr::s_vFaceCubeVtx[i][j], &matTrans);
				DefaultCubeVtx[i][j].vTexUV = vHeadUV[j];
				vecVtxCube2.push_back(DefaultCubeVtx[i][j]);
			}
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 1, m_dwVtxCnt + 2});
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 2, m_dwVtxCnt + 3});
			m_dwVtxCnt += 4;
			m_dwTriCnt += 2;
		}
	}
	{
		//arrow body
		CGameUtilMgr::MatWorldComposeEuler(matTrans, {1.f, 1.f, 7.f}, {0.f, 0.f, 0.f}, {0.f,0.f,2.5f});
		for (int i = 0; i < FACE_END; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				D3DXVec3TransformCoord(&DefaultCubeVtx[i][j].vPos, &CGameUtilMgr::s_vFaceCubeVtx[i][j], &matTrans);
				DefaultCubeVtx[i][j].vTexUV = vBodyUV[j];
				vecVtxCube2.push_back(DefaultCubeVtx[i][j]);
			}
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 1, m_dwVtxCnt + 2});
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 2, m_dwVtxCnt + 3});
			m_dwVtxCnt += 4;
			m_dwTriCnt += 2;
		}
	}
	{//arrow tail left
		CGameUtilMgr::MatWorldComposeEuler(matTrans, {1.f, 2.f, 1.f}, {0.f, 0.f, 0.f}, {0.5f,0.f,-3.5f});
		for (int i = 0; i < FACE_END; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				D3DXVec3TransformCoord(&DefaultCubeVtx[i][j].vPos, &CGameUtilMgr::s_vFaceCubeVtx[i][j], &matTrans);
				DefaultCubeVtx[i][j].vTexUV = vTailUV[j];
				vecVtxCube2.push_back(DefaultCubeVtx[i][j]);
			}
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 1, m_dwVtxCnt + 2});
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 2, m_dwVtxCnt + 3});
			m_dwVtxCnt += 4;
			m_dwTriCnt += 2;
		}
	}
	{//arrow tail 
		CGameUtilMgr::MatWorldComposeEuler(matTrans, {1.f, 2.f, 1.f}, {0.f, 0.f, 0.f}, {-0.5f,0.f,-3.5f});
		for (int i = 0; i < FACE_END; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				D3DXVec3TransformCoord(&DefaultCubeVtx[i][j].vPos, &CGameUtilMgr::s_vFaceCubeVtx[i][j], &matTrans);
				DefaultCubeVtx[i][j].vTexUV = vTailUV[j];
				vecVtxCube2.push_back(DefaultCubeVtx[i][j]);
			}
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 1, m_dwVtxCnt + 2});
			vecIdx.push_back({m_dwVtxCnt, m_dwVtxCnt + 2, m_dwVtxCnt + 3});
			m_dwVtxCnt += 4;
			m_dwTriCnt += 2;
		}
	}

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCUBE2* pVertex = nullptr;
	INDEX32* pIndex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy(pVertex, vecVtxCube2.data(), sizeof(VTXCUBE2) * vecVtxCube2.size());
	m_pVB->Unlock();

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	memcpy(pIndex, vecIdx.data(), sizeof(INDEX32) * vecIdx.size());
	m_pIB->Unlock();

	return S_OK;
}

void CArrowCube::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CArrowCube::Clone()
{
	// _CRASH("Dont clone this");
	return new CArrowCube(*this);
}

void CArrowCube::Free()
{
	CVIBuffer::Free();
}

CArrowCube* CArrowCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CArrowCube* pInstance = new CArrowCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
