#include "stdafx.h"
#include "Player.h"
#include "Controller.h"
#include "GameUtilMgr.h"
#include "StatComponent.h"
#include "TerrainCubeMap.h"
#include "StaticCamera.h"
#include "Monster.h"
#include "PlayerController.h"

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

	m_arrAnim[ANIM_IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_idle.anim");
	m_arrAnim[ANIM_IDLE].bLoop = true;
	m_arrAnim[ANIM_WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_walk.anim");
	m_arrAnim[ANIM_WALK].bLoop = true;
	m_arrAnim[ANIM_ATTACK1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_attack_a.anim");
	m_arrAnim[ANIM_ATTACK2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_attack_b.anim");
	m_arrAnim[ANIM_ATTACK3] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_attack_c.anim");
	m_arrAnim[ANIM_ROLL] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/roll.anim");
	m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
	m_pCurAnim = m_pIdleAnim;

	CController* pController = Add_Component<CPlayerController>(L"Proto_PlayerController", L"Proto_PlayerController", ID_DYNAMIC);
	pController->SetOwner(this);

	CCollisionCom* pColl = Add_Component<CCollisionCom>(L"Proto_CollisionCom", L"Proto_CollisionCom", ID_DYNAMIC);
	pColl->SetOwner(this);
	pColl->SetOwnerTransform(m_pRootPart->pTrans);
	pColl->SetCollOffset(_vec3{0.f, 1.f, 0.f});
	pColl->SetRadius(0.5f);

	m_pStat = Add_Component<CStatComponent>(L"Proto_StatCom", L"Proto_StatCom", ID_DYNAMIC);
	m_pStat->SetMaxHP(100);
	m_pStat->SetTransform(m_pRootPart->pTrans);

	// 항상 카메라 먼저 만들고 플레이어 만들기!
	Get_GameObject<CStaticCamera>(LAYER_ENV, L"StaticCamera")->SetTarget(this);

	return S_OK;
}

_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	CSkeletalCube::Update_Object(fTimeDelta);

	if (m_pCurAnim == m_pIdleAnim) // 이전 애니메이션 종료
		m_bCanPlayAnim = true;

	// 상태 변경 조건 설정
	StateChange();

	// 각 상태에 따른 프레임 마다 실행할 함수 지정
	switch (m_eState)
	{
	case IDLE:
		break;
	case WALK:
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_vMoveDirNormal * m_fSpeed * fTimeDelta;
		break;
	case ATTACK:
		MeleeAttack(); // 근접, 원거리 분기하기
		break;
	case STUN:
		break;
	case ROLL:
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fRollSpeed * fTimeDelta;
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
			set<CGameObject*> objSet;
			_vec3 vAttackPos = m_pRootPart->pTrans->m_vInfo[INFO_POS] + (m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * 2.f);
			Engine::GetOverlappedObject(OUT objSet, vAttackPos, 2.f);
			for (auto& obj : objSet)
			{
				if (CMonster* monster = dynamic_cast<CMonster*>(obj))
				{
					DamageType eDT = DT_END;
					if (m_iAttackCnt == 0) eDT = DT_KNOCK_BACK;
					monster->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
					       ->TakeDamage(30, m_pRootPart->pTrans->m_vInfo[INFO_POS], this, eDT);
				}
			}


			DEBUG_SPHERE(vAttackPos, 2.f, 1.f);

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
	Safe_Release(m_pController);
}

void CPlayer::AnimationEvent(const string& strEvent)
{
	if (strEvent == "ActionEnd")
	{
		m_bCanPlayAnim = true;
		// SetMove(0.f, 0.f);
		// m_bRoll = false;
	}
}

void CPlayer::SetMoveDir(_float fX, _float fZ)
{
	m_vMoveDirNormal.x = fX;
	m_vMoveDirNormal.z = fZ;

	// x, z 중 하나가 0 이 아니면 움직임 요청
	m_bMove = CGameUtilMgr::FloatCmp(fX, 0.f) == false || CGameUtilMgr::FloatCmp(fZ, 0.f) == false;
	if (m_bMove)
	{
		int a= 3;
	}
}

void CPlayer::MeleeAttack()
{
	if (m_bCanPlayAnim == false) return;

	m_bCanPlayAnim = false;
	if (m_iAttackCnt == 0)
	{
		PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK1]);
	}
	else if (m_iAttackCnt == 1)
	{
		PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK2]);
	}
	else
	{
		PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK3]);
	}
	m_iAttackCnt = (m_iAttackCnt + 1) % 3;
	m_bApplyMeleeAttack = true;
}



void CPlayer::StateChange()
{
	if (m_pStat->IsDead())
	{
		m_eState = DEAD;
		PlayAnimationOnce(&m_arrAnim[ANIM_DEAD], true);
		m_bRoll = false;
		return;
	}

	if (m_pStat->IsStun())
	{
		m_eState = STUN;
		m_bRoll = false;
		return;
	}

	if (m_bRoll)
	{
		m_eState = ROLL;
		RotateToCursor();
		m_bCanPlayAnim = false;
		PlayAnimationOnce(&m_arrAnim[ANIM_ROLL]);
		m_bRoll = false;
		return;
	}

	if (m_bMeleeAttack && m_bCanPlayAnim)
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
