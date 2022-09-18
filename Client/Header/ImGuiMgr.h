#pragma once
#include "Base.h"
#include <functional>

class CImGuiMgr : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

private:
	explicit CImGuiMgr();
	~CImGuiMgr();

public:
	virtual void Free() override;

private:
	
};

