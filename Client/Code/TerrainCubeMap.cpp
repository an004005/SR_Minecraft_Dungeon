#include "stdafx.h"
#include "..\Header\TerrainCubeMap.h"
#include "MapCube.h"

CTerrainCubeMap::CTerrainCubeMap()
{
	ZeroMemory(&m_tMapTool, sizeof(MapTool));
	ZeroMemory(&m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ);
}

CTerrainCubeMap::~CTerrainCubeMap()
{
}

void CTerrainCubeMap::LoadMap(LPDIRECT3DDEVICE9 pGrahpicDev, 
	map<const _tchar*, CLayer*>& m_mapLayer, 
	CLayer* pLayer)
{
	HANDLE hFile = CreateFile(L"../Bin/Resource/Map/MapTest.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed Load Map");
		return;
	}
	DWORD	dwByte = 0;

	size_t vecSize;
	_matrix matCubeWorld;
	MapTool tMapTool;

	ReadFile(hFile, &vecSize, sizeof(size_t), &dwByte, nullptr);

	while (vecSize--)
	{

		ReadFile(hFile, &matCubeWorld, sizeof(_matrix), &dwByte, nullptr);
		ReadFile(hFile, &tMapTool, sizeof(MapTool), &dwByte, nullptr);

		Create_Cube(matCubeWorld, tMapTool, pGrahpicDev, m_mapLayer, pLayer);
	}

	ReadFile(hFile, &m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ, &dwByte, nullptr);

	CloseHandle(hFile);
	Set_CubeCoordinate();
}

void CTerrainCubeMap::Create_Cube(_matrix & CubeWorld,
	MapTool & tMapTool,
	LPDIRECT3DDEVICE9 pGrahpicDev,
	map<const _tchar*, CLayer*>& m_mapLayer, 
	CLayer* pLayer)
{
	CGameObject* pGameObject = nullptr;
	CMapCube* pMapCube = nullptr;

	pGameObject = pMapCube = CMapCube::Create(pGrahpicDev, CubeWorld, tMapTool);

	if (pGameObject == nullptr)
		return;

	pLayer->Add_GameObject(pMapCube->m_wstrName.c_str(), pGameObject);

	Divide_CubeType(pMapCube);
	
	m_mapLayer.insert({ L"Layer_Terrain", pLayer });
	m_tMapTool.iCubeCount++;
}

void CTerrainCubeMap::Divide_CubeType(CMapCube * pMapCube)
{
	m_vecTotalCube.push_back(pMapCube);

	switch (pMapCube->m_tMapTool.iCubeType)
	{
	case TYPE_LAND:
		m_vecLand.push_back(pMapCube);
		break;
	case TYPE_COLLISION:
		m_vecCollision.push_back(pMapCube);
		break;
	case TYPE_DECO:
		m_vecCollision.push_back(pMapCube);
		break;
	default:
		break;
	}
}

void CTerrainCubeMap::Set_CubeCoordinate(void)
{
	m_pPos = new _vec3[VTXCNTX * VTXCNTZ];

	_ulong	dwIndex = 0;

	for (_ulong i = 0; i < VTXCNTZ; ++i)
	{
		for (_ulong j = 0; j < VTXCNTX; ++j)
		{
			m_pPos[dwIndex] = { _float(j) * VTXITV,
								m_fHeight[j* VTXITV][i * VTXITV],
								_float(i) * VTXITV };
		}
	}
}


CTerrainCubeMap * CTerrainCubeMap::Create()
{
	CTerrainCubeMap*	pInstance = new CTerrainCubeMap();

	return pInstance;
}

void CTerrainCubeMap::Free(void)
{
	for_each(m_vecTotalCube.begin(), m_vecTotalCube.end(), Safe_Release<CMapCube*>);
	m_vecTotalCube.clear();
	m_vecLand.clear();
	m_vecCollision.clear();
	m_vecDeco.clear();

	Safe_Delete_Array(m_pPos);
}
