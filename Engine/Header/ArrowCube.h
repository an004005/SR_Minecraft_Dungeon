#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CArrowCube : public CVIBuffer
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
	static CArrowCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END