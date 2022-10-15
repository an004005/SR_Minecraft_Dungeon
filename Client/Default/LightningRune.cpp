#include "stdafx.h"
#include "LightningRune.h"
#include "Monster.h"
#include "Weapon.h"
#include "Player.h"
#include "StatComponent.h"
#include <random>

CLightningRune::CLightningRune(LPDIRECT3DDEVICE9 pGraphicDev): CRune(pGraphicDev)
{
	m_eTargetType = WEAPON_SWORD;
}

CLightningRune::~CLightningRune()
{
}

HRESULT CLightningRune::Ready_Object()
{
	return CRune::Ready_Object();
}

_int CLightningRune::Update_Object(const _float& fTimeDelta)
{
	CRune::Update_Object(fTimeDelta);

	if (m_bSpark)
	{
		if (m_fCurSparkTime < m_fSparkTime)
			m_fCurSparkTime += fTimeDelta;

		if (m_fCurSparkTime >= m_fSparkTime && m_WeaponTop && m_WeaponBot)
		{
			auto pSpark = CEffectFactory::Create<CChainLightning>("ChainLightning", L"ChainLightning");
			pSpark->AddRef();
			pSpark->SetSpark();
			
			m_vecSparks.push_back(pSpark);


			m_fCurSparkTime = 0.f;
		}

		const _matrix matTop = m_WeaponTop->GetWorldMat();
		const _matrix matBot = m_WeaponBot->GetWorldMat();
		const _vec3 vBot{matBot._41, matBot._42, matBot._43};
		const _vec3 vTop{matTop._41, matTop._42, matTop._43};

		for (auto itr = m_vecSparks.begin(); itr != m_vecSparks.end();)
		{
			auto pSpark = (*itr);
			if (pSpark->GetRef() == 0)
			{
				Safe_Release(pSpark);
				itr = m_vecSparks.erase(itr);
			}
			else
			{
				pSpark->SetSparkPos(vBot, vTop);
				++itr;
			}
		}

	}

	return OBJ_NOEVENT;
}

template<class BidiIter>
BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        BidiIter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

void CLightningRune::Collision()
{
	CRune::Collision();
	const _vec3& vOwnerPos = m_pOwner->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
	CSoundMgr::GetInstance()->PlaySoundRandom({L"sfx_env_thunderPassing-001_soundWave.ogg", L"sfx_env_thunderPassing-002_soundWave.ogg", L"sfx_env_thunderPassing-003_soundWave.ogg"}, 
		vOwnerPos);

	CPlayer* pPlayer =m_pOwner;
	_vec3 vPos = pPlayer->GetInfo(INFO_POS);

	set<CGameObject*> objSet;
	Engine::GetOverlappedObject(OUT objSet, vPos, 4.f);

	vector<CGameObject*> tmp{objSet.begin(), objSet.end()};
	if (tmp.size() > 5)
		random_unique(tmp.begin(), tmp.end(), 5);

	for (auto& obj : objSet)
	{
		if (CMonster* monster = dynamic_cast<CMonster*>(obj))
		{
			monster->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(10, vPos, m_pOwner);
			const _vec3 vMonsterPos = monster->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS] + _vec3{0.f, 1.5f, 0.f};
			CEffectFactory::Create<CChainLightning>("ChainLightning", L"ChainLightning", vMonsterPos);
		}
	}
}

CLightningRune* CLightningRune::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightningRune* pInstance = new CLightningRune(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLightningRune::EquipRune(CWeapon* pWeapon)
{
	m_bSpark = true;
	m_WeaponTop = m_pOwner->Get_SkeletalPart("weapon_top");
	m_WeaponBot = m_pOwner->Get_SkeletalPart("weapon_bot");
}

void CLightningRune::UnEquipRune(CWeapon* pWeapon)
{
	for(auto& e : m_vecSparks)
		Safe_Release(e);
	m_bSpark = false;
	m_WeaponTop = nullptr;
	m_WeaponBot = nullptr;
}

void CLightningRune::Free()
{
	for(auto& e : m_vecSparks)
		Safe_Release(e);
	CRune::Free();
}
