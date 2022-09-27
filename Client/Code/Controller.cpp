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
	if (MouseKeyDown(DIM_LB))
	{
		// move or attack melee
		dynamic_cast<CPlayer*>(m_pOwner)->CheckCursor();
	}
	else if (MouseKeyDown(DIM_RB))
	{
		// attack arrow
	}

	if (DIKeyDown(DIK_SPACE))
	{
		// dotge
	}
}

CPlayerController* CPlayerController::Create()
{
	return new CPlayerController;
}
