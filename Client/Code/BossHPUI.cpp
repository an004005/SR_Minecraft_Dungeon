#include "stdafx.h"
#include "..\Header\BossHPUI.h"
#include "StatComponent.h"

CBossHPUI::CBossHPUI(LPDIRECT3DDEVICE9 m_pGraphicDev) : CUI(m_pGraphicDev)
{
}

CBossHPUI::CBossHPUI(LPDIRECT3DDEVICE9 m_pGraphicDev, wstring wstrObj) : CUI(m_pGraphicDev)
{
}


CBossHPUI::~CBossHPUI()
{
}

HRESULT CBossHPUI::Ready_Object()
{
	FAILED_CHECK_RETURN(CUI::Ready_Object(), E_FAIL);
	m_iTexNum = 24;

	m_pHeart[0] = CRcBossHPTex::Create(m_pGraphicDev);
	m_pHeart[1] = CRcBossHPTex::Create(m_pGraphicDev);
	m_pHeart[2] = CRcBossHPTex::Create(m_pGraphicDev);
	m_pHeart[3] = CRcBossHPTex::Create(m_pGraphicDev);
	m_pHeart[4] = CRcBossHPTex::Create(m_pGraphicDev);
	
	return S_OK;
}

_int CBossHPUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CUI::Update_Object(fTimeDelta);

	
	if (m_iProgress != m_iCurHp)
	{
		if (m_iCurHp > m_fPersent * 4)
		{
			_float fRate = (m_iCurHp - (m_fPersent * 4)) / m_fPersent;
			m_pHeart[4]->SetProgress(fRate);
			m_strNum = to_wstring(5);
		}
		else if(m_iCurHp > m_fPersent * 3)
		{
			m_pHeart[4]->RendEnd();
			_float fRate = (m_iCurHp - (m_fPersent * 3)) / m_fPersent;
			m_pHeart[3]->SetProgress(fRate);
			m_strNum = to_wstring(4);
		}
		else if (m_iCurHp > m_fPersent * 2)
		{
			m_pHeart[3]->RendEnd();
			_float fRate = (m_iCurHp - (m_fPersent * 2)) / m_fPersent;
			m_pHeart[2]->SetProgress(fRate);
			m_strNum = to_wstring(3);
		}
		else if (m_iCurHp > m_fPersent * 1)
		{
			m_pHeart[2]->RendEnd();
			_float fRate = (m_iCurHp - (m_fPersent * 1)) / m_fPersent;
			m_pHeart[1]->SetProgress(fRate);
			m_strNum = to_wstring(2);
		}
		else
		{
			m_pHeart[1]->RendEnd();
			_float fRate = (m_iCurHp - (m_fPersent * 0)) / m_fPersent;
			m_pHeart[0]->SetProgress(fRate);
			m_strNum = to_wstring(1);
		}
		
		
		m_iProgress = m_iCurHp;
	}
	
	return OBJ_NOEVENT;
}

void CBossHPUI::Render_Object()
{
	
	Render_Font(L"Gothic_Regular25", m_wstrName.c_str(),
		&_vec2( WINCX * 0.435f, WINCY *0.07f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f));



	m_pGraphicDev->SetTransform(D3DTS_VIEW, &CGameUtilMgr::s_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iTexNum);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(19);
	m_pHeart[0]->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(20);
	m_pHeart[1]->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(21);
	m_pHeart[2]->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(22);
	m_pHeart[3]->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(23);
	m_pHeart[4]->Render_Buffer();

	wstring wstr = m_strLine + m_strNum;
	Render_Font(L"Gothic_Bold15", wstr.c_str(),
		&_vec2(WINCX * 0.67f, WINCY *0.135f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CBossHPUI::Free()
{
	for_each(m_pHeart.begin(), m_pHeart.end(), CDeleteObj());
	CUI::Free();
}

CBossHPUI * CBossHPUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum)
{
	CBossHPUI*		pInstance = new CBossHPUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

