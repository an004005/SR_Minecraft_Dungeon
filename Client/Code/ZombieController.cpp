#include "stdafx.h"
#include "ZombieController.h"
#include "Zombie.h"
#include "Player.h"

CZombieController::CZombieController()
{
}

CZombieController::CZombieController(const CZombieController& rhs)
{
}

CZombieController::~CZombieController()
{
}

_int CZombieController::Update_Component(const _float& fTimeDelta)
{
	// cooltime
	if (m_fCurAttackCoolTime < m_fAttackCoolTime)
		m_fCurAttackCoolTime += fTimeDelta;

	if (m_iTick++ < 20) return 0;
	m_iTick = 0;

	CZombie* pZombie = dynamic_cast<CZombie*>(m_pOwner);
	NULL_CHECK_RETURN(pZombie, 0);

	_vec3 vPos = pZombie->Get_Component<Engine::CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[INFO_POS];
	CPlayer* pTargetPlayer = nullptr;
	_vec3 vTargetPos;
	_float fTargetDist = 9999.f;

	for (auto& ele : Get_Layer(LAYER_PLAYER)->Get_MapObject())
	{
		if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(ele.second))
		{
			vTargetPos = pPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[INFO_POS];
			_vec3 vDiff = vTargetPos - vPos;
			_float fDist = D3DXVec3Length(&vDiff);

			if (fDist < m_fDetectRange && fTargetDist > fDist) // 플레이어 감지
			{
				pTargetPlayer = pPlayer;
				fTargetDist = fDist;
			}
		}
	}

	if (pTargetPlayer == nullptr) return 0;
	
	if (m_fCurAttackCoolTime >= m_fAttackCoolTime && fTargetDist <= m_fAttackDist)
	{
		m_fCurAttackCoolTime = 0.f;
		pZombie->AttackPress(vTargetPos);
		return 0;
	}
	
	if(fTargetDist > m_fAttackDist) // 공격 사거리 밖이라면 더 가까이 감
		pZombie->WalkToTarget(vTargetPos);
	

	
	



	return 0;
}

CComponent* CZombieController::Clone()
{
	return new CZombieController(*this);
}

void CZombieController::Free()
{
	CController::Free();
}

CZombieController* CZombieController::Create()
{
	return new CZombieController;
}
