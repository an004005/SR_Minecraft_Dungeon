#include "stdafx.h"
#include "..\Header\KoukuController.h"

#include "Kouku.h"


CKoukuController::CKoukuController()
{
}

CKoukuController::CKoukuController(const CKoukuController& rhs)
{
}

CKoukuController::~CKoukuController()
{
}

_int CKoukuController::Update_Component(const _float& fTimeDelta)
{
	{
		m_fCurDoubleHammerCoolTime += fTimeDelta;
		m_fCurHorrorAttackCoolTime += fTimeDelta;
	}
	CKouku* pKouku = dynamic_cast<CKouku*>(m_pOwner);
	NULL_CHECK_RETURN(pKouku, 0);

	_vec3 vPos = pKouku->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
	_vec3 vTargetPos;
	_float fTargetDist = 9999.f;

	if (m_fCurTargetingCoolTime > m_fTargetingCoolTime)
	{
		for (auto& ele : Get_Layer(LAYER_PLAYER)->Get_MapObject())
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(ele.second))
			{
				vTargetPos = pPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
				_vec3 vDiff = vTargetPos - vPos;
				_float fDist = D3DXVec3Length(&vDiff);

				if (fTargetDist > fDist) // 플레이어 감지
				{
					m_pTargetPlayer = pPlayer;
					fTargetDist = fDist;
				}
			}
		}
		m_fCurTargetingCoolTime = 0.f;

		if (fTargetDist > 20.f)
			m_pTargetPlayer = nullptr;
	}
	else
	{
		m_fCurTargetingCoolTime += fTimeDelta;
		if (m_pTargetPlayer == nullptr) return 0;

		vTargetPos = m_pTargetPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
		_vec3 vDiff = vTargetPos - vPos;
		_float fDist = D3DXVec3Length(&vDiff);
	}

	if (m_fCurDoubleHammerCoolTime >= m_fDoubleHammerCoolTime && fTargetDist <= m_fDoubleHammerDist)
	{
		m_fCurDoubleHammerCoolTime = 0.f;
		pKouku->DoubleHammer(vTargetPos);
		return 0;
	}

	if (m_fCurHorrorAttackCoolTime >= m_fHorrorAttackCoolTime && fTargetDist <= m_fHorrorAttackDist)
	{
		m_fCurHorrorAttackCoolTime = 0.f;
		pKouku->HorrorAttack(vTargetPos);
		return 0;
	}

	pKouku->WalkToTarget(vTargetPos);

	return 0;
}

CComponent* CKoukuController::Clone()
{
	return new CKoukuController(*this);
}

void CKoukuController::Free()
{
	CController::Free();
}

CKoukuController* CKoukuController::Create()
{
	return new CKoukuController;
}
