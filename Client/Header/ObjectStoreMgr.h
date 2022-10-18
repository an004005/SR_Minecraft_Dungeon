#pragma once
#include "Base.h"
class CObjectStoreMgr : public CBase
{
	DECLARE_SINGLETON(CObjectStoreMgr)

private:
	explicit CObjectStoreMgr();
	virtual ~CObjectStoreMgr() override;

public:
	void SetPlayerName(const string& strPlayerName) { m_strPlayerName = strPlayerName; }

	virtual void Free() override;

private:
	string m_strPlayerName;
	
};

