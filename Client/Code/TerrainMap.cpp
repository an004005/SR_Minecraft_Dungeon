#include "stdafx.h"
#include "..\Header\TerrainMap.h"
#include "MapCube.h"

CTerrainMap::CTerrainMap()
{
	ZeroMemory(&m_tMapTool, sizeof(MapTool));
}

CTerrainMap::~CTerrainMap()
{
}

void CTerrainMap::LoadMap(LPDIRECT3DDEVICE9 pGrahpicDev, 
	map<const _tchar*, 
	CLayer*>& m_mapLayer, 
	CLayer* pLayer)
{
	HANDLE hFile = CreateFile(L"../Bin/Resource/Map/Map.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed Load Map");
		return;
	}
	DWORD	dwByte = 0;

	_matrix matCubeWorld;
	MapTool tMapTool;

	while (true)
	{

		ReadFile(hFile, &matCubeWorld, sizeof(_matrix), &dwByte, nullptr);
		ReadFile(hFile, &tMapTool, sizeof(MapTool), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		Create_Cube(matCubeWorld, tMapTool, pGrahpicDev, m_mapLayer, pLayer);
	}

	CloseHandle(hFile);

}

void CTerrainMap::Create_Cube(_matrix & CubeWorld,
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

void CTerrainMap::Divide_CubeType(CMapCube * pMapCube)
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
CTerrainMap * CTerrainMap::Create()
{
	CTerrainMap*	pInstance = new CTerrainMap();

	return pInstance;
}

void CTerrainMap::Free(void)
{
	for_each(m_vecTotalCube.begin(), m_vecTotalCube.end(), Safe_Release<CMapCube*>);
	m_vecTotalCube.clear();
	m_vecLand.clear();
	m_vecCollision.clear();
	m_vecDeco.clear();
}
