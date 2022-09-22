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
	CMapCube(LPDIRECT3DDEVICE9 pGraphicDev);
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
public:
	static CMapCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

