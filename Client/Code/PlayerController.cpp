#include "stdafx.h"
#include "PlayerController.h"
#include "Player.h"
#include "Box.h"
#include "Inventory.h"
#include "EquipItem.h"

CPlayerController::CPlayerController() : CController()
{
}

CPlayerController::CPlayerController(const CPlayerController& rhs) : CController(rhs)
{
}


CPlayerController::~CPlayerController()
{
}

_int CPlayerController::Update_Component(const _float& fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pOwner);
	NULL_CHECK_RETURN(pPlayer, 0);

	// 움직임 입력
	{
		if (DIKeyDown(DIK_W))
		{
			m_vPressDir.z += 1.f;
		}
		if (DIKeyUp(DIK_S))
		{
			m_vPressDir.z += 1.f;
		}

		if (DIKeyDown(DIK_A))
		{
			m_vPressDir.x += -1.f;
		}
		if (DIKeyUp(DIK_D))
		{
			m_vPressDir.x += -1.f;
		}

		if (DIKeyDown(DIK_S))
		{
			m_vPressDir.z += -1.f;
		}
		if (DIKeyUp(DIK_W))
		{
			m_vPressDir.z += -1.f;
		}

		if (DIKeyDown(DIK_D))
		{
			m_vPressDir.x += 1.f;
		}
		if (DIKeyUp(DIK_A))
		{
			m_vPressDir.x += 1.f;
		}
	}
	
	//box open
	if (DIKeyDown(DIK_F))
	{
		//박스 상호작용 (임시)
		Get_GameObject<CBox>(LAYER_GAMEOBJ, L"Box")->BoxOpen();

		//아이템 먹기
		_vec3 vTargetPos;
		_float fShortestDist = 2;
		CEquipItem* pEquipItem = nullptr;

		for (auto& ele : Get_Layer(LAYER_ITEM)->Get_MapObject())
		{
			if (CEquipItem* pItem = dynamic_cast<CEquipItem*>(ele.second))
			{
				vTargetPos = pPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[INFO_POS];
				_vec3 vDiff = vTargetPos - pItem->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
				_float fDist = D3DXVec3Length(&vDiff);

				if (fDist < fShortestDist)
				{
					pEquipItem = pItem;
					fShortestDist = fDist;
				}

			}
		}

		if (pEquipItem == nullptr)
			return 0;

		pPlayer->GetInventory()->Put(pEquipItem);


		

	}

	if (false == CGameUtilMgr::Vec3Cmp(m_vPressDir, m_vPrevPressDir)) // 이동 입력 없으면 방향 계산 안하기
	{
		D3DXVec3Normalize(&m_vMoveDir, &m_vPressDir);
		_float fCamYaw = Engine::Get_Component<Engine::CTransform>(LAYER_ENV, L"StaticCamera", L"Proto_TransformCom", ID_DYNAMIC)->
		                 m_vAngle.y;
		_matrix matYaw;
		D3DXMatrixRotationY(&matYaw, fCamYaw);
		D3DXVec3TransformNormal(&m_vMoveDir, &m_vMoveDir, &matYaw);
		pPlayer->SetMoveDir(m_vMoveDir.x, m_vMoveDir.z);
	}

	m_vPrevPressDir = m_vPressDir;


	if (DIKeyDown(DIK_SPACE))
	{
		pPlayer->RollPress();
	}

	if (MouseKeyDown(DIM_LB))
		pPlayer->MeleeAttackPress(true);
	if (MouseKeyUp(DIM_LB))
		pPlayer->MeleeAttackPress(false);

	if (MouseKeyDown(DIM_RB))
		pPlayer->RangeAttackPress(true);
	if (MouseKeyUp(DIM_RB))
		pPlayer->RangeAttackPress(false);

	if (DIKeyDown(DIK_1))
	{
		pPlayer->Legacy1Press();
	}
	if (DIKeyDown(DIK_2))
	{
		pPlayer->Legacy2Press();
	}
	
	if (DIKeyDown(DIK_4))
	{
		pPlayer->Legacy4Press();
	}

	return 0;
}

CComponent* CPlayerController::Clone()
{
	return new CPlayerController(*this);
}

CPlayerController* CPlayerController::Create()
{
	return new CPlayerController;
}
