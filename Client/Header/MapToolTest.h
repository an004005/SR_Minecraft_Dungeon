#pragma once

#include "Engine_Include.h"
#include "Scene.h"


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
private:
	virtual void	Free(void);
};

