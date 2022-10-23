#include "stdafx.h"
#include "..\Header\SkeletalGhostTrail.h"

class CStatComponent;

CSkeletalGhostTrail::CSkeletalGhostTrail(LPDIRECT3DDEVICE9 pGraphicDev) : CSkeletalCube(pGraphicDev)
{
}

CSkeletalGhostTrail::CSkeletalGhostTrail(const CSkeletalGhostTrail& rhs) : CSkeletalCube(rhs)
{
}

CSkeletalGhostTrail::~CSkeletalGhostTrail()
{
}

HRESULT CSkeletalGhostTrail::Ready_Object()
{
	CSkeletalCube::Ready_Object();
	m_pShaderCom = Add_Component<CShader>(L"Proto_GhostTrail", L"Proto_GhostTrail", ID_STATIC);

	return S_OK;
}

_int CSkeletalGhostTrail::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	m_fLife -= fTimeDelta;
	if (m_fLife < 0.f) return OBJ_DEAD;

	Add_RenderGroup(RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

void CSkeletalGhostTrail::Render_Object()
{
	_matrix ViewMatrix, ProjMatrix;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
	_float fS = (m_fInitLife - m_fLife) / m_fInitLife;

	m_pShaderCom->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_matrix));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_matrix));
	m_pShaderCom->Set_RawValue("g_Rate", &fS, sizeof(_float));

	m_bRenderMachine = false;

	m_pRootPart->matParents = m_pRootPart->pTrans->m_matWorld;
	for (const auto& child : m_pRootPart->vecChild)
	{
		RenderObjectRecur(child);
	}
}

void CSkeletalGhostTrail::SetColorTime(_float fLife, D3DXCOLOR BaseColor)
{
	m_fLife = fLife;
	m_fInitLife = fLife;
	m_BaseColor = BaseColor;
	m_pShaderCom->Set_RawValue("g_baseColor", &m_BaseColor, sizeof(D3DXCOLOR));
}

CSkeletalGhostTrail* CSkeletalGhostTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, CSkeletalCube* pSkeletal)
{
	NULL_CHECK_RETURN(pSkeletal, nullptr);

	auto pInst = new CSkeletalGhostTrail(pGraphicDev);
	if (FAILED(pInst->Ready_Object()))
	{
		Safe_Release(pInst);
		return nullptr;
	}
	
	pInst->LoadSkeletal(pSkeletal->GetCubePath());
	if (pInst->Get_SkeletalPart("weapon_r"))
		pInst->Get_SkeletalPart("weapon_r")->pBuf = nullptr;
	pInst->m_fAccTime = pSkeletal->GetCurFrameTime();
	pInst->m_pCurAnim = pSkeletal->GetCurAnimFrame();
	pInst->AnimFrameConsume(0.f);

	return pInst;
}
