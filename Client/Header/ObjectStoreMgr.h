#pragma once
#include "Base.h"
class CObjectStoreMgr : public CBase
{
	DECLARE_SINGLETON(CObjectStoreMgr)

private:
	explicit CObjectStoreMgr();
	virtual ~CObjectStoreMgr() override;

public:


	virtual void Free() override;

private:
	vector<CScene*> m_vecScene;
};

