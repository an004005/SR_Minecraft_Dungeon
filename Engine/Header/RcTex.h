#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CVIBuffer& rhs);
	virtual ~CRcTex() override;

public:
	virtual HRESULT Ready_Buffer() override;
	virtual void Render_Buffer() override;

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;
	virtual void Free() override;
};
END