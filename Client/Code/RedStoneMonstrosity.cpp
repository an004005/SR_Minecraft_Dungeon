#include "stdafx.h"
#include "..\Header\RedStoneMonstrosity.h"
#include "StatComponent.h"
#include "RedStoneMonstrosityController.h"
#include "AbstFactory.h"
#include "RedStoneCube.h"
#include <ctime>

CRedStoneMonstrosity::CRedStoneMonstrosity(LPDIRECT3DDEVICE9 pGraphicDev) : CMonster(pGraphicDev)
{
}

CRedStoneMonstrosity::CRedStoneMonstrosity(const CMonster& rhs) : CMonster(rhs)
{
}

CRedStoneMonstrosity::~CRedStoneMonstrosity()
{
}

HRESULT CRedStoneMonstrosity::Ready_Object()
{
	srand(_uint(time(NULL)));

	CMonster::Ready_Object();

	m_arrAnim[INTRO] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/intro.anim");
	m_arrAnim[WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/walk.anim");
	m_arrAnim[DEAD] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/dead.anim");
	m_arrAnim[CHOP] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/chop.anim");
	m_arrAnim[SPIT] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/spit.anim");
	m_arrAnim[WINDMILL] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/windmill.anim");
	m_arrAnim[SUMMON] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/summon.anim");

	m_pIdleAnim = &m_arrAnim[WALK];
	m_pCurAnim = &m_arrAnim[INTRO];
	m_eState = INTRO;
	m_fSpeed = 2.f;

	m_pStat->SetMaxHP(100);

	CController* pController = Add_Component<CRedStoneMonstrosityController>(L"Proto_RedStoneMonstrosityController", L"Proto_RedStoneMonstrosityController", ID_DYNAMIC);
	pController->SetOwner(this);

	//cc면역
	m_bCantCC = true;

	m_bCanPlayAnim = false;
	PlayAnimationOnce(&m_arrAnim[INTRO]);
	return S_OK;
}

void CRedStoneMonstrosity::AnimationEvent(const string& strEvent)
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

_int CRedStoneMonstrosity::Update_Object(const _float& fTimeDelta)
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
		break;
	case WALK:
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fSpeed * fTimeDelta;
		break;
	case CHOP:
		break;
	case SPIT:
		break;
	case SUMMON:
	{
		
		if (m_fSummonCoolTime < 0.000001f)
		{
			for (_int i = 0; i < 6; ++i)
			{
				_matrix matWorld;
				_vec3 vPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
				_int randomx = rand() % 20 - 10;
				_int randomz = rand() % 20 - 10;
				CGameUtilMgr::MatWorldComposeEuler(matWorld, { 1.f, 1.f, 1.f }, { 0.f, 0.f ,0.f }, { vPos.x + (_float)randomx, vPos.y , vPos.z + (_float)randomz });
				CEnemyFactory::Create<CRedStoneCube>("RedStoneCube", L"RedStoneCube", matWorld);
			}
		}

		m_fSummonCoolTime += fTimeDelta;
		
	}		
		break;
	case WINDMILL:
		break;
	case DEAD:
		break;
	default:
		break;
	}


	return OBJ_NOEVENT;
}

void CRedStoneMonstrosity::LateUpdate_Object()
{
	CMonster::LateUpdate_Object();

	if (m_bAttackFire)
	{
		set<CGameObject*> setObj;
		_vec3 vAttackPos = m_pRootPart->pTrans->m_vInfo[INFO_POS] + (m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * 2.f);
		Engine::GetOverlappedObject(setObj, vAttackPos, 1.f);

		for (auto& obj : setObj)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(1, m_pRootPart->pTrans->m_vInfo[INFO_POS], this);
		}
		DEBUG_SPHERE(vAttackPos, 1.f, 1.f);
		IM_LOG("Fire");

		m_bAttackFire = false;
	}
}

void CRedStoneMonstrosity::Free()
{
	CMonster::Free();
}

CRedStoneMonstrosity* CRedStoneMonstrosity::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	CRedStoneMonstrosity* pInstance = new CRedStoneMonstrosity(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

void CRedStoneMonstrosity::StateChange()
{
	if (m_pStat->IsDead() && m_bReserveStop == false)
	{
		m_eState = DEAD;
		PlayAnimationOnce(&m_arrAnim[DEAD], true);
	
		m_bCanPlayAnim = false;
		return;
	}


	if (m_bChop && m_bCanPlayAnim)
	{
		m_eState = CHOP;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[CHOP]);
		m_bCanPlayAnim = false;
		SetOff();	
		return;
	}

	if (m_bSpit && m_bCanPlayAnim)
	{
		m_eState = SPIT;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[SPIT]);
		m_bCanPlayAnim = false;
		SetOff();
		return;
	}

	if (m_bSummon && m_bCanPlayAnim)
	{
		m_eState = SUMMON;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[SUMMON]);
		m_bCanPlayAnim = false;
		SetOff();
		m_fSummonCoolTime = 0.f;
		return;
	}

	if (m_bWindmill && m_bCanPlayAnim)
	{
		m_eState = WINDMILL;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[WINDMILL]);
		m_bCanPlayAnim = false;
		SetOff();
		return;
	}


	if (m_bCanPlayAnim)
	{
		m_eState = WALK;
		RotateToTargetPos(m_vTargetPos);
		m_pIdleAnim = &m_arrAnim[WALK];
		m_pCurAnim = &m_arrAnim[WALK];
		return;
	}

}