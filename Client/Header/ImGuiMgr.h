#pragma once

#define IM_LOG(...) CImGuiMgr::Logging(__VA_ARGS__)

class CSkeletalCube;

class CImGuiMgr
{
public:
	static void TransformEdit(CCamera* pCamera, CTransform* pTransform);
	static void LoggerWindow();
	static void Logging(const char* fmt, ...);
	static void SkeletalSelector(CSkeletalCube* pSkeletal, CTransform* pSelected);

private:
	static ImGuiTextBuffer s_log;
};

