#include "stdafx.h"
#include "..\Header\ShockPowder.h"
#include "AbstFactory.h"
#include "Particle.h"

CShockPowder::CShockPowder(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipItem(pGraphicDev)
{
}


CShockPowder::~CShockPowder()
{
}

HRESULT CShockPowder::Ready_Object()
{
	m_arrAnim[ANIM_ROLL] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/roll.anim");
	m_arrAnim[ANIM_LEGACY1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/shock_powder.anim");
	m_arrAnim[ANIM_LEGACY2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/shock_powder.anim");
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

	CEquipItem::Update_Object(fTimeDelta);

	return OBJ_NOEVENT;
}

void CShockPowder::LateUpdate_Object()
{
	
	
	CEquipItem::LateUpdate_Object();
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
