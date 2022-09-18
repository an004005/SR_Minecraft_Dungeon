#pragma once
#include "GameObject.h"
#include "Player.h"

BEGIN(Engine)
	class CTriCol;
	class CTransform;
END

class CMonster : public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CGameObject& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct() override;

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;

private:
	CTriCol*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;

	_vec3				m_vDirection;
	_float				m_fTimeDelta = 0.f;

	_float m_fSpeed;
	_float m_fRotSpeed;
};

