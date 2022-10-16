#include "stdafx.h"
#include "SatonController.h"

#include "Kouku.h"
#include "Saton.h"
#include "StatComponent.h"

CSatonController::CSatonController()
{
}

CSatonController::CSatonController(const CSatonController& rhs)
{
}

_int CSatonController::Update_Component(const _float& fTimeDelta)
{
	{
		// m_fCurFirstHammerCoolTime += fTimeDelta;
		// m_fCurSatonBirdCoolTime += fTimeDelta;
		// m_fCurSatonGrapCoolTime += fTimeDelta;
		m_fCurSatonFascinateTime += fTimeDelta;
	}

	IM_BEGIN("Saton_CoolTIme");
	ImGui::Text("Fascinate Cur CoolTime : %f", m_fCurSatonFascinateTime);


	IM_END;


	CSaton* saton = dynamic_cast<CSaton*>(m_pOwner);
	NULL_CHECK_RETURN(saton, 0);

	_vec3 vPos = saton->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
	_vec3 vTargetPos;
	_float fTargetDist = 9999.f;

	if (m_fCurTargetingCoolTime > m_fTargetingCoolTime)
	{
		for (auto& ele : Get_Layer(LAYER_PLAYER)->Get_MapObject())
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(ele.second))
			{
				vTargetPos = pPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
				_vec3 vDiff = vTargetPos - vPos;
				_float fDist = D3DXVec3Length(&vDiff);

				if (fTargetDist > fDist) // 플레이어 감지
				{
					m_pTargetPlayer = pPlayer;
					fTargetDist = fDist;
				}
			}
		}
		m_fCurTargetingCoolTime = 0.f;

		if (fTargetDist > 20.f)
			m_pTargetPlayer = nullptr;
	}
	else
	{
		m_fCurTargetingCoolTime += fTimeDelta;
		if (m_pTargetPlayer == nullptr) return 0;

		vTargetPos = m_pTargetPlayer->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
		_vec3 vDiff = vTargetPos - vPos;
		_float fDist = D3DXVec3Length(&vDiff);
	}

	CKouku* pKouku = Engine::Get_GameObject<CKouku>(LAYER_ENEMY, L"Kouku");
	m_vLookFront = saton->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_LOOK] - _vec3(62.5f, 0, 45.f);
	m_vLookFront = pKouku->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
	if(pKouku->Check_SymbolGimmick() && !m_bIsKoukuSymbol)
	{
		saton->SatonSymbolAnim(_vec3(m_vLookFront));
		m_bIsKoukuSymbol = !m_bIsKoukuSymbol;
	}

	if (m_fCurFirstHammerCoolTime >= m_fFirstHammerCoolTime && fTargetDist <= m_fFirstHammerDist)
	{
		m_fCurFirstHammerCoolTime = 0.f;
		saton->FirstAttack(vTargetPos);
		return 0;
	}

	if (m_fCurSatonBirdCoolTime >= m_fSatonBirdCoolTime && fTargetDist <= m_fSatonBirdkDist)
	{
		m_fCurSatonBirdCoolTime = 0.f;
		saton->SatonBird(vTargetPos);
		return 0;
	}

	if (m_fCurSatonFascinateTime >= m_fSatonFascinateCoolTime && fTargetDist <= m_fSatonFascinateDist)
	{
		m_fCurSatonFascinateTime = 0.f;
		saton->SatonFascinate(m_vLookFront);
		return 0;
	}

	if (m_fCurSatonGrapCoolTime >= m_fSatonGrapCoolTime && fTargetDist <= m_fSatonGrapkDist)
	{
		m_fCurSatonGrapCoolTime = 0.f;
		saton->SatonGrap(vTargetPos);
		return 0;
	}

	_vec3  Pos = saton->Get_Component<Engine::CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS] - _vec3(62.5f, 21.5f, 47.f);

	saton->WalkToTarget(vTargetPos);

	return 0;
}

CComponent* CSatonController::Clone()
{
	return new CSatonController(*this);
}

void CSatonController::Free()
{
	CController::Free();
}

CSatonController* CSatonController::Create()
{
	return new CSatonController;
}

CSatonController::~CSatonController()
{
}
