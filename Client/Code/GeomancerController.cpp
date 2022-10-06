#include "stdafx.h"
#include "GeomancerController.h"
#include "Geomancer.h"
#include "Player.h"

CGeomancerController::CGeomancerController()
{
}

CGeomancerController::CGeomancerController(const CGeomancerController& rhs)
{
}

CGeomancerController::~CGeomancerController()
{
}

_int CGeomancerController::Update_Component(const _float& fTimeDelta)
{
	// cooltime
	if (m_fCurAttackCoolTime < m_fAttackCoolTime)
		m_fCurAttackCoolTime += fTimeDelta;

	if (m_iTick++ < 20) return 0;
	m_iTick = 0;

	CGeomancer* pGeomancer = dynamic_cast<CGeomancer*>(m_pOwner);
	NULL_CHECK_RETURN(pGeomancer, 0);

	_vec3 vPos = pGeomancer->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[INFO_POS];
	CPlayer* pTargetPlayer = nullptr;
	_vec3 vTargetPos;
	_float fTargetDist = 9999.f;

	for (auto& ele : Get_Layer(LAYER_PLAYER)->Get_MapObject())
	{
		if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(ele.second))
		{
			vTargetPos = pPlayer->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[INFO_POS];
			_vec3 vDiff = vPos - vTargetPos;
			_float fDist = D3DXVec3Length(&vDiff);

			if (fDist < m_fAttackRange && fTargetDist > fDist) // 화면 밖에서 공격할 수 있는 범위인듯
			{
				pTargetPlayer = pPlayer;
				fTargetDist = fDist;
			}
		}
	}

	if (pTargetPlayer == nullptr) return 0;


	// 공격 가능
	if (m_fCurAttackCoolTime >= m_fAttackCoolTime)
	{
		m_fCurAttackCoolTime = 0.f;
		pGeomancer->AttackPress(vTargetPos);
		return 0;
	}

	if (fTargetDist < m_fRunDist)
	{
		pGeomancer->Run(vTargetPos);
		return 0;
	}
	


	return 0;
}

CComponent* CGeomancerController::Clone()
{
	return new CGeomancerController(*this);
}

void CGeomancerController::Free()
{
	CController::Free();
}

CGeomancerController* CGeomancerController::Create()
{
	return new CGeomancerController;
}
