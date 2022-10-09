#include "stdafx.h"
#include "..\Header\Box.h"
#include "Player.h"
#include "SkeletalCube.h"
#include "AbstFactory.h"
#include"Glaive.h"
#include "Emerald.h"
CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CSkeletalCube(pGraphicDev)
{
}

CBox::~CBox()
{
}

void CBox::AnimationEvent(const string& strEvent)
{
	if (strEvent == "ActionEnd")
	{		

		CGlaive* pGlaive = CItemFactory::Create<CGlaive>("Glaive", L"Glaive", m_pRootPart->pTrans->m_vInfo[INFO_POS], IS_DROP);
		pGlaive->SetVelocity(m_pRootPart->pTrans->m_vInfo[INFO_LOOK], 15.f);

		for (_int i = 0; i < 7; ++i)
		{
			_float fPower = _float(rand() % 5 + 15);
			CEmerald* pEmerald = CItemFactory::Create<CEmerald>("Emerald", L"Emerald", m_pRootPart->pTrans->m_vInfo[INFO_POS], IS_DROP);
			pEmerald->SetVelocity(m_pRootPart->pTrans->m_vInfo[INFO_LOOK], fPower);
		}
	
	}
	
}

HRESULT CBox::Ready_Object()
{
	CSkeletalCube::Ready_Object();
	LoadSkeletal(L"../Bin/Resource/SkeletalCube/Object/Box.cube");

	m_AnimClosen = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Box/box_closen.anim");
	m_AnimClosen.bLoop = true;
	m_AnimPlay = CubeAnimFrame::Load(L"../Bin/Resource/CubeAnim/Box/box_play.anim");


	m_pIdleAnim = &m_AnimClosen;
	m_pCurAnim = m_pIdleAnim;
	m_pRootPart->pTrans->m_vInfo[INFO_POS] = { 3.f, 8.f, 6.f };

	Engine::Add_StaticCollision(m_pRootPart->pTrans->m_vInfo[INFO_POS], 1.f);

	return S_OK;
}

_int CBox::Update_Object(const _float & fTimeDelta)
{
	CSkeletalCube::Update_Object(fTimeDelta);

	_int iResult = Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	

	return 0;
}

void CBox::LateUpdate_Object()
{
}


void CBox::Free()
{
	CGameObject::Free();
}

CBox * CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBox* pInstance = new CBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBox::BoxOpen()
{
	if (m_bOpened)
		return;

	CPlayer* pPlayer = Get_GameObject<CPlayer>(LAYER_PLAYER, L"Player");
	if (pPlayer == nullptr)
		return;

	_vec3 vPlayerPos = pPlayer->GetInfo(INFO_POS);
	_vec3 vBoxPos = m_pRootPart->pTrans->m_vInfo[INFO_POS];

	_float fLenth = D3DXVec3Length(&(vBoxPos - vPlayerPos));

	if (fLenth - 2.f <= 0.000001f)
	{
		PlayAnimationOnce(&m_AnimPlay);
	}

	m_bOpened = true;
}
