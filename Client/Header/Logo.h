#pragma once
#include "Scene.h"

class CEditBox;
class CUI;

enum LogoState
{
	INPUT_NAME,
	MOVETO_PLAYER,
	SELECT_PLAYER,
	LOGO_STATE_END
};

class CLogo : public CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

	void Confirm();

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);

private:
	LogoState eState = INPUT_NAME;

	CUI* m_pLogo = nullptr;
	CEditBox* m_pEdit = nullptr;
	CUI* m_pButton = nullptr;

	_float m_fS = 0.f;
	_vec3 m_fLogoTo{};
	_vec3 m_fEditTo{};
	_vec3 m_fButtonTo{};

	CSkeletalCube* m_pCamMove = nullptr;
	_vec3 m_vCamSelectPos = {2.5f, 2.5f, 2.5f};
};

