#include "stdafx.h"
#include "..\Header\Creeper.h"

#include "AbstFactory.h"
#include "StatComponent.h"
#include "CreeperController.h"
#include "Particle.h"
#include "StaticCamera.h"

CCreeper::CCreeper(LPDIRECT3DDEVICE9 pGraphicDev) : CMonster(pGraphicDev)
{
}

CCreeper::CCreeper(const CMonster& rhs) : CMonster(rhs)
{
}

CCreeper::~CCreeper()
{
}

HRESULT CCreeper::Ready_Object()
{
	CMonster::Ready_Object();

	m_arrAnim[ANIM_IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Creeper/idle.anim");
	m_arrAnim[ANIM_WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Creeper/walk.anim");
	m_arrAnim[ANIM_DEAD] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Creeper/dead.anim");
	m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
	m_pCurAnim = m_pIdleAnim;
	m_eState = IDLE;
	m_fSpeed = 3.f;

	m_pStat->SetMaxHP(100);

	m_pStat->SetHurtSound({
		L"sfx_mob_creeperHurt-001.ogg",
		L"sfx_mob_creeperHurt-002.ogg",
		L"sfx_mob_creeperHurt-003.ogg"});
	CController* pController = Add_Component<CCreeperController>(L"Proto_CreeperController", L"Proto_CreeperController", ID_DYNAMIC);
	pController->SetOwner(this);

	return S_OK;
}

void CCreeper::AnimationEvent(const string& strEvent)
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
	}
}

_int CCreeper::Update_Object(const _float& fTimeDelta)
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
	case IDLE:
		break;
	case WALK:
		m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fSpeed * fTimeDelta;
		break;
	case STUN:
		break;
	case ATTACK:
		//폭발 애니메이션 실행
		//if 폭발 애니메이션이 끝나면 
		m_bAttackFire = true;
		break;
	case DEAD:
		break;
	default:
		break;
	}


	return OBJ_NOEVENT;
}

void CCreeper::LateUpdate_Object()
{
	CMonster::LateUpdate_Object();


	if (m_bAttackFire)
	{
		Get_GameObject<CStaticCamera>(LAYER_ENV, L"StaticCamera")
			->PlayShake(0.15f, 0.4f);
		CSoundMgr::GetInstance()->PlaySoundRandom({L"twinblast_grenade_explosion_01.ogg", L"twinblast_grenade_explosion_02.OGG"}, m_pRootPart->pTrans->m_vInfo[INFO_POS]);

		set<CGameObject*> setObj;
		_vec3 vAttackPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
		Engine::GetOverlappedObject(setObj, vAttackPos, 3.5f);

		for (auto& obj : setObj)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				       ->TakeDamage(70, m_pRootPart->pTrans->m_vInfo[INFO_POS], this, DT_KNOCK_BACK);
		}
		DEBUG_SPHERE(vAttackPos, 3.5f, 1.f);

		Get_GameObject<CFireWork>(LAYER_EFFECT, L"FireWork")->Add_Particle(
			m_pRootPart->pTrans->m_vInfo[INFO_POS], 0.3f, D3DXCOLOR(1.f, 1.f, 0.2f, 0), 256, 0.4f);
		CEffectFactory::Create<CUVCircle>("Creeper_Explosion", L"Creeper_Explosion",
		                                  m_pRootPart->pTrans->m_vInfo[INFO_POS]);
		Get_GameObject<CAttack_P>(LAYER_EFFECT, L"Attack_Basic")->Add_Particle(
			m_pRootPart->pTrans->m_vInfo[INFO_POS], 0.3f, RED, 30, 0.5f);
		for (int i = 0; i < 5; i++)
		{
			CEffectFactory::Create<CCloud>("Creeper_Cloud", L"Creeper_Cloud", m_pRootPart->pTrans->m_vInfo[INFO_POS]);
		}

		m_bAttackFire = false;
		m_bDelete = true;
	}
}

void CCreeper::Free()
{
	CMonster::Free();
}

CCreeper* CCreeper::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	CCreeper* pInstance = new CCreeper(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

void CCreeper::StateChange()
{
	if (m_pStat->IsDead())
	{
		if (m_bReserveStop == false)
		{
			m_eState = DEAD;
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
		m_eState = STUN;
		m_pIdleAnim = &m_arrAnim[ANIM_IDLE];
		m_pCurAnim = &m_arrAnim[ANIM_IDLE];
		m_bAttack = false;
		m_bMove = false;
		StopCurAnimation();
		return;
	}

	if (m_bAttack && m_bCanPlayAnim)
	{
		m_eState = ATTACK;
		m_bCanPlayAnim = false;
		m_bMove = false;
		m_bAttack = false;
		return;
	}

	if (m_bMove && m_bCanPlayAnim)
	{
		m_eState = WALK;
		RotateToTargetPos(m_vTargetPos);
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
