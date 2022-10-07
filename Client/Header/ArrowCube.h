#pragma once
#include "VIBuffer.h"

struct BoxVTXInstance
{
	_vec3		vPos;
	_vec2		vTexUV;
};

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

	LPDIRECT3DVERTEXBUFFER9		m_pVBMatrix; // 정점 위치 보관용

	vector<_matrix> m_vecArrowWorld;
	size_t m_iPreSize = 0;

};
