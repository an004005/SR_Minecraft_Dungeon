#include "stdafx.h"
#include "..\Header\DamageFontMgr.h"

IMPLEMENT_SINGLETON(CDamageFontMgr)

CDamageFontMgr::CDamageFontMgr()
{
	m_fSpeed = 1.5f;
}

CDamageFontMgr::~CDamageFontMgr()
{
	Free();
}

void CDamageFontMgr::Update_DamageFontMgr(const _float& fTimeDelta)
{
	for (auto itr = m_vecDamageFont.begin(); itr != m_vecDamageFont.end();)
	{
		if (itr->fTime <= 0.f)
		{
			itr = m_vecDamageFont.erase(itr);
		}
		else
		{
			itr->vPos += itr->vDir * fTimeDelta * m_fSpeed;
			if (itr->fTime < 0.15f)
				itr->Color.a = itr->fTime / 0.15f;
			itr->fTime -= fTimeDelta;
			++itr;
		}
	}

	_matrix view, proj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &view);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &proj);
	m_matViewProj = view * proj;
	ZeroMemory(&m_ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&m_ViewPort);
}

void CDamageFontMgr::Add_DamageFontFromWorld(_int iDamage, const _vec3& vPos, const _vec3& vFrom, D3DXCOLOR Color)
{
	_vec3 vPosToScreen = vPos;
	D3DXVec3TransformCoord(&vPosToScreen, &vPosToScreen, &m_matViewProj);

	vPosToScreen.x = m_ViewPort.Width * ( vPosToScreen.x + 1.0f ) / 2.0f + m_ViewPort.X;
	vPosToScreen.y = m_ViewPort.Height * ( 2.0f - ( vPosToScreen.y + 1.0f ) ) / 2.0f + m_ViewPort.Y;

	_vec3 vFromToScreen = vFrom;
	D3DXVec3TransformCoord(&vFromToScreen, &vFromToScreen, &m_matViewProj);
	vFromToScreen.x = (_float)m_ViewPort.Width * ( vFromToScreen.x + 1.0f ) / 2.0f + (_float)m_ViewPort.X;
	vFromToScreen.y = (_float)m_ViewPort.Height * ( 2.0f - ( vFromToScreen.y + 1.0f ) ) / 2.0f + (_float)m_ViewPort.Y;

	_vec3 vDir = vPosToScreen - vFromToScreen;

	m_vecDamageFont.push_back({iDamage, {vDir.x, vDir.y}, {vPosToScreen.x, vPosToScreen.y}, Color});
}

void CDamageFontMgr::Add_DamageFontFromScreen(_int iDamage, const _vec2& vScreen, D3DXCOLOR Color)
{
	m_vecDamageFont.push_back({iDamage, {0.f, 0.f}, vScreen, Color});
}

void CDamageFontMgr::Render_DamageFontMgr()
{
	for (const auto& damageFont : m_vecDamageFont)
	{
		Engine::Render_Font(
			L"Gothic_Bold20", 
			to_wstring(damageFont.iDamage).c_str(),
			&damageFont.vPos,
			damageFont.Color);
	}
}

void CDamageFontMgr::Free()
{
	Safe_Release(m_pGraphicDev);
	m_vecDamageFont.clear();
}
