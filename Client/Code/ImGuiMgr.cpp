#include "stdafx.h"
#include "..\Header\ImGuiMgr.h"
#include "ImGuizmo.h"
#include "SkeletalCube.h"
#include "ImGuiFileDialog.h"
#include "ImSequencerImpl.h"
#include "MapTool.h"

ImGuiTextBuffer CImGuiMgr::s_log;
SkeletalPart* CImGuiMgr::s_SelectedPart = nullptr;

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
	// if (ImGui::IsKeyPressed(83))
		// useSnap = !useSnap;
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
	ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, io.DisplaySize.x, io.DisplaySize.y);

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
	// if (ImGui::IsKeyPressed(83))
		// useSnap = !useSnap;
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
	ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, io.DisplaySize.x, io.DisplaySize.y);

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
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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
	NULL_CHECK(pCamera);

	if (ImGui::Button("Load Skeletal"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileToLoad", "Choose File", ".cube",
		                                        "../Bin/Resource/SkeletalCube/");
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseFileToLoad"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			wstring tmp;
			tmp.assign(filePathName.begin(), filePathName.end());
			pSkeletal->LoadSkeletal(tmp);
		}
		ImGuiFileDialog::Instance()->Close();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Skeletal"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileToSave", "Choose File", ".cube",
		                                        "../Bin/Resource/SkeletalCube/");
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseFileToSave"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			wstring tmp;
			tmp.assign(filePathName.begin(), filePathName.end());
			pSkeletal->SaveSkeletal(tmp);
		}
		ImGuiFileDialog::Instance()->Close();
	}

	//////////////////////////////////////////////////
	static SkeletalPart* pSelectedPart = nullptr;
	static string strSelected = CSkeletalCube::s_strRoot;

	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_DefaultOpen;

	ImGui::Text("Skeletal Selector");
	SkeletalRecursive(pSkeletal->m_pRootPart, strSelected, baseFlags);

	const auto itr = pSkeletal->m_mapParts.find(strSelected);
	if (itr != pSkeletal->m_mapParts.end())
		pSelectedPart = (*itr).second;

	string tmpTex, tmpBuf;
	tmpTex.assign(pSelectedPart->strTexProto.begin(), pSelectedPart->strTexProto.end());
	tmpBuf.assign(pSelectedPart->strBufProto.begin(), pSelectedPart->strBufProto.end());
	
	ImGui::Text("Texture : %s %d", tmpTex.c_str(), pSelectedPart->iTexIdx);
	ImGui::Text("Buffer : %s", tmpBuf.c_str());

	//////////////////////////////////////////////////////
	ImGui::Separator();
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

	//////////////////////////////////////////////////////
	ImGui::Separator();
	static wstring strSelectTex;
	static _uint iSelectTexNum;
	static wstring strSelectBuf;
	static char szPartName[16] = "";
	CImGuiMgr::TextureSelector(strSelectTex, iSelectTexNum);
	ImGui::Separator();
	CImGuiMgr::VIBufferSelector(strSelectBuf);
	if (ImGui::Button("Add Part"))
	{
		if (pSelectedPart != nullptr && !strSelectTex.empty() && !strSelectBuf.empty() && strcmp(szPartName, "") != 0)
		{
			pSkeletal->AddSkeletalPart(string(szPartName), strSelected, strSelectBuf, strSelectTex, iSelectTexNum);
		}
		else
		{
			IM_LOG("Warning : Select Part, texture, Part Name and buffer to add");
		}
	}
	ImGui::SameLine();
	ImGui::InputText("Part Name", &szPartName[0], sizeof(szPartName));
	if (ImGui::Button("Change Texture Index"))
	{
		if (pSelectedPart != nullptr && !strSelectTex.empty())
		{
			pSelectedPart->iTexIdx = iSelectTexNum;
		}
		else
		{
			IM_LOG("Warning : Select Part and Texture to Change");
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete Select Part"))
	{
		if (pSelectedPart != nullptr)
		{
			pSkeletal->DeleteSkeletalPart(strSelected);
			strSelected = CSkeletalCube::s_strRoot;
		}
		else
		{
			IM_LOG("Warning : Select Part to Delete");
		}
	}

	s_SelectedPart = pSelectedPart; // for AnimationEditor
}

void CImGuiMgr::TextureSelector(wstring& strTex, _uint& iTexIdx)
{
#ifndef _DEBUG
	return;
#endif

	ImGui::Text("Texture Selector");
	static const vector<wstring> vecTexName{
		L"Proto_CubeTexture",
		L"Proto_MinecraftCubeTexture",
		L"Proto_WeaponTexture",
		L"Proto_BossCubeTile"
	};
	static vector<_int> vecTexIdx(vecTexName.size(), 0);
	static size_t iCurIdx = 0;

	if (ImGui::BeginListBox("##1"))
	{
		for (size_t i = 0; i < vecTexName.size(); ++i)
		{
			CTexture* pTexture = dynamic_cast<CTexture*>(Find_Proto(vecTexName[i]));
			NULL_CHECK(pTexture);

			size_t iTexSize = pTexture->GetTexSize() - 1;
			string _TexName;
			_TexName.assign(vecTexName[i].begin(), vecTexName[i].end());

			const bool bSelected = (iCurIdx == i);
			if (ImGui::Selectable(_TexName.c_str(), bSelected))
			{
				iCurIdx = i;
				strTex = vecTexName[i];
				iTexIdx = vecTexIdx[i];
			}

			if (bSelected)
			{
				ImGui::SliderInt("TexIdx", &vecTexIdx[i], 0, static_cast<int>(iTexSize));
				ImGui::InputInt("TexIdxInput", &vecTexIdx[i]);
				iTexIdx = vecTexIdx[i];
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
}

void CImGuiMgr::VIBufferSelector(wstring& strBuf)
{
	ImGui::Text("VIBuffer Selector");
	static const vector<wstring> vecBufName{
		L"Proto_CubeTexCom",
		L"Proto_VoxelTex_Sword",
	};
	static size_t iCurIdx = 0;

	if (ImGui::BeginListBox("##2"))
	{
		for (size_t i = 0; i < vecBufName.size(); ++i)
		{
			CVIBuffer* pBuf = dynamic_cast<CVIBuffer*>(Find_Proto(vecBufName[i]));
			NULL_CHECK(pBuf);

			string _BufName;
			_BufName.assign(vecBufName[i].begin(), vecBufName[i].end());

			const bool bSelected = (iCurIdx == i);
			if (ImGui::Selectable(_BufName.c_str(), bSelected))
			{
				iCurIdx = i;
				strBuf = vecBufName[i];
			}
		}
		ImGui::EndListBox();
	}
}

void CImGuiMgr::AnimationEditor(CSkeletalCube* pSkeletal)
{
#ifndef _DEBUG
	return;
#endif

	ImGui::Text("60 frame = 1 sec / only use world transform");
	static CImSequencerImpl mySequence;
	static bool bOnce = true;
	if (bOnce)
	{
		pSkeletal->m_pCurAnim = &mySequence.m_CubeAnim;
		mySequence.m_CubeAnim.bLoop = true;
		mySequence.m_iFrameMin = 0;
		mySequence.m_iFrameMax = 100;
		bOnce = false;
	}

	static int selectedEntry = -1;
	static int firstFrame = 0;
	static bool expanded = true;
	static int currentFrame = 0;
	static char szPartName[128];
	static char szEventName[128];
	static int iFrameAmount = 0;

	if (mySequence.m_iFrameMin < 0)
		mySequence.m_iFrameMin = 0;

	if (pSkeletal->m_bStopAnim == false)
	{
		currentFrame = static_cast<int>(pSkeletal->m_fAccTime * 60.f); // 60 frame == 1 sec
	}
	else
	{
		pSkeletal->m_fAccTime = (_float)currentFrame / 60.f;
	}

	mySequence.m_CubeAnim.fTotalTime = (_float)mySequence.m_iFrameMax / 60.f;

	ImGui::PushItemWidth(130);
	ImGui::InputInt("Frame Min", &mySequence.m_iFrameMin);
	ImGui::SameLine();
	ImGui::InputInt("Frame ", &currentFrame);
	ImGui::SameLine();
	ImGui::InputInt("Frame Max", &mySequence.m_iFrameMax);
	ImGui::InputText("PartName", szPartName, 128);
	ImGui::SameLine();
	if (ImGui::Button("Add Part"))
	{
		mySequence.AddPart(szPartName);
	}
	if (ImGui::Button("Play"))
	{
		pSkeletal->m_bStopAnim = !pSkeletal->m_bStopAnim;
	}
	if (ImGui::Button("Add Selected Part Trans"))
	{
		mySequence.AddTransFrame(currentFrame, s_SelectedPart);
	}
	ImGui::SameLine();
	if (ImGui::Button("Add All Part Trans"))
	{
		mySequence.AddTransFrameRecur(currentFrame, pSkeletal->m_pRootPart);
	}
	ImGui::InputText("Event Name", szEventName, 128);
	ImGui::SameLine();
	if (ImGui::Button("Add Event"))
	{
		mySequence.AddEvent(currentFrame, szEventName);
	}
	ImGui::InputInt("Move Frame Amount", &iFrameAmount);
	ImGui::SameLine();
	if (ImGui::Button("Move Frame"))
	{
		mySequence.MoveFrame(currentFrame, iFrameAmount);
	}

	ImGui::PopItemWidth();
	Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame,
	          ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_CHANGE_FRAME);


	if (ImGui::Button("Load Anim"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileToLoadAnim", "Choose File", ".anim",
		                                        "../Bin/Resource/CubeAnim/");
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseFileToLoadAnim"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			wstring tmp;
			tmp.assign(filePathName.begin(), filePathName.end());
			mySequence.LoadAnimFrame(tmp);
			pSkeletal->m_pCurAnim = &mySequence.m_CubeAnim;
		}
		ImGuiFileDialog::Instance()->Close();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Anim"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileToSaveAnim", "Choose File", ".anim",
		                                        "../Bin/Resource/CubeAnim/");
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseFileToSaveAnim"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			wstring tmp;
			tmp.assign(filePathName.begin(), filePathName.end());
			mySequence.m_CubeAnim.Save(tmp);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void CImGuiMgr::MapControl(Engine::MapTool& tMaptool , CMapTool& CMapTool, size_t CubeCount)
{
#ifndef _DEBUG
	return;
#endif
	ImGui::Separator();

	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Set Block"))
		{

			ImGui::Text("Set Block Texture");
			ImGui::InputInt("input Index", &tMaptool.iTexIdx);
			ImGui::NewLine();

			ImGui::Text("Set Height");
			ImGui::SliderFloat("Y", &tMaptool.fHeight, 1.f, 2.0f, "ratio = %.3f");
			ImGui::InputFloat("Insert Y", &tMaptool.fHeight);
			ImGui::NewLine();

			ImGui::Text("Total Block Count :");
			ImGui::SameLine();
			ImGui::Text("%d", CubeCount);
			ImGui::NewLine();

			ImGui::Text("Select Options");
			ImGui::RadioButton("Terrain", &tMaptool.iPickingOption, PICK_TERRAIN); ImGui::SameLine();
			ImGui::RadioButton("Cube", &tMaptool.iPickingOption, PICK_CUBE); ImGui::SameLine();
			ImGui::RadioButton("Delete", &tMaptool.iPickingOption, PICK_DELETE); ImGui::SameLine();

			ImGui::NewLine();
			ImGui::NewLine();

			ImGui::Text("Cube Type");
			ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "     (Please Check Type!!)");
			ImGui::RadioButton("Land", &tMaptool.iCubeType, TYPE_LAND); ImGui::SameLine();
			ImGui::RadioButton("Collision", &tMaptool.iCubeType, TYPE_COLLISION); ImGui::SameLine();
			ImGui::RadioButton("Decoration", &tMaptool.iCubeType, TYPE_DECO); ImGui::SameLine();

			ImGui::NewLine();
			ImGui::NewLine();

			static int clicked = 0;
			if (ImGui::Button("Debug Mode"))
				clicked++;

			if (clicked & 1)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Checked!!");
				tMaptool.bRendState = true;
			}
			else
				tMaptool.bRendState = false;


			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Set Rect"))
		{
			ImGui::Text("holl");

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Set Element"))
		{
			ImGui::Text("checkyt");

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Save / Load"))
		{
			
			if (ImGui::Button("Load Map"))
			{
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileToLoadMap", "Choose File", ".map",
					"../Bin/Resource/Map/");
			}
			if (ImGuiFileDialog::Instance()->Display("ChooseFileToLoadMap"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

					wstring tmp;
					tmp.assign(filePathName.begin(), filePathName.end());
					CMapTool.LoadMap(tmp);
				}
				ImGuiFileDialog::Instance()->Close();
			}
			ImGui::SameLine();

			if (ImGui::Button("Save Map"))
			{
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileToSaveMap", "Choose File", ".map",
					"../Bin/Resource/Map/");
			}
			if (ImGuiFileDialog::Instance()->Display("ChooseFileToSaveMap"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

					wstring tmp;
					tmp.assign(filePathName.begin(), filePathName.end());
					CMapTool.SaveMap(tmp);
				}
				ImGuiFileDialog::Instance()->Close();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	
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
