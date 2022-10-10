#include "stdafx.h"
#include "PlayerController.h"
#include "Player.h"
#include "Box.h"
#include "Inventory.h"
#include "EquipItem.h"
#include "ConsumeItem.h"
#include "Dynamite.h"

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
		_vec3 vTargetPos = pPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[INFO_POS];
		//박스 열기 , 폭탄 줍기
		pickGameObj(pPlayer, vTargetPos);
		//아이템 먹기
		putItem(pPlayer, vTargetPos);
	
		
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
	{
		if (m_pDynamite && m_pDynamite->GetState() == DYNAMITE_PICK)
		{
			m_pDynamite->SetState(DYNAMITE_THROW);
			m_pDynamite = nullptr;
			return OBJ_NOEVENT;
		}
	}
	if (MouseKeyUp(DIM_RB))
		pPlayer->RangeAttackPress(false);
	if (MouseKeyPress(DIM_RB) == false)
		pPlayer->RangeAttackPress(false);
	if (MouseKeyDown(DIM_RB))
		pPlayer->RangeAttackPress(true);


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
	if (DIKeyDown(DIK_E))
	{
		pPlayer->UsePotion();
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

void CPlayerController::putItem(CPlayer* pPlayer, const  _vec3& vTargetPos)
{
	_float fEquipItemDist = 3.f;
	_float fConsumItemDist = 3.f;
	CEquipItem* pEquipItem = nullptr;
	CConsumeItem* pConsumItem = nullptr;

	for (auto& ele : Get_Layer(LAYER_ITEM)->Get_MapObject())
	{
		if (CEquipItem* pItem = dynamic_cast<CEquipItem*>(ele.second))
		{
			_vec3 vDiff = vTargetPos - pItem->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
			_float fDist = D3DXVec3Length(&vDiff);

			if (fDist < fEquipItemDist)
			{
				pEquipItem = pItem;
				fEquipItemDist = fDist;
			}
		}

		if (CConsumeItem* pItem = dynamic_cast<CConsumeItem*>(ele.second))
		{
			_vec3 vDiff = vTargetPos - pItem->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
			_float fDist = D3DXVec3Length(&vDiff);

			if (fDist < fConsumItemDist)
			{
				pConsumItem = pItem;
				fConsumItemDist = fDist;
			}
		}
	}

	if (!pEquipItem && !pConsumItem)
		return;

	fEquipItemDist >= fConsumItemDist ? pPlayer->GetInventory()->Put(pConsumItem) : pPlayer->GetInventory()->Put(pEquipItem);

}

void CPlayerController::pickGameObj(CPlayer* pPlayer, const  _vec3& vTargetPos)
{
	_float fDynamiteDist = 3.f;
	_float fBoxDist = 3.f;
	CBox*	pBox = nullptr;
	CDynamite* pDynamite = nullptr;

	for (auto& ele : Get_Layer(LAYER_GAMEOBJ)->Get_MapObject())
	{
		if (CDynamite* pGameObj = dynamic_cast<CDynamite*>(ele.second))
		{
			_vec3 vDiff = vTargetPos - pGameObj->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
			_float fDist = D3DXVec3Length(&vDiff);

			if (fDist < fDynamiteDist)
			{
				pDynamite = pGameObj;
				fDynamiteDist = fDist;
			}
		}

		if (CBox* pGameObj = dynamic_cast<CBox*>(ele.second))
		{
			_vec3 vDiff = vTargetPos - pGameObj->Get_Component<Engine::CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[INFO_POS];
			_float fDist = D3DXVec3Length(&vDiff);

			if (fDist < fBoxDist)
			{
				pBox = pGameObj;
				fBoxDist = fDist;
			}
		}
	}

	if (pBox)
		pBox->BoxOpen();

	if (pDynamite)
	{
		pDynamite->SetState(DYNAMITE_PICK);
		m_pDynamite = pDynamite;
	}
}
