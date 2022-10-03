#include "..\..\Header\GameObject.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}


HRESULT CGameObject::Ready_Object(void)
{
	return S_OK;
}

_int CGameObject::Update_Object(const _float & fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

void CGameObject::LateUpdate_Object(void)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();
}

void CGameObject::Render_Object(void)
{
}

void CGameObject::Delete_Component(const wstring& pComponentTag, COMPONENTID eID)
{
	auto	iter = m_mapComponent[eID].find(pComponentTag);

	if (iter == m_mapComponent[eID].end())
		return;

	_ulong dwCnt = 0;

	dwCnt = iter->second->Release();

	if (dwCnt == 0)
		iter->second = nullptr;

	m_mapComponent[eID].erase(iter);
}

CComponent * CGameObject::Find_Component(const wstring& pComponentTag, COMPONENTID eID)
{
	auto	iter = m_mapComponent[eID].find(pComponentTag);

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}
