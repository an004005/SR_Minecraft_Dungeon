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
	_float	GetHeight(_float fX, _float fZ) { return m_fHeight[(_int)fX][(_int)fZ]; }
	//x, z 값에 따라 충돌인지 아닌지 판별
	_bool	IsCollision(_float fx, _float fz) { return m_fCollisionPos[(_int)fx][(_int)fz]; }

public:
	virtual HRESULT Ready_Object(const wstring& wstrPath);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


private:
	HRESULT				Add_Component(void);
	
public:
	void			LoadMap(const wstring& wstrPath);


private:
	CLayer*			m_pLayer = nullptr;		
	MapTool			m_tMapTool;

	vector<MapCubeInfo> m_vecTotalCube;
	vector<MapCubeInfo> m_vecLand;
	vector<MapCubeInfo> m_vecCollision;
	vector<MapCubeInfo> m_vecDeco;


	vector<pair<wstring, CTerrainCubeTex*>> m_vecTerrainCom;
	CTexture* m_pTextureCom = nullptr;


public:
	_float			m_fHeight[VTXCNTX][VTXCNTZ];
	_bool			m_fCollisionPos[VTXCNTX][VTXCNTZ];

public:
	static CTerrainCubeMap*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);

private:
	virtual void	Free(void);
};

