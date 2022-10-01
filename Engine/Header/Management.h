#pragma once

#include "Base.h"
#include "Scene.h"
#include "Engine_Include.h"

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
	CLayer* Get_Layer(LAYERID eLayerID);
	void AddGameObject(LAYERID eLayerID, const wstring& pObjTag, CGameObject* pObject);
public:
	HRESULT		Set_Scene(CScene* pScene);
	_int		Update_Scene(const _float& fTimeDelta);
	void		LateUpdate_Scene(void);
	void		Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	CScene*		m_pScene;

public:
	virtual void Free(void);
};

END
