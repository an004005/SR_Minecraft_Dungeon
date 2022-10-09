#include "stdafx.h"
#include "..\Header\Logo.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene(void)
{
	// if (FAILED(Engine::CScene::Ready_Scene()))
	// 	return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);

	// 로딩 클래스 생성
		
	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iResult = Engine::CScene::Update_Scene(fTimeDelta);

	return iResult;
}

void CLogo::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CLogo::Render_Scene(void)
{
	// 개발자 모드 출력 함수
}

HRESULT CLogo::Ready_Layer_Environment()
{
	CGameObject*		pGameObject = nullptr;

	// backGround

	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	// TestPlayer
	// pGameObject = CPlayer::Create(m_pGraphicDev);
	// NULL_CHECK_RETURN(pGameObject, E_FAIL);
	// FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);

	// // TestMonster
	// pGameObject = CTestMonster::Create(m_pGraphicDev);
	// NULL_CHECK_RETURN(pGameObject, E_FAIL);
	// FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestMonster", pGameObject), E_FAIL);


	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free(void)
{
	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}

HRESULT CLogo::Ready_Proto(void)
{

	return S_OK;

}
