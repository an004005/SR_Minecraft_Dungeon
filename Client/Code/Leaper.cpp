#include "stdafx.h"
#include "..\Header\Leaper.h"
#include "AbstFactory.h"
#include "StatComponent.h"
#include "LeaperController.h"
#include "TerrainCubeMap.h"

CLeaper::CLeaper(LPDIRECT3DDEVICE9 pGraphicDev) : CMonster(pGraphicDev)
{
}

CLeaper::CLeaper(const CMonster & rhs) : CMonster(rhs)
{
}

CLeaper::~CLeaper()
{
}

HRESULT CLeaper::Ready_Object()
{
	CMonster::Ready_Object();

	m_arrAnim[ANIM_IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Leaper/idle.anim");
	m_arrAnim[ANIM_WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Leaper/walk.anim");
	m_arrAnim[ANIM_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Leaper/attack.anim");
	m_arrAnim[ANIM_JUMP] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Leaper/jump.anim");
	//m_arrAnim[ANIM_DEAD] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Leaper/dead.anim");

	m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
	m_pCurAnim = m_pIdleAnim;
	m_eState = State::IDLE;
	m_fSpeed = 2.5f;

	m_pStat->SetMaxHP(500);

	/*m_pStat->SetHurtSound({
	L"DLC_sfx_mob_monster_Hurt-001.ogg",
	L"DLC_sfx_mob_monster_Hurt-002.ogg" ,
	L"DLC_sfx_mob_monster_Hurt-003.ogg" });*/


	CController* pController = Add_Component<CLeaperController>(L"Proto_LeaperController", L"Proto_LeaperController", ID_DYNAMIC);
	pController->SetOwner(this);


	return S_OK;
}

void CLeaper::AnimationEvent(const string & strEvent)
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
	else if (strEvent == "Step")
	{
		CSoundMgr::GetInstance()->PlaySoundRandom({
			L"sfx_mob_zombieStepGeneric-001.ogg",
			L"sfx_mob_zombieStepGeneric-002.ogg",
			L"sfx_mob_zombieStepGeneric-003.ogg",
			L"sfx_mob_zombieStepGeneric-004.ogg",
			L"sfx_mob_zombieStepGeneric-005.ogg", },
			m_pRootPart->pTrans->m_vInfo[INFO_POS], 0.3f);
	}
}

_int CLeaper::Update_Object(const _float & fTimeDelta)
{
	if (m_bDelete) return OBJ_DEAD;

	CMonster::Update_Object(fTimeDelta);
	

	if (m_pCurAnim == m_pIdleAnim) // ���� �ִϸ��̼� ����
		m_bCanPlayAnim = true;

	IM_BEGIN("LOGO");

	_vec3 vPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
	ImGui::Text("%f, %f, %f", vPos.x, vPos.y, vPos.z);

	IM_END;

	// ���� ���� ���� ����
	StateChange();
	JumpToPlayer(fTimeDelta);

	// �� ���¿� ���� ������ ���� ������ �Լ� ����
	switch (m_eState)
	{
	case State::IDLE:
		break;
	case State::WALK:
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fSpeed * fTimeDelta;
		break;
	case State::CHOP:
		break;
	case State::ATTACK:
		break;
	case State::JUMP:
		break;
	case State::STUN:
		break;
	case State::DEAD:
		break;
	default:
		break;
	}

	return OBJ_NOEVENT;
}

void CLeaper::LateUpdate_Object()
{
	CMonster::LateUpdate_Object();

	if (m_bAttackFire)
	{
		set<CGameObject*> setObj;
		_vec3 vAttackPos = m_pRootPart->pTrans->m_vInfo[INFO_POS] + (m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * 2.f);
		Engine::GetOverlappedObject(setObj, vAttackPos, 2.f);

		for (auto& obj : setObj)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(25, m_pRootPart->pTrans->m_vInfo[INFO_POS], this);
		}


		CSoundMgr::GetInstance()->PlaySoundRandom({
			L"sfx_mob_zombieAttack-001.ogg",
			L"sfx_mob_zombieAttack-002.ogg",
			L"sfx_mob_zombieAttack-003.ogg",
			L"sfx_mob_zombieAttack-004.ogg" }, vAttackPos, 0.2f);
		m_bAttackFire = false;
	}
}

void CLeaper::Free()
{
	CMonster::Free();
}

CLeaper * CLeaper::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrPath, _bool bRemote)
{
	CLeaper* pInstance = new CLeaper(pGraphicDev);
	pInstance->m_bRemote = bRemote;

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

void CLeaper::StateChange()
{
	if (m_pStat->IsDead())
	{
		if (m_bReserveStop == false)
		{
			CSoundMgr::GetInstance()->PlaySoundRandom({
				L"sfx_mob_zombieDeath-001.ogg",
				L"sfx_mob_zombieDeath-002.ogg",
				L"sfx_mob_zombieDeath-003.ogg" },
				m_pRootPart->pTrans->m_vInfo[INFO_POS], 0.5f);

			m_eState = State::DEAD;
			PlayAnimationOnce(&m_arrAnim[ANIM_DEAD], true);
			m_bAttack = false;
			m_bMove = false;
			m_bCanPlayAnim = false;
			m_pColl->SetStop();
		}
		return;
	}

	

	if (m_pStat->IsStun())
	{
		m_eState = State::STUN;
		m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
		m_pCurAnim = &m_arrAnim[ANIM_IDLE];
		m_bAttack = false;
		m_bMove = false;

		return;
	}

	if (m_bAttack && m_bCanPlayAnim)
	{
		m_eState = State::ATTACK;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK]);
		m_bCanPlayAnim = false;
		m_bMove = false;
		m_bAttack = false;
		return;
	}

	if (m_bJump && m_bCanPlayAnim)
	{
		m_eState = State::JUMP;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[ANIM_JUMP]);
		m_vJumpTargetPos = m_vTargetPos;
		m_bCanPlayAnim = false;
		m_bMove = false;
		m_bJump = false;
		m_bLanding = false;
		return;
	}

	if (m_bMove && m_bCanPlayAnim)
	{
		m_eState = State::WALK;
		RotateToTargetPos(m_vTargetPos);
		m_pIdleAnim = &m_arrAnim[ANIM_WALK];
		m_pCurAnim = &m_arrAnim[ANIM_WALK];
		return;
	}

	if (m_bCanPlayAnim)
	{
		m_eState = State::IDLE;
		m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
		m_pCurAnim = &m_arrAnim[ANIM_IDLE];
		return;
	}
}

void CLeaper::JumpToPlayer(const _float& fTimeDelta)
{
	if (!m_bLanding)
	{
		_float fHeight = Get_GameObject<CTerrainCubeMap>(LAYER_ENV, L"TerrainCubeMap")->GetHeight(m_vJumpTargetPos.x, m_vJumpTargetPos.z);
		_vec3 vLook = m_vJumpTargetPos - m_pRootPart->pTrans->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vLook, &vLook);

		//m_pRootPart->pTrans->m_vInfo[INFO_POS] += vLook * 2.f * fTimeDelta;
		m_pRootPart->pTrans->m_vInfo[INFO_POS].y += 10.f * fTimeDelta;/*m_fJumpTime * fTimeDelta - (9.8f * m_fJumpTime * m_fJumpTime * fTimeDelta * 0.5f);*/

		m_fJumpTime += 3.f * fTimeDelta;

		m_bCanPlayAnim = false;
		m_bMove = false;

	/*	if (m_vTargetPos.y <= fHeight)
		{
			m_vTargetPos.y = fHeight;
			m_fJumpTime = 0.1f;
			m_bLanding = true;
		}*/
		
		

	}
	
}