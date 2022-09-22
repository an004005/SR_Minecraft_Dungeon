#include "stdafx.h"
#include "..\Header\ImGuiMgr.h"
#include "ImGuizmo.h"
#include "SkeletalCube.h"

ImGuiTextBuffer CImGuiMgr::s_log;

void CImGuiMgr::TransformEditor(CCamera* pCamera, CTransform* pTransform)
{
#ifndef _DEBUG
	return;
#endif

	NULL_CHECK(pCamera);
	if (pTransform == nullptr)
	{
		IM_LOG("Warning : TransformEditor's pTransform is nullptr");
		return;
	}

	ImGuizmo::BeginFrame();
	static float snap[3] = {1.f, 1.f, 1.f};
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

	ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld,
	                     NULL, useSnap ? &snap[0] : NULL);

	pTransform->m_matWorld = matWorld;

	ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
	matrixRotation[0] = D3DXToRadian(matrixRotation[0]);
	matrixRotation[1] = D3DXToRadian(matrixRotation[1]);
	matrixRotation[2] = D3DXToRadian(matrixRotation[2]);
	memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
	memcpy(&pTransform->m_vAngle, matrixRotation, sizeof(matrixRotation));
	memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));
}

void CImGuiMgr::LocalTransformEditor(CCamera* pCamera, _matrix& matLocal)
{
#ifndef _DEBUG
	return;
#endif

	NULL_CHECK(pCamera);

	ImGuizmo::BeginFrame();
	static float snap[3] = {1.f, 1.f, 1.f};
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
	ImGuizmo::DecomposeMatrixToComponents(matLocal, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation);
	ImGui::InputFloat3("Rt", matrixRotation);
	ImGui::InputFloat3("Sc", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matLocal);

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

	// ImGuizmo::DrawGrid(pCamera->GetView(), pCamera->GetProj(), matId, 100.f);

	ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matLocal,
	                     NULL, useSnap ? &snap[0] : NULL);

	ImGuizmo::DecomposeMatrixToComponents(matLocal, matrixTranslation, matrixRotation, matrixScale);
}

void CImGuiMgr::LoggerWindow()
{
#ifndef _DEBUG
	return;
#endif

	ImGui::Begin("Logger");

	if (ImGui::Button("Clear")) { s_log.clear(); }
	ImGui::SameLine();

	ImGui::BeginChild("Log");
	ImGui::SetWindowFontScale(1.1f);
	ImGui::TextUnformatted(s_log.begin(), s_log.end());

	ImGui::EndChild();
	ImGui::End();
}

void CImGuiMgr::Logging(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	s_log.appendfv(fmt, args);
	s_log.append("\n");
	va_end(args);
}

void CImGuiMgr::SkeletalEditor(CCamera* pCamera, CSkeletalCube* pSkeletal)
{
#ifndef _DEBUG
	return;
#endif

	NULL_CHECK(pSkeletal);
	static SkeletalPart* pSelectedPart = nullptr;
	static string strSelected = CSkeletalCube::s_strRoot;

	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_DefaultOpen;

	ImGui::Text("Skeletal Selector");
	SkeletalRecursive(pSkeletal->m_pRootPart, strSelected, baseFlags);

	const auto itr = pSkeletal->m_mapParts.find(strSelected);
	if (itr != pSkeletal->m_mapParts.end())
		pSelectedPart = (*itr).second;

	ImGui::NewLine();
	ImGui::Text("Part Transform");
	enum TransMode
	{
		TransMode_World,
		TransMode_Local
	};
	static int mode = TransMode_World;
	if (ImGui::RadioButton("World", mode == TransMode_World)) { mode = TransMode_World; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Local", mode == TransMode_Local)) { mode = TransMode_Local; }

	switch (mode)
	{
	case TransMode_World:
		if (pSelectedPart == nullptr) break;
		ImGui::Text("World Transform");
		TransformEditor(pCamera, pSelectedPart->pTrans);
		break;
	case TransMode_Local:
		if (pSelectedPart == nullptr) break;
		ImGui::Text("Local Transform");
		LocalTransformEditor(pCamera, pSelectedPart->matLocal);
		break;
	default:
		IM_LOG("Warning : selected TransMode is not valid");
	}
}

void CImGuiMgr::MapControl()
{
#ifndef _DEBUG
	return;
#endif


}

void CImGuiMgr::SkeletalRecursive(SkeletalPart* Part, string& strSelected, ImGuiTreeNodeFlags baseFlags)
{
#ifndef _DEBUG
	return;
#endif

	NULL_CHECK(Part);

	ImGuiTreeNodeFlags nodeFlags = baseFlags;
	if (strSelected == Part->strName)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx(Part->strName.c_str(), nodeFlags, Part->strName.c_str()))
	{
		if (ImGui::IsItemClicked())
			strSelected = Part->strName;

		for (const auto& child : Part->vecChild)
			SkeletalRecursive(child, strSelected, baseFlags);

		ImGui::TreePop();
	}
}
