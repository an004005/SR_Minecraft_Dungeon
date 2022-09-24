#pragma once

#ifdef _DEBUG
#define IM_LOG(...) CImGuiMgr::Logging(__VA_ARGS__)
#define IM_BEGIN(win_name) ImGui::Begin(win_name)
#define IM_END ImGui::End()
#else
#define IM_LOG(...) 
#define IM_BEGIN(win_name) 
#define IM_END 
#endif


class CSkeletalCube;
class CMapToolTest;
struct SkeletalPart;

class CImGuiMgr
{
public:
	static void TransformEditor(CCamera* pCamera, CTransform* pTransform); // for CTransform
	static void LocalTransformEditor(CCamera* pCamera, _matrix& matLocal); // for SkeletalPart
	static void LoggerWindow();
	static void Logging(const char* fmt, ...);
	static void SkeletalEditor(CCamera* pCamera, CSkeletalCube* pSkeletal); // for CSkeletalCube
	static void TextureSelector(wstring& strTex, _uint& iTexIdx);
	static void VIBufferSelector(wstring& strBuf);
	static void AnimationEditor(CSkeletalCube* pSkeletal);
	static void MapControl(Engine::MapTool& sMaptool, CMapToolTest& MapToolTest, size_t CubeCount);


private:
	static void SkeletalRecursive(SkeletalPart* Part, string& strSelected, ImGuiTreeNodeFlags baseFlags);

private:
	static ImGuiTextBuffer s_log;
	static SkeletalPart* s_SelectedPart; // for AnimationEditor


};
