#pragma once
#include "VIBuffer.h"

struct BoxVTXInstance
{
	_vec3		vPos;
	_vec2		vTexUV;
	_float      vInstNum;
	// _vec2      vInstNum;
};

#define INST_CNT 100

class CArrowCube : public CVIBuffer
{
private:
	explicit CArrowCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CArrowCube(const CArrowCube& rhs);
	virtual ~CArrowCube() override;

public:
	virtual HRESULT Ready_Buffer(_float fSize = 0.1f);
	virtual void Render_Buffer() override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
	static CArrowCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	IDirect3DBaseTexture9* m_pTexture = nullptr;
	LPD3DXEFFECT m_pEffect = nullptr;
	LPDIRECT3DVERTEXDECLARATION9 m_pVtxDeclare = nullptr;

	_matrix m_pMatWorlds[INST_CNT];

};
