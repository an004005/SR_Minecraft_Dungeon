
HRESULT	Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagement = pManagement;

	return S_OK;
}

HRESULT		Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}

_int		Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}
void		LateUpdate_Scene(void)
{
	CManagement::GetInstance()->LateUpdate_Scene();
}
void		Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}
inline CLayer* Get_Layer(LAYERID eLayerID)
{
	return CManagement::GetInstance()->Get_Layer(eLayerID);
}

void AddGameObject(LAYERID eLayerID, const wstring& pObjTag, CGameObject* pObject)
{
	CManagement::GetInstance()->AddGameObject(eLayerID, pObjTag, pObject);
}

inline HRESULT			Ready_Proto(const wstring& pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}

inline CComponent*		Clone_Proto(const wstring& pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

inline CComponent* Find_Proto(const wstring& pProtoTag)
{
	return CProtoMgr::GetInstance()->Find_Proto(pProtoTag);
}

void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup(void)
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}


void Add_CollisionCom(CCollisionCom* pCollision)
{
	CCollider::GetInstance()->Add_CollisionCom(pCollision);
}
void Add_StaticCollision(const _vec3& vCenter, _float fRadius)
{
	CCollider::GetInstance()->Add_StaticCollision(vCenter, fRadius);
}
void GetOverlappedObject(OUT list<CGameObject*>& objList, const _vec3& vPos, _float fRadius)
{
	CCollider::GetInstance()->GetOverlappedObject(objList, vPos, fRadius);
}
void Clear_ColliderAll()
{
	CCollider::GetInstance()->Clear_ColliderAll();
}


inline void			Release_Utility(void)
{
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CCollider::GetInstance()->DestroyInstance();
	CMainCamera::GetInstance()->DestroyInstance();
}
