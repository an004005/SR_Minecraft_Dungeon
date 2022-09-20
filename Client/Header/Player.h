#pragma once

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcCol;
class CTransform;

END
class CPlayer :	public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer() override;

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;


private:
	HRESULT				Add_Component(void);

private:
	CTransform*			m_pTransCom = nullptr;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	virtual void Free(void);

private:
	_float m_fSpeed;
	_float m_fRotSpeed;


};

