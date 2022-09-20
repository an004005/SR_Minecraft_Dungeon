#include "stdafx.h"
#include "..\Header\ImGuiMgr.h"
#include "ImGuizmo.h"

IMPLEMENT_SINGLETON(CImGuiMgr)

ImGuiTextBuffer CImGuiMgr::log;

CImGuiMgr::CImGuiMgr()
{
}


CImGuiMgr::~CImGuiMgr()
{
}

void CImGuiMgr::TransformEdit(CCamera* pCamera, CTransform* pTransform)
{
	ImGui::Begin("Transform");
	ImGuizmo::BeginFrame();
	static float snap[3] = { 1.f, 1.f, 1.f };
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
	_matrix matWorld = pTransform->m_matWorld;

	ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation);
	ImGui::InputFloat3("Rt", matrixRotation);
	ImGui::InputFloat3("Sc", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matWorld);

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }

    static bool useSnap(false);
    if (ImGui::IsKeyPressed(83))
        useSnap = !useSnap;
    ImGui::Checkbox("##something", &useSnap);
    ImGui::SameLine();
	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}

	_matrix matId;
	D3DXMatrixIdentity(&matId);

	ImGuiIO& io = ImGui::GetIO();
	RECT rt;
	GetClientRect(g_hWnd, &rt);
	POINT lt{rt.left, rt.top};
	ClientToScreen(g_hWnd, &lt);
	ImGuizmo::SetRect(lt.x, lt.y, io.DisplaySize.x, io.DisplaySize.y);

	// ImGuizmo::DrawGrid(m_pCam->GetView(), m_pCam->GetPrj(), matId, 100.f);

	ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld, NULL, useSnap ? &snap[0] : NULL);

	pTransform->m_matWorld = matWorld;

	ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
	matrixRotation[0] = D3DXToRadian(matrixRotation[0]);
	matrixRotation[1] = D3DXToRadian(matrixRotation[1]);
	matrixRotation[2] = D3DXToRadian(matrixRotation[2]);
	memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
	memcpy(&pTransform->m_vAngle, matrixRotation,sizeof(matrixRotation));
	memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));

	ImGui::End();
}

void CImGuiMgr::LoggerWindow()
{
	ImGui::Begin("Logger");

	if (ImGui::Button("Clear")) { log.clear(); }
	ImGui::SameLine();

	ImGui::BeginChild("Log");
	ImGui::SetWindowFontScale(1.1f);
	ImGui::TextUnformatted(log.begin(), log.end());

	ImGui::EndChild();
	ImGui::End();
}

void CImGuiMgr::Free()
{
}
