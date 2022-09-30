#include "..\..\Header\Scene.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}


CScene::~CScene()
{
}

CComponent * CScene::Get_Component(const wstring& pLayerTag, const wstring& pObjTag, const wstring& pComponentTag, COMPONENTID eID)
{
	// auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));
	auto iter = m_mapLayer.find(pLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

CGameObject* CScene::Get_GameObject(const wstring& pLayerTag, const wstring& pObjTag)
{
	// auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));
	auto iter = m_mapLayer.find(pLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}

void CScene::AddGameObject(const wstring& pLayerTag, const wstring& pObjTag, CGameObject* pObject)
{
	if (pObject == nullptr)
		return;

	// const auto itr = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));
	auto itr = m_mapLayer.find(pLayerTag);

	if (itr != m_mapLayer.end())
		return;

	itr->second->Add_GameObject(pObjTag, pObject);
}

HRESULT CScene::Ready_Scene(void)
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	_int iResult = 0;

	for (auto& iter : m_mapLayer)
	{
		iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}
	
	return iResult;
}

void CScene::LateUpdate_Scene(void)
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer();
}

void Engine::CScene::Free(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
