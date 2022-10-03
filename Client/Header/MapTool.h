#pragma once

#include "Engine_Include.h"
#include "Scene.h"
#include "MapCube.h"
#include "Player.h"

class CDynamicCamera;

class CMapTool : public Engine::CScene
{
private:
	explicit CMapTool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapTool();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

public:
	_bool			PickingOnCube(_vec3& CubeCenter);

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Proto(void);
	void			Create_Cube(_matrix& CubeWorld, MapTool& tMapTool);
	void			Cube_DebugShow(void);


public:
	void			SaveMap(wstring wstrFileName);
	void			LoadMap(wstring wstrFileName);
	//const _vec3*	Get_VtxPos(void) const {return m_pPos;}

private:
	void			Set_CubeCoordinate(void);
	void			CubeHeight(_float x, _float z);

public:
	static CMapTool*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	DWORD			m_dwTime = GetTickCount();
	CLayer*			m_pLayer = nullptr;
	MapTool			m_tMapTool;
	wstring			m_wDeleteName;

	vector<CMapCube*> m_vecTotalCube;

	vector<CMapCube*> m_vecLand;
	_float			m_fHeight[VTXCNTX][VTXCNTZ];

	CDynamicCamera* m_pDCamera = nullptr;
	_float			m_fFar = 50.f;

private:
	virtual void	Free(void);
};

