#include "stdafx.h"
#include "..\Header\CubeAnimMgr.h"

IMPLEMENT_SINGLETON(CCubeAnimMgr)

CCubeAnimMgr::CCubeAnimMgr()
{
	//Load();
}

CCubeAnimMgr::~CCubeAnimMgr()
{
	Free();
}

void CCubeAnimMgr::AddAnim(const string& strTag, CubeAnimFrame* pAnim)
{
	m_mapAnim.insert({strTag, pAnim});
}

CubeAnimFrame* CCubeAnimMgr::GetAnim(const string& strTag)
{
	return m_mapAnim.find(strTag)->second;
}

void CCubeAnimMgr::Load(const wstring& wstrPath)
{
}

void CCubeAnimMgr::Free()
{
	for_each(m_mapAnim.begin(), m_mapAnim.end(), [](const pair<string, CubeAnimFrame*>& Pair)
	{
		delete Pair.second;
	});
}
