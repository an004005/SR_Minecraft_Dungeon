#pragma once
#include "VIBuffer.h"
class CCubeNorTex : public CVIBuffer
{
private:
	explicit CCubeNorTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeNorTex(const CCubeNorTex& rhs);
	virtual ~CCubeNorTex() override;

public:
	virtual HRESULT Ready_Buffer() override;
	virtual CComponent* Clone() override;
	static CCubeNorTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

};

