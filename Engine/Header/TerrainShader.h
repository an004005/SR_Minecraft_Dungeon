#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

typedef struct tagTerrainVtx
{
	_vec3		vPos;
	_vec2		vUV;
} VTXUVTERRAIN;



class ENGINE_DLL CTerrainShader :public CVIBuffer
{
private:
	explicit CTerrainShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainShader(const CTerrainShader& rhs);
	virtual ~CTerrainShader()override;

public:
	virtual HRESULT Ready_Buffer(const wstring& _shaderfile, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv,
								_vec2 _uv0, _vec2 _uv1, _vec2 _uv2, _vec2 _uv3);
	virtual void Render_Buffer() override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
	static CTerrainShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& _shaderfile, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv,
									_vec2 _uv0, _vec2 _uv1, _vec2 _uv2, _vec2 _uv3);

	void Set_Texture(IDirect3DBaseTexture9* _ptexture)
	{
		m_pTexture = _ptexture;
		m_pTexture->AddRef();
	}

	void Set_TextureOption(_uint _framecnt, _uint _texturecntw, _uint _texturecnth)
	{
		m_iFrameCnt = _framecnt;
		m_iTextureCnt_W = _texturecntw;
		m_iTextureCnt_H = _texturecnth;
	}
	_matrix					 m_matWorld;
	_matrix					 m_ViewMatrix;
	_matrix					 m_PrjMatrix;
	_vec4					 m_WorldCamPos;
	_vec4					 m_WorldLightPos{ 100.f, 0.f, 100.f, 1.f };

public:
	void LoadTexture(const wstring& filename);
private:
	LPD3DXEFFECT					m_pEffect = nullptr;
	LPDIRECT3DVERTEXDECLARATION9	m_pVtxDeclare = nullptr;
	IDirect3DBaseTexture9*			m_pTexture = nullptr;


public:

	_uint m_iPlayOnFrameCnt = 0;
	_uint m_iWidthTextureCnt = 0;
	_uint m_iHeightTextureCnt = 0;

	_uint m_iFrameCnt = 0;
	_uint m_iTextureCnt_W = 0;
	_uint m_iTextureCnt_H = 0;

	_float m_fUSize = 0.f;
	_float m_fVSize = 0.f;

	_vec2 UV_0;
	_vec2 UV_1;
	_vec2 UV_2;
	_vec2 UV_3;

	_float m_fTime = 0;
	_float m_fSpeed = 0;
	_float m_fWaveHeight = 0.f;
	_float m_fWaveFreq = 0.f;
	_float m_fUVSpeed = 0.f;

};
END
