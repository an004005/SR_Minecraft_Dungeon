#pragma once
#include "Component.h"

enum DamageType
{
	DT_STUN,
	DT_KNOCK_BACK,
	// posion .. etc
	DT_END
};

class CTerrainCubeMap;

class CStatComponent : public CComponent
{
protected:
	explicit CStatComponent();
	explicit CStatComponent(const CStatComponent& rhs);
	virtual ~CStatComponent() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual CComponent* Clone() override;
	virtual void Free() override;
	static CStatComponent* Create();

	void SetTransform(Engine::CTransform* pOwnerTrans) { NULL_CHECK(pOwnerTrans); m_pOwnerTrans = pOwnerTrans; m_pOwnerTrans->AddRef(); };
	void SetMaxHP(_uint iMaxHP) { m_iMaxHP = iMaxHP; m_iHP = static_cast<_int>(m_iMaxHP); }

	void ModifyHP(_int iModifyingHP);
	void TakeDamage(_int iDamage, _vec3 vFromPos, CGameObject* pCauser, DamageType eType = DT_END);

	_bool IsStun() const { return m_bStun; }
	_bool IsDead() const { return m_bDead; }
	_bool IsDamaged() const { return m_bDamaged; }

private:
	Engine::CTransform* m_pOwnerTrans = nullptr;
	CTerrainCubeMap* m_pCubeMap = nullptr;

	_uint m_iMaxHP = 50;
	_int m_iHP = 50;
	_bool m_bDead = false;

	_bool m_bStun = false;
	_float m_fStunTime = 1.f;
	_float m_fCurStunTime = 1.f;
	_vec3 m_vKnockBackVelocity = CGameUtilMgr::s_vZero;

	_bool m_bDamaged = false;
	_float m_fDamagedTime = 0.5f;
	_float m_fCurDamagedTime = 1.f;


};

