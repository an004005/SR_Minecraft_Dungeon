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
public:
	CMapCube(LPDIRECT3DDEVICE9 pGraphicDev, _float Height);
	virtual ~CMapCube();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT				Add_Component(void);
	_vec3				PickUp_OnTerrain(void);

private:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	_vec3				m_vDirection;

	_float				m_fFloor = 1.f;
	_float				m_fHeight = 1.f;

public:
	static CMapCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float Height);
	virtual void	Free(void);
};

