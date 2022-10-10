#include "stdafx.h"
#include "Player.h"
#include "Controller.h"
#include "GameUtilMgr.h"
#include "StatComponent.h"
#include "TerrainCubeMap.h"
#include "AbstFactory.h"
#include "Particle.h"
#include "StaticCamera.h"
#include "Monster.h"
#include "PlayerController.h"
#include "Crossbow.h"
#include "Sword.h"
#include "Glaive.h"
#include "Axe.h"
#include "SphereEffect.h"
#include "Inventory.h"
#include "Emerald.h"

/*-----------------------
 *    CCharacter
 ----------------------*/

const _float CPlayer::s_PotionCollTime = 20.f;
const _float CPlayer::s_RollCoolTime = 3.f;

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev) : CSkeletalCube(pGraphicDev)
{
	m_fSpeed = 4.5f;
	m_fRollSpeed = 12.f;
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object()
{
	CSkeletalCube::Ready_Object();


	m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
	m_pCurAnim = m_pIdleAnim;

	CController* pController = Add_Component<CPlayerController>(L"Proto_PlayerController", L"Proto_PlayerController", ID_DYNAMIC);
	pController->SetOwner(this);

	m_pColl = Add_Component<CCollisionCom>(L"Proto_CollisionCom", L"Proto_CollisionCom", ID_DYNAMIC);
	m_pColl->SetOwner(this);
	m_pColl->SetOwnerTransform(m_pRootPart->pTrans);
	m_pColl->SetCollOffset(_vec3{0.f, 1.5f, 0.f});
	m_pColl->SetRadius(0.5f);
	m_pColl->SetCollType(COLL_PLAYER);

	m_CurRollCoolTime = 3.f;
	m_CurPotionCoolTime = 20.f;

	m_pStat = Add_Component<CStatComponent>(L"Proto_StatCom", L"Proto_StatCom", ID_DYNAMIC);
	m_pStat->SetMaxHP(100);
	m_pStat->SetTransform(m_pRootPart->pTrans);

	// 항상 카메라 먼저 만들고 플레이어 만들기!
	Get_GameObject<CStaticCamera>(LAYER_ENV, L"StaticCamera")->SetTarget(this);
	m_dwWalkDust = GetTickCount();
	m_dwRollDust = GetTickCount();


	m_pInventory = CObjectFactory::Create<CInventory>("Inventory", L"Inventory");
	m_pInventory->AddRef();
	m_arrAnim = m_pInventory->CurWeapon(IT_MELEE)->SetarrAnim();
	return S_OK;
}

_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	CSkeletalCube::Update_Object(fTimeDelta);
	DEBUG_SPHERE(m_pColl->GetCollPos(), m_pColl->GetRadius(), 0.1f);

	if (m_pCurAnim == m_pIdleAnim) // 이전 애니메이션 종료
		m_bCanPlayAnim = true;

	if (s_RollCoolTime > m_CurRollCoolTime)
		m_CurRollCoolTime += fTimeDelta;
	if (s_PotionCollTime > m_CurPotionCoolTime)
		m_CurPotionCoolTime += fTimeDelta;

	// 상태 변경 조건 설정
	StateChange();
	m_bRoll = false;

	// 각 상태에 따른 프레임 마다 실행할 함수 지정
	switch (m_eState)
	{
	case IDLE:
		break;
	case WALK:
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_vMoveDirNormal * m_fSpeed * fTimeDelta;
		break;
	case ATTACK:
		AttackState(); // 근접, 원거리 분기하기
		break;
	case STUN:
		break;
	case ROLL:
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fRollSpeed * fTimeDelta;
		if (m_dwRollDust + 300 < GetTickCount())
		{
			CEffectFactory::Create<CCloud>("Roll_Cloud", L"Roll_Cloud");
			m_dwRollDust = GetTickCount();
		}
		break;
	case LEGACY:
		break;
	case DEAD:
		break;
	default:
		break;;
	}

	return OBJ_NOEVENT;
}

