#include "stdafx.h"
#include "..\Header\MapTool.h"
#include "Terrain.h"

#include "DynamicCamera.h"
#include "ImGuiMgr.h"
#include "TerrainCubeMap.h"

_float fP = 0.5f;
_float fM = -0.5f;
const _vec3 CMapTool::s_vFaceVtx[FACE_END][4]
{
{
	{ fM, fP, fP },
	{ fP, fP, fP },
	{ fP, fM, fP },
	{ fM, fM, fP },
},
{
	{ fM, fP, fM },
	{ fP, fP, fM },
	{ fP, fM, fM },
	{ fM, fM, fM },
},
{
	{ fP, fP, fP },
	{ fP, fM, fP },
	{ fP, fM, fM },
	{ fP, fP, fM },
},
{
	{ fM, fP,  fP },
	{ fM, fM,  fP },
	{ fM, fM,  fM },
	{ fM, fP,  fM },
},
{
	{ fP, fP, fP },
	{ fP, fP, fM },
	{ fM, fP, fM },
	{ fM, fP, fP },
},
{
	{ fP, fM, fP },
	{ fP, fM, fM },
	{ fM, fM, fM },
	{ fM, fM, fP },
}
};

CMapTool::CMapTool(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
	ZeroMemory(&m_tMapTool, sizeof(MapTool));
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
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	m_pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pLayer, E_FAIL);
	
	return S_OK;
}

_int CMapTool::Update_Scene(const _float & fTimeDelta)
{
	//ImGui::ShowDemoWindow(nullptr);
	IM_BEGIN("Map Editor Window");


	CImGuiMgr::MapControl(m_tMapTool,m_fFar, m_pCubeMap);
	
	m_pDCamera->Set_Far(m_fFar);

	//마우스 피킹 때 MapCube 생성
	if (MouseKeyDown(DIM_LB))
	{
		_vec3 PickPos;
		int iToDelIdx = 0;

		switch (m_tMapTool.iPickingOption)
		{
		case PICK_CUBE:
			if (!PickingOnCube(PickPos, iToDelIdx))
				break;

			// todo 높이값 설정하기
			{
				_matrix matWorld;
				CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, m_tMapTool.fHeight, 1.f }, { 0.f, 0.f, 0.f }, PickPos);
				m_pCubeMap->AddCube({ matWorld, m_tMapTool.iTexIdx, (CUBETYPE)m_tMapTool.iCubeType, m_tMapTool.fHeight });
			}
			break;

		case PICK_DELETE:
			if (!PickingOnCube(PickPos, iToDelIdx))
				break;

			m_pCubeMap->DeleteCube(iToDelIdx);
		
			break;

		default:
			break;
		}
	}

	Cube_DebugShow();

	IM_END;

	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CMapTool::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CMapTool::Render_Scene(void)
{
}

HRESULT CMapTool::Ready_Layer_Environment()
{

	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = m_pDCamera = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f), D3DXToDegree(60.f), (_float)WINCX/WINCY, 0.1f, m_fFar);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);
	g_cam = m_pDCamera;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	m_pCubeMap = CTerrainCubeMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCubeMap, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"TerrainCubeMap", m_pCubeMap), E_FAIL);


	_matrix firstCube;
	D3DXMatrixIdentity(&firstCube);
	firstCube._41 = 0.5f;
	firstCube._42 = 0.5f;
	firstCube._43 = 0.5f;
	m_pCubeMap->AddCube({firstCube, 0, TYPE_LAND, 1.f});

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


void CMapTool::LoadMap(wstring wstrFileName)
{

	//m_pLayer->Free();
	//m_vecTotalCube.clear();
	//m_vecLand.clear();
	//ZeroMemory(&m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ);
	//m_tMapTool.iCubeCount = 0;

	//HANDLE hFile = CreateFile(wstrFileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MSG_BOX("Failed Load Map");
	//	return;
	//}
	//DWORD	dwByte = 0;

	//size_t vecSize;
	//_matrix matCubeWorld;
	//MapTool tMapTool;

	//ReadFile(hFile, &vecSize, sizeof(size_t), &dwByte, nullptr);

	//while (vecSize--)
	//{
	//	
	//	ReadFile(hFile, &matCubeWorld, sizeof(_matrix), &dwByte, nullptr);
	//	ReadFile(hFile, &tMapTool, sizeof(MapTool), &dwByte, nullptr);
	//	Create_Cube(matCubeWorld, tMapTool);
	//}

	//ReadFile(hFile, &m_fHeight, sizeof(_float) * VTXCNTX * VTXCNTZ, &dwByte, nullptr);

	//CloseHandle(hFile);
}

