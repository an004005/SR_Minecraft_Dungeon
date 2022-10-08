#include "stdafx.h"
#include "..\Header\Crossbow.h"
#include "SkeletalCube.h"
#include "Player.h"

CCrossbow::CCrossbow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEquipItem(pGraphicDev)
{
}

CCrossbow::~CCrossbow()
{
}

HRESULT CCrossbow::Ready_Object()
{
	m_pBufferCom = Add_Component<CVoxelTex>(L"Proto_VoxelTex_Crossbow", L"Proto_VoxelTex_Crossbow", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_WeaponTexture", L"Proto_WeaponTexture", ID_STATIC);

	//Attack_start
	m_arrAnim[ANIM_ATTACK1] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_start.anim");
	//Attack_roof
	m_arrAnim[ANIM_ATTACK2] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_loop.anim");
	//Attack_end
	m_arrAnim[ANIM_ATTACK3] = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/CubeMan/crossbow_attack_end.anim");

	m_eItemType = IT_RANGE;

	return S_OK;
}

_int CCrossbow::Update_Object(const _float & fTimeDelta)
{
	CEquipItem::Update_Object(fTimeDelta);
	return 0;
}

void CCrossbow::LateUpdate_Object()
{
	CEquipItem::LateUpdate_Object();
}

void CCrossbow::Render_Object()
{
	/*m_pTextureCom->Set_Texture(2);
	m_pBufferCom->Render_Buffer();*/
}

CCrossbow * CCrossbow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossbow* pInstance = new CCrossbow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCrossbow::Free()
{
	CEquipItem::Free();
}

_int CCrossbow::Attack()
{
	CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
	if (pPlayer == nullptr)
		return 0;

	if (m_iAttackCnt == 0)
	{

		pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK1]);
	}
	else
	{
		pPlayer->PlayAnimationOnce(&m_arrAnim[ANIM_ATTACK2]);
	}

	m_iAttackCnt = (m_iAttackCnt + 1) % 2;

	return m_iAttackCnt;
}

void CCrossbow::Equipment(SkeletalPart* pSkeletalPart)
{
	pSkeletalPart->pBuf = m_pBufferCom;
	pSkeletalPart->pTex = m_pTextureCom;
	pSkeletalPart->iTexIdx = 2;
}
