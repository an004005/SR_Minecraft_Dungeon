#include "stdafx.h"
#include "..\Header\Geomancer.h"
#include "StatComponent.h"
#include "Controller.h"

CGeomancer::CGeomancer(LPDIRECT3DDEVICE9 pGraphicDev): CMonster(pGraphicDev)
{
}

CGeomancer::CGeomancer(const CMonster& rhs): CMonster(rhs)
{
}

CGeomancer::~CGeomancer()
{
}

HRESULT CGeomancer::Ready_Object()
{
	CMonster::Ready_Object();
	m_pController = CGeomancerController::Create();

	m_arrAnim[IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Geomancer/idle.anim");
	m_arrAnim[WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Geomancer/walk.anim");
	m_arrAnim[DEAD] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Geomancer/dead.anim");
	m_arrAnim[ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Geomancer/attack.anim");
	m_pIdleAnim = &m_arrAnim[IDLE];
	m_pCurAnim = m_pIdleAnim;
	m_eState = IDLE;
	m_fSpeed = 3.f;

	m_pStat->SetMaxHP(100);

	return S_OK;
}

void CGeomancer::AnimationEvent(const string& strEvent)
{
	if (strEvent == "AttackFire")
	{
		m_bAttackFire = true;
	}
	else if (strEvent == "ActionEnd")
	{
		m_bCanPlayAnim = true;
	}
	else if (strEvent == "AnimStopped")
	{
		m_bDelete = true;
	}
}

_int CGeomancer::Update_Object(const _float& fTimeDelta)
{
	if (m_bDelete) return OBJ_DEAD;

	CMonster::Update_Object(fTimeDelta);
	m_pController->Update(this);

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
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fSpeed * fTimeDelta;
		break;
	case ATTACK:
		break;
	case STUN:
		break;
	case DEAD:
		break;
	default:
		break;
	}

	if (m_fCurAttackCoolTime < m_fAttackCoolTime)
		m_fCurAttackCoolTime += fTimeDelta;

	return OBJ_NOEVENT;
}

void CGeomancer::LateUpdate_Object()
{
	CMonster::LateUpdate_Object();
	// if (m_bAttack)
	// {
	// 	set<CGameObject*> setObj;
	// 	Engine::GetOverlappedObject(setObj, m_vAttackDest, 3.f);
	//
	// 	for (auto& obj : setObj)
	// 	{
	// 		if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
	// 			pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
	// 			       ->TakeDamage(1, m_pRootPart->pTrans->m_vInfo[INFO_POS], this, DT_KNOCK_BACK);
	//
	// 		DEBUG_SPHERE(m_vAttackDest, 3.f, 1.f);
	// 		IM_LOG("Fire");
	// 	}
	//
	// 	m_bAttack = false;
	// }
}

void CGeomancer::Free()
{
	CMonster::Free();
}

CGeomancer* CGeomancer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	CGeomancer* pInstance = new CGeomancer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

void CGeomancer::StateChange()
{
	if (m_pStat->IsDead())
	{
		m_eState = DEAD;
		PlayAnimationOnce(&m_arrAnim[ANIM_DEAD], true);
		m_bAttack = false;
		m_bMove = false;
		return;
	}

	if (m_pStat->IsStun())
	{
		m_eState = STUN;
		m_bAttack = false;
		m_bMove = false;
		return;
	}

	if (m_bAttack && m_bCanPlayAnim)
	{
		m_eState = ATTACK;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK]);
		m_bCanPlayAnim = false;
		m_bMove = false;
		m_bAttack = false;
		return;
	}

	if (m_bMove && m_bCanPlayAnim)
	{
		m_eState = WALK;
		RotateToTargetPos(m_vTargetPos, true);
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