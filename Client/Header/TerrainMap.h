#pragma once
#include "GameObject.h"

class CMapCube;

class CTerrainMap
{
public:
	explicit CTerrainMap();
	virtual ~CTerrainMap();

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

private:
	CLayer*			m_pLayer = nullptr;		
	MapTool			m_tMapTool;

	vector<CMapCube*> m_vecTotalCube;
	vector<CMapCube*> m_vecLand;
	vector<CMapCube*> m_vecCollision;
	vector<CMapCube*> m_vecDeco;
public:
	static CTerrainMap*		Create();

private:
	virtual void	Free(void);
};

