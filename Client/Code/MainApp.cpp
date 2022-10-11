#include "stdafx.h"
#include "..\Header\MainApp.h"
#include "AbstFactory.h"
#include "ImGuiMgr.h"
#include "AnimationTool.h"
#include "MapTool.h"
#include "UItool.h"
#include "TerrainCubeMap.h"
#include "Stage.h"
#include "time.h"
#include "ArrowCubeMgr.h"
#include "PlayerController.h"
#include "StatComponent.h"
#include "PlayerController.h"
#include "GeomancerController.h"
#include "ZombieController.h"
#include "CreeperController.h"
#include "SkeletonController.h"
#include "EnchanterController.h"
#include "RedStoneCubeController.h"
#include "RedStoneMonstrosityController.h"


USING(Engine)

CMainApp::CMainApp()
	: m_pGraphicDev(nullptr)
{
}


CMainApp::~CMainApp()
{
	
}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);
	CAbstFactory::Ready_Factories(m_pGraphicDev);

	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	// CClientServiceMgr::GetInstance()->ReadyClientService();
	srand((unsigned)time(NULL));


	return S_OK;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	CGameUtilMgr::s_fTimeDelta = fTimeDelta;

	Engine::SetUp_InputDev();

	NULL_CHECK_RETURN(m_pManagementClass, -1);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	CImGuiMgr::LoggerWindow();
	CImGuiMgr::SceneSwitcher();

	m_pManagementClass->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(void)
{
	NULL_CHECK(m_pManagementClass);

	m_pManagementClass->LateUpdate_Scene();


}

void CMainApp::Render_MainApp(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	Engine::Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	// 디바이스 초기화
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	// InputDev
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	// Font
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"바탕", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default_Normal", L"바탕", 10, 13, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Jinji", L"궁서", 15, 15, FW_NORMAL), E_FAIL);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDev);

	// 매니저 초기화
	CArrowCubeMgr::Create(m_pGraphicDev);

	

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement)
{

	// scene 생성시 Management에 자동 set됩니다. scene을 보존하고 싶으면 ref 1올려서 저장하기
	// Scene::Ready_Scene 에서 Management에 set
	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	CSceneFactory::LoadScene("Loading1", "Stage_Default", true ,0);

	return S_OK;
}

