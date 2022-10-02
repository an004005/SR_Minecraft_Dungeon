#pragma once
#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)
class CGameObject;
class CTransform;

enum CamMode
{
	CAM_NORMAL,
	CAM_END
};

class ENGINE_DLL CMainCamera : public CBase
{
	DECLARE_SINGLETON(CMainCamera)

private:
	explicit CMainCamera();
	~CMainCamera();

public:
	void UpdateCamera(const _float& fTimeDelta);
	virtual void Free() override;

	void SetGraphicDev(LPDIRECT3DDEVICE9 pGraphicDev) { m_pGraphicDev = pGraphicDev; m_pGraphicDev->AddRef(); }
	void SetTargetObj(CGameObject* pTargetObj);
	void SetMatProj(_float fNear = 0.1f, _float fFar = 1000.f, _float fFov = D3DXToRadian(60.f), _float fAspect = (float)WINCX / WINCY);

	CGameObject* GetTargetObj() const {return m_pTargetObj;}
	const _matrix& GetView() const { return m_matView; }
	const _matrix& GetProj() const { return m_matProj; }

private:
	void UpdateCameraNormal(const _float& fTimeDelta);


private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
	CGameObject* m_pTargetObj = nullptr;
	CTransform* m_pTargetTrans = nullptr;

	// normal mode
	_vec3 m_vCamAwayDir{-1.f, 1.f, -1.f};
	_float m_fDist = 5.f;
	_vec3 m_vPrePos{0.f, 0.f, 0.f};
	//~normal mode

	_vec3		m_vEye, m_vAt;
	_vec3 m_vUp{0.f, 1.f, 0.f};

	_matrix m_matView;
	_matrix m_matProj;
	_float m_fFov, m_fAspect, m_fNear, m_fFar;
};



END
