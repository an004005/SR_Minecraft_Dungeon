#include "stdafx.h"
#include "..\Header\ObjectStoreMgr.h"

IMPLEMENT_SINGLETON(CObjectStoreMgr)

CObjectStoreMgr::CObjectStoreMgr()
{
}

CObjectStoreMgr::~CObjectStoreMgr()
{
}

void CObjectStoreMgr::Free()
{
	for (auto& scene : m_vecScene)
		Safe_Release(scene);
	m_vecScene.clear();
}


