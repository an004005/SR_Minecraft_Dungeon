#include "stdafx.h"
#include "Saton.h"
#include "AbstFactory.h"
#include "Kouku.h"
#include "Particle.h"
#include "SatonController.h"
#include "Skeleton.h"
#include "StatComponent.h"

CSaton::CSaton(LPDIRECT3DDEVICE9 pGraphicDev) : CMonster(pGraphicDev)
{
}

CSaton::CSaton(const CMonster& rhs) : CMonster(rhs)
{
}

HRESULT CSaton::Ready_Object()
{
	CMonster::Ready_Object();

	// m_arrAnim[INTRO] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/RedStoneMonstrosity/intro.anim");
	m_arrAnim[FIRSTATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/saton_doubleattack.anim");
	// m_arrAnim[SECONDATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/saton_attack2.anim");
	m_arrAnim[IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/saton_idle.anim");
	m_arrAnim[SATON_BIRD] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/saton_bird.anim");
	m_arrAnim[SATON_GRAP] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/saton_grap.anim");
	m_arrAnim[SATON_SYMBOL] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/saton_symbol.anim");
	m_arrAnim[SATON_FASCINATE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/KoukuSaton/saton_fascinate.anim");

	m_pIdleAnim = &m_arrAnim[IDLE];
	// m_pCurAnim = &m_arrAnim[INTRO];
	m_pCurAnim = m_pIdleAnim;

	m_eState = IDLE;
	m_fSpeed = 2.f;
	// m_pStat->IsSaton();
	m_pStat->SetMaxHP(100);
	// m_pRootPart->pTrans->Set_Pos(62.5f, 20.5f, 49.4f);
	m_pRootPart->pTrans->m_vInfo[INFO_POS].y = 20.5f;

	CController* pController = Add_Component<CSatonController>(L"Proto_SatonController", L"Proto_SatonController", ID_DYNAMIC);
	pController->SetOwner(this);
	m_fCurTime = 0.f;
	m_fTime = 2.2f;
	//cc면역
	m_bCantCC = true;

	m_bCanPlayAnim = false;
	PlayAnimationOnce(&m_arrAnim[IDLE]);

	return S_OK;
}

void CSaton::AnimationEvent(const string& strEvent)
{
	if (strEvent == "FirstAttack")
	{
		m_bFirstHammerAttack = true;
	}
	else if (strEvent == "SatonGrap")
	{
		m_bSatonGrap = true;
	}
	else if (strEvent == "SatonBird")
	{
		m_bSatonBird = true;
	}
	else if (strEvent == "SymbolAttack")
	{
		// m_bSatonSymbol_Blue = true;
	}
	else if (strEvent == "AnimStopped")
	{
		m_bDelete = true;
	}
	else if (strEvent == "MakeMoon") // 플레이어 머리 위에 달 띄우기
	{

	}
	else if (strEvent == "DrawMoon") // 플레이어 위치 받아서 바닥에 달 그리기
	{
	}
	else if (strEvent == "ExplodeMoon") // 바닥에 있던 달 위치에 쇼크파우더 뿌리기 
	{
		m_bStatonExplodeMoon = true;
	}
}

_int CSaton::Update_Object(const _float& fTimeDelta)
{
	if (m_bDelete) return OBJ_DEAD;


	CMonster::Update_Object(fTimeDelta);

	if (m_pCurAnim == m_pIdleAnim) // 이전 애니메이션 종료
		m_bCanPlayAnim = true;




	//
	// NULL_CHECK_RETURN(pkouku, 0);
	//
	// _vec3 vPos = saton->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];

	// 상태 변경 조건 설정
	StateChange();

	// 각 상태에 따른 프레임 마다 실행할 함수 지정
	switch (m_eState)
	{
	case INTRO:
		m_strState = "INTRO";
		break;
	case IDLE:
		// m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fSpeed * fTimeDelta;
		m_strState = "IDLE";
		break;
	case FIRSTATTACK:
		m_strState = "FIRSTATTACK";
		break;
	case SATON_BIRD:
		m_strState = "SATON_BIRD";
		break;
	case SATON_GRAP:
		m_strState = "SATON_GRAP";
		break;
	case SATON_SYMBOL:
		m_strState = "SATON_SYMBOL";
		break;
	case SATON_FASCINATE:
		m_strState = "SATON_FASCINATE";
		break;
	case DEAD:
		m_strState = "DEAD";
	default:
		break;
	}

	// if (m_eState != HORROR_ATTACK)
	// {
	// 	if (m_fCurTime > m_fTime)
	// 		m_fCurTime = 0.f;
	// }

	IM_BEGIN("StatePos");

	ImGui::Text("%f,%f,%f",m_pRootPart->pTrans->m_vInfo[INFO_POS].x, m_pRootPart->pTrans->m_vInfo[INFO_POS].y, m_pRootPart->pTrans->m_vInfo[INFO_POS].z);

	IM_END;

	return OBJ_NOEVENT;
}

void CSaton::LateUpdate_Object()
{
	CMonster::LateUpdate_Object();

	if (m_bStatonExplodeMoon)
	{
		set<CGameObject*> setPlayer;
		// _vec3 KoukuPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];

		Engine::GetOverlappedObject(setPlayer, m_vExplodMoonPos, 3.5f);

		for (auto& obj : setPlayer)
		{

			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
			{
				CStatComponent* PlayerStatCom = pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC);

				if (!PlayerStatCom->IsSatonSymbol_Blue())
				{
					PlayerStatCom->TakeDamage(0, pPlayer->GetInfo(INFO_POS), this, DT_STUN);
				}
			}
		}

		for (int j = 0; j < 10; j++)
		{
			CEffectFactory::Create<CShock_Powder>("Shock_Powder", L"UV_Shock_Powder",
				_vec3(m_vExplodMoonPos.x + CGameUtilMgr::GetRandomFloat(-3.f,3.f),
					m_vExplodMoonPos.y, m_vExplodMoonPos.z + CGameUtilMgr::GetRandomFloat(-3.f, 3.f)));
			CEffectFactory::Create<CCloud>("ShockPowder_Cloud", L"ShockPowder_Cloud", m_vExplodMoonPos);
		}

		CEffectFactory::Create<CUVCircle>("Shock_Circle", L"Shock_Circle");
		CSoundMgr::GetInstance()->PlaySound(L"sfx_item_shockpowder-001.ogg", m_vExplodMoonPos);
		m_bStatonExplodeMoon = false;
	}
}

void CSaton::Free()
{
	CMonster::Free();
}

CSaton* CSaton::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	CSaton* pInstance = new CSaton(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

void CSaton::StateChange()
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

	if (m_bFirstHammerAttack && m_bCanPlayAnim)
	{
		m_eState = FIRSTATTACK;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[FIRSTATTACK]);
		m_bCanPlayAnim = false;
		
		SetOff();
		return;
	}

	if (m_bSatonBird && m_bCanPlayAnim)
	{
		m_eState = SATON_BIRD;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[SATON_BIRD]);
		m_bCanPlayAnim = false;
		SetOff();
		return;
	}

	if (m_bSatonGrap && m_bCanPlayAnim)
	{
		m_eState = SATON_GRAP;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[SATON_GRAP]);
		m_bCanPlayAnim = false;
		SetOff();
		return;
	}
	if (m_bSatonSymbolAnim && m_bCanPlayAnim && m_eState == IDLE)
	{
		m_eState = SATON_SYMBOL;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[SATON_SYMBOL]);
		m_bCanPlayAnim = false;
		SetOff();
		return;
	}
	if (m_bSatonFascinate && m_bCanPlayAnim)
	{
		m_eState = SATON_FASCINATE;
		RotateToTargetPos(m_vTargetPos);
		PlayAnimationOnce(&m_arrAnim[SATON_FASCINATE]);
		m_bCanPlayAnim = false;
		SetOff();
		return;
	}
	if (m_bCanPlayAnim)
	{
		m_eState = IDLE;
		RotateToTargetPos(m_vTargetPos);
		m_pIdleAnim = &m_arrAnim[IDLE];
		m_pCurAnim = &m_arrAnim[IDLE];
		return;
	}
}

void CSaton::SatonShockPowder()
{

}

CSaton::~CSaton()
{
}
