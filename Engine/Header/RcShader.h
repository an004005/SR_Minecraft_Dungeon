#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

typedef struct tagParticleVtx
{
	_vec3		vPos;
	_vec2		vUV;
	// _vec3		vNormal;
} VTXPARTICLE;


class ENGINE_DLL CRcShader :public CVIBuffer
{
private:
	explicit CRcShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcShader(const CRcShader& rhs);
	virtual ~CRcShader() override;

public:
	virtual HRESULT Ready_Buffer() override;
	virtual void Render_Buffer() override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
	static CRcShader* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	_matrix					 m_matWorld;
	_matrix					 m_ViewMatrix;
	_matrix					 m_PrjMatrix;
	_vec4					 m_WorldCamPos;
	_vec4					 m_WorldLightPos{ 100.f, 0.f, 100.f, 1.f };
	//IDirect3DBaseTexture9*	 m_pTexture;

public:
	void LoadTexture(const wstring& filename);
private:
	LPD3DXEFFECT					m_pEffect = nullptr;
	LPDIRECT3DVERTEXDECLARATION9	m_pVtxDeclare = nullptr;
	IDirect3DBaseTexture9*			m_pTexture = nullptr;


private:
	static _int m_i;
	static _int m_j;
	static _int m_k;
	 

	_float fTime =0.f;

};

END