void CPlayer::LateUpdate_Object()
{
	
	if (m_bApplyMeleeAttackNext)
	{
		m_pInventory->CurWeapon(IT_MELEE)->Collision();
		m_bApplyMeleeAttackNext = false;
	}

	if (m_bApplyMeleeAttack)
	{
		// RotateToCursor()로 회전하고, 이 회전값이 INFO_LOOK로 적용되려면 다음 update를 기다려야한다.
		// attack이 발생하고 다음 프레임에서 실제 공격을 적용하기 위한 코드
		m_bApplyMeleeAttack = false;
		m_bApplyMeleeAttackNext = true;
	}

	//에메랄드가 땅에 떨어졌을 때, 플레이어쪽으로 오게 한다
	for (auto& ele : Get_Layer(LAYER_ITEM)->Get_MapObject())
	{
		if (CEmerald* pItem = dynamic_cast<CEmerald*>(ele.second))
		{
			_float fDist = D3DXVec3Length(&(m_pRootPart->pTrans->m_vInfo[INFO_POS] - pItem->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS]));

			if (fDist < 5.f && pItem->OnGround())
				pItem->GoToPlayer();


			if (fDist < 0.5f && pItem->OnGround())
			{
				m_pInventory->Put(pItem);
			}
		}
	}
}

void CPlayer::Free()
{
	Safe_Release(m_pInventory);
	CSkeletalCube::Free();
}

void CPlayer::AnimationEvent(const string& strEvent)
{
	if (strEvent == "ActionEnd")
	{
		m_bCanPlayAnim = true;
	}
	else if (strEvent == "MeleeAttackFire")
	{
		m_bApplyMeleeAttack = true;

		// axe crack
		if (m_iAttackCnt == 0 && dynamic_cast<CAxe*>(m_pInventory->CurWeapon(IT_MELEE)))
		{
			CEffectFactory::Create<CCrack>("Exe_Decal", L"Exe_Decal");
			for (int i = 0; i < 5; i++)
			{
				CEffectFactory::Create<CCloud>("Decal_Cloud", L"Decal_Cloud");
			}
		}
		// axe crack
	}
	else if (strEvent == "step")
	{
		if (m_dwWalkDust + 500 < GetTickCount())
		{
			CEffectFactory::Create<CCloud>("Walk_Cloud", L"Walk_Cloud");
			m_dwWalkDust = GetTickCount();
		}
	}
}

void CPlayer::SetMoveDir(_float fX, _float fZ)
{
	m_vMoveDirNormal.x = fX;
	m_vMoveDirNormal.z = fZ;

	// x, z 중 하나가 0 이 아니면 움직임 요청
	m_bMove = CGameUtilMgr::FloatCmp(fX, 0.f) == false || CGameUtilMgr::FloatCmp(fZ, 0.f) == false;
}

void CPlayer::AttackState()
{
	if (m_bCanPlayAnim == false) return;

	if (m_bMeleeAttack)
	{
		m_bCanPlayAnim = false;
		m_iAttackCnt = m_pInventory->CurWeapon(IT_MELEE)->Attack();// 애니메이션 실행
	}
	else if (m_bRangeAttack)
	{
		//기본이 근거리라 원거리로 바꿔줘야 텍스처가 나옴
		WeaponChange(IT_RANGE);
		m_bCanPlayAnim = false;
		m_iAttackCnt = m_pInventory->CurWeapon(IT_RANGE)->Attack();
	}

#pragma region GolemSmash
	// 	CEffectFactory::Create<CSphereEffect>("Golem_Melee_Shpere_L", L"Golem_Melee_Shpere_L");
	// 	CEffectFactory::Create<CSphereEffect>("Golem_Melee_Shpere_M", L"Golem_Melee_Shpere_M");
	//
	// 	CEffectFactory::Create<CSphereEffect>("Golem_Melee_L", L"Golem_Melee_L");
	// 	CEffectFactory::Create<CSphereEffect>("Golem_Melee_M", L"Golem_Melee_M");
	// 	CEffectFactory::Create<CSphereEffect>("Golem_Melee_S", L"Golem_Melee_S");
	// 	for (int i = 0; i < 15; i++)
	// 	{
	// 		CEffectFactory::Create<CCloud>("ShockPowder_Cloud", L"ShockPowder_Cloud");
	// 	}
	// //완전히 찍을 때
	// 	Get_GameObject<CAttack_P>(LAYER_EFFECT, L"Attack_Basic")->Add_Particle(m_pRootPart->pTrans->m_vInfo[INFO_POS], 0.5f, D3DXCOLOR(0.88f,0.35f,0.24f,1.0f), 12, 0.8f);
#pragma endregion

#pragma region GolemSpit
	// for (int i = 0; i < 10; i++)
	// {
		// CEffectFactory::Create<CGolemSpit>("Golem_Spit", L"Golem_Spit");
	//}
#pragma endregion

#pragma region RedCube_Spawn
		// CEffectFactory::Create<CCrack>("Red_Cube_Crack", L"Red_Cube_Crack");
#pragma endregion

#pragma region Lava_Paticle
	// CEffectFactory::Create<CLava_Particle>("Lava_Particle", L"Lava_Particle");
#pragma endregion

#pragma region Attack_Basic
	// Get_GameObject<CAttack_P>(LAYER_EFFECT, L"Attack_Basic")->Add_Particle(m_pRootPart->pTrans->m_vInfo[INFO_POS], 0.3f, RED, 4, 0.2f);
#pragma endregion

}



