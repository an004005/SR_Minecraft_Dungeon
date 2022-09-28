#pragma once
#include "GameObject.h"

class CMapCube;

BEGIN(Engine)

class CTexture;
class CTerrainCubeTex;

END

struct MapCubeInfo
{
	_matrix matWorld;
	_int iTexIdx = 0;
	CUBETYPE eType = TYPE_END;
	MapCubeInfo() { D3DXMatrixIdentity(&matWorld); }
	MapCubeInfo(_matrix matWorld, _int iTexIdx, CUBETYPE eType)
		: matWorld(matWorld), iTexIdx(iTexIdx), eType(eType) {}
};

class CTerrainCubeMap : public CGameObject
{
public:
	explicit CTerrainCubeMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrainCubeMap();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


private:
	HRESULT				Add_Component(void);
	
public:
	void			LoadMap();

	void			Divide_CubeType(CMapCube* pMapCube);
	const _vec3*	Get_Pos() { return m_pPos; }
	void			Set_CubeCoordinate(void);

private:
	CLayer*			m_pLayer = nullptr;		
	MapTool			m_tMapTool;

	vector<MapCubeInfo> m_vecTotalCube;
	vector<MapCubeInfo> m_vecLand;
	vector<MapCubeInfo> m_vecCollision;
	vector<MapCubeInfo> m_vecDeco;


	vector<pair<wstring, CTerrainCubeTex*>> m_vecTerrainCom;
	CTexture* m_pTextureCom = nullptr;

	_vec3*			m_pPos = nullptr;
public:
	_float			m_fHeight[VTXCNTX][VTXCNTZ];

public:
	static CTerrainCubeMap*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);
};

