#include "stdafx.h"
#include "..\Header\BirdsBrown.h"

#include "Export_Function.h"

CBirdsBrown::CBirdsBrown(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBirdsBrown::~CBirdsBrown()
{
}

HRESULT CBirdsBrown::Ready_Object(void)
{
	m_pBufferCom = Add_Component<CRcTex>(L"Proto_BirdBrown", L"Proto_BirdBrown", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_BirdsBrownTextCom", L"Proto_BirdsBrownTextCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);

	m_fSpeed = 10.f;

	return S_OK;
}

_int CBirdsBrown::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	const _vec3& vPlayerPos = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC)
		->m_vInfo[INFO_POS];

	m_fFrame2 += 8.f * fTimeDelta;

	if (m_bBrown == false)
	{
		m_fFrame2 = 0.f;
	}

	_float fDist = D3DXVec3Length(&(m_pTransCom->m_vInfo[INFO_POS] - vPlayerPos));
	
	if (fDist < 5.f)
	{
		if (m_bBrown == false)
		{
			m_vDir = { CGameUtilMgr::GetRandomFloat(-1.f, 1.f), 0.5f, CGameUtilMgr::GetRandomFloat(-1.f, 1.f) };
			D3DXVec3Normalize(&m_vDir, &m_vDir);

			_matrix		matCamWorld;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
			D3DXMatrixInverse(&matCamWorld, nullptr, &matCamWorld);

			_vec3	vCamLook{ matCamWorld._31,matCamWorld._32,matCamWorld._33 };
			D3DXVec3Normalize(&vCamLook, &vCamLook);

			_vec3	vCross;
			D3DXVec3Cross(&vCross, &vCamLook, &m_vDir);

			if (vCross.y > 0.f)
				m_bReverese = true;
		}

		m_bBrown = true;
	}

	if (m_bBrown)
	{
		if (m_fFrame2 >= 5.f)
			m_fFrame2 = 1.f;

		m_pTransCom->m_vInfo[INFO_POS] += fTimeDelta * m_vDir * m_fSpeed;

		if (fDist > 30.f)
			return OBJ_DEAD;
	}

	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	if (m_bReverese)
	{
		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;
	}
	else
	{
		matBill._11 = -matView._11;
		matBill._13 = -matView._13;
		matBill._31 = -matView._31;
		matBill._33 = -matView._33;
	}
	

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransCom->Set_WorldMatrix(&(matBill * matWorld));

	Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBirdsBrown::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CBirdsBrown::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture((_ulong)m_fFrame2);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CBirdsBrown * CBirdsBrown::Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJTYPE eType)
{
	CBirdsBrown*		pInstance = new CBirdsBrown(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBirdsBrown::Free(void)
{
	CGameObject::Free();
}
