#include "stdafx.h"
#include "..\Header\TerrainCubeMap.h"
#include <set>

CTerrainCubeMap::CTerrainCubeMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	ZeroMemory(&m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ);
}

CTerrainCubeMap::~CTerrainCubeMap()
{
}


HRESULT CTerrainCubeMap::Ready_Object(const wstring& wstrPath)
{
	m_pTextureCom = Add_Component<CTexture>(L"Proto_MinecraftCubeTexture", L"Proto_MinecraftCubeTexture", ID_STATIC);

	if (!wstrPath.empty())
	{
		LoadMap(wstrPath);
	}

	return S_OK;
}

_int CTerrainCubeMap::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);
	return 0;
}

void CTerrainCubeMap::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CTerrainCubeMap::Render_Object(void)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	for (auto& cubeTex : m_mapTerrainCom)
	{
		if (cubeTex.second->GetCubeCnt() > 0)
		{
			m_pTextureCom->Set_Texture(cubeTex.first);
			cubeTex.second->Render_Buffer();
		}

	}
}

void CTerrainCubeMap::LoadMap(const wstring& wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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

		MapCubeInfo tMapCubeInfo{ matCubeWorld, tMapTool.iTexIdx, (CUBETYPE)tMapTool.iCubeType, tMapTool.fHeight };
		m_vecTotalCube.push_back(tMapCubeInfo);
		
		//divide cubetype
		switch (tMapCubeInfo.eType)
		{
		case TYPE_LAND:
			m_vecLand.push_back(tMapCubeInfo);
			break;
		case TYPE_COLLISION:
			m_vecCollision.push_back(tMapCubeInfo);
			break;
		default:
			MSG_BOX("Invalid MapCubeInfo when load CTerrainCubeMap");
			break;
		}
	}

	ReadFile(hFile, &m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ, &dwByte, nullptr);


	CloseHandle(hFile);

	// 유일한 tex idx를 찾아낸다. 
	set<_int> SetTexIdx;
	for (auto& cube : m_vecTotalCube)
		SetTexIdx.insert(cube.iTexIdx);
	

	// 수정 하기!!!!!!!!!!!!!
	for (auto texIdx : SetTexIdx)
	{
		//CTerrainCubeTex* pTerrainCubeTex = CTerrainCubeTex::Create(m_pGraphicDev, wstrPath, texIdx);
		CTerrainCubeTex* pTerrainCubeTex = nullptr;
		NULL_CHECK(pTerrainCubeTex);

		wstring wstrTmp = L"TerrainCubeTexCom" + to_wstring(texIdx);

		m_mapTerrainCom.insert({ texIdx, pTerrainCubeTex });
		m_mapComponent[ID_STATIC].insert({ wstrTmp, pTerrainCubeTex });
	}
}

void CTerrainCubeMap::SaveMap(const wstring & wstrPath)
{
}

void CTerrainCubeMap::AddCube(const MapCubeInfo& tInfo)
{
	m_vecTotalCube.push_back(tInfo);

	switch (tInfo.eType)
	{
	case TYPE_LAND:
		m_vecLand.push_back(tInfo);
		break;
	case TYPE_COLLISION:
		m_vecCollision.push_back(tInfo);
		break;
	case TYPE_DECO:
		break;
	default:
		_CRASH("Invalid type");
	}

	auto itr = m_mapTerrainCom.find(tInfo.iTexIdx);
	if (itr == m_mapTerrainCom.end())
	{
		vector<_matrix> vecTmp;
		vecTmp.push_back(tInfo.matWorld);
		CTerrainCubeTex* pTerrainTex = CTerrainCubeTex::Create(m_pGraphicDev, vecTmp);
		m_mapTerrainCom.insert({tInfo.iTexIdx, pTerrainTex });
		m_mapComponent[ID_STATIC].insert({ to_wstring(tInfo.iTexIdx), pTerrainTex });
	}
	else
	{
		vector<_matrix> vecTmp;
		for (const auto& info : m_vecTotalCube)
		{
			if (info.iTexIdx == tInfo.iTexIdx)
				vecTmp.push_back(info.matWorld);
		}

		itr->second->ReCreateBuffer(vecTmp);
	}
}


void CTerrainCubeMap::DeleteCube(int iToDel)
{
	int iTexId = m_vecTotalCube[iToDel].iTexIdx;
	m_vecTotalCube.erase(m_vecTotalCube.begin() + iToDel);

	auto itr = m_mapTerrainCom.find(iTexId);
	if (itr == m_mapTerrainCom.end())
	{
		_CRASH("Invalid");
	}

	vector<_matrix> vecTmp;
	for (const auto& info : m_vecTotalCube)
	{
		if (info.iTexIdx == iTexId)
			vecTmp.push_back(info.matWorld);
	}

	
	itr->second->ReCreateBuffer(vecTmp);
}


CTerrainCubeMap * CTerrainCubeMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	CTerrainCubeMap*	pInstance = new CTerrainCubeMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(wstrPath)))
	{
		Safe_Release(pInstance);
		return nullptr;

	}

	return pInstance;
}

void CTerrainCubeMap::Free(void)
{
	CGameObject::Free();
	m_vecTotalCube.clear();
	m_vecLand.clear();
	m_vecCollision.clear();
	m_mapTerrainCom.clear();
	//for_each(m_vecTotalCube.begin(), m_vecTotalCube.end(), Safe_Release<CMapCube*>);

	
}
