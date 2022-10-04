#pragma once
#include "GameObject.h"
class CArrow : public CGameObject
{
private:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CArrow(const CArrow& rhs);
	virtual ~CArrow() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object() override;
	virtual void Free() override;
	static CArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CTexture* m_pTexture = nullptr;
	CTransform* m_pTransform = nullptr;
	CArrowCube* m_pBuffer = nullptr;

};