void CMapTool::Create_Cube(_matrix & CubeWorld, MapTool& tMapTool)
{
	//CGameObject* pGameObject = nullptr;
	//CMapCube* pMapCube = nullptr;

	//pGameObject = pMapCube = CMapCube::Create(m_pGraphicDev);

	//if (pGameObject == nullptr)
	//	return;

	//m_pLayer->Add_GameObject(pMapCube->m_wstrName.c_str(), pGameObject);

	//m_vecTotalCube.push_back(dynamic_cast<CMapCube*>(pGameObject));
	//m_tMapTool.iCubeCount++;

}

void CMapTool::Cube_DebugShow(void)
{
	//if (m_tMapTool.bRendState)
	//{
	//	for (auto& iter : m_vecTotalCube)
	//	{
	//		iter->m_tMapTool.bRendState = true;
	//	}

	//}
	//else
	//{
	//	for (auto& iter : m_vecTotalCube)
	//	{
	//		iter->m_tMapTool.bRendState = false;
	//	}
	//}


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



_bool CMapTool::PickingOnCube(_vec3& CubeCenter, int& iToDelIdx)
{
	const vector<MapCubeInfo>& vecTotalCube = m_pCubeMap->GetTotalCubes();

	if (vecTotalCube.empty())
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

	
	_vec3 vCam = m_pDCamera->GetEys();
	_vec3 vFaceVtx[FACE_END][4];


	for (_ulong i = 0; i < vecTotalCube.size(); ++i)
	{
		_vec3 vPos;
		vPos.x = vecTotalCube[i].matWorld._41;
		vPos.y = vecTotalCube[i].matWorld._42;
		vPos.z = vecTotalCube[i].matWorld._43;
		_vec3 vDiff = vPos - vCam;

		if (D3DXVec3LengthSq(&vDiff) > 900.f) continue; // 큐브 거리가 카메라부터 30 초과면 체크 안함

		// 현재 큐브의 world를 통해 face 생성
		memcpy(&vFaceVtx, &s_vFaceVtx, sizeof(vFaceVtx));
		for (int j = 0; j < FACE_END; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				D3DXVec3TransformCoord(&vFaceVtx[j][k], &vFaceVtx[j][k], &vecTotalCube[i].matWorld);
			}
		}


		for (_ulong j = 0; j < FACE_END; ++j)
		{
				// 오른쪽 위
				dwVtxIdxRU[0] = vFaceVtx[j][0];
				dwVtxIdxRU[1] = vFaceVtx[j][1];
				dwVtxIdxRU[2] = vFaceVtx[j][2];

				// 왼쪽 아래
				dwVtxIdxLD[0] = vFaceVtx[j][0];
				dwVtxIdxLD[1] = vFaceVtx[j][2];
				dwVtxIdxLD[2] = vFaceVtx[j][3];

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


	_vec3 vCenter{ 0.f, 0.f, 0.f };
	D3DXVec3TransformCoord(&vCenter, &vCenter, &vecTotalCube[iCurCube].matWorld);
	
	if (m_tMapTool.iPickingOption == PICK_DELETE)
	{
		iToDelIdx = iCurCube;
		return true;
	}

	//Block Height Option
	_float fCurHeight = vecTotalCube[iCurCube].fHeight;
	_float fCenterY = fCurHeight * 0.5f + m_tMapTool.fHeight * 0.5f + vCenter.y;


	switch (eFace)
	{
	case FACE_LOOK:
		CubeCenter = _vec3(vCenter.x, vCenter.y, vCenter.z + 1.f);
		break;
	case FACE_BACK:
		CubeCenter = _vec3(vCenter.x,	vCenter.y,vCenter.z - 1.f);
		break;
	case FACE_LEFT:
		CubeCenter = _vec3(vCenter.x + 1.f, vCenter.y, vCenter.z );
		break;
	case FACE_RIGHT:
		CubeCenter = _vec3(vCenter.x - 1.f, vCenter.y, vCenter.z);
		break;
	case FACE_UP:
		CubeCenter = _vec3(vCenter.x, fCenterY, vCenter.z);
		break;
	case FACE_DOWN:
		CubeCenter = _vec3(vCenter.x, fCenterY, vCenter.z);
		break;
	default:
		return false;
	}

	

	return true;
}



