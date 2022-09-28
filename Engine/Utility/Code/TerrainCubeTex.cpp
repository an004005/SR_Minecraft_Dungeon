#include "TerrainCubeTex.h"

USING(Engine)
CTerrainCubeTex::CTerrainCubeTex(LPDIRECT3DDEVICE9 pGraphicDev) :CVIBuffer(pGraphicDev)
{
}

CTerrainCubeTex::CTerrainCubeTex(const CTerrainCubeTex & rhs)
	: CVIBuffer(rhs)
{
}

CTerrainCubeTex::~CTerrainCubeTex()
{
}

HRESULT CTerrainCubeTex::Ready_Buffer(const wstring& wstrPath, _int iTex)
{
	m_iTexIdx = iTex;
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed Load Map");
		return E_FAIL;
	}
	vector<_matrix> vecWorld;

	DWORD	dwByte = 0;

	size_t vecSize;
	_matrix matCubeWorld;
	MapTool tMapTool;

	ReadFile(hFile, &vecSize, sizeof(size_t), &dwByte, nullptr);

	while (vecSize--)
	{
		ReadFile(hFile, &matCubeWorld, sizeof(_matrix), &dwByte, nullptr);
		ReadFile(hFile, &tMapTool, sizeof(MapTool), &dwByte, nullptr);

		if (tMapTool.iTexIdx != iTex)
			continue;
		vecWorld.push_back(matCubeWorld);
	}

	CloseHandle(hFile);

	m_dwVtxCnt = vecWorld.size() * 8;
	m_dwTriCnt = vecWorld.size() * 12;
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwFVF = FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	VTXCUBE VtxTmp[8]{};
	INDEX32 IdxTmp[12]{};

	vector<VTXCUBE> vecVtxTmps;
	vector<INDEX32> vecIdxTmps;


	for (size_t i = 0; i < vecWorld.size(); ++i)
	{
		// vtx buffer init
		VtxTmp[0].vPos = { -0.5f, 0.5f, -0.5f };
		VtxTmp[0].vTexUV = VtxTmp[0].vPos * 2;

		VtxTmp[1].vPos = { 0.5f, 0.5f, -0.5f };
		VtxTmp[1].vTexUV = VtxTmp[1].vPos * 2;

		VtxTmp[2].vPos = { 0.5f, -0.5f, -0.5f };
		VtxTmp[2].vTexUV = VtxTmp[2].vPos * 2;

		VtxTmp[3].vPos = { -0.5f, -0.5f, -0.5f };
		VtxTmp[3].vTexUV = VtxTmp[3].vPos * 2;

		// µÞ¸é
		VtxTmp[4].vPos = { -0.5f, 0.5f, 0.5f };
		VtxTmp[4].vTexUV = VtxTmp[4].vPos * 2;

		VtxTmp[5].vPos = { 0.5f, 0.5f, 0.5f };
		VtxTmp[5].vTexUV = VtxTmp[5].vPos * 2;

		VtxTmp[6].vPos = { 0.5f, -0.5f, 0.5f };
		VtxTmp[6].vTexUV = VtxTmp[6].vPos * 2;

		VtxTmp[7].vPos = { -0.5f, -0.5f, 0.5f };
		VtxTmp[7].vTexUV = VtxTmp[7].vPos * 2;


		for (int j = 0; j < 8; ++j)
		{
			D3DXVec3TransformCoord(&VtxTmp[j].vPos, &VtxTmp[j].vPos, &vecWorld[i]);
			vecVtxTmps.push_back(VtxTmp[j]);
		}

		// index buffer init
		{
			IdxTmp[0]._0 = 1;
			IdxTmp[0]._1 = 5;
			IdxTmp[0]._2 = 6;

			IdxTmp[1]._0 = 1;
			IdxTmp[1]._1 = 6;
			IdxTmp[1]._2 = 2;

			// X-
			IdxTmp[2]._0 = 4;
			IdxTmp[2]._1 = 0;
			IdxTmp[2]._2 = 3;

			IdxTmp[3]._0 = 4;
			IdxTmp[3]._1 = 3;
			IdxTmp[3]._2 = 7;

			// Y+
			IdxTmp[4]._0 = 4;
			IdxTmp[4]._1 = 5;
			IdxTmp[4]._2 = 1;

			IdxTmp[5]._0 = 4;
			IdxTmp[5]._1 = 1;
			IdxTmp[5]._2 = 0;

			// Y-
			IdxTmp[6]._0 = 3;
			IdxTmp[6]._1 = 2;
			IdxTmp[6]._2 = 6;

			IdxTmp[7]._0 = 3;
			IdxTmp[7]._1 = 6;
			IdxTmp[7]._2 = 7;

			// Z+
			IdxTmp[8]._0 = 7;
			IdxTmp[8]._1 = 6;
			IdxTmp[8]._2 = 5;

			IdxTmp[9]._0 = 7;
			IdxTmp[9]._1 = 5;
			IdxTmp[9]._2 = 4;

			// Z-
			IdxTmp[10]._0 = 0;
			IdxTmp[10]._1 = 1;
			IdxTmp[10]._2 = 2;

			IdxTmp[11]._0 = 0;
			IdxTmp[11]._1 = 2;
			IdxTmp[11]._2 = 3;
		}

		int iVtxCnt = 8 * i;
		for (int j = 0; j < 12; ++j)
		{
			IdxTmp[j]._0 += iVtxCnt;
			IdxTmp[j]._1 += iVtxCnt;
			IdxTmp[j]._2 += iVtxCnt;
			vecIdxTmps.push_back(IdxTmp[j]);
		}
	}


	VTXCUBE*		pVertex = nullptr;
	INDEX32*		pIndex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy(pVertex, vecVtxTmps.data(), sizeof(VTXCUBE) * vecVtxTmps.size());
	m_pVB->Unlock();

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	memcpy(pIndex, vecIdxTmps.data(), sizeof(INDEX32) * vecIdxTmps.size());

	m_pIB->Unlock();

	return S_OK;
}

CComponent * CTerrainCubeTex::Clone(void)
{
	return new CTerrainCubeTex(*this);
}

CTerrainCubeTex * CTerrainCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrPath, _int iTex)
{
	CTerrainCubeTex* pInstance = new CTerrainCubeTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(wstrPath, iTex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
