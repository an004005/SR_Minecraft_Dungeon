#include "stdafx.h"
#include "ToolTest.h"
#include "DynamicCamera.h"
#include "TestCube.h"
#include "ImGuiMgr.h"
#include "SkeletalCube.h"

CToolTest::CToolTest(LPDIRECT3DDEVICE9 pGraphicDev): CScene(pGraphicDev)
{
}

CToolTest::~CToolTest()
{
}

HRESULT CToolTest::Ready_Scene()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 141)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/weapon/weapon_%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Sword", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_0.png", 0.08f)), E_FAIL);


	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = m_pCam = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);
		
	// skeletal
	pGameObject = m_Skel = CSkeletalCube::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Skeletal", pGameObject), E_FAIL);
	// m_pSelectedTransform = dynamic_cast<CTestCube*>(pGameObject)->m_pTransCom;


	m_mapLayer.insert({ L"TestLayer", pLayer });

	return CScene::Ready_Scene();
}

_int CToolTest::Update_Scene(const _float& fTimeDelta)
{
	// ImGui::ShowDemoWindow(nullptr);

	IM_BEGIN("Skeletal Editor");
	CImGuiMgr::SkeletalEditor(m_pCam, m_Skel);
	IM_END;

	// ImGui::SetNextWindowSize(ImVec2(960, 300));
	IM_BEGIN("Animation Editor");
	CImGuiMgr::AnimationEditor(m_Skel);
	IM_END;

	return CScene::Update_Scene(fTimeDelta);
}

void CToolTest::Free()
{
	CScene::Free();
}

CToolTest* CToolTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolTest*	pInstance = new CToolTest(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

