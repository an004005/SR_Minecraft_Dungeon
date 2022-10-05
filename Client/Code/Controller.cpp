#include "stdafx.h"
#include "Controller.h"

#include "Player.h"

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

	// ¿òÁ÷ÀÓ
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
