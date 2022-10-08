#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

enum PartOrder
{
	HEAD, // 0.f
	BODY, // 0.1f
	ARM_L, // 0.2f;
	ARM_R, // 0.3f
	LEG_L, // 0.4f
	LEG_R, // 0.5f
	PART_END 
};

struct OrderMatrix
{
	_matrix matWorld;
	_float fOrder;
	OrderMatrix(){}
	OrderMatrix(const _matrix& matWorld, PartOrder eOrder)
		: matWorld(matWorld)
	{
		switch (eOrder)
		{
		case HEAD:
			fOrder = 0.f;
			break;
		case BODY:
			fOrder = 0.1f;
			break;
		case ARM_L:
			fOrder = 0.2f;
			break;
		case ARM_R:
			fOrder = 0.3f;
			break;
		case LEG_L:
			fOrder = 0.4f;
			break;
		case LEG_R:
			fOrder = 0.5f;
			break;
		case PART_END:
			fOrder = 0.f;
			break;
		default: ;
		}
	}
};

class ENGINE_DLL CCubeTex6 : public CVIBuffer
{
private:
	explicit CCubeTex6(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex6(const CCubeTex6& rhs);
	virtual ~CCubeTex6() override;

public:
	virtual CComponent* Clone() override;
	virtual HRESULT Ready_Buffer() override;
	virtual void Render_Buffer() override;
	virtual void Free() override;
	static CCubeTex6* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void SetPartTexture(IDirect3DBaseTexture9* pTexture, PartOrder eOrder);
	void SetPartWorldMatrix(const _matrix& matPartWorld, PartOrder eOrder);

private:
	array<IDirect3DBaseTexture9*, PART_END> m_arrTexture{};
	LPD3DXEFFECT m_pEffect = nullptr;
	LPDIRECT3DVERTEXDECLARATION9 m_pVtxDeclare = nullptr;

	LPDIRECT3DVERTEXBUFFER9		m_pVBMatrix = nullptr; // 정점 위치 보관용

	array<OrderMatrix, PART_END> m_arrPartWorld{};
};

END