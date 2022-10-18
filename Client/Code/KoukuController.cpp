#include "stdafx.h"
#include "KoukuController.h"

#include "Kouku.h"
#include "StatComponent.h"


CKoukuController::CKoukuController()
{
}

CKoukuController::CKoukuController(const CKoukuController& rhs)
{
}

CKoukuController::~CKoukuController()
{
}

_int CKoukuController::Update_Component(const _float& fTimeDelta)
{
	{
		// m_fCurDoubleHammerCoolTime += fTimeDelta;
		// m_fCurHorrorAttackCoolTime += fTimeDelta;
		// m_fCurBasicAttackCoolTime += fTimeDelta;

		if (m_bIsSymbolGimmick)
		{
			m_fCurSymbolGimmickCoolTime += fTimeDelta;
		}
	}
	CKouku* pKouku = dynamic_cast<CKouku*>(m_pOwner);
	NULL_CHECK_RETURN(pKouku, 0);

	_vec3 vPos = pKouku->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
	_vec3 vTargetPos;
	// _float fTargetDist = 9999.f;

	if (m_fCurTargetingCoolTime > m_fTargetingCoolTime)
	{
		for (auto& ele : Get_Layer(LAYER_PLAYER)->Get_MapObject())
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(ele.second))
			{
				vTargetPos = pPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
				_vec3 vDiff = vTargetPos - vPos;
				_float fDist = D3DXVec3Length(&vDiff);

				if (m_fTargetDist > fDist) // 플레이어 감지
				{
					m_pTargetPlayer = pPlayer;
					m_fTargetDist = fDist;
				}
			}
		}
		m_fCurTargetingCoolTime = 0.f;

		if (m_fTargetDist > 20.f)
			m_pTargetPlayer = nullptr;
	}
	else
	{
		m_fCurTargetingCoolTime += fTimeDelta;
		if (m_pTargetPlayer == nullptr) return 0;

		vTargetPos = m_pTargetPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
		_vec3 vDiff = vTargetPos - vPos;
		m_fTargetDist = D3DXVec3Length(&vDiff);
	}

	CStatComponent* pkouku = Engine::Get_Component<CStatComponent>(LAYER_ENEMY, L"Kouku", L"Proto_StatCom", ID_DYNAMIC);
	
	const _int koukuMaxHP = pkouku->GetMaxHP();
	const _int koukuHP = pkouku->GetHP();

	if(koukuHP <= koukuMaxHP / 2 && !m_bIsFirstSymbolGimmick)
	{
		pKouku->KoukuSymbol(vTargetPos);
		m_strState = "KoukuSymbol_On";
		pKouku->KoukuSymbol_OnOff(true);
		m_bIsFirstSymbolGimmick = true;
		m_bIsSymbolGimmick = true;
	}
	// 심볼 기믹false로 돌려줘야함

	if (m_fCurSymbolGimmickCoolTime >= m_fSymbolGimmickCoolTime && m_bIsSymbolGimmick)
	{
		m_fCurSymbolGimmickCoolTime = 0.f;
		pKouku->KoukuSymbol_OnOff(false);
		m_bIsSymbolGimmick = false;
		return 0;
	}

	if (m_fCurDoubleHammerCoolTime >= m_fDoubleHammerCoolTime && m_fTargetDist <= m_fDoubleHammerDist)
	{
		m_fCurDoubleHammerCoolTime = 0.f;
		pKouku->DoubleHammer(vTargetPos);
		m_strState = "DoubleHammer_On";
		return 0;
	}

	if (m_fCurBasicAttackCoolTime >= m_fBasicAttackCoolTime && m_fTargetDist <= m_fBasicAttackDist)
	{
		m_fCurBasicAttackCoolTime = 0.f;
		pKouku->BasicAttack(vTargetPos);
		m_strState = "BasicAttack_On";
		return 0;
	}

	if (m_fCurHorrorAttackCoolTime >= m_fHorrorAttackCoolTime && m_fTargetDist <= m_fHorrorAttackDist)
	{
		m_fCurHorrorAttackCoolTime = 0.f;
		pKouku->HorrorAttack(vTargetPos);
		m_strState = "HorrorAttack_On";
		return 0;
	}



	IM_BEGIN("test");
	ImGui::Text("%f", m_fTargetDist);
	IM_END;


	if(m_fTargetDist > 3.f)
	{

		// pKouku->WalkToTarget(vTargetPos);
	}
	
		
	// IM_BEGIN("Kouku_ControlloerData");
	//
	// ImGui::Text("CurDoubleHammerCoolTime : %f,DoubleHammerCoolTime : %f", m_fCurDoubleHammerCoolTime, m_fDoubleHammerCoolTime);
	// // ImGui::Text("m_bIsSymbolGimmick", m_bIsSymbolGimmick);
	// // ImGui::Text("%s, ", m_strState.c_str());
	// // ImGui::Text("%d", m_pStat->GetHP());
	// // ImGui::Text("%d", m_pStat->GetHP());
	// // ImGui::Text("%d", m_pStat->GetHP());
	// // ImGui::Text("%d", m_pStat->GetHP());
	// // ImGui::Text("%d", m_pStat->GetHP());
	//
	// ImGui::Separator();
	//
	// IM_END;


	return 0;
}

CComponent* CKoukuController::Clone()
{
	return new CKoukuController(*this);
}

void CKoukuController::Free()
{
	CController::Free();
}

CKoukuController* CKoukuController::Create()
{
	return new CKoukuController;
}