void CPlayer::StateChange()
{
	if (m_pStat->IsDead())
	{
		m_eState = DEAD;
		PlayAnimationOnce(&m_arrAnim[ANIM_DEAD], true);
		m_bRoll = false;
		m_bCanPlayAnim = false;
		return;
	}

	if (m_pStat->IsStun())
	{
		m_eState = STUN;
		m_bRoll = false;
		return;
	}

	if (m_bRoll && s_RollCoolTime <= m_CurRollCoolTime)
	{
		m_eState = ROLL;
		RotateToCursor();
		m_bCanPlayAnim = false;
		PlayAnimationOnce(&m_arrAnim[ANIM_ROLL]);
		m_bRoll = false;
		m_CurRollCoolTime = 0.f;
		return;
	}

	if (m_bLegacy1 && m_bCanPlayAnim)
	{
		m_eState = LEGACY;
		PlayAnimationOnce(&m_arrAnim[ANIM_LEGACY1]);
		m_bLegacy1 = false;
		m_bCanPlayAnim = false;

		for (int j = 0; j < 10; j++)
		{
			CEffectFactory::Create<CShock_Powder>("Shock_Powder", L"UV_Shock_Powder");
			CEffectFactory::Create<CCloud>("ShockPowder_Cloud", L"ShockPowder_Cloud");
		}
		CEffectFactory::Create<CUVCircle>("Shock_Circle", L"Shock_Circle");
		return;
	}

	if (m_bLegacy2 && m_bCanPlayAnim)
	{
		m_eState = LEGACY;
		PlayAnimationOnce(&m_arrAnim[ANIM_LEGACY2]);
		m_bLegacy2 = false;
		m_bCanPlayAnim = false;

		Get_GameObject<C3DBaseTexture>(LAYER_EFFECT, L"3D_Base")->Add_Particle(m_pRootPart->pTrans->m_vInfo[INFO_POS], 3.f, D3DXCOLOR(0.f,0.63f,0.82f,0.f), 1, 1.5f);
		Get_GameObject<CSpeedBoots>(LAYER_EFFECT, L"Speed_Boots")->Add_Particle(m_pRootPart->pTrans->m_vInfo[INFO_POS], 3.f, D3DXCOLOR(0.2f, 0.2f, 0.5f, 1.f), 1, 1.5f);
		Get_GameObject<CSpeedBoots_Particle>(LAYER_EFFECT, L"Speed_Boots_Particle")->Add_Particle(
			_vec3(m_pRootPart->pTrans->m_vInfo[INFO_POS].x, m_pRootPart->pTrans->m_vInfo[INFO_POS].y + 15.f, m_pRootPart->pTrans->m_vInfo[INFO_POS].z),
			1.f, D3DXCOLOR(0.3f, 0.4f, 0.7f, 1.f), 18, 20.f);
		return;
	}

	if (m_bMeleeAttack && m_bCanPlayAnim)
	{
		m_eState = ATTACK;
		RotateToCursor();
		WeaponChange(IT_MELEE);
		return;
	}

	if (m_bRangeAttack && m_bCanPlayAnim)
	{
		m_eState = ATTACK;
		RotateToCursor();
		m_bDelay = true;
		WeaponChange(IT_MELEE);
		return;
	}

	if (m_bMove && m_bCanPlayAnim)
	{
		m_eState = WALK;
		RotateToMove();
		m_pIdleAnim = &m_arrAnim[ANIM_WALK];
		m_pCurAnim = &m_arrAnim[ANIM_WALK];

		if (m_bDelay) m_bDelay = false;
		else WeaponChange(IT_MELEE);
		return;
	}

	if (m_bCanPlayAnim)
	{
		m_eState = IDLE;
		m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
		m_pCurAnim = &m_arrAnim[ANIM_IDLE];

		if (m_bDelay) m_bDelay = false;
		else WeaponChange(IT_MELEE);
		return;
	}
}

