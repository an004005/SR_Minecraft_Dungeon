#include "stdafx.h"
#include "..\Header\Kouku.h"
#include "AbstFactory.h"
#include "Skeleton.h"
#include "StatComponent.h"
#include "KoukuController.h"
#include "Particle.h"

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
	m_arrAnim[BASIC_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_basicattack.anim");
	m_arrAnim[IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_idle.anim");
	m_arrAnim[HAMMER_IN] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_hammer_x.anim");
	m_arrAnim[HAMMER_OUT] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_hammer_o.anim");
	m_arrAnim[HAMMER_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_hamer.anim");
	m_arrAnim[HORROR_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_runattack.anim");
	m_arrAnim[SYMBOL_HIDE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_hide.anim");
	m_arrAnim[REST] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/kouku_rest.anim");

	m_pIdleAnim = &m_arrAnim[IDLE];
	// m_pCurAnim = &m_arrAnim[INTRO];
	m_pCurAnim = m_pIdleAnim;

	m_eState = IDLE;
	m_fSpeed = 2.f;

	m_iRedSymbolCnt = 0;


	m_pStat->SetMaxHP(120);

	CController* pController = Add_Component<CKoukuController>(L"Proto_KoukuController", L"Proto_KoukuController", ID_DYNAMIC);
	pController->SetOwner(this);
	m_fCurTime = 0.f;
	m_fTime = 2.2f;
	//cc면역
	m_bCantCC = true;

	// m_bCanPlayAnim = false;
	// PlayAnimationOnce(&m_arrAnim[IDLE]);

	return S_OK;
}

void CKouku::AnimationEvent(const string& strEvent)
{
	if (strEvent == "SymbolCloud")
	{
		for (int i = 0; i < 5; i++)
		{
			CEffectFactory::Create<CCloud>("Creeper_Cloud", L"Creeper_Cloud", m_pRootPart->pTrans->m_vInfo[INFO_POS]);
		}
	}
	else if (strEvent == "HorrorAttack")
	{
		m_bHorrorAttack = true;
		m_bCountable = true;
	}
	else if (strEvent == "SymbolStart")
	{
		for (int i = 0; i < 40; i++)
		{
			CEffectFactory::Create<CHealCircle>("Blue_Circle", L"Blue_Circle", _vec3(51.5f, 25.f, 42.5f));
			CEffectFactory::Create<CHealCircle>("Blue_Circle", L"Blue_Circle", _vec3(57.5f, 25.f, 38.5f));
			CEffectFactory::Create<CHealCircle>("Red_Circle", L"Red_Circle", _vec3(67.5f, 25.f, 38.5f));
			CEffectFactory::Create<CHealCircle>("Blue_Circle", L"Blue_Circle", _vec3(73.5f, 25.f, 42.5f));
		}
		Get_GameObject<CMoonParticle>(LAYER_EFFECT, L"MoonParticle")->Add_Particle(_vec3(51.5f, 25.f, 42.5f), 1.f, BLUE, 12, 6.2f);
		Get_GameObject<CMoonParticle>(LAYER_EFFECT, L"MoonParticle")->Add_Particle(_vec3(57.5f, 25.f, 38.5f), 1.f, BLUE, 12, 6.2f);
		Get_GameObject<CMoonParticle>(LAYER_EFFECT, L"MoonParticle")->Add_Particle(_vec3(67.5f, 25.f, 38.5f), 1.f, RED, 12, 6.2f);
		Get_GameObject<CMoonParticle>(LAYER_EFFECT, L"MoonParticle")->Add_Particle(_vec3(73.5f, 25.f, 42.5f), 1.f, BLUE, 12, 6.2f);

		
	}
	else if (strEvent == "SymbolAttack")
	{
		m_bIsSymbolAttackCycle = true; // 이벤트 간격 늘리기 완료
	}
	else if (strEvent == "Kouku_Hide")
	{
		m_pRootPart->pTrans->m_vInfo[INFO_POS] = _vec3(62.5f, 0.f, 48.7f);
		PlayAnimationOnce(&m_arrAnim[REST]);
	}

	else if (strEvent == "AnimStopped")
	{
		m_bDelete = true;
		m_bCountable = false;
	}
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
	case BASIC_ATTACK:
		m_strState = "BASICATTACK";
		break;

	case SYMBOL_HIDE:
		m_strState = "SYMBOL_HIDE";
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

	

	return OBJ_NOEVENT;
}

void CKouku::LateUpdate_Object()
{
	CMonster::LateUpdate_Object();

	if (m_bIsSymbolGimmick)
	{
		set<CGameObject*> setPlayer_1;
		_vec3 Left_BludCircle_1 = _vec3(51.5f, 25.f, 42.5f);
		Engine::GetOverlappedObject(setPlayer_1, Left_BludCircle_1, 2.f);

		for (auto& obj : setPlayer_1)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(0, _vec3(51.5f, 25.f, 42.5f), this, DT_KOUKU_SYMBOL_BLUE);
		}

		DEBUG_SPHERE(Left_BludCircle_1, 10.f, 6.2f);
		IM_LOG("Create Left_Blue_1 Circle");


		set<CGameObject*> setPlayer_2;
		_vec3 Left_BludCircle_2 = _vec3(57.5f, 25.f, 38.5f);
		Engine::GetOverlappedObject(setPlayer_2, Left_BludCircle_2, 2.f);

		for (auto& obj : setPlayer_2)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(0, _vec3(57.5f, 25.f, 38.5f), this, DT_KOUKU_SYMBOL_BLUE);
		}

		DEBUG_SPHERE(Left_BludCircle_2, 10.f, 6.2f);
		IM_LOG("Create Left_Blue_2 Circle");


		set<CGameObject*> setPlayer_3;
		_vec3 Right_RedCircle_3 = _vec3(67.5f, 25.f, 38.5f);
		Engine::GetOverlappedObject(setPlayer_3, Right_RedCircle_3, 2.f);


		m_iRedSymbolCnt = 0;
		for (auto& obj : setPlayer_3)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(0, _vec3(67.5f, 25.f, 38.5f), this, DT_KOUKU_SYMBOL_RED);

			m_iRedSymbolCnt++;
		}

		DEBUG_SPHERE(Right_RedCircle_3, 10.f, 6.2f);
		IM_LOG("Create Right_Red_3 Circle");

		set<CGameObject*> setPlayer_4;
		_vec3 Left_BlueCircle_4 = _vec3(73.5f, 25.f, 42.5f);
		Engine::GetOverlappedObject(setPlayer_4, Left_BlueCircle_4, 2.f);

		for (auto& obj : setPlayer_4)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(0, _vec3(73.5f, 25.f, 42.5f), this, DT_KOUKU_SYMBOL_BLUE);
		}

		DEBUG_SPHERE(Left_BlueCircle_4, 10.f, 6.2f);
		IM_LOG("Create Right_Blue_4 Circle");


		if (m_bIsSymbolAttackCycle)
		{
			set<CGameObject*> setPlayer;
			_vec3 KoukuPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];

			Engine::GetOverlappedObject(setPlayer, KoukuPos, 100.f);

			for (auto& obj : setPlayer)
			{
				if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				{
					CStatComponent* PlayerStatCom = pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC);

					if (PlayerStatCom->IsSatonSymbol_Blue() == false && m_iRedSymbolCnt != 2)
					{
						PlayerStatCom->TakeDamage(20, pPlayer->GetInfo(INFO_POS), this, DT_END);
					}
				}
			}
			m_bIsSymbolAttackCycle = false;
		}
	}

	
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

	if (m_bBasicAttack && m_bCanPlayAnim)
	{
		m_eState = BASIC_ATTACK;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[BASIC_ATTACK]);
		m_bCanPlayAnim = false;
		m_bMove = false;
		SetOff();
		return;
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

	if (m_bKoukuSymbol && m_bCanPlayAnim)
	{
		m_eState = SYMBOL_HIDE;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[SYMBOL_HIDE]);
		m_bMove = false;
		m_bCanPlayAnim = false;
		SetOff();
		return;
	}
	//
	if (m_bMove && m_bCanPlayAnim)
	{
		m_eState = WALK;
		RotateToTargetPos(m_vTargetPos);
		m_pIdleAnim = &m_arrAnim[WALK];
		m_pCurAnim = &m_arrAnim[WALK];
		return;
	}
	// if (m_bCanPlayAnim)
	// {
	// 	m_eState = WALK;
	// 	RotateToTargetPos(m_vTargetPos);
	// 	m_pIdleAnim = &m_arrAnim[WALK];
	// 	m_pCurAnim = &m_arrAnim[WALK];
	// 	return;
	// }


	if (m_bCanPlayAnim)
	{
		m_eState = IDLE;
		// RotateToTargetPos(m_vTargetPos);
		m_pIdleAnim = &m_arrAnim[IDLE];
		m_pCurAnim = &m_arrAnim[IDLE];
		return;
	}
}

void CKouku::Symbol_Attack()
{
	set<CGameObject*> setPlayer;
	_vec3 KoukuPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];

	Engine::GetOverlappedObject(setPlayer, KoukuPos, 10.f);

	for (auto& obj : setPlayer)
	{

		if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
		{
			CStatComponent* PlayerStatCom = pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC);

			if (!PlayerStatCom->IsSatonSymbol_Blue())
			{
				PlayerStatCom->TakeDamage(20, pPlayer->GetInfo(INFO_POS), this, DT_KOUKU_SYMBOL_BLUE);
			}
		}
	}
}
