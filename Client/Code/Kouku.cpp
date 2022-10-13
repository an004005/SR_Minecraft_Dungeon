#include "stdafx.h"
#include "..\Header\Kouku.h"
#include "AbstFactory.h"
#include "Skeleton.h"
#include "StatComponent.h"
#include "KoukuController.h"

CKouku::CKouku(LPDIRECT3DDEVICE9 pGraphicDev) : CMonster(pGraphicDev)
{
}

CKouku::CKouku(const CMonster& rhs) : CMonster(rhs)
{
}

CKouku::~CKouku()
{
}

HRESULT CKouku::Ready_Object()
{
	CMonster::Ready_Object();

	// m_arrAnim[INTRO] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/intro.anim");
	m_arrAnim[WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_walk.anim");
	// m_arrAnim[DEAD] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/dead.anim");
	m_arrAnim[IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_idle.anim");
	m_arrAnim[HAMMER_IN] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_hammer_x.anim");
	m_arrAnim[HAMMER_OUT] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_hammer_o.anim");
	m_arrAnim[HAMMER_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_hamer.anim");
	m_arrAnim[HORROR_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_runattack.anim");

	m_pIdleAnim = &m_arrAnim[IDLE];
	// m_pCurAnim = &m_arrAnim[INTRO];
	m_pCurAnim = m_pIdleAnim;

	m_eState = IDLE;
	m_fSpeed = 2.f;

	m_pStat->SetMaxHP(1000);

	CController* pController = Add_Component<CKoukuController>(L"Proto_KoukuController", L"Proto_KoukuController", ID_DYNAMIC);
	pController->SetOwner(this);
	m_fCurTime = 0.f;
	m_fTime = 2.2f;
	//cc면역
	m_bCantCC = true;

	m_bCanPlayAnim = false;
	PlayAnimationOnce(&m_arrAnim[IDLE]);

	return S_OK;
}

void CKouku::AnimationEvent(const string& strEvent)
{
	if (strEvent == "DoubleHammer")
	{
		m_bDoubleHammer = true;
	}
	else if (strEvent == "HorrorAttack")
	{
		m_bHorrorAttack = true;
		m_bCountable = true;
	}
	else if (strEvent == "AnimStopped")
	{
		m_bDelete = true;
		m_bCountable = false;
	}
	// else if (strEvent == "SpitFire")
	// {
	// 	_matrix matWorld;
	// 	_vec3 vPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
	// 	CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, 0.f ,0.f }, { vPos.x, vPos.y + 5.f , vPos.z });
	// 	CObjectFactory::Create<CRedStoneMonstrosityBullet>("RedStoneMonstrosityBullet", L"RedStoneMonstrosityBullet", matWorld);
	// }
}

_int CKouku::Update_Object(const _float& fTimeDelta)
{
	if (m_bDelete) return OBJ_DEAD;


	CMonster::Update_Object(fTimeDelta);

	if (m_pCurAnim == m_pIdleAnim) // 이전 애니메이션 종료
		m_bCanPlayAnim = true;

	// 상태 변경 조건 설정
	StateChange();

	// 각 상태에 따른 프레임 마다 실행할 함수 지정
	switch (m_eState)
	{
	case INTRO:
		m_strState = "INTRO";
		break;
	case WALK:
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fSpeed * fTimeDelta;
		m_strState = "WALK";
		break;
	case HAMMER_IN:
		break;
	case HAMMER_OUT:
		break;
	case HAMMER_ATTACK:
		m_strState = "HAMMER_ATTACK";
	break;
	case HORROR_ATTACK:
		m_fCurTime += fTimeDelta;

		if (m_fCurTime <= m_fTime)
		{
			m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fSpeed * fTimeDelta;
		}
		m_strState = "HORROR_ATTACK";
		break;
	case DEAD:
		break;
	default:
		break;
	}

	if (m_eState != HORROR_ATTACK)
	{
		if (m_fCurTime > m_fTime)
			m_fCurTime = 0.f;
	}

	IM_BEGIN("State");

	ImGui::Text(m_strState.c_str());

	IM_END;

	return OBJ_NOEVENT;
}

void CKouku::LateUpdate_Object()
{
	CMonster::LateUpdate_Object();

	// if (m_bChopFire)
	// {
	// 	set<CGameObject*> setObj;
	// 	_vec3 vAttackPos = m_pRootPart->pTrans->m_vInfo[INFO_POS] + (m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * 2.f);
	// 	Engine::GetOverlappedObject(setObj, vAttackPos, 6.f);
	//
	// 	for (auto& obj : setObj)
	// 	{
	// 		if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
	// 			pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
	// 			->TakeDamage(1, m_pRootPart->pTrans->m_vInfo[INFO_POS], this, DT_KNOCK_BACK);
	// 	}
	// 	DEBUG_SPHERE(vAttackPos, 6.f, 1.f);
	// 	IM_LOG("Fire");
	//
	// 	m_bChopFire = false;
	// }
}

void CKouku::Free()
{
	CMonster::Free();
}

CKouku* CKouku::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	CKouku* pInstance = new CKouku(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

void CKouku::StateChange()
{
	if (m_pStat->IsDead())
	{
		if (m_bReserveStop == false)
		{
			m_eState = DEAD;
			PlayAnimationOnce(&m_arrAnim[DEAD], true);
			m_bCanPlayAnim = false;
			return;
		}
	}

	if (m_bDoubleHammer && m_bCanPlayAnim)
	{
		m_eState = HAMMER_ATTACK;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[HAMMER_ATTACK]);
		m_bCanPlayAnim = false;
		m_bMove = false;
		SetOff();
		return;
	}

	if (m_bHorrorAttack && m_bCanPlayAnim)
	{
		m_eState = HORROR_ATTACK;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[HORROR_ATTACK]);
		m_bMove = false;
		m_bCanPlayAnim = false;
		SetOff();
		return;
	}
	//
	// if (m_bMove && m_bCanPlayAnim)
	// {
	// 	m_eState = WALK;
	// 	RotateToTargetPos(m_vTargetPos, true);
	// 	m_pIdleAnim = &m_arrAnim[WALK];
	// 	m_pCurAnim = &m_arrAnim[WALK];
	// 	return;
	// }
	if (m_bCanPlayAnim)
	{
		m_eState = WALK;
		RotateToTargetPos(m_vTargetPos);
		m_pIdleAnim = &m_arrAnim[WALK];
		m_pCurAnim = &m_arrAnim[WALK];
		return;
	}


	// if (m_bCanPlayAnim)
	// {
	// 	m_eState = IDLE;
	// 	RotateToTargetPos(m_vTargetPos);
	// 	m_pIdleAnim = &m_arrAnim[IDLE];
	// 	m_pCurAnim = &m_arrAnim[IDLE];
	// 	return;
	// }
}
