#include "stdafx.h"
#include "BootsOfSwiftness.h"
#include "Particle.h"
#include "AbstFactory.h"
#include "Player.h"

CBootsOfSwiftness::CBootsOfSwiftness(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipItem(pGraphicDev)
{
}


CBootsOfSwiftness::~CBootsOfSwiftness()
{
}

HRESULT CBootsOfSwiftness::Ready_Object()
{
	m_iUItexNum = 11;
	return S_OK;
}

_int CBootsOfSwiftness::Update_Object(const _float & fTimeDelta)
{
	if (m_bUse)
	{
		m_fAge = 0.f;
	
		CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
		pPlayer->SetSpeed(7.5f);
		_vec3 vPos = pPlayer->GetInfo(INFO_POS);

		Get_GameObject<C3DBaseTexture>(LAYER_EFFECT, L"3D_Base")->Add_Particle(vPos, 3.f, D3DXCOLOR(0.f, 0.63f, 0.82f, 0.f), 1, 1.5f);
		Get_GameObject<CSpeedBoots>(LAYER_EFFECT, L"Speed_Boots")->Add_Particle(vPos, 3.f, D3DXCOLOR(0.2f, 0.2f, 0.5f, 1.f), 1, 1.5f);
		Get_GameObject<CSpeedBoots_Particle>(LAYER_EFFECT, L"Speed_Boots_Particle")->Add_Particle(
			_vec3(vPos.x, vPos.y + 15.f, vPos.z),
			1.f, D3DXCOLOR(0.3f, 0.4f, 0.7f, 1.f), 18, m_fLifeTime);
		m_bEnd = true;
		m_bUse = false;
		
	}

	m_fAge += fTimeDelta;

	if (m_bEnd && m_fAge >= m_fLifeTime)
	{
		Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player")->SetSpeed(4.5f);
		m_bEnd = false;
	}


	CEquipItem::Update_Object(fTimeDelta);
	return OBJ_NOEVENT;
}

void CBootsOfSwiftness::LateUpdate_Object()
{
	CEquipItem::LateUpdate_Object();
}

void CBootsOfSwiftness::Render_Object()
{
}

void CBootsOfSwiftness::Free()
{
	CEquipItem::Free();
}

CBootsOfSwiftness * CBootsOfSwiftness::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBootsOfSwiftness* pInstance = new CBootsOfSwiftness(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBootsOfSwiftness::Equipment(SkeletalPart * pSkeletalPart)
{
}
