#pragma once
#include "GameObject.h"

class CMapCube;

class CTerrainCubeMap
{
public:
	explicit CTerrainCubeMap();
	virtual ~CTerrainCubeMap();

private:
	void			Create_Cube(_matrix& CubeWorld, 
								MapTool& tMapTool, 
								LPDIRECT3DDEVICE9 pGrahpicDev,
								map<const _tchar*, 
								CLayer*>& m_mapLayer, 
								CLayer* pLayer);

public:
	void			LoadMap(LPDIRECT3DDEVICE9 pGrahpicDev,
							map<const _tchar*, 
							CLayer*>& m_mapLayer,
							CLayer* pLayer);

	void			Divide_CubeType(CMapCube* pMapCube);

	const _vec3*	Get_Pos() { return m_pPos; }

	void			Set_CubeCoordinate(void);

private:
	CLayer*			m_pLayer = nullptr;		
	MapTool			m_tMapTool;

	vector<CMapCube*> m_vecTotalCube;
	vector<CMapCube*> m_vecLand;
	vector<CMapCube*> m_vecCollision;
	vector<CMapCube*> m_vecDeco;

	_vec3*			m_pPos = nullptr;
public:
	_float			m_fHeight[VTXCNTX][VTXCNTZ];

public:
	static CTerrainCubeMap*		Create();

private:
	virtual void	Free(void);
};

