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
	m_pController = CGeomancerController::Create();
	CMonster::Ready_Object();
	m_arrLoopAnim[IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Geomancer/idle.anim");
	m_arrLoopAnim[WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Geomancer/walk.anim");
	m_arrLoopAnim[DEAD] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Geomancer/dead.anim");
	m_arrOnceAnim[ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Geomancer/attack.anim");
	// m_pIdleAnim = &m_arrLoopAnim[IDLE];
	// m_pCurAnim = m_pIdleAnim;

	m_pStat->SetMaxHP(100);

	return S_OK;
}

void CGeomancer::AnimationEvent(const string& strEvent)
{
	if (strEvent == "AttackFire")
	{
		m_bAttack = true;
	}
	else if (strEvent == "ActionEnd")
	{
		m_bAction = false;
		SetIdle();
	}
}

_int CGeomancer::Update_Object(const _float& fTimeDelta)
{
	int iEvent = CMonster::Update_Object(fTimeDelta);
	m_pController->Update(this);

	if (m_fCurAttackCoolTime < m_fAttackCoolTime)
		m_fCurAttackCoolTime += fTimeDelta;

	m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_vMoveDir * fTimeDelta * 2.5f;

	return iEvent;
}

void CGeomancer::LateUpdate_Object()
{
	CMonster::LateUpdate_Object();
	if (m_bAttack)
	{
		set<CGameObject*> setObj;
		Engine::GetOverlappedObject(setObj, m_vAttackDest, 3.f);

		for (auto& obj : setObj)
		{
			if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(obj))
				pPlayer->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				       ->TakeDamage(1, m_pRootPart->pTrans->m_vInfo[INFO_POS], this, DT_KNOCK_BACK);

			DEBUG_SPHERE(m_vAttackDest, 3.f, 1.f);
			IM_LOG("Fire");
		}

		m_bAttack = false;
	}
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

void CGeomancer::SetIdle()
{
	// m_pIdleAnim = &m_arrLoopAnim[IDLE];
	m_pCurAnim = &m_arrLoopAnim[IDLE];
	m_vMoveDir = CGameUtilMgr::s_vZero;
}

void CGeomancer::SetAttack(CPlayer* pPlayer)
{
	const _vec3& vTargetPos = pPlayer->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[
		INFO_POS];
	SetRotationTo(vTargetPos);
	m_bAction = true;

	SetIdle();
	PlayAnimationOnce(&m_arrOnceAnim[ATTACK]);
	m_fCurAttackCoolTime = 0.f;
	m_vMoveDir = CGameUtilMgr::s_vZero;

	m_vAttackDest = vTargetPos;
}

void CGeomancer::Run(CPlayer* pPlayer)
{
	if (m_bAction == true) return;

	const _vec3& vPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];
	const _vec3 vPlayerPos = pPlayer->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC)->m_vInfo[
		INFO_POS];
	SetRotationTo(vPlayerPos, true);

	m_vMoveDir = vPos - vPlayerPos;
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

	// m_pIdleAnim = &m_arrLoopAnim[WALK];
	m_pCurAnim = &m_arrLoopAnim[WALK];
}
