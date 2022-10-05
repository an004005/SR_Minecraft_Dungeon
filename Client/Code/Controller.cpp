#include "stdafx.h"
#include "Controller.h"

#include "Player.h"
#include "Geomancer.h"

/*----------------
 *	Controller
 ----------------*/
CController::CController()
{
}


CController::~CController()
{
}


void CController::Free()
{
}

CPlayerController::CPlayerController()
	: CController()
{
}

/*----------------
 *	CPlayerController
 ----------------*/
CPlayerController::~CPlayerController()
{
}

void CPlayerController::Update(CSkeletalCube* m_pOwner)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pOwner);
	NULL_CHECK(pPlayer);

	// 움직임
	{
		if (DIKeyDown(DIK_W))
		{
			pPlayer->SetMove(0.f, 1.f);
		}
		if (DIKeyUp(DIK_S))
		{
			pPlayer->SetMove(0.f, 1.f);
		}

		if (DIKeyDown(DIK_A))
		{
			pPlayer->SetMove(-1.f, 0.f);
		}
		if (DIKeyUp(DIK_D))
		{
			pPlayer->SetMove(-1.f, 0.f);
		}

		if (DIKeyDown(DIK_S))
		{
			pPlayer->SetMove(0.f, -1.f);
		}
		if (DIKeyUp(DIK_W))
		{
			pPlayer->SetMove(0.f, -1.f);
		}

		if (DIKeyDown(DIK_D))
		{
			pPlayer->SetMove(1.f, 0.f);
		}
		if (DIKeyUp(DIK_A))
		{
			pPlayer->SetMove(1.f, 0.f);
		}
	}


	if (DIKeyDown(DIK_SPACE))
	{
		pPlayer->Roll();
	}

	if (MouseKeyDown(DIM_LB))
		pPlayer->MeleeAttackOn(true);
	if (MouseKeyUp(DIM_LB))
		pPlayer->MeleeAttackOn(false);
}

CPlayerController* CPlayerController::Create()
{
	return new CPlayerController;
}

/*----------------
 *	CGeomancerController
 ----------------*/
CGeomancerController::CGeomancerController()
{
}

CGeomancerController::~CGeomancerController()
{
}

void CGeomancerController::Update(CSkeletalCube* m_pOwner)
{
	if (m_iTick++ > 5) 
	{
		m_iTick = 0;
		return;
	}

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

	if (pTargetPlayer == nullptr)
	{
		pGeomancer->SetIdle();
		return;
	}

	if (pGeomancer->CanAttack())
	{
		pGeomancer->SetAttack(pTargetPlayer);
		return;
	}

	if (fTargetDist < 10.f && fTargetDist > 6.f)
	{
		pGeomancer->SetIdle();
		return;
	}

	if (fTargetDist < 4.f)
	{
		pGeomancer->Run(pTargetPlayer);
		return;
	}
}

CGeomancerController* CGeomancerController::Create()
{
	return new CGeomancerController;
}
