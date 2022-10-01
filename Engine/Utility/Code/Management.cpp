#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pScene(nullptr)
{
}


CManagement::~CManagement()
{
	Free();
}

CComponent* Engine::CManagement::Get_Component(LAYERID eLayerID, const wstring& pObjTag, const wstring& pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(eLayerID, pObjTag, pComponentTag, eID);
}

CGameObject* CManagement::Get_GameObject(LAYERID eLayerID, const wstring& pObjTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(eLayerID, pObjTag);
}

CLayer* CManagement::Get_Layer(LAYERID eLayerID)
{
	NULL_CHECK_RETURN(m_pScene, nullptr);
	return m_pScene->Get_Layer(eLayerID);
}

void CManagement::AddGameObject(LAYERID eLayerID, const wstring& pObjTag, CGameObject* pObject)
{
	if (pObject == nullptr)
		return;

	NULL_CHECK(m_pScene);
	m_pScene->AddGameObject(eLayerID, pObjTag, pObject);
}

HRESULT CManagement::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);
	Engine::Clear_RenderGroup(); // 기존 scene에 그려지고 있던 모든 렌더 요소들을 삭제

	m_pScene = pScene;
	
	return S_OK;
}

_int CManagement::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene(void)
{
	if (nullptr == m_pScene)
		return;

	m_pScene->LateUpdate_Scene();
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Render_GameObject(pGraphicDev);


	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

