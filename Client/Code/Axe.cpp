#include "stdafx.h"
#include "..\Header\Axe.h"

#include "AbstFactory.h"
#include "SkeletalCube.h"
#include "Player.h"
#include "Monster.h"
#include "Particle.h"
#include "StatComponent.h"

CAxe::CAxe(LPDIRECT3DDEVICE9 pGraphicDev): CEquipItem(pGraphicDev)
{
}

CAxe::~CAxe()
{
}

HRESULT CAxe::Ready_Object()
{
	CEquipItem::Ready_Object();
	m_pBufferCom = Add_Component<CVoxelTex>(L"Proto_VoxelTex_Axe", L"Proto_VoxelTex_Axe", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_WeaponTexture", L"Proto_WeaponTexture", ID_STATIC);
	
	m_arrAnim[ANIM_IDLE] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_idle.anim");
	m_arrAnim[ANIM_IDLE].bLoop = true;
	m_arrAnim[ANIM_WALK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/sword_walk.anim");
	m_arrAnim[ANIM_WALK].bLoop = true;
	m_arrAnim[ANIM_ATTACK1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/axe_attack_a.anim");
	m_arrAnim[ANIM_ATTACK2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/axe_attack_b.anim");
	m_arrAnim[ANIM_ROLL] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/roll.anim");
	m_arrAnim[ANIM_RANGE_ATTACK] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_start.anim");
	m_arrAnim[ANIM_LEGACY1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/shock_powder.anim");
	m_arrAnim[ANIM_LEGACY2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/shock_powder.anim");

	return S_OK;
}

_int CAxe::Update_Object(const _float& fTimeDelta)
{
	CEquipItem::Update_Object(fTimeDelta);
	return OBJ_NOEVENT;
}

void CAxe::Render_Object()
{
}

CAxe* CAxe::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAxe* pInstance = new CAxe(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

_int CAxe::Attack()
{
	CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");

	if (pPlayer == nullptr)
		return 0;

	if (m_iAttackCnt == 0)
	{
		pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK1]);
	}
	else if (m_iAttackCnt == 1)
	{
		pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK2]);
	}
	m_iAttackCnt = (m_iAttackCnt + 1) % 2;

	return m_iAttackCnt;
}

void CAxe::Equipment(SkeletalPart* pSkeletalPart)
{
	pSkeletalPart->pBuf = m_pBufferCom;
	pSkeletalPart->pTex = m_pTextureCom;
	pSkeletalPart->iTexIdx = 3;
}

void CAxe::Collision()
{
	set<CGameObject*> objSet;

	CPlayer* pPlayer =Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
	_vec3 vPos = pPlayer->GetInfo(INFO_POS);
	_vec3 vLook = pPlayer->GetInfo(INFO_LOOK);

	_vec3 vAttackPos = vPos;
	if (m_iAttackCnt == 0)
		vAttackPos += vLook * 2.5;

	Engine::GetOverlappedObject(OUT objSet, vAttackPos, 3.f);
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

	DEBUG_SPHERE(vAttackPos, 3.f, 1.f);
}
