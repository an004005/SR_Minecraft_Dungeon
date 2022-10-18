#pragma once
#include "UI.h"

class CBossHPUI : public CUI
{
public:
	explicit CBossHPUI(LPDIRECT3DDEVICE9 m_pGraphicDev);
	explicit CBossHPUI(LPDIRECT3DDEVICE9 m_pGraphicDev, wstring wstrObj);
	virtual ~CBossHPUI();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object() override;
	virtual void Free() override;
	static CBossHPUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum);

	//���� �� �� ���
	void SetOwner(const wstring& wstrName, CGameObject* pOwner, _int iMaxHp)
	{
		m_pOwner = pOwner;
		m_wstrName = wstrName;
		m_iMaxHP = iMaxHp;
		m_fPersent = (_float)m_iMaxHP * 0.2f;

	}

	//������Ʈ�� ���
	void SetCurHp(_int iHp) { m_iCurHp = iHp; }

	//�ױ� ���� ���
	void KillHpbar() { m_bDead = true; }
	

private:
	array<CRcBossHPTex*, 5> m_pHeart{};

	_int m_iProgress = 0;
	_int m_iMaxHP = 0;

	_int m_iCurHp = 0;
	_float m_fPersent = 0;

	_bool m_bDead = false;
	CGameObject* m_pOwner = nullptr;
	wstring m_wstrName = L"";
	wstring m_strLine = L"X ";
	wstring m_strNum = L"";
};

