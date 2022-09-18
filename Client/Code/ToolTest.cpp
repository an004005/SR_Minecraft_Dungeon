#include "stdafx.h"
#include "ToolTest.h"
#include "DynamicCamera.h"
#include "TestCube.h"

#include "ImGuizmo.h"

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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TransformCom", CTransform::Create()), E_FAIL);



	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = m_pCam = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	// skybox
	pGameObject = CTestCube::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);
	m_pSelectedTransform = dynamic_cast<CTestCube*>(pGameObject)->m_pTransCom;

	m_mapLayer.insert({ L"TestLayer", pLayer });
	return CScene::Ready_Scene();
}

_int CToolTest::Update_Scene(const _float& fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

void CToolTest::ImGuiFrameMain()
{
	ImGuiIO& io = ImGui::GetIO();
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    if (ImGui::IsKeyPressed(90))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(69))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(82)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;

    float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	memcpy(matrixTranslation, &m_pSelectedTransform->m_vInfo[INFO_POS], sizeof(matrixTranslation));
	memcpy(matrixRotation, &m_pSelectedTransform->m_vAngle, sizeof(matrixRotation));
	memcpy(matrixScale, &m_pSelectedTransform->m_vScale, sizeof(matrixScale));

    ImGui::InputFloat3("Tr", matrixTranslation);
    ImGui::InputFloat3("Rt", matrixRotation);
    ImGui::InputFloat3("Sc", matrixScale);

	memcpy(&m_pSelectedTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
	memcpy( &m_pSelectedTransform->m_vAngle, matrixRotation,sizeof(matrixRotation));
	memcpy(&m_pSelectedTransform->m_vScale, matrixScale, sizeof(matrixScale));

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }

    // static bool useSnap(false);
    // if (ImGui::IsKeyPressed(83))
    //     useSnap = !useSnap;
    // ImGui::Checkbox("##something", &useSnap);
    // ImGui::SameLine();
    // _vec3 snap;
    // switch (mCurrentGizmoOperation)
    // {
    // case ImGuizmo::TRANSLATE:
    //     snap = config.mSnapTranslation;
    //     ImGui::InputFloat3("Snap", &snap.x);
    //     break;
    // case ImGuizmo::ROTATE:
    //     snap = config.mSnapRotation;
    //     ImGui::InputFloat("Angle Snap", &snap.x);
    //     break;
    // case ImGuizmo::SCALE:
    //     snap = config.mSnapScale;
    //     ImGui::InputFloat("Scale Snap", &snap.x);
    //     break;
    // }
    // ImGuiIO& io = ImGui::GetIO();
    // ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    // ImGuizmo::Manipulate(camera.mView.m16, camera.mProjection.m16, mCurrentGizmoOperation, mCurrentGizmoMode, matrix.m16, NULL, useSnap ? &snap.x : NULL);

	_matrix tmp;
	D3DXMatrixIdentity(&tmp);
	float fview[16];
	float fpro[16];
	float fiden[16];
	memcpy(fiden, &tmp, sizeof(_matrix));
	memcpy(fview, m_pCam->GetView(), sizeof(_matrix));
	memcpy(fpro, m_pCam->GetPrj(), sizeof(_matrix));

	ImGuizmo::DrawGrid(fview, fpro, fiden, 100.f);
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

