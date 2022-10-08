#include "stdafx.h"
#include "..\Header\Glaive.h"
#include "SkeletalCube.h"
#include "Player.h"
#include "StatComponent.h"
#include "Monster.h"

CGlaive::CGlaive(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipItem(pGraphicDev)
{
}

CGlaive::~CGlaive()
{
}

HRESULT CGlaive::Ready_Object()
{
	m_pBufferCom = Add_Component<CVoxelTex>(L"Proto_VoxelTex_Glaive", L"Proto_VoxelTex_Glaive", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_WeaponTexture", L"Proto_WeaponTexture", ID_STATIC);

	m_arrAnim[ANIM_IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/glaive_idle.anim");
	m_arrAnim[ANIM_IDLE].bLoop = true;
	m_arrAnim[ANIM_WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/glaive_walk.anim");
	m_arrAnim[ANIM_WALK].bLoop = true;
	m_arrAnim[ANIM_ATTACK1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/glaive_attack_a.anim");
	m_arrAnim[ANIM_ATTACK2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/glaive_attack_a.anim");
	m_arrAnim[ANIM_ATTACK3] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/glavie_attack_b.anim");
	m_arrAnim[ANIM_ROLL] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/roll.anim");
	m_arrAnim[ANIM_RANGE_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_start.anim");
	m_arrAnim[ANIM_LEGACY1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/shock_powder.anim");
	m_arrAnim[ANIM_LEGACY2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/shock_powder.anim");

	m_eItemType = IT_MELEE;
	return S_OK;
}

_int CGlaive::Update_Object(const _float & fTimeDelta)
{
	CEquipItem::Update_Object(fTimeDelta);
	return 0;
}

void CGlaive::LateUpdate_Object()
{
	CEquipItem::LateUpdate_Object();

}

void CGlaive::Render_Object()
{

}

_int CGlaive::Attack()
{
	CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
	if (pPlayer == nullptr)
		return 0;

	if (m_iAttackCnt == 0)
	{
		pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK1]);
	}
	else if(m_iAttackCnt == 1)
	{
		pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK2]);
	}
	else
	{
		pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK2]);
	}

	m_iAttackCnt = (m_iAttackCnt + 1) % 3;

	return m_iAttackCnt;
}

CGlaive * CGlaive::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGlaive* pInstance = new CGlaive(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGlaive::Free()
{
	CEquipItem::Free();
}

void CGlaive::Equipment(SkeletalPart * pSkeletalPart)
{
	pSkeletalPart->pBuf = m_pBufferCom;
	pSkeletalPart->pTex = m_pTextureCom;
	pSkeletalPart->iTexIdx = 1;
}

void CGlaive::Collision()
{
	set<CGameObject*> objSet;

	CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
	_vec3 vPos = pPlayer->GetInfo(INFO_POS);
	_vec3 vLook = pPlayer->GetInfo(INFO_LOOK);

	_vec3 vAttackPos = vPos + (vLook * 2.5f);
	Engine::GetOverlappedObject(OUT objSet, vAttackPos, 2.5f);
	for (auto& obj : objSet)
	{
		if (CMonster* monster = dynamic_cast<CMonster*>(obj))
		{
			DamageType eDT = DT_END;
			if (m_iAttackCnt == 0) eDT = DT_KNOCK_BACK;
			if (monster->CheckCC()) eDT = DT_END;
			monster->Get_Component<CStatComponent>(L"Proto_StatCom", ID_DYNAMIC)
				->TakeDamage(30, vPos, this, eDT);
		}
	}

	DEBUG_SPHERE(vAttackPos, 2.5f, 1.f);
}
