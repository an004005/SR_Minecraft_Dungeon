#include "stdafx.h"
#include "..\Header\TerrainCubeMap.h"
#include "MapCube.h"
#include <set>

CTerrainCubeMap::CTerrainCubeMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tMapTool, sizeof(MapTool));
	ZeroMemory(&m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ);
	ZeroMemory(&m_fCollisionPos, sizeof(_bool) * VTXCNTX * VTXCNTZ);
}

CTerrainCubeMap::~CTerrainCubeMap()
{
}


HRESULT CTerrainCubeMap::Ready_Object(const wstring& wstrPath)
{
	LoadMap(wstrPath);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

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
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& cube : m_vecTerrainCom)
	{
		m_pTextureCom->Set_Texture(cube.second->m_iTexIdx);
		cube.second->Render_Buffer();
	}

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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

		MapCubeInfo tMapCubeInfo{ matCubeWorld, tMapTool.iTexIdx, (CUBETYPE)tMapTool.iCubeType };
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
		case TYPE_DECO:
			m_vecDeco.push_back(tMapCubeInfo);
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

	for (auto texIdx : SetTexIdx)
	{
		CTerrainCubeTex* pTerrainCubeTex = CTerrainCubeTex::Create(m_pGraphicDev, wstrPath, texIdx);
		NULL_CHECK(pTerrainCubeTex);

		wstring wstrTmp = L"TerrainCubeTexCom" + to_wstring(texIdx);
		m_vecTerrainCom.push_back({ wstrTmp, pTerrainCubeTex });

		m_mapComponent[ID_STATIC].insert({ m_vecTerrainCom.back().first.c_str(), pTerrainCubeTex });
	}

	// terrain에 충돌 큐브가 있는지 저장한다.

	_int SetPosX[4] = { 0.5f, 0.5f, -0.5f, -0.5f };
	_int SetPosZ[4] = { 0.5f, -0.5f, 0.5f, -0.5f };

	for (auto iter : m_vecCollision)
	{
		_vec3 vCenter{ 0.f,0.f,0.f };
		D3DXVec3TransformCoord(&vCenter, &vCenter, &iter.matWorld);
		//////////// coll
		Engine::Add_StaticCollision(vCenter, 1.f);

		for (_int i = 0; i < 4; ++i)
		{
			_int iCenterx = (_int)vCenter.x + SetPosX[i];
			_int iCenterz = (_int)vCenter.z + SetPosZ[i];
			m_fCollisionPos[iCenterx][iCenterz] = true;
		}
	}
}

HRESULT CTerrainCubeMap::Add_Component(void)
{
	CComponent* pComponent = nullptr;


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MinecraftCubeTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MinecraftCubeTexture", pComponent });

	
	return S_OK;
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
	m_vecDeco.clear();
	m_vecTerrainCom.clear();
	//for_each(m_vecTotalCube.begin(), m_vecTotalCube.end(), Safe_Release<CMapCube*>);

	
}
