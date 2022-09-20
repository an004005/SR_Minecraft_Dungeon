#pragma once
#include "Base.h"

#define LOGOUT(...) CImGuiMgr::LogOut(__VA_ARGS__)

class CImGuiMgr : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

private:
	explicit CImGuiMgr();
	~CImGuiMgr();

public:
	static void TransformEdit(CCamera* pCamera, CTransform* pTransform);
	static void LoggerWindow();
	static void LogOut(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		log.appendfv(fmt, args);
		log.append("\n");
		va_end(args);
	}
	virtual void Free() override;

private:
	static ImGuiTextBuffer log;
};

