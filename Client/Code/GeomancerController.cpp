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

_int CGeomancerController::Update_Component(const _float& fTimeDelta)
{
	if (m_iTick++ < 20) return;

	CGeomancer* pGeomancer = dynamic_cast<CGeomancer*>(m_pOwner);
	NULL_CHECK(pGeomancer);

	_vec3 vPos = pGeomancer->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[INFO_POS];
	CPlayer* pTargetPlayer = nullptr;
	_float fTargetDist = 9999.f;

	for (auto& ele : Get_Layer(LAYER_PLAYER)->Get_MapObject())
	{
		if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(ele.second))
		{
			_vec3 vPlayerPos = pPlayer->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[
				INFO_POS];
			_vec3 vDiff = vPos - vPlayerPos;
			_float fDist = D3DXVec3Length(&vDiff);

			if (fDist < 15.f && fTargetDist > fDist) // 화면 밖에서 공격할 수 있는 범위인듯
			{
				pTargetPlayer = pPlayer;
				fTargetDist = fDist;
			}
		}
	}

	// if (pTargetPlayer == nullptr)
	// {
	// 	pGeomancer->SetIdle();
	// 	return;
	// }
	//
	// if (pGeomancer->CanAttack())
	// {
	// 	pGeomancer->SetAttack(pTargetPlayer);
	// 	return;
	// }
	//
	// if (fTargetDist < 10.f && fTargetDist > 6.f)
	// {
	// 	pGeomancer->SetIdle();
	// 	return;
	// }
	//
	// if (fTargetDist < 4.f)
	// {
	// 	pGeomancer->Run(pTargetPlayer);
	// 	return;
	// }

	m_iTick = 0;

	return 0;
}

CComponent* CGeomancerController::Clone()
{
	return new CGeomancerController(*this);
}

CGeomancerController* CGeomancerController::Create()
{
	return new CGeomancerController;
}
