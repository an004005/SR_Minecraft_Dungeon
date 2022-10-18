#include "stdafx.h"
#include "..\Header\StormRune.h"
#include "Player.h"
#include "Monster.h"
#include "StatComponent.h"

CStormRune::CStormRune(LPDIRECT3DDEVICE9 pGraphicDev): CRune(pGraphicDev)
{
	m_eTargetType = WEAPON_GLAIVE;
	m_iUItexNum = 18;
	m_eItemType = IT_RUNE;
}

CStormRune::~CStormRune()
{
}

HRESULT CStormRune::Ready_Object()
{
	m_pItemUI = CUIFactory::Create<CItemUI>("ItemUI", L"StormRuneUI", 0);
	m_pItemUI->SetUITexture(m_iUItexNum);

	return CRune::Ready_Object();
}

_int CStormRune::Update_Object(const _float& fTimeDelta)
{
	if (!m_bUse)
		return OBJ_NOEVENT;

	CRune::Update_Object(fTimeDelta);


	if (m_fCurFlameTime < m_fFlameTime)
		m_fCurFlameTime += fTimeDelta;

	if (m_fCurFlameTime >= m_fFlameTime)
	{
		CEffectFactory::Create<CLava_Particle>("Lava_Particle", L"Lava_Particle");

		m_fCurFlameTime = m_fFlameTime;
	}


	return S_OK;
}

void CStormRune::LateUpdate_Object()
{
	CRune::LateUpdate_Object();
}

void CStormRune::Render_Object()
{
	CRune::Render_Object();
}

void CStormRune::Free()
{
	CRune::Free();
}

void CStormRune::Collision()
{
	CRune::Collision();

	/*CSoundMgr::GetInstance()->PlaySoundRandom({ L"sfx_env_thunderPassing-001_soundWave.ogg", L"sfx_env_thunderPassing-002_soundWave.ogg", L"sfx_env_thunderPassing-003_soundWave.ogg" },
		vOwnerPos);*/

	CPlayer* pPlayer = m_pOwner;
	_vec3 vPos = pPlayer->GetInfo(INFO_POS);

	set<CGameObject*> objSet;
	Engine::GetOverlappedObject(OUT objSet, vPos, 4.f);

	
	for (auto& obj : objSet)
	{
		if (CMonster* monster = dynamic_cast<CMonster*>(obj))
		{
			monster->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(10, vPos, m_pOwner);
			const _vec3 vMonsterPos = monster->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS] + _vec3{ 0.f, 1.5f, 0.f };
			
		}
	}
}

void CStormRune::Use()
{
	CRune::Use();
}

CStormRune* CStormRune::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStormRune* pInstance = new CStormRune(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CStormRune::EquipRune(CWeapon * pWeapon)
{
	m_bUse = true;
}

void CStormRune::UnEquipRune(CWeapon * pWeapon)
{
	m_bUse = false;
}
