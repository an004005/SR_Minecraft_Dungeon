#pragma once

#include "Engine_Include.h"
#include "Scene.h"
#include "MapCube.h"
#include "Player.h"

class CMapToolTest : public Engine::CScene
{
private:
	explicit CMapToolTest(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapToolTest();

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
	void			Cube_Type(_int eType, CGameObject* pGameObject);
	void			Cube_DebugShow(void);


public:
	void			SaveMap();
	void			LoadMap();
	const _vec3*	Get_VtxPos(void) const {return m_pPos;}

public:
	static CMapToolTest*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	DWORD			m_dwTime = GetTickCount();
	CLayer*			m_pLayer = nullptr;
	MapTool			m_tMapTool;
	wstring			m_wDeleteName;

	vector<CMapCube*> m_vecCube;

	_vec3*			 m_pPos;
	//vector<CMapCube*> m_vecLand;
	//vector<CMapCube*> m_vecCollision;
	//vector<CMapCube*> m_vecDeco;

private:
	virtual void	Free(void);
};

