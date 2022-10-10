#include "stdafx.h"
#include "..\Header\ShockPowder.h"
#include "AbstFactory.h"
#include "Particle.h"
#include "Player.h"
#include "StatComponent.h"
#include "Monster.h"

CShockPowder::CShockPowder(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipItem(pGraphicDev)
{
}


CShockPowder::~CShockPowder()
{
}

HRESULT CShockPowder::Ready_Object()
{
	return S_OK;
}

_int CShockPowder::Update_Object(const _float & fTimeDelta)
{
	if (!m_bUse)
		return 0;

	for (int j = 0; j < 10; j++)
	{
		CEffectFactory::Create<CShock_Powder>("Shock_Powder", L"UV_Shock_Powder");
		CEffectFactory::Create<CCloud>("ShockPowder_Cloud", L"ShockPowder_Cloud");
	}

	CEffectFactory::Create<CUVCircle>("Shock_Circle", L"Shock_Circle");

	m_bUse = false;
	m_bColl = true;
	CEquipItem::Update_Object(fTimeDelta);

	return OBJ_NOEVENT;
}

void CShockPowder::LateUpdate_Object()
{
	CEquipItem::LateUpdate_Object();
	if (!m_bColl)
		return;

	set<CGameObject*> objSet;

	CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
	_vec3 vPos = pPlayer->GetInfo(INFO_POS);

	Engine::GetOverlappedObject(OUT objSet, vPos, 3.5f);
	for (auto& obj : objSet)
	{
		if (CMonster* monster = dynamic_cast<CMonster*>(obj))
		{
			DamageType eDT = DT_END;
			if (m_iAttackCnt == 0) eDT = DT_STUN;
			if (monster->CheckCC()) eDT = DT_END;
			monster->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(0, vPos, this, eDT);
		}
	}

	DEBUG_SPHERE(vPos, 3.5f, 1.f);
	m_bColl = false;
	
}

void CShockPowder::Render_Object()
{
}

void CShockPowder::Free()
{
	CEquipItem::Free();
}

CShockPowder * CShockPowder::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShockPowder* pInstance = new CShockPowder(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CShockPowder::Equipment(SkeletalPart * pSkeletalPart)
{
}


