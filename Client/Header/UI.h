#pragma once
#include "GameObject.h"

/*------------------------
 * CUI
 ------------------------*/
class CUI : public CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUI();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;
	static CUI*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum);
	virtual void		Free(void);

protected:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	_uint m_iTexNum = 0;

	_matrix				m_ProjMatrix;
};

/*------------------------
 * CCountUI
 ------------------------*/
enum CountUIType
{
	CNT_EMERALD,
	CNT_ARROW,
	CNT_END
};

class CCountUI : public CUI
{
private:
	explicit CCountUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCountUI() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object() override;
	static CCountUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, CountUIType eType);

private:
	 CountUIType m_eType = CNT_END;
	_uint m_iCnt = 0;
};