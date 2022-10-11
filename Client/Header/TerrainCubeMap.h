#pragma once
#include "GameObject.h"

class CMapCube;

BEGIN(Engine)

class CTexture;
class CTerrainCubeTex;
class CTerrainRcTex;

END

struct MapCubeInfo
{
	_matrix matWorld;
	_int iTexIdx = 0;
	CUBETYPE eType = TYPE_END;
	_float fHeight = 0.f;
	MapCubeInfo() { D3DXMatrixIdentity(&matWorld); }
	MapCubeInfo(_matrix matWorld, _int iTexIdx, CUBETYPE eType, _float Height)
		: matWorld(matWorld), iTexIdx(iTexIdx), eType(eType), fHeight(Height) {}

};

class CTerrainCubeMap : public CGameObject
{
	friend class CImGuiMgr;
public:
	explicit CTerrainCubeMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrainCubeMap();

public:
	_float	GetHeight(_float fX, _float fZ) { return m_fHeight[(_int)fX][(_int)fZ]; }

public:
	virtual HRESULT Ready_Object(const wstring& wstrPath = L"");
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


public:
	void				 LoadMap(const wstring& wstrPath);
	void				 SaveMap(const wstring& wstrPath);
	void				Set_CubeCoordinate(void);
	const vector<MapCubeInfo>& GetTotalCubes() { return m_vecTotalCube; }
	void				 AddCube(const MapCubeInfo& tInfo);
	void			     DeleteCube(int iToDel);
	void				 AddTex(const MapCubeInfo& tInfo);
	void			     DeleteTex(_vec3 PickPos);
	

private:

	vector<MapCubeInfo> m_vecTotalCube;
	vector<MapCubeInfo> m_vecLand;
	vector<MapCubeInfo> m_vecCollision;

	vector<MapCubeInfo> m_vecTotalTex;


	map<_uint, CTerrainCubeTex*> m_mapTerrainCubeCom;
	map<_uint, CTerrainRcTex*> m_mapTerrainRcCom;
	CTexture* m_pCubeTextureCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

public:
	_float			m_fHeight[VTXCNTX][VTXCNTZ];
	_bool			m_bRendState = false;

public:
	static CTerrainCubeMap*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath = L"");

private:
	virtual void	Free(void);
};




