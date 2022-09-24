#pragma once
#include "Base.h"
#include "SkeletalCube.h"


class CCubeAnimMgr : public CBase
{
	DECLARE_SINGLETON(CCubeAnimMgr)
private:
	CCubeAnimMgr();
	virtual ~CCubeAnimMgr() override;

public:
	void AddAnim(const string& strTag, CubeAnimFrame* pAnim);
	CubeAnimFrame* GetAnim(const string& strTag);
	void Load(const wstring& wstrPath);
	virtual void Free() override;

private:
	map<string, CubeAnimFrame*> m_mapAnim;
};

