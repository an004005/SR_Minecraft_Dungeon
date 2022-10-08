#include "stdafx.h"
#include "..\Header\StatComponent.h"

#include "AbstFactory.h"
#include "Particle.h"
#include "TerrainCubeMap.h"


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

	if (m_bDamaged)
	{
		if (m_fDamagedTime < m_fCurDamagedTime)
			m_bStun = false;
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
		// ณหน้ ป๓ลย
		vPos += m_vKnockBackVelocity * fTimeDelta;
		m_vKnockBackVelocity.y -= 120.f * fTimeDelta;

		if (vPos.y < m_pCubeMap->GetHeight(vPos.x, vPos.z))
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
		// IM_LOG("Damaged : %d", iModifyingHP);
	}


	if (m_iHP <= 0)
		m_bDead = true;
}

void CStatComponent::TakeDamage(_int iDamage, _vec3 vFromPos, CGameObject* pCauser, DamageType eType)
{
	switch (eType)
	{
	case DT_STUN:
		m_bStun = true;
		m_fCurStunTime = 0.f;
		break;
	case DT_KNOCK_BACK:
		m_bStun = true;
		m_fCurStunTime = 0.f;

		m_vKnockBackVelocity = m_pOwnerTrans->m_vInfo[INFO_POS] - vFromPos;
		D3DXVec3Normalize(&m_vKnockBackVelocity, &m_vKnockBackVelocity);
		m_vKnockBackVelocity *= 15.f;
		m_vKnockBackVelocity.y = 10.f;
		break;
	case DT_END:
		break;
	default:
		IM_LOG("Error : Invalid DamageType!");
		break;
	}


	ModifyHP(-iDamage);
}

