#include "stdafx.h"
#include "..\Header\MapTool.h"
#include "Terrain.h"

#include "DynamicCamera.h"
#include "ImGuiMgr.h"
#include "TerrainCubeMap.h"

CMapTool::CMapTool(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	ZeroMemory(&m_tMapTool, sizeof(MapTool));
	ZeroMemory(&m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ);
	m_tMapTool.fHeight = 1.f;
}


CMapTool::~CMapTool()
{
}

HRESULT CMapTool::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);

	m_pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pLayer, E_FAIL);

	//LoadMap();
	return S_OK;
}

_int CMapTool::Update_Scene(const _float & fTimeDelta)
{
	ImGui::ShowDemoWindow(nullptr);
	IM_BEGIN("Map Editor Window");


	CImGuiMgr::MapControl(m_tMapTool, *this, m_vecTotalCube.size());

	//마우스 피킹 때 MapCube 생성
	if (Engine::Get_DIMouseState(DIM_LB) & 0X80 && m_dwTime + 200 < GetTickCount())
	{
		_vec3 PickPos;
		CGameObject*		pGameObject = nullptr;
		CMapCube* pCube;

		switch (m_tMapTool.iPickingOption)
		{
		case PICK_CUBE:
			if (!PickingOnCube(PickPos))
				break;

			pGameObject = pCube = CMapCube::Create(m_pGraphicDev, m_tMapTool, PickPos);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);

			pCube->m_wstrName = L"MapCube_" + to_wstring(m_tMapTool.iCubeCount);
			FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pCube->m_wstrName.c_str(), pGameObject), E_FAIL);

			m_vecTotalCube.push_back(dynamic_cast<CMapCube*>(pGameObject));

			m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });
			m_tMapTool.iCubeCount++;
			break;

		case PICK_TERRAIN:
			pGameObject = pCube = CMapCube::Create(m_pGraphicDev, m_tMapTool, PickPos);
			if (pGameObject == nullptr)
				break;

			pCube->m_wstrName = L"MapCube_" + to_wstring(m_tMapTool.iCubeCount);
			FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pCube->m_wstrName.c_str(), pGameObject), E_FAIL);

			m_vecTotalCube.push_back(dynamic_cast<CMapCube*>(pGameObject));

			m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });
			m_tMapTool.iCubeCount++;
			break;

		case PICK_DELETE:
			if (!PickingOnCube(PickPos))
				break;

			m_vecTotalCube.erase(remove_if(m_vecTotalCube.begin(), m_vecTotalCube.end(),
				[this](const CMapCube* mapCube)
			{
				return mapCube->m_wstrName == this->m_wDeleteName;
			}), m_vecTotalCube.end());

			if (FAILED(m_pLayer->Delete_GameObject(m_wDeleteName.c_str())))
				break;

			break;

		default:
			break;
		}

		m_dwTime = GetTickCount();
	}

	Cube_DebugShow();

	IM_END;




	//Test



	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CMapTool::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CMapTool::Render_Scene(void)
{
}

HRESULT CMapTool::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	/*CMapCube* pCube;
	for (int i = 0; i < VTXCNTZ; ++i)
	{
		for (int j = 0; j < VTXCNTX; ++j)
		{
			pGameObject = pCube = CMapCube::Create(m_pGraphicDev, m_tMapTool, _vec3((_float)i, 1.f, (_float)j));
			NULL_CHECK_RETURN(pGameObject, E_FAIL);

			pCube->m_wstrName = L"MapCube_" + to_wstring(m_tMapTool.iCubeCount);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pCube->m_wstrName.c_str(), pGameObject), E_FAIL);

			m_vecTotalCube.push_back(dynamic_cast<CMapCube*>(pGameObject));
			m_tMapTool.iCubeCount++;
		}
	}*/
	

		
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMapTool::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 107)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);
	
	return S_OK;
}



void CMapTool::SaveMap(wstring wstrFileName)
{
	//input data in m_vecLand
	for (auto iter : m_vecTotalCube)
	{
		if (iter->m_tMapTool.iCubeType == TYPE_LAND)
			m_vecLand.push_back(iter);
	}

	//Set data in m_fHeight
	Set_CubeCoordinate();

	HANDLE hFile = CreateFile(wstrFileName.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
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
		WriteFile(hFile, &iter->m_pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
		WriteFile(hFile, &iter->m_tMapTool, sizeof(MapTool), &dwByte, nullptr);
	}
	

	WriteFile(hFile, &m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ, &dwByte, nullptr);
		
	CloseHandle(hFile);

}

void CMapTool::LoadMap(wstring wstrFileName)
{

	m_pLayer->Free();
	m_vecTotalCube.clear();
	m_vecLand.clear();
	ZeroMemory(&m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ);
	m_tMapTool.iCubeCount = 0;

	HANDLE hFile = CreateFile(wstrFileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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
		Create_Cube(matCubeWorld, tMapTool);
	}

	ReadFile(hFile, &m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ, &dwByte, nullptr);

	CloseHandle(hFile);

}

void CMapTool::Create_Cube(_matrix & CubeWorld, MapTool& tMapTool)
{
	CGameObject* pGameObject = nullptr;
	CMapCube* pMapCube = nullptr;

	pGameObject = pMapCube = CMapCube::Create(m_pGraphicDev, CubeWorld, tMapTool);

	if (pGameObject == nullptr)
		return;

	m_pLayer->Add_GameObject(pMapCube->m_wstrName.c_str(), pGameObject);

	m_vecTotalCube.push_back(dynamic_cast<CMapCube*>(pGameObject));
	m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });
	m_tMapTool.iCubeCount++;

}

