#pragma once
#include "VIBuffer.h"
class CArrowCube : public CVIBuffer
{
private:
	explicit CArrowCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CArrowCube(const CArrowCube& rhs);
	virtual ~CArrowCube() override;

public:
	virtual HRESULT Ready_Buffer() override;
	virtual void Render_Buffer() override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
};