HRESULT CMainApp::Ready_Proto()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexCom", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Terrain_WaterCom", CTerrainShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Terrain_Water_Test1.fx", VTXCNTX, VTXCNTZ, VTXITV
		, _vec2(0.f, 0.f), _vec2(1.f, 0.f), _vec2(1.f, 1.f), _vec2(0.f, 1.f))), E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Terrain_Water_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Water4.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexCom", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinecraftCubeTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MinscraftCubeTile/CubeTile_%d.dds", TEX_CUBE, 155)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", Engine::CTransform::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossCubeTile", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/BossCubeTile/boss_%d.dds", TEX_CUBE, 12)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WeaponTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/weapon/weapon_%d.png", TEX_NORMAL, 5)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Electric", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/T_ElectricArcs.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Cloud", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/T_Smoke_deformedBall_Small.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Circle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/T_fullcircle_explosion_124_spritesheet.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Spit", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/3DView_Spotlight_Pattern.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Crack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Crack1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Stun", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Stun.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Heal", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Heal_Circle1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Heal_Particle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Heart_Particle.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Lava_Particle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/JJH/Lava_Particlel.png", TEX_NORMAL)), E_FAIL);

	// FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcShaderCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.0f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcShaderCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx",_vec2(0.0f,0.0f),_vec2(0.25f,0.f),_vec2(0.25f,0.25f),_vec2(0.0f,0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShockPowderCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/ShockPowder_Cloud.fx",_vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CreeperExCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/CreeperEx_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WalkCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Walk_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RollCloudCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Roll_Cloud.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereCom_Ring", CSphereMesh::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Half_Shpere.fx", L"../Bin/Resource/Texture/JJH/Test.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereCom_Lava", CSphereMesh::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Half_Shpere.fx", L"../Bin/Resource/Texture/JJH/Test.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereCom_L", CSphereMesh::Create(m_pGraphicDev, L"../Bin/Resource/Shader/TextureMapping.fx", L"../Bin/Resource/Texture/JJH/YellowSphere.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SphereCom_M", CSphereMesh::Create(m_pGraphicDev, L"../Bin/Resource/Shader/TextureMapping.fx", L"../Bin/Resource/Texture/JJH/YellowSphere.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StunCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation.fx", _vec2(0.0f, 0.0f), _vec2(1.f, 0.0f), _vec2(1.f, 1.0f), _vec2(0.0f, 1.f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HealCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Crack.fx", _vec2(0.0f, 0.0f), _vec2(1.f, 0.0f), _vec2(1.f, 1.0f), _vec2(0.0f, 1.f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HeartParticleCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Crack.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.f), _vec2(0.25f, 1.0f), _vec2(0.0f, 1.f))), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Lava_ParticleCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Crack.fx", _vec2(0.0f, 0.0f), _vec2(0.25f, 0.0f), _vec2(0.25f, 0.25f), _vec2(0.0f, 0.25f))), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CrackCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Crack.fx", _vec2(0.0f, 0.0f), _vec2(0.5f, 0.0f), _vec2(0.5f, 0.5f), _vec2(0.0f, 0.5f))), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Crack_DecalCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Decal.fx", _vec2(0.0f, 0.0f), _vec2(0.5f, 0.0f), _vec2(0.5f, 0.5f), _vec2(0.0f, 0.5f))), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShaderCircleCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/UVAnimation_Circle.fx",_vec2(0.0f, 0.0f), _vec2(0.25f, 0.0f), _vec2(0.25f, 0.5f), _vec2(0.0f, 0.5f))), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpitCom", CRcShader::Create(m_pGraphicDev, L"../Bin/Resource/Shader/Golem_Spit.fx", _vec2(0.0f, 0.0f), _vec2(1.f, 0.0f), _vec2(1.f, 1.f), _vec2(0.0f, 1.f))), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Sword", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_0.png", 0.08f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Glaive", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_1.png", 0.08f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Crossbow", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_2.png", 0.08f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Axe", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_3.png", 0.08f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_VoxelTex_Bow", CVoxelTex::Create(m_pGraphicDev, "../Bin/Resource/Texture/weapon/weapon_4.png", 0.08f)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainCubeTexCom",CTerrainCubeTex::Create(m_pGraphicDev, L"../Bin/Resource/Map/MapTest.dat", 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CalculatorCom", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CollisionCom", CCollisionCom::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StatCom", CStatComponent::Create()), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlantTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Plant/plant_%d.png", TEX_NORMAL, 3)), E_FAIL);


	// controller
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerController", CPlayerController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GeomancerController", CGeomancerController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ZombieController", CZombieController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CreeperController", CCreeperController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkeletonController", CSkeletonController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EnchanterController", CEnchanterController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RedStoneCubeController", CRedStoneCubeController::Create()), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RedStoneMonstrosityController", CRedStoneMonstrosityController::Create()), E_FAIL);

	// UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTexCom", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UI_Texture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/UI_%d.png", TEX_NORMAL, 18)), E_FAIL);
	// FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Loading", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Loading/loading%d.png", TEX_NORMAL, 1)), E_FAIL);

	//items
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Emerald", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SingleTex/Emerald.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Apple", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SingleTex/Apple.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Bread", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SingleTex/Bread.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DenseBrew", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SingleTex/DenseBrew.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ArrowBundle", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SingleTex/ArrowBundle.png", TEX_NORMAL)), E_FAIL);

	
	return S_OK;
}

CMainApp * CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("MainApp Create Failed");
		Safe_Release<CMainApp*>(pInstance);		
	}
	
	return pInstance;
}

void CMainApp::Free(void)
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	CArrowCubeMgr::DeleteInst();
	CAbstFactory::ReleaseGraphicDev();
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);


	Engine::Release_Utility();
	Engine::Release_System();
	// CClientServiceMgr::GetInstance()->DestroyInstance();
}
