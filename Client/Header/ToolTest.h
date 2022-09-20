#pragma once
#include "Scene.h"
class CToolTest : public CScene
{
private:
	explicit CToolTest(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolTest() override;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;

	virtual void Free() override;
	static CToolTest* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CTransform* m_pSelectedTransform = nullptr;
	class CDynamicCamera* m_pCam = nullptr;
};