void CPlayer::UsePotion()
{
	if (s_PotionCollTime <= m_CurPotionCoolTime)
	{
		m_pStat->ModifyHP(_int(_float(m_pStat->GetMaxHP()) * 0.7f));
		m_CurPotionCoolTime = 0.f;

		// particle
		CEffectFactory::Create<CHealCircle>("Heal_Circle_L", L"Heal_Circle_L");
		for (int i = 0; i < 12; i++)
		{
			CEffectFactory::Create<CHeartParticle>("HeartParticle", L"HeartParticle");
		}
		// particle
	}
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

void CPlayer::RotateToCursor()
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3 vPoint;
	_vec3 vAt;

	D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vPoint.x = (_float)ptMouse.x / ((_float)ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = (_float)ptMouse.y / -((_float)ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;
	vAt.x = vPoint.x;
	vAt.y = vPoint.y;
	vAt.z = 1.f;

	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);
	D3DXVec3TransformCoord(&vAt, &vAt, &matProj);

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matView);
	D3DXVec3TransformCoord(&vAt, &vAt, &matView);

	D3DXPLANE tmpPlane;
	D3DXPlaneFromPointNormal(&tmpPlane, &m_pRootPart->pTrans->m_vInfo[INFO_POS], &CGameUtilMgr::s_vUp);

	_vec3 vLookAt;
	D3DXPlaneIntersectLine(&vLookAt, &tmpPlane, &vPoint, &vAt);

	_vec3 vLook = vLookAt - m_pRootPart->pTrans->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vLook, &vLook);

	const _vec2 v2Look{0.f, 1.f};
	_vec2 v2ToDest{vLook.x, vLook.z};

	const _float fDot = D3DXVec2Dot(&v2Look, &v2ToDest);

	if (vLook.x < 0)
		m_pRootPart->pTrans->m_vAngle.y = -acosf(fDot);
	else
		m_pRootPart->pTrans->m_vAngle.y = acosf(fDot);

	m_pRootPart->pTrans->Update_Component(0.f);

	m_pIdleAnim = &m_arrAnim[ANIM_WALK];
	m_pCurAnim = m_pIdleAnim;
}

void CPlayer::RotateToMove()
{
	const _vec2 v2Look{0.f, 1.f};
	const _vec2 v2ToDest{m_vMoveDirNormal.x, m_vMoveDirNormal.z};
	const _float fDot = D3DXVec2Dot(&v2Look, &v2ToDest);


	if (m_vMoveDirNormal.x < 0)
		m_pRootPart->pTrans->m_vAngle.y = -acosf(fDot);
	else
		m_pRootPart->pTrans->m_vAngle.y = acosf(fDot);
}

void CPlayer::Legacy4Press()
{ 
	WeaponChange(IT_MELEE);
}

void CPlayer::WeaponChange(ITEMTYPE eIT)
{
	if (m_pWeaponPart == nullptr)
	{
		auto& itr = m_mapParts.find("weapon_r");
		if (itr == m_mapParts.end())
			return;
		m_pWeaponPart = itr->second;
	}
	
	m_pInventory->Equip_Item(m_pWeaponPart, eIT);
	m_arrAnim = m_pInventory->CurWeapon(eIT)->SetarrAnim();
}