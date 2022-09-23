#include "stdafx.h"
#include "..\Header\MapToolTest.h"
#include "Terrain.h"

#include "DynamicCamera.h"
#include "ImGuiMgr.h"
#include "Export_Function.h"

CMapToolTest::CMapToolTest(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
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

	return S_OK;
}

_int CMapToolTest::Update_Scene(const _float & fTimeDelta)
{

	ImGui::ShowDemoWindow(nullptr);
	IM_BEGIN("Map Editor Window");
	CImGuiMgr::MapControl(m_fFloor, m_fHeight, m_iIndex);
	
	//마우스 피킹 때 MapCube 생성
	if (Engine::Get_DIMouseState(DIM_LB) & 0X80 && m_dwTime + 1000 < GetTickCount())
	{
		PickPos = PickingOnCube(g_hWnd);
		CGameObject*		pGameObject = nullptr;
		CMapCube* pCube;
		pGameObject = pCube = CMapCube::Create(m_pGraphicDev, m_fHeight, m_iIndex, PickPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		pCube->m_wstrName = L"MapCube_" + m_iCubeCount;
		FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pCube->m_wstrName.c_str(), pGameObject), E_FAIL);
		m_vecCube.push_back(dynamic_cast<CMapCube*>(pGameObject));

		m_mapLayer.insert({ L"Layer_Terrain", m_pLayer });

		m_iCubeCount++;

		m_dwTime = GetTickCount();
	}
	
	IM_END;
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


 	pGameObject = CMapCube::Create(m_pGraphicDev, m_fHeight, m_iIndex, PickPos);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MapCube", pGameObject), E_FAIL);
		
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMapToolTest::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 103)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);

	return S_OK;
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



_vec3 CMapToolTest::PickingOnCube(HWND hWnd)
{
	//if(m_vecCube.empty())
	if(m_iCubeCount == 0)
		return _vec3(0.f, 0.f, 0.f);

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;
	_vec3		vPointAt;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	//vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	//vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	//vPoint.z = 0.f;
	vPointAt.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPointAt.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPointAt.z = 1.f;

	//vPoint 는 위치 벡터. 그래서 위치값을 저장할 수 있게 w = 1을 만들어주는 D3DXCoord 함수를 사용해야 한다.

	// 투영 -> 뷰 스페이스
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);
	D3DXVec3TransformCoord(&vPointAt, &vPointAt, &matProj);

	// 뷰 스페이스 -> 월드

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

	

	for (_ulong i = 0; i < m_iCubeCount; ++i)
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

				_bool LDtriCheck = D3DXIntersectTri(&dwVtxIdxLD[2], &dwVtxIdxLD[1], &dwVtxIdxLD[0], &vRayPos, &vRayDir, &fU, &fV, &fDist);
				if (LDtriCheck && MinDist > fDist)
				{				
					iCurCube = i;
					eFace = j;
					MinDist = fDist;		
				}

		}

	}

	if (eFace == FACE_END)
		return _vec3(0.f, 0.f, 0.f);

	_vec3& vCenter = m_vecCube[iCurCube]->vCenter;

	switch (eFace)
	{
	case FACE_LOOK:
		return _vec3(vCenter.x, vCenter.y, vCenter.z + 1.f);

	case FACE_BACK:
		return _vec3(vCenter.x,	vCenter.y,vCenter.z - 1.f);

	case FACE_LEFT:
		return _vec3(vCenter.x - 1.f, vCenter.y, vCenter.z );

	case FACE_RIGHT:
		return _vec3(vCenter.x + 1.f, vCenter.y, vCenter.z);

	case FACE_UP:
		return _vec3(vCenter.x, vCenter.y + 1.f, vCenter.z);

	case FACE_DOWN:
		return _vec3(vCenter.x,	vCenter.y - 1.f, vCenter.z);
	default:
		break;
	}

	return _vec3(0.f, 0.f, 0.f);
}