void CMapTool::Cube_DebugShow(void)
{
	if (m_tMapTool.bRendState)
	{
		for (auto& iter : m_vecTotalCube)
		{
			iter->m_tMapTool.bRendState = true;
		}

	}
	else
	{
		for (auto& iter : m_vecTotalCube)
		{
			iter->m_tMapTool.bRendState = false;
		}
	}


}
CMapTool * CMapTool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapTool *	pInstance = new CMapTool(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMapTool::Free(void)
{
	CScene::Free();
}



_bool CMapTool::PickingOnCube(_vec3& CubeCenter)
{
	if (m_vecTotalCube.empty())
		return false;

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3		vPoint;
	_vec3		vPointAt;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);


	vPointAt.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPointAt.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPointAt.z = 1.f;


	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);
	D3DXVec3TransformCoord(&vPointAt, &vPointAt, &matProj);


	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matView);
	D3DXVec3TransformCoord(&vPointAt, &vPointAt, &matView);


	_vec3 vRayPos = vPoint;
	_vec3 vRayDir = vPointAt - vPoint;

	_vec3	dwVtxIdxRU[3]{};
	_vec3	dwVtxIdxLD[3]{};
	_float	fU, fV, fDist;


	_float	MinDist = 10000000;
	_int eFace = FACE_END;
	_ulong iCurCube = 0;

	

	for (_ulong i = 0; i < m_vecTotalCube.size(); ++i)
	{
		for (_ulong j = 0; j < FACE_END; ++j)
		{
				// 오른쪽 위
				dwVtxIdxRU[0] = m_vecTotalCube[i]->vFaceVtx[j][0];
				dwVtxIdxRU[1] = m_vecTotalCube[i]->vFaceVtx[j][1];
				dwVtxIdxRU[2] = m_vecTotalCube[i]->vFaceVtx[j][2];

				// 왼쪽 아래
				dwVtxIdxLD[0] = m_vecTotalCube[i]->vFaceVtx[j][0];
				dwVtxIdxLD[1] = m_vecTotalCube[i]->vFaceVtx[j][2];
				dwVtxIdxLD[2] = m_vecTotalCube[i]->vFaceVtx[j][3];

				//광선과 닿은 면이 있다면 vRayPos와의 거리가 최소값인지 확인한다.
				_bool RUtriCheck = D3DXIntersectTri(&dwVtxIdxRU[1], &dwVtxIdxRU[2], &dwVtxIdxRU[0], &vRayPos, &vRayDir, &fU, &fV, &fDist);
				if (RUtriCheck && MinDist > fDist)
				{
					iCurCube = i;
					eFace = j;
					MinDist = fDist;
					continue;
				}

				_bool LDtriCheck = D3DXIntersectTri(&dwVtxIdxLD[0], &dwVtxIdxLD[1], &dwVtxIdxLD[2], &vRayPos, &vRayDir, &fU, &fV, &fDist);
				if (LDtriCheck && MinDist > fDist)
				{				
					iCurCube = i;
					eFace = j;
					MinDist = fDist;		
				}

		}

	}

	if (eFace == FACE_END)
		return false;

	_vec3& vCenter = m_vecTotalCube[iCurCube]->vCenter;
	m_wDeleteName = m_vecTotalCube[iCurCube]->m_wstrName;

	//Block Height Option
	_float fCurHeight = m_vecTotalCube[iCurCube]->m_tMapTool.fHeight;

	_float fCenterY = fCurHeight * 0.5f + m_tMapTool.fHeight * 0.5f;


	switch (eFace)
	{
	case FACE_LOOK:
		CubeCenter = _vec3(vCenter.x, vCenter.y, vCenter.z + 1.f);
		break;
	case FACE_BACK:
		CubeCenter = _vec3(vCenter.x,	vCenter.y,vCenter.z - 1.f);
		break;
	case FACE_LEFT:
		CubeCenter = _vec3(vCenter.x - 1.f, vCenter.y, vCenter.z );
		break;
	case FACE_RIGHT:
		CubeCenter = _vec3(vCenter.x + 1.f, vCenter.y, vCenter.z);
		break;
	case FACE_UP:
		CubeCenter = _vec3(vCenter.x, vCenter.y + fCenterY, vCenter.z);
		break;
	case FACE_DOWN:
		CubeCenter = _vec3(vCenter.x,	vCenter.y - fCenterY, vCenter.z);
		break;
	default:
		return false;
	}

	return true;
}

void CMapTool::Set_CubeCoordinate(void)
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


void CMapTool::CubeHeight(_float x, _float z)
{
	_float fMostHeightValue = 0;
	_float fLength = 0;
	for (auto iter : m_vecLand)
	{
		if ((iter->vCenter.x - 0.5f) == x && (iter->vCenter.z - 0.5f) == z)
		{
			if (fMostHeightValue < iter->vCenter.y)
			{
				fMostHeightValue = iter->vCenter.y;
				fLength = iter->m_tMapTool.fHeight / 2.f;
			}
		}
	}

	m_fHeight[(_int)x][(_int)z] = fMostHeightValue + fLength;
}
