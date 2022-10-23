#pragma once
#include "UI.h"
#include "SkeletalCube.h"

enum PLAYER_MVP_TYPE
{
	PLAYER_MVP, // ����
	PLAYER_COUNTER, // �ִ� ī����
	PLAYER_WALK, // ��å
	PLAYER_POTION_, // ���� �ߵ���
	PLAYER_MVP_END
};

class CPortrait : public CUI
{
protected:
	explicit CPortrait(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortrait();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void	Render_Object(void) override;
	static CPortrait*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum);
	virtual void		Free(void);

	void AddRenderObj(CGameObject* pObj) {m_pTargetTexture->AddRender(pObj);}
	void SetPlayerSkeletal(Protocol::PlayerSkin eSkin, PLAYER_MVP_TYPE eMvp, _float fDamagePercent, _uint iCnt, const wstring& wstrName);

private:
	CTargetTexture* m_pTargetTexture = nullptr;

	CubeAnimFrame m_Idle;
	CubeAnimFrame m_Dance;
	_matrix m_matPortraitCam;

	array<CSkeletalCube*, PLAYER_MVP_END> m_arrPlayer{};
	array<_float, PLAYER_MVP_END> m_arrPlayerDamage{};
	array<wstring, PLAYER_MVP_END> m_arrPlayerName{};
	array<_uint, PLAYER_MVP_END> m_arrPlayerCnt{};
};

