#pragma once

#include "Engine_Include.h"
#include "Scene.h"
#include "MapCube.h"

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
	_vec3			PickingOnCube(HWND hWnd);

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Proto(void);


public:
	static CMapToolTest*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	DWORD			m_dwTime = GetTickCount();
	CLayer*			m_pLayer = nullptr;
	_float			m_fHeight = 1.f;
	_float			m_fFloor = 1.f;
	_int			m_iIndex = 0;
	_int			m_iCubeCount = 0;

	vector<CMapCube*> m_vecCube;
public:
	_vec3 PickPos;
private:
	virtual void	Free(void);
};

