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

/*-----------------------
 *    CCharacter
 ----------------------*/



CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev) : CSkeletalCube(pGraphicDev)
{
	m_fSpeed = 4.f;
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

	CCollisionCom* pColl = Add_Component<CCollisionCom>(L"Proto_CollisionCom", L"Proto_CollisionCom", ID_DYNAMIC);
	pColl->SetOwner(this);
	pColl->SetOwnerTransform(m_pRootPart->pTrans);
	pColl->SetCollOffset(_vec3{0.f, 1.f, 0.f});
	pColl->SetRadius(0.5f);
	pColl->SetCollType(COLL_PLAYER);

	m_pStat = Add_Component<CStatComponent>(L"Proto_StatCom", L"Proto_StatCom", ID_DYNAMIC);
	m_pStat->SetMaxHP(100);
	m_pStat->SetTransform(m_pRootPart->pTrans);

	// 항상 카메라 먼저 만들고 플레이어 만들기!
	Get_GameObject<CStaticCamera>(LAYER_ENV, L"StaticCamera")->SetTarget(this);
	m_dwWalkDust = GetTickCount();
	m_dwRollDust = GetTickCount();

	m_RollCoolTime = 3.f;
	m_CurRollCoolTime = 0.f;

	m_pCrossbow = Get_GameObject<CCrossbow>(LAYER_ITEM, L"Crossbow");
	m_pSword = Get_GameObject<CSword>(LAYER_ITEM, L"Sword");
	m_pGlaive = Get_GameObject<CGlaive>(LAYER_ITEM, L"Glaive");

	m_pCurWeapon = m_pSword;

	m_arrAnim = m_pSword->SetarrAnim();
	return S_OK;
}

_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	CSkeletalCube::Update_Object(fTimeDelta);

	if (m_pCurAnim == m_pIdleAnim) // 이전 애니메이션 종료
		m_bCanPlayAnim = true;

	if (m_RollCoolTime > m_CurRollCoolTime)
		m_CurRollCoolTime += fTimeDelta;

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
	{
		if (m_bApplyMeleeAttackNext)
		{
			m_pCurWeapon->Collision();
			m_bApplyMeleeAttackNext = false;
		}

		if (m_bApplyMeleeAttack)
		{
			// RotateToCursor()로 회전하고, 이 회전값이 INFO_LOOK로 적용되려면 다음 update를 기다려야한다.
			// attack이 발생하고 다음 프레임에서 실제 공격을 적용하기 위한 코드
			m_bApplyMeleeAttack = false;
			m_bApplyMeleeAttackNext = true;
		}
	}
}

void CPlayer::Free()
{
	CSkeletalCube::Free();
}

void CPlayer::AnimationEvent(const string& strEvent)
{
	if (strEvent == "ActionEnd")
	{
		m_bCanPlayAnim = true;
		// SetMove(0.f, 0.f);
		// m_bRoll = false;
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
		
		//원거리 무기는 생략.
		m_iAttackCnt = m_pCurWeapon->Attack();
		m_bApplyMeleeAttack = true;
	}
	else if (m_bRangeAttack)
	{
		m_bCanPlayAnim = false;
		PlayAnimationOnce(&m_arrAnim[ANIM_RANGE_ATTACK]);
		Get_GameObject<CFireWork_Fuze>(LAYER_EFFECT, L"FireWork_Fuze")->Add_Particle(m_pRootPart->pTrans->m_vInfo[INFO_POS], 1.f, WHITE, 1, 0.5f);

		WeaponChange(m_pCrossbow);
	}



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

	if (m_bRoll && m_RollCoolTime <= m_CurRollCoolTime)
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
			1.f, D3DXCOLOR(0.3f, 0.4f, 0.7f, 1.f), 7, 20.f);
		return;
	}

	if (m_bMeleeAttack && m_bCanPlayAnim)
	{
		m_eState = ATTACK;
		RotateToCursor();
		return;
	}

	if (m_bRangeAttack && m_bCanPlayAnim)
	{
		m_eState = ATTACK;
		RotateToCursor();
		return;
	}

	if (m_bMove && m_bCanPlayAnim)
	{
		m_eState = WALK;
		RotateToMove();
		m_pIdleAnim = &m_arrAnim[ANIM_WALK];
		m_pCurAnim = &m_arrAnim[ANIM_WALK];
		return;
	}

	if (m_bCanPlayAnim)
	{
		m_eState = IDLE;
		m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
		m_pCurAnim = &m_arrAnim[ANIM_IDLE];
		WeaponChange(m_pCurWeapon);
		return;
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
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
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

void CPlayer::Legacy3Press()
{ 
	m_arrAnim = m_pGlaive->SetarrAnim();
	m_pCurWeapon = m_pGlaive;
}
void CPlayer::Legacy4Press() 
{ 
	m_arrAnim = m_pSword->SetarrAnim();
	m_pCurWeapon = m_pSword;
	
}