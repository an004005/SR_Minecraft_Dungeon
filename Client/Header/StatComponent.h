#pragma once
#include "Component.h"

enum DamageType
{
	DT_STUN,
	DT_KNOCK_BACK,
	DT_STIFFEN,
	// posion .. etc
	DT_END
};

class CTerrainCubeMap;
class CStun;
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

	_int GetHP() const { return m_iHP; }
	_uint GetMaxHP() const { return m_iMaxHP;}

	void ModifyHP(_int iModifyingHP);
	void TakeDamage(_int iDamage, _vec3 vFromPos, CGameObject* pCauser, DamageType eType = DT_END);

	_bool IsStun() const { return m_bStun || m_bKnockback; }
	_bool IsDead() const { return m_bDead; }
	_bool IsDamaged() const { return m_bDamaged; }

	void Revive();

	// cur hp, max hp, damage
	BaseDelegater<_int, _uint, _int> m_DamageDelegater;
	void SetHurtSound(const vector<wstring>& vHurtSound)	{ m_vHurtSound = vHurtSound;}

private:
	Engine::CTransform* m_pOwnerTrans = nullptr;
	CTerrainCubeMap* m_pCubeMap = nullptr;

	vector<wstring> m_vHurtSound;

	_uint m_iMaxHP = 50;
	_int m_iHP = 50;
	_bool m_bDead = false;

	// stun
	_bool m_bStun = false;
	_float m_fStunTime = 2.f;
	_float m_fCurStunTime = 2.f;
	CStun* m_pStun = nullptr;

	// knock back
	_bool m_bKnockback = false;
	_float m_fKnockbackTime = 0.3f;
	_float m_fCurKnockbackTime = 0.3f;
	_float m_fPreYPos = 0.f;

	_bool m_bStiffen = false;
	_float m_fStiffeTime = 10.2f;
	_float m_fCurStiffeTime = 0.2f;

	_vec3 m_vKnockBackVelocity = CGameUtilMgr::s_vZero;

	// damaged
	_bool m_bDamaged = false;
	_float m_fDamagedTime = 0.5;
	_float m_fCurDamagedTime = 0.5;


};

