#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Include.h"
#include "RcTex.h"
#include "Texture.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "VoxelTex.h"
#include "TerrainCubeTex.h"

#include "Transform.h"
#include "Calculator.h"
#include "Camera.h"


#include "ProtoMgr.h"
#include "Management.h"
#include "Renderer.h"
#include "GameUtilMgr.h"


BEGIN(Engine)
	// Management


	inline HRESULT Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement);
	inline HRESULT Set_Scene(CScene* pScene);
	inline _int Update_Scene(const _float& fTimeDelta);
	inline void LateUpdate_Scene(void);
	inline void Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

	template <typename T>
	T* Get_Component(LAYERID eLayerID,
	                                 const wstring& pObjTag,
	                                 const wstring& pComponentTag,
	                                 COMPONENTID eID)
	{
		CComponent* pComponent = CManagement::GetInstance()->Get_Component(eLayerID, pObjTag, pComponentTag, eID);
		T* pCasted = dynamic_cast<T*>(pComponent);
		NULL_CHECK_RETURN(pCasted, nullptr);

		return pCasted;
	}

	template <typename T>
	T* Get_GameObject(LAYERID eLayerID, const wstring& pObjTag)
	{
		CGameObject* pGameObject = CManagement::GetInstance()->Get_GameObject(eLayerID, pObjTag);
		T* pCasted = dynamic_cast<T*>(pGameObject);
		NULL_CHECK_RETURN(pCasted, nullptr);

		return pCasted;
	}

	inline CLayer* Get_Layer(LAYERID eLayerID);
	inline void AddGameObject(LAYERID eLayerID, const wstring& pObjTag, CGameObject* pObject);


	// ProtoMgr
	inline HRESULT Ready_Proto(const wstring& pProtoTag, CComponent* pComponent);
	inline CComponent* Clone_Proto(const wstring& pProtoTag);
	inline CComponent* Find_Proto(const wstring& pProtoTag);


	// Renderer

	inline void Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	inline void Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	inline void Clear_RenderGroup(void);


	inline void Release_Utility(void);


#include "Export_Utility.inl"

END
#endif
