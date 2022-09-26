#include "stdafx.h"
#include "..\Header\MapToolTest.h"
#include "Terrain.h"

#include "DynamicCamera.h"
#include "ImGuiMgr.h"
#include "Export_Function.h"

CMapToolTest::CMapToolTest(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	ZeroMemory(&m_tMapTool, sizeof(MapTool));
	m_tMapTool.fHeight = 1.f;
}


CMapToolTest::~CMapToolTest()
{
}

HRESULT CMapToolTest::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);

	m_pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pLayer, E_FAIL);

	LoadMap();
	return S_OK;
}

_int CMapToolTest::Update_Scene(const _float & fTimeDelta)
{
	ImGui::ShowDemoWindow(nullptr);
	IM_BEGIN("Map Editor Window");


	CImGuiMgr::MapControl(m_tMapTool, *this, m_vecCube.size());
	
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

			m_vecCube.push_back(dynamic_cast<CMapCube*>(pGameObject));
			Cube_Type(m_tMapTool.iCubeType, pGameObject);

			m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });
			m_tMapTool.iCubeCount++;
			break;

		case PICK_TERRAIN:
			pGameObject = pCube = CMapCube::Create(m_pGraphicDev, m_tMapTool, PickPos);
			if (pGameObject == nullptr)
				break;

			pCube->m_wstrName = L"MapCube_" + to_wstring(m_tMapTool.iCubeCount);
			FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pCube->m_wstrName.c_str(), pGameObject), E_FAIL);

			m_vecCube.push_back(dynamic_cast<CMapCube*>(pGameObject));
			Cube_Type(m_tMapTool.iCubeType, pGameObject);

			m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });
			m_tMapTool.iCubeCount++;
			break;

		case PICK_DELETE:
			if (!PickingOnCube(PickPos))
				break;

			m_vecCube.erase(remove_if(m_vecCube.begin(), m_vecCube.end(),
				[this](const CMapCube* mapCube)
			{
				return mapCube->m_wstrName == this->m_wDeleteName;
			}), m_vecCube.end());

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

void CMapToolTest::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CMapToolTest::Render_Scene(void)
{
}

HRESULT CMapToolTest::Ready_Layer_Environment(const _tchar * pLayerTag)
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

	
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMapToolTest::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 107)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);
	
	return S_OK;
}



void CMapToolTest::SaveMap()
{

	HANDLE hFile = CreateFile(L"../Bin/Resource/Map/Map.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed Save Map");
		return;
	}

	DWORD	dwByte = 0;
	
	for (auto iter : m_vecCube)
	{
		WriteFile(hFile, &iter->m_pTransCom->m_matWorld, sizeof(_matrix), &dwByte, nullptr);
		WriteFile(hFile, &iter->m_tMapTool, sizeof(MapTool), &dwByte, nullptr);
	}
		
	CloseHandle(hFile);

}

void CMapToolTest::LoadMap()
{

	m_pLayer->Free();
	m_vecCube.clear();
	m_tMapTool.iCubeCount = 0;

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

		Create_Cube(matCubeWorld, tMapTool);
	}

	CloseHandle(hFile);

}

void CMapToolTest::Create_Cube(_matrix & CubeWorld, MapTool& tMapTool)
{
	CGameObject* pGameObject = nullptr;
	CMapCube* pMapCube = nullptr;

	pGameObject = pMapCube = CMapCube::Create(m_pGraphicDev, CubeWorld, tMapTool);

	if (pGameObject == nullptr)
		return;

	m_pLayer->Add_GameObject(pMapCube->m_wstrName.c_str(), pGameObject);

	m_vecCube.push_back(dynamic_cast<CMapCube*>(pGameObject));
	m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });
	m_tMapTool.iCubeCount++;

}
void CMapToolTest::Cube_Type(_int eType, CGameObject* pGameObject)
{
	/*switch (eType)
	{
	case TYPE_LAND:
		m_vecLand.push_back(dynamic_cast<CMapCube*>(pGameObject));
		break;
	case TYPE_COLLISION:
		m_vecCollision.push_back(dynamic_cast<CMapCube*>(pGameObject));
		break;
	case TYPE_DECO:
		m_vecDeco.push_back(dynamic_cast<CMapCube*>(pGameObject));
		break;
	default:
		break;
	}*/
}
void CMapToolTest::Cube_DebugShow(void)
{
	if (m_tMapTool.bRendState)
	{
		for (auto& iter : m_vecCube)
		{
			iter->m_tMapTool.bRendState = true;
		}

	}
	else
	{
		for (auto& iter : m_vecCube)
		{
			iter->m_tMapTool.bRendState = false;
		}
	}


}
CMapToolTest * CMapToolTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapToolTest *	pInstance = new CMapToolTest(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMapToolTest::Free(void)
{
	CScene::Free();
}



_bool CMapToolTest::PickingOnCube(_vec3& CubeCenter)
{
	if (m_vecCube.empty())
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

	

	for (_ulong i = 0; i < m_vecCube.size(); ++i)
	{
		for (_ulong j = 0; j < FACE_END; ++j)
		{
				// 오른쪽 위
				dwVtxIdxRU[0] = m_vecCube[i]->vFaceVtx[j][0];
				dwVtxIdxRU[1] = m_vecCube[i]->vFaceVtx[j][1];
				dwVtxIdxRU[2] = m_vecCube[i]->vFaceVtx[j][2];

				// 왼쪽 아래
				dwVtxIdxLD[0] = m_vecCube[i]->vFaceVtx[j][0];
				dwVtxIdxLD[1] = m_vecCube[i]->vFaceVtx[j][2];
				dwVtxIdxLD[2] = m_vecCube[i]->vFaceVtx[j][3];

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

	_vec3& vCenter = m_vecCube[iCurCube]->vCenter;
	m_wDeleteName = m_vecCube[iCurCube]->m_wstrName;

	//Block Height Option
	_float fCurHeight = m_vecCube[iCurCube]->m_tMapTool.fHeight;

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
