#pragma once

#include "Engine_Include.h"
#include "GameObject.h"


BEGIN(Engine)

class CRcCol;
class CTransform;
class CCalculator;

END

class CTerrainCubeMap;

class CPlayer :	public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* VtxPos);
	virtual ~CPlayer() override;

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:
	void		Set_TerrainMap(CTerrainCubeMap* terrainmap) { m_pTerrainMap = terrainmap; }
private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);
	void				Set_OnTerrain(void);
	_vec3				PickUp_OnTerrain(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	_vec3				m_vDirection;
	CTerrainCubeMap*	m_pTerrainMap = nullptr;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* VtxPos);

private:

	virtual void Free(void);

private:
	_float m_fSpeed;
	_float m_fRotSpeed;


};

