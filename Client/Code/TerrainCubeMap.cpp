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
	m_vecTotalCube.clear();
	m_vecLand.clear();
	m_vecCollision.clear();

	//for_each(m_mapTerrainCom.begin(), m_mapTerrainCom.end(), CDeleteMap());
	//m_mapTerrainCom.clear();

	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed Load Map");
		return;
	}
	DWORD	dwByte = 0;

	size_t vecSize;
	MapCubeInfo tMapCubeInfo;

	ReadFile(hFile, &vecSize, sizeof(size_t), &dwByte, nullptr);

	while (vecSize--)
	{
		ReadFile(hFile, &tMapCubeInfo, sizeof(MapCubeInfo), &dwByte, nullptr);
	
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


	set<_int> stexidx;
	for (auto i : m_vecTotalCube)
		stexidx.insert(i.iTexIdx);

	for (auto texidx : stexidx)
	{
		vector<_matrix> vecmatWorld;

		for (auto i : m_vecTotalCube)
		{
			if (texidx == i.iTexIdx)
				vecmatWorld.push_back(i.matWorld);	
		}


		auto iter = m_mapTerrainCom.find(texidx);
		if (iter == m_mapTerrainCom.end())
		{
			CTerrainCubeTex* pTerrainTex = CTerrainCubeTex::Create(m_pGraphicDev, vecmatWorld);
			m_mapTerrainCom.insert({ texidx, pTerrainTex });
			m_mapComponent[ID_STATIC].insert({ to_wstring(texidx), pTerrainTex });
		}		
		else
			iter->second->ReCreateBuffer(vecmatWorld);
	}

	for (auto& coll : m_vecCollision)
	{
		_vec3 vCenter;
		vCenter.x = coll.matWorld._41;
		vCenter.y = coll.matWorld._42;
		vCenter.z = coll.matWorld._43;
		Engine::Add_StaticCollision(vCenter, 0.5f);
	}
}

void CTerrainCubeMap::SaveMap(const wstring & wstrPath)
{
	//Set data in m_fHeight
	Set_CubeCoordinate();

	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed Save Map");
		return;
	}

	DWORD	dwByte = 0;
	
	size_t m_vecTotalCubeSize = m_vecTotalCube.size();


	WriteFile(hFile, &m_vecTotalCubeSize, sizeof(size_t), &dwByte, nullptr);

	for (auto iter : m_vecTotalCube)
	{
		WriteFile(hFile, &iter, sizeof(MapCubeInfo), &dwByte, nullptr);
	}
	
	WriteFile(hFile, &m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ, &dwByte, nullptr);
		
	CloseHandle(hFile);
}


void CTerrainCubeMap::Set_CubeCoordinate(void)
{
	_ulong	dwIndex = 0;

	for (_ulong i = 0; i < VTXCNTZ; ++i)
	{
		for (_ulong j = 0; j < VTXCNTX; ++j)
		{
			CubeHeight(_float(j) * VTXITV, _float(i) * VTXITV);
		}
	}
}


void CTerrainCubeMap::CubeHeight(_float x, _float z)
{
	_float fMostHeightValue = 0;
	_float fLength = 0;
	for (auto iter : m_vecLand)
	{
		_vec3 vCenter{ 0.f, 0.f, 0.f };
		D3DXVec3TransformCoord(&vCenter, &vCenter, &iter.matWorld);

		if ((vCenter.x - 0.5f) == x && (vCenter.z - 0.5f) == z)
		{
			if (fMostHeightValue < vCenter.y)
			{
				fMostHeightValue = vCenter.y;
				fLength = iter.fHeight / 2.f;
			}
		}
	}

	m_fHeight[(_int)x][(_int)z] = fMostHeightValue + fLength;
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
}
