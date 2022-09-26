#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CCubeTex;
class CTransform;
class CCalculator;

END

class CMapCube :
	public CGameObject
{
	friend class CMapToolTest;
	friend class CImGuiMgr;

private:
	CMapCube(LPDIRECT3DDEVICE9 pGraphicDev, MapTool& tMapTool);
	CMapCube(LPDIRECT3DDEVICE9 pGraphicDev, MapTool maptool, _vec3 PickPos);
	virtual ~CMapCube();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


private:
	HRESULT				Add_Component(void);
	_vec3				PickUp_OnTerrain(void);
	void				Initalize_vFaceVtx(void);
	void				Render_State(void);

private:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	_vec3				m_vDirection;

	_vec3				m_vPickPos;

	
public:
	MapTool				m_tMapTool;
	wstring				m_wstrName;
	//_int				m_iDebugShowTxtIdx;
	_vec3 vFaceVtx[FACE_END][4]; // 어떤 면인지, 어느 삼각형인지, 어떤 점인지
	_vec3 vCenter{ 0.f, 0.f, 0.f };

public:
	static CMapCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev, MapTool maptool, _vec3 PickPos);
	static CMapCube*		Create(LPDIRECT3DDEVICE9 pGrahpicDev, _matrix & CubeWorld, MapTool& tMapTool);
	virtual void	Free(void);
};


