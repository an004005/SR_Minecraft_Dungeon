#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainCubeTex : public CVIBuffer
{
protected:
	explicit CTerrainCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainCubeTex(const CTerrainCubeTex& rhs);
	virtual ~CTerrainCubeTex();

public:
	virtual HRESULT		Ready_Buffer(const wstring& wstrPath, _int iTex);
	virtual CComponent*	Clone(void) override;

	static CTerrainCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath, _int iTex);

	int m_iTexIdx;
};

END