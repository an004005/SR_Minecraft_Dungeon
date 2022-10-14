#include "stdafx.h"
#include "../Header/StatComponent.h"
#include "AbstFactory.h"
#include "Particle.h"
#include "TerrainCubeMap.h"
#include "DamageFontMgr.h"

CStatComponent::CStatComponent()
{
}

CStatComponent::CStatComponent(const CStatComponent& rhs): CComponent(rhs)
{
}

CStatComponent::~CStatComponent()
{
}

_int CStatComponent::Update_Component(const _float& fTimeDelta)
{
	if (m_bDead) return 0;

	if (m_pCubeMap == nullptr)
	{
		m_pCubeMap = Get_GameObject<CTerrainCubeMap>(LAYER_ENV, L"TerrainCubeMap");
		m_pCubeMap->AddRef();
	}

	if (m_bStun)
	{
		if (m_fStunTime < m_fCurStunTime)
			m_bStun = false;
		else
			m_fCurStunTime += fTimeDelta;
	}

	if (m_bKnockback)
	{
		if (m_fKnockbackTime < m_fCurKnockbackTime)
			m_bKnockback = false;
		else
			m_fCurKnockbackTime += fTimeDelta;
	}

	if (m_bDamaged)
	{
		if (m_fDamagedTime < m_fCurDamagedTime)
			m_bDamaged = false;
		else
			m_fCurDamagedTime += fTimeDelta;
	}




	_vec3& vPos = m_pOwnerTrans->m_vInfo[INFO_POS];
	if (CGameUtilMgr::Vec3Cmp(m_vKnockBackVelocity, CGameUtilMgr::s_vZero))
	{
		vPos.y = m_pCubeMap->GetHeight(vPos.x, vPos.z);
	}
	else
	{
		// ³Ë¹é »óÅÂ
		if (m_fPreYPos + 4.f < vPos.y)
		{
			m_bKnockback = false;
		}

		vPos += m_vKnockBackVelocity * fTimeDelta;

		m_vKnockBackVelocity.y -= 80.f * fTimeDelta;

		if (vPos.y < m_pCubeMap->GetHeight(vPos.x, vPos.z) || m_bKnockback == false)
		{
			m_vKnockBackVelocity = CGameUtilMgr::s_vZero;
		}
		
	}

	return 0;
}

CComponent* CStatComponent::Clone()
{
	return new CStatComponent(*this);
}

void CStatComponent::Free()
{
	CComponent::Free();
	Safe_Release(m_pOwnerTrans);
	Safe_Release(m_pCubeMap);
}

CStatComponent* CStatComponent::Create()
{
	return new CStatComponent();
}

void CStatComponent::ModifyHP(_int iModifyingHP)
{
	m_iHP += iModifyingHP;

	if (iModifyingHP < 0)
	{
		m_bDamaged = true;
		m_fCurDamagedTime = 0.f;

		// ÇÇÅ¸°Ý ÀÌÆåÆ®
		Get_GameObject<CAttack_P>(LAYER_EFFECT, L"Attack_Basic")
			->Add_Particle(m_pOwnerTrans->m_vInfo[INFO_POS] +_vec3{0.f, 1.2f, 0.f}, CGameUtilMgr::GetRandomFloat(0.15f,0.3f), RED, 20, 0.2f);

		if(m_vHurtSound.size() > 0)
			CSoundMgr::GetInstance()->PlaySoundRandom(m_vHurtSound, m_pOwnerTrans->m_vInfo[INFO_POS], 0.2f);
	}

	if (m_iHP <= 0)
		m_bDead = true;

	if (m_iHP > (_int)m_iMaxHP)
		m_iHP = (_int)m_iMaxHP;

	m_DamageDelegater.broadcast(m_iHP, m_iMaxHP, iModifyingHP);
}

void CStatComponent::TakeDamage(_int iDamage, _vec3 vFromPos, CGameObject* pCauser, DamageType eType)
{
	if (m_bDead) return;

	switch (eType)
	{
	case DT_STUN:
		m_bStun = true;
		m_fCurStunTime = 0.f;
		break;
	case DT_KNOCK_BACK:
		m_bKnockback = true;
		m_fCurKnockbackTime = 0.f;
		m_fPreYPos = m_pOwnerTrans->m_vInfo[INFO_POS].y;

		m_vKnockBackVelocity = m_pOwnerTrans->m_vInfo[INFO_POS] - vFromPos;
		D3DXVec3Normalize(&m_vKnockBackVelocity, &m_vKnockBackVelocity);
		m_vKnockBackVelocity *= 10.f;
		m_vKnockBackVelocity.y = 10.f;
		break;
	case DT_END:
		break;
	default:
		IM_LOG("Error : Invalid DamageType!");
		break;
	}


	ModifyHP(-iDamage);
	if (iDamage != 0)
	{
		// if (dynamic_cast<CPlayer*>())
		CDamageFontMgr::GetInstance()->Add_DamageFontFromWorld(
			iDamage,
			m_pOwnerTrans->m_vInfo[INFO_POS] + _vec3{0.f, 1.5f, 0.f},
			vFromPos,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CStatComponent::Revive()
{
	m_iHP = m_iMaxHP;
	m_bDead = false;
	m_bStun = false;
	m_bDamaged = false;
	m_bKnockback = false;
}
