#pragma once

#include "Base.h"
#include "Scene.h"
#include "Engine_Include.h"
#include <atomic>

BEGIN(Engine)

class ENGINE_DLL CManagement :	public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CComponent* Get_Component(LAYERID eLayerID, const wstring& pObjTag, const wstring& pComponentTag, COMPONENTID eID);
	CGameObject* Get_GameObject(LAYERID eLayerID, const wstring& pObjTag);
	void Get_AllGameObject(LAYERID eLayerID, const wstring& pObjTag, list<CGameObject*>& outList);
	CLayer* Get_Layer(LAYERID eLayerID);
	void AddGameObject(LAYERID eLayerID, const wstring& pObjTag, CGameObject* pObject);
public:
	void SwitchSceneLoading(CScene* pLoading, std::function<CScene*()>& pSceneCreate, long long delay = 700/*0.7��*/);
	void SwitchSceneLoadingDeletePrev(CScene* pLoading, std::function<CScene*()>& pSceneCreate, long long delay = 700/*0.7��*/);

	// ���� ���� �����ϰ� �Է¹��� ������ �̵��ϴ� �Լ�
	HRESULT		Set_Scene(CScene* pScene);
	// ������� �����ϰ� ���� ������ ���ư��� �Լ�
	HRESULT     Go_PrevScene();
	// ������ ���� ���� ��� �����ϴ� �Լ�
	void        Clear_PrevScene();
	_int		Update_Scene(const _float& fTimeDelta);
	void		LateUpdate_Scene(void);
	void		Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	CScene*		m_pScene = nullptr;
	CScene* m_pLoadingScene = nullptr;
	vector<CScene*> m_vecScene;
	std::atomic<bool> m_bLoading{false};

public:
	virtual void Free(void);
};

END